"""Provides section template for flash partitions"""

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


import dataclasses
from gapylib.flash import FlashSection
from gapylib.utils import CStruct, CStructParent



@dataclasses.dataclass
class PartitionTableHeader(CStruct):
    """
    Class for generating partition table sub-section containing the main header.
    This header contains global information like number of partitions.

    Attributes
    ----------
    name : str
        Name of this sub-section.

    parent : str
        Parent, which is aggregating all readfs sub-sections.
    """
    def __init__(self, name: str, parent: CStructParent):
        super().__init__(name, parent)

        # Magic number
        self.add_field('magic_number', 'H')
        # Partition table version
        self.add_field('partition_table_version', 'B')
        # Number of sections
        self.add_field('nb_entries', 'B')
        # 1 if MD5 sum of sections is enabled
        self.add_field('crc', 'B')
        # Unused, just for padding
        self.add_field_array('padding', 11)
        # MD5 sum of all the sections
        self.add_field_array('md5', 16)



@dataclasses.dataclass
class PartitionTableSectionHeader(CStruct):
    """
    Class for generating partition table sub-section containing a section header.
    This header contains section information like size.

    Attributes
    ----------
    name : str
        Name of this sub-section.

    parent : str
        Parent, which is aggregating all readfs sub-sections.
    """
    def __init__(self, name: str, parent: CStructParent):
        super().__init__(name, parent)

        # Magic number
        self.add_field('magic_number', 'H')
        # Partition type
        self.add_field('type', 'B')
        # Partition subtype
        self.add_field('subtype', 'B')
        # Offset of the partition in the flash
        self.add_field('offset', 'I')
        # Size of the partition
        self.add_field('size', 'I')
        # Name of the partition
        self.add_field_array('name', 16)
        # Flags of the partition
        self.add_field('flags', 'I')



class PartitionTableSection(FlashSection):
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

    def __init__(self, parent, name: str, section_id: int):
        super().__init__(parent, name, section_id)

        self.section_headers = []
        self.top_struct = None
        self.header = None
        self.sections = None


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

        # Get all the sections after the partition table to include them in the table
        self.sections = self.get_flash().get_sections()[self.get_id()+1:]

        # First declare the sub-sections so that the right offsets are computed

        # Top structure which will gather all sub-sections
        self.top_struct = CStructParent('partition table', parent=self)

        # Main header for readfs size and number of files
        self.header = PartitionTableHeader('header', parent=self.top_struct)

        # One header per section
        for i, __ in enumerate(self.sections):
            self.section_headers.append(
                PartitionTableSectionHeader(f'section{i} header', parent=self.top_struct))



    def finalize(self):
        """Finalize the section.

        This can be called to set internal section fields which requires some knowledge of the
        offset or size of other sections.
        The structure of the section should not be changed in this step
        """
        super().finalize()

        # Now that the offsets have been computed, we can fill-in the various fields

        # Get all the sections after the partition table to include them in the table
        sections = self.get_flash().get_sections()[self.get_id()+1:]

        # Main header
        self.header.set_field('magic_number', 0x02BA)
        self.header.set_field('partition_table_version', 1)
        self.header.set_field('nb_entries', len(sections))

        # Per-section header
        for i, section in enumerate(self.section_headers):
            section.set_field('magic_number', 0x01BA)
            section.set_field('type', 1)
            section.set_field('subtype', sections[i].get_partition_type())
            section.set_field('offset', sections[i].get_offset())
            section.set_field('size', sections[i].get_size())
            section.set_field('name', sections[i].get_name().encode('utf-8') + bytes([0]))

    def is_empty(self):

        # Partition table is considered empty in auto mode if all the partitions are considered
        # empty.
        for section in self.sections:
            if not section.is_empty():
                return False

        return True
