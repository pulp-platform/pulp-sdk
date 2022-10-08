"""Provides section template for hostfs, to copy host files to work folder."""

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
import shutil
from gapylib.flash import FlashSection, Flash




class HostfsSection(FlashSection):
    """
    Class for generating a hostfs section.

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

        self.declare_property(name='files', value=[],
            description="List of files to be included in the HostFS."
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
            abspath = self.get_flash().get_target().get_abspath(os.path.basename(file))
            shutil.copy(file, abspath)
