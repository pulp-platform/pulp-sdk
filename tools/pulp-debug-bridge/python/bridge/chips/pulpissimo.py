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
import time

JTAG_RISCV_IRLEN = 5
JTAG_RISCV_BYPASS = 0x1f

JTAG_SOC_CONFREG_ID = 6
JTAG_SOC_CONFREG = (JTAG_SOC_CONFREG_ID << 0) | (JTAG_RISCV_BYPASS << 4)
JTAG_SOC_CONFREG_WIDTH = 8 + 1
JTAG_SOC_IRLEN = 4

JTAG_IRLEN = JTAG_SOC_IRLEN + JTAG_RISCV_IRLEN

class pulpissimo_debug_bridge(debug_bridge):

    def __init__(self, config, binaries=[], verbose=False):

        super(pulpissimo_debug_bridge, self).__init__(config=config, binaries=binaries, verbose=verbose)

    def load_jtag(self, binaries):

        if self.verbose:
            print ('Loading binary through jtag')

        #if self.stop():
        #    return -1

        # Load the binary through jtag
        if self.verbose:
            print ("Loading binaries")
        for binary in binaries:
            if self.load_elf(binary=binary):
                return 1

        return 0


    def start(self):
        # First stall the core
        self.write_dmi(0x10, 0x00000001) # DMACTIVE

        self.write_dmi(0x10, 0x03E00001) # HART SEL
        self.write_dmi(0x10, 0x83E00001) # HALT REQ

        # Wait until it is halted
        while True:
            status = self.read_dmi(0x11)

            if ((status >> 9) & 1) == 1:
                break

        # Set PC
        self.write_dmi(0x04, 0x1c008080)         # PC into DATA0
        self.write_dmi(0x17, 0x00230000 | 0x7b1) # Abstract cmd to set DPC

        # Resume the core
        self.write_dmi(0x10, 0x43E00001)

        return 0

    def write_dmi(self, reg, value):
        self.write_reg_int(reg, value, 4, 1) # DMACTIVE



    def read_dmi(self, reg):
        return self.read_reg_int(reg, 4, 1) # DMACTIVE
