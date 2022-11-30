"""Provides section template for raw partition."""

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



from gapylib.flash import FlashSection, Flash
from gapylib.utils import CStruct, CStructParent



class RawHeader(CStruct):
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
    def __init__(self, name: str, parent: CStructParent, size: int):
        super().__init__(name, parent)

        self.add_field_array('data', size)



class RawSection(FlashSection):
    """
    Class for generating a raw section.

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

        # Just add a structure with an array to occupy the specified space
        top_struct = CStructParent('readfs', parent=self)

        size = self.get_property('size')

        # Size is a string to be converted if it comes from command-line
        if isinstance(size, str):
            size = int(size, 0)

        # If size is -1, just take the rest of the flash
        if size == -1:
            size = self.parent.get_size() - self.get_offset()

        RawHeader('header', parent=top_struct, size=size)

    def get_partition_type(self) -> int:
        return 0x1

    def get_partition_subtype(self) -> int:
        return 0x80
