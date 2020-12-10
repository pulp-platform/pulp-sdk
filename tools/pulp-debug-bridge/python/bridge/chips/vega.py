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
JTAG_SOC_CONFREG = (JTAG_SOC_CONFREG_ID << 5) | (JTAG_RISCV_BYPASS << 0)
JTAG_SOC_CONFREG_WIDTH = 8 + 1
JTAG_SOC_IRLEN = 4

JTAG_IRLEN = JTAG_SOC_IRLEN + JTAG_RISCV_IRLEN

class vega_debug_bridge(debug_bridge):

    def __init__(self, config, binaries=[], verbose=False):

        super(vega_debug_bridge, self).__init__(config=config, binaries=binaries, verbose=verbose)

        self.start_cores = False
        self.first_reset = True
        self.boot_mode = None


    def reset(self, stop=True):

        


        if self.first_reset:
            # The first time, we need to wait enough time to let the voltage
            # regulator converge
            self.get_cable().chip_reset(True, 5000000)
            self.first_reset = False

        # Reset the chip and tell him we want to load via jtag
        # We keep the reset active until the end so that it sees
        # the boot mode as soon as it boots from rom

        # Use bootsel pad to tell boot code to stop
        if stop:
            self.get_cable().chip_config(1)

        # Due to voltage convergence and so on we need to wait
        # 200ms when the reset is low
        #self.get_cable().chip_reset(True, 200000000)
        self.get_cable().chip_reset(True, 100000000)
        # It also takes some time before the JTAG is ready
        self.get_cable().chip_reset(False, 4000000)

        #self.get_cable().jtag_reset(True)
        self.get_cable().jtag_reset(False)

        return 0


    def wait_eoc(self):

        while True:
            value = self.read_32(0x1a1040a0)

            if (value >> 31) == 1:
                return value & 0x7fffffff

            time.sleep(0.1)


    def jtag_hyper_boot(self):

        self.get_cable().jtag_set_reg(JTAG_SOC_CONFREG, JTAG_SOC_CONFREG_WIDTH, ((((2 << 0) | (1<<3)) << 1) | 1) << 1, JTAG_IRLEN)


    def jtag_mram_boot(self):

        self.get_cable().jtag_set_reg(JTAG_SOC_CONFREG, JTAG_SOC_CONFREG_WIDTH, ((((2 << 0) | (2<<3)) << 1) | 1) << 1, JTAG_IRLEN)


    def jtag_spim_boot(self):

        self.get_cable().jtag_set_reg(JTAG_SOC_CONFREG, JTAG_SOC_CONFREG_WIDTH, ((((2 << 0) | (0<<3)) << 1) | 1) << 1, JTAG_IRLEN)


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



    def clear(self):
        self.get_cable().chip_config(0)



    def wait_available(self):
        
        boot_mode = 0
        if self.boot_mode is not None:
            boot_mode = (self.boot_mode << 1) | 1

        # Loop until we see bit 0 becoming 1, this will indicate that the
        # target is ready to accept bridge requests
        while True:
            reg_value = self.get_cable().jtag_get_reg(JTAG_SOC_CONFREG, JTAG_SOC_CONFREG_WIDTH, boot_mode, JTAG_IRLEN) >> 1

            rt_req = (reg_value >> 1) & 0x7

            if rt_req == 4 or rt_req == 1:
                break

        if self.verbose:
            print ("Target is available")



    def write_dmi(self, reg, value):
        self.write_reg_int(reg, value, 4, 0) # DMACTIVE



    def read_dmi(self, reg):
        return self.read_reg_int(reg, 4, 0) # DMACTIVE



    def stop(self):

        return 0