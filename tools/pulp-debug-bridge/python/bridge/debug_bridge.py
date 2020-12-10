#
# Copyright (C) 2018 ETH Zurich and University of Bologna
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Authors: Germain Haugou, ETH (germain.haugou@iis.ee.ethz.ch)

from bridge.default_debug_bridge import *
import bridge.chips.gap as gap
import bridge.chips.gap_rev1 as gap_rev1
import bridge.chips.gap8_revc as gap8_revc
import bridge.chips.wolfe as wolfe
import bridge.chips.usoc_v1 as usoc_v1
import bridge.chips.vega as vega
import bridge.chips.arnold as arnold
import bridge.chips.fulmine as fulmine
import bridge.chips.pulpissimo as pulpissimo
import bridge.chips.pulp as pulp
import bridge.chips.vivosoc3 as vivosoc3


def get_bridge(config, binaries=[], verbose=False):

    chip_config = config.get('**/board/chip')
    if chip_config is None:
        raise Exception('Wrong JSON configuration, do not contain any chip information')

    chip = config.get('**/board/chip').get('name').get()

    if chip == 'gap':
        bridge_class = gap.gap_debug_bridge
    elif chip == 'gap_rev1':
        bridge_class = gap_rev1.gap_debug_bridge
    elif chip == 'gap8_revc':
        bridge_class = gap8_revc.gap_debug_bridge
    elif chip == 'fulmine' or chip == 'vivosoc2' or chip == 'vivosoc2_1':
        bridge_class = fulmine.fulmine_debug_bridge
    elif chip == 'wolfe':
        bridge_class = wolfe.wolfe_debug_bridge
    elif chip == 'usoc_v1':
        bridge_class = usoc_v1.usoc_v1_debug_bridge
    elif chip == 'vega':
        bridge_class = vega.vega_debug_bridge
    elif chip == 'arnold':
        bridge_class = arnold.arnold_debug_bridge
    elif chip == 'pulp':
        bridge_class = pulp.pulp_debug_bridge
    elif chip == 'pulpissimo':
        bridge_class = pulpissimo.pulpissimo_debug_bridge
    elif chip == 'vivosoc3':
        bridge_class = vivosoc3.vivosoc3_debug_bridge
    else:
        bridge_class = debug_bridge

    return bridge_class(config=config, binaries=binaries, verbose=verbose)
