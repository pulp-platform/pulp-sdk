"""Provides section template for lfs, to generate a lfs image"""

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



import logging
import shlex
import subprocess
from gapylib.flash import FlashSection, Flash
from gapylib.utils import CStruct, CStructParent


class LfsHeader(CStruct):
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


class LfsSection(FlashSection):
    """
    Class for generating a lfs section.

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

        # Workstation root dir to be embedded in littlefs
        self.root_dir = None
        # Littlefs size in target
        self.size = 0

        self.declare_property(name='size', value=0,
            description="Size in bytes of the whole LittleFS."
        )

        self.declare_property(name='root_dir', value=None,
            description="Workstation directory content to be included in the LittleFS."
        )

    def set_content(self, offset: int, content_dict: dict):
        super().set_content(offset, content_dict)

        align = self.parent.get_flash_attribute('littlefs_align')
        self.set_offset((offset + align - 1) & ~(align - 1))

        # Top structure which will gather all sub-sections
        top_struct = CStructParent('readfs', parent=self)

        # Get LFS properties from flash and command-line
        image_path = self.parent.get_image_path() + '.' + self.get_name()
        self.root_dir = content_dict.get('properties').get('root_dir')
        self.size = content_dict.get('properties').get('size')

        # Size is a string to be converted if it comes from command-line
        if isinstance(self.size, str):
            self.size = int(self.size, 0)

        block_size = self.parent.get_flash_attribute('littlefs_block_size')

        # Only initialize the FS if its size is not 0, otherwise keep it as an empty section
        if self.size > 0:
            header = LfsHeader('header', parent=top_struct, size=self.size)

            # Initialize the FS only if has a content since mklfs requires it. Otherwise, keep
            # it as non-empty blank section, so that it can still be formatted at runtime
            if self.root_dir is not None:
                # Use mklfs to format the section with the specified folder
                cmd = f'mklfs -b {block_size} -r 16 -p 16 -s {self.size} -i {image_path}'

                cmd += f' -c {self.root_dir}'

                logging.debug('Generating LittleFS images with command:')
                logging.debug('  %s', cmd)

                proc = subprocess.run(shlex.split(cmd), stdout=subprocess.PIPE)

                for line in proc.stdout.splitlines():
                    logging.debug('mklfs: %s', line.decode("utf-8"))

                if proc.returncode != 0:
                    raise RuntimeError('Failed to create LFS image file')

                # Mklfs will dump it to a file, redump it to the flash image
                with open(image_path, 'rb') as file_desc:
                    header.set_field('data', file_desc.read())


    def is_empty(self):
        # To avoid uploading flash content for nothing as soon as there is a lfs section in the
        # flash, we upload it only if it contains something
        return self.root_dir is None

    def get_partition_type(self) -> int:
        return 0x82
