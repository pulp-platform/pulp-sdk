"""Provides section template for readfs, to generate a readfs image"""

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



import os.path
import dataclasses
from gapylib.flash import FlashSection, Flash
from gapylib.utils import CStruct, CStructParent



@dataclasses.dataclass
class ReadfsHeader(CStruct):
    """
    Class for generating readfs sub-section containing the main header.
    This header contains global information like number of files.

    Attributes
    ----------
    name : str
        Name of this sub-section.

    parent : str
        Parent, which is aggregating all readfs sub-sections.
    """
    def __init__(self, name: str, parent: CStructParent):
        super().__init__(name, parent)

        # Size of the readfs section. Can be used to get the next section.
        # The size is on 8 bytes due to an old constraint on vega
        # This could be removed once we have switched to new gapy so that we can also adapt it
        # in the BSP.
        self.add_field('fs_size', 'Q')
        # Number of files in the readfs
        self.add_field('nb_files', 'I')


@dataclasses.dataclass
class ReadfsFileHeader(CStruct):
    """
    Class for generating readfs sub-section containing a file header.
    This header contains information about the file like its name and size.

    Attributes
    ----------
    name : str
        Name of this sub-section.

    name_len : int
        Len of the file name, including the terminating 0.

    parent : str
        Parent, which is aggregating all readfs sub-sections.
    """

    def __init__(self, name: str, name_len: int, parent: CStructParent):
        super().__init__(name, parent)

        # Offset in the flash where the file content is
        self.add_field('offset', 'I')
        # Size of the file
        self.add_field('file_size', 'I')
        # Size of its name including the terminating 0.
        self.add_field('name_len', 'I')
        # Name of the file including the terminating 0.
        self.add_field_array('name', name_len)



@dataclasses.dataclass
class ReadfsFile(CStruct):
    """
    Class for generating readfs sub-section containing a file content.

    Attributes
    ----------
    name : str
        Name of this sub-section.

    size : int
        File size.

    parent : str
        Parent, which is aggregating all readfs sub-sections.
    """

    def __init__(self, name: str, size: int, parent: CStructParent):
        super().__init__(name, parent)

        # File content
        self.add_field_array('data', size)



class ReadfsSection(FlashSection):
    """
    Class for generating a readfs section.

    Attributes
    ----------
    parent: gapylib.flash.Flash
        Name of the section.
    name : str
        Name of the section.
    section_id : int
        Id of the section.
    """

    def __init__(self, parent: Flash, name, section_id: int):
        super().__init__(parent, name, section_id)

        self.file_paths = []
        self.file_headers = []
        self.files = []
        self.top_struct = None
        self.header = None

        self.declare_property(name='files', value=[],
            description="List of files to be included in the ReadFS."
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

        # Get the list of files from the properties and determine basenames from the path, which
        # will be used as name in the readfs
        for file in content_dict.get('properties').get('files'):
            self.file_paths.append([os.path.basename(file), file])

        # First declare the sub-sections so that the right offsets are computed

        # Top structure which will gather all sub-sections
        self.top_struct = CStructParent('readfs', parent=self)

        # Main header for readfs size and number of files
        self.header = ReadfsHeader('header', parent=self.top_struct)

        # One header per file containig file size, name and flash offset
        for i, path in enumerate(self.file_paths):
            filename, filepath = path
            self.file_headers.append(ReadfsFileHeader(f'file{i} header', len(filename)+1,
                parent=self.top_struct))

        # File contents
        for i, path in enumerate(self.file_paths):
            filename, filepath = path
            self.files.append(ReadfsFile(f'file{i}', os.path.getsize(filepath),
                parent=self.top_struct))



        # Now that the offsets have been computed, we can fill-in the various fields

        # Main header
        header_size = self.header.get_size()
        for file_header in self.file_headers:
            header_size += file_header.get_size()

        self.header.set_field('fs_size', header_size)
        self.header.set_field('nb_files', len(self.files))

        for i, path in enumerate(self.file_paths):
            filename, filepath = self.file_paths[i]
            file_header = self.file_headers[i]
            file = self.files[i]

            # Per-file header
            file_header.set_field('offset', file.get_offset() - self.get_offset())
            file_header.set_field('file_size', os.path.getsize(filepath))
            file_header.set_field('name_len', len(filename)+1)
            file_header.set_field('name', filename.encode('utf-8') + bytes([0]))

            # Per-file content
            with open(filepath, 'rb') as file_desc:
                file.set_field('data', file_desc.read())


    def is_empty(self) -> bool:
        return len(self.file_paths) == 0

    def get_partition_type(self) -> int:
        return 0x1

    def get_partition_subtype(self) -> int:
        return 0x81
