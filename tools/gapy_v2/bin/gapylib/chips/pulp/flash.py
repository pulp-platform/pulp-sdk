"""Default flash description for all gap targets"""

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


import json
from collections import OrderedDict
from gapylib.target import Target
from gapylib.flash import Flash
from gapylib.fs.littlefs import LfsSection
from gapylib.fs.readfs import ReadfsSection
from gapylib.fs.hostfs import HostfsSection
from gapylib.fs.raw import RawSection
from gapylib.chips.pulp.rom_v2 import RomFlashSection
from gapylib.fs.partition import PartitionTableSection


class DefaultFlashRomV2(Flash):
    """
    Default class for all flash for gap targets.
    Mostly describes the allowed section templates (rom and all FS).

    Attributes
    ----------
    target : gapylib.target.Target
        Target containing the flash.
    name : str
        Name of the flash
    size : int
        Size of the flash
    """

    def __init__(self, target: Target, name: str, size: int, *kargs, **kwargs):
        super().__init__(target, name, size, *kargs, **kwargs)

        # Declare all the available flash section
        self.register_section_template('rom', RomFlashSection)
        self.register_section_template('partition table', PartitionTableSection)
        self.register_section_template('readfs', ReadfsSection)
        self.register_section_template('hostfs', HostfsSection)
        self.register_section_template('lfs', LfsSection)
        self.register_section_template('raw', RawSection)

        # And give the default layout
        content_file = 'gapylib/chips/pulp/default_flash_content.json'
        content_path = target.get_file_path(content_file)

        if content_path is None:
            raise RuntimeError('Could not find flash property file: ' + content_file)

        try:
            with open(content_path, 'rb') as file_desc:
                self.set_content(json.load(file_desc, object_pairs_hook=OrderedDict))
        except OSError as exc:
            raise RuntimeError('Unable to open flash content file: ' + str(exc)) from exc
