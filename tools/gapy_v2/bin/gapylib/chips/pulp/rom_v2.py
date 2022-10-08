"""Provides section template for gap rom v2 (starting from gap9_v2)"""

#
# Copyright (C) 2022 GreenWaves Technologies
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# Authors: Germain Haugou, GreenWaves Technologies (germain.haugou@greenwaves-technologies.com)
#


import typing
import dataclasses
from elftools.elf.elffile import ELFFile
from gapylib.flash import FlashSection, Flash
from gapylib.utils import CStruct, CStructParent



@dataclasses.dataclass
class BinarySegment():
    """
    Class for describing an ELF segment.

    Attributes
    ----------
    base : int
        Name of this sub-section.

    data : bytes
        Content of the section
    """
    def __init__(self, base: int, data: bytes):
        self.base = base
        self.data = data
        self.size = len(data)
        self.crc = self._compute_crc()

    def _compute_crc(self):
        """
        Compute the CRC32 for an ELF segment.
        """
        crc = 0xffffffff
        for data in self.data:
            crc = crc ^ data
            for _ in range(7, -1, -1):
                if crc & 1 == 1:
                    mask = 0xffffffff
                else:
                    mask = 0
                crc = (crc >> 1) ^ (0xEDB88320 & mask)

        return crc ^ 0xffffffff




@dataclasses.dataclass
class Binary():
    """
    Class for describing an ELF binary.

    Attributes
    ----------
    fd
        File descriptor
    """

    def __init__(self, file_desc: typing.BinaryIO):

        # Go through the ELF binary to find the entry point and the segments
        self.segments = []

        elffile = ELFFile(file_desc)
        self.entry = elffile['e_entry']

        for segment in elffile.iter_segments():
            if segment['p_type'] == 'PT_LOAD':
                self.segments.append(BinarySegment(segment['p_paddr'], segment.data()))



@dataclasses.dataclass
class RomEmptyHeader(CStruct):
    """
    Class for generating rom sub-section containing the main header when the ROM section is empty
    (when there is no binary)

    Attributes
    ----------
    name : str
        Name of this sub-section.

    parent : str
        Parent, which is aggregating all rom sub-sections.
    """

    def __init__(self, name, parent):
        super().__init__(name, parent)

        # Size of the whole rom section, used by the runtime to find the next section.
        self.rom_header_size    = self.add_field('rom_header_size', 'I')



@dataclasses.dataclass
class RomHeader(CStruct):
    """
    Class for generating rom sub-section containing the main header when the ROM section is not
    empty (when there is a binary)

    Attributes
    ----------
    name : str
        Name of this sub-section.

    parent : str
        Parent, which is aggregating all rom sub-sections.
    """

    def __init__(self, name, parent: CStructParent):
        super().__init__(name, parent)

        # Size of the whole rom section, used by the runtime to find the next section.
        self.add_field('rom_header_size', 'I')
        # Number of binary segments to be loaded from flash to chip memory
        self.add_field('nb_segments', 'I')
        # Binary entry point
        self.add_field('entry', 'I')
        # Unused
        self.add_field('unused', 'I')




@dataclasses.dataclass
class RomSegmentHeader(CStruct):
    """
    Class for generating rom sub-section containing a segment header.

    Attributes
    ----------
    name : str
        Name of this sub-section.

    parent : str
        Parent, which is aggregating all rom sub-sections.
    """

    def __init__(self, name, parent: CStructParent):
        super().__init__(name, parent)

        # Flash offset of the segment content
        self.add_field('flash_offset', 'I')
        # Chip memory address where to load the segment
        self.add_field('mem_addr', 'I')
        # Size of the segment
        self.add_field('size', 'I')
        # Unused
        self.add_field('crc', 'I')



@dataclasses.dataclass
class RomSegment(CStruct):
    """
    Class for generating rom sub-section containing a segment.

    Attributes
    ----------
    name : str
        Name of this sub-section.

    parent : str
        Parent, which is aggregating all rom sub-sections.
    """

    def __init__(self, name, size, parent: CStructParent):
        super().__init__(name, parent)

        # Segment content
        self.add_field_array('data', size)



class RomFlashSection(FlashSection):
    """
    Class for generating a ROM section.

    Attributes
    ----------
    parent: gapylib.flash.Flash
        Name of the section.
    name : str
        Name of the section.
    section_id : int
        Id of the section.
    """

    def __init__(self, parent: Flash, name: str, section_id: int):
        super().__init__(parent, name, section_id)

        self.segment_headers = []
        self.segments = []
        self.binary = None
        self.top_struct = None
        self.header = None
        self.boot = False

        self.declare_property(name='binary', value=None,
            description="Executable to be loaded by the ROM."
        )

        self.declare_property(name='boot', value=False,
            description="True if the ROM will boot using this ROM section."
        )

    def set_content(self, offset: int, content_dict: dict):
        """Set the content of the section.

        Parameters
        ----------
        offset : int
            Starting offset of the section.
        content_dict : dict
            Content of the section
        """
        super().set_content(offset, content_dict)

        # First parse the ELF binary if any
        self.binary = self.__parse_binary()

        self.boot = self.get_property('boot')

        # Then declare the sub-sections so that the right offsets are computed

        # Top structure which will gather all sub-sections
        self.top_struct = CStructParent('rom', parent=self)

        if self.binary is not None:

            # Main header
            self.header = RomHeader('ROM header', parent=self.top_struct)

            binary_segments = self.binary.segments

            # One header per binary segment for describing it (size, etc)
            for __ in binary_segments:
                segment = RomSegmentHeader('Binary segment header', parent=self.top_struct)
                self.segment_headers.append(segment)

            # One header per binary segment
            for binary_segment in binary_segments:
                segment = RomSegment('Binary segment', binary_segment.size, parent=self.top_struct)
                self.segments.append(segment)

            # Now that the offsets have been computed, we can fill-in the various fields

            # Main header
            self.header.set_field('rom_header_size', self.get_size())
            self.header.set_field('nb_segments', len(self.binary.segments))
            self.header.set_field('entry', self.binary.entry)

            for i, binary_segment in enumerate(binary_segments):
                segment_header = self.segment_headers[i]
                segment = self.segments[i]

                # Per-segment header
                segment_header.set_field('flash_offset', segment.get_field('data').get_offset())
                segment_header.set_field('mem_addr', binary_segment.base)
                segment_header.set_field('size', binary_segment.size)
                block_size = 4096
                nb_blocks = int((binary_segment.size + block_size - 1) / block_size)
                segment_header.set_field('crc', nb_blocks)

                # Per segment content
                segment.set_field('data', binary_segment.data)

        else:
            # Case where the ROM is empty. In this case, we just have the ROM section size
            self.header = RomEmptyHeader('ROM header', parent=self.top_struct)

            self.header.set_field('rom_header_size', self.get_size())


    def __parse_binary(self):
        binary_path = self.get_property('binary')

        if binary_path is not None:
            try:
                with open(binary_path, "rb") as file_desc:
                    binary = Binary(file_desc)
            except OSError as esc:
                raise RuntimeError('Invalid rom binary, got error while opening content file: ' + \
                    str(esc)) from esc

        else:
            binary = None

        return binary


    def is_empty(self):
        # In auto-mode flash it only if it has a binary and we are booting from this flash
        return self.binary is None or not self.boot
