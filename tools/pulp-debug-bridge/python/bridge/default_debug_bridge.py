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

import ctypes
import os
import os.path
import json_tools as js
from elftools.elf.elffile import ELFFile
import time


class Ctype_cable(object):

    def __init__(self, module, config, system_config):

        self.module = module
        self.gdb_handle = None

        # Register entry points with appropriate arguments
        self.module.cable_new.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        self.module.cable_new.restype = ctypes.c_void_p

        self.module.cable_write.argtypes = \
            [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_char_p]

        self.module.cable_read.argtypes = \
            [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_char_p]

        self.module.cable_reg_write.argtypes = \
            [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_int]

        self.module.cable_reg_read.argtypes = \
            [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p]

        self.module.chip_reset.argtypes = \
            [ctypes.c_void_p, ctypes.c_bool, ctypes.c_int]

        self.module.chip_config.argtypes = \
            [ctypes.c_void_p, ctypes.c_int]
            
        self.module.jtag_reset.argtypes = \
            [ctypes.c_void_p, ctypes.c_bool]
            
        self.module.jtag_soft_reset.argtypes = \
            [ctypes.c_void_p]
            
        self.module.cable_jtag_set_reg.argtypes = \
            [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_uint, ctypes.c_int]
        self.module.cable_jtag_set_reg.restype = ctypes.c_bool

        self.module.cable_jtag_get_reg.argtypes = \
            [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int), ctypes.c_int]
        self.module.cable_jtag_get_reg.restype = ctypes.c_bool

        self.module.cable_lock.argtypes = \
            [ctypes.c_void_p]
            
        self.module.cable_unlock.argtypes = \
            [ctypes.c_void_p]
            
        self.module.bridge_get_error.argtypes = []
        self.module.bridge_get_error.restype = ctypes.c_char_p
            
        self.module.bridge_init.argtypes = [ctypes.c_char_p, ctypes.c_int]

        self.module.gdb_server_open.argtypes = [ctypes.c_void_p, ctypes.c_int]
        self.module.gdb_server_open.restype = ctypes.c_void_p

        self.module.gdb_server_close.argtypes = [ctypes.c_void_p, ctypes.c_int]

        config_string = None

        if config is not None:
            config_string = config.dump_to_string().encode('utf-8')

        self.instance = self.module.cable_new(config_string, system_config.dump_to_string().encode('utf-8'))

        if self.instance == None:
            raise Exception('Failed to initialize cable with error: ' + self.module.bridge_get_error().decode('utf-8'))

    def get_instance(self):
        return self.instance

    def write(self, addr, size, buffer):
        data = (ctypes.c_char * size).from_buffer(bytearray(buffer))
        self.module.cable_write(self.instance, addr, size, data)

    def reg_write(self, addr, size, buffer, device=-1):
        data = (ctypes.c_char * size).from_buffer(bytearray(buffer))
        self.module.cable_reg_write(self.instance, addr, data, device)

    def read(self, addr, size):
        data = (ctypes.c_char * size)()
        self.module.cable_read(self.instance, addr, size, data)

        result = []
        for elem in data:
            result.append(elem)

        return result

    def reg_read(self, addr, size, device=-1):
        data = (ctypes.c_char * size)()
        self.module.cable_reg_read(self.instance, addr, data, device)

        result = []
        for elem in data:
            result.append(elem)

        return result

    def chip_reset(self, value, duration=1000000):
        self.module.chip_reset(self.instance, value, duration)

    def chip_config(self, value):
        self.module.chip_config(self.instance, value)

    def jtag_reset(self, value):
        self.module.jtag_reset(self.instance, value)

    def jtag_soft_reset(self):
        self.module.jtag_soft_reset(self.instance)

    def jtag_set_reg(self, reg, width, value, ir_len=-1):
        self.module.cable_jtag_set_reg(self.instance, reg, width, value, ir_len)

    def jtag_get_reg(self, reg, width, value, ir_len=-1):
        out_value = ctypes.c_int()
        self.module.cable_jtag_get_reg(self.instance, reg, width, ctypes.byref(out_value), value, ir_len)
        return out_value.value

    def lock(self):
        self.module.cable_lock(self.instance)

    def unlock(self):
        self.module.cable_unlock(self.instance)




class debug_bridge(object):

    def __init__(self, config, binaries=[], verbose=False):
        self.config = config
        self.cable = None
        self.cable_name = config.get_child_str('**/debug_bridge/cable/type')
        if self.cable_name is None:
            self.cable_name = 'ftdi'
        self.binaries = binaries
        self.reqloop_handle = None
        self.verbose = verbose
        self.gdb_handle = None
        self.cable_config = config.get('**/debug_bridge/cable')



        # Load the library which provides generic services through
        # python / C++ bindings
        lib_path=os.path.join('libpulpdebugbridge.so')
        self.module = ctypes.CDLL(lib_path)

        self.module.bridge_reqloop_open.argtypes = [ctypes.c_void_p, ctypes.c_uint]
        self.module.bridge_reqloop_open.restype = ctypes.c_void_p
        
        self.module.bridge_reqloop_buffer_alloc.argtypes = [ctypes.c_void_p, ctypes.c_int]
        self.module.bridge_reqloop_buffer_alloc.restype = ctypes.c_uint
        
        self.module.bridge_reqloop_buffer_free.argtypes = [ctypes.c_void_p, ctypes.c_uint, ctypes.c_int]
        
        self.module.bridge_reqloop_efuse_access.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_uint, ctypes.c_uint]
        
        self.module.bridge_reqloop_eeprom_access.argtypes = [ctypes.c_void_p, ctypes.c_uint, ctypes.c_uint, ctypes.c_int, ctypes.c_uint, ctypes.c_uint, ctypes.c_uint]
        self.module.bridge_reqloop_eeprom_access.restype = ctypes.c_int
        
        self.module.bridge_reqloop_flash_access.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_uint, ctypes.c_uint, ctypes.c_int, ctypes.c_uint, ctypes.c_uint, ctypes.c_uint]
        self.module.bridge_reqloop_flash_access.restype = ctypes.c_int
        
        self.module.bridge_reqloop_flash_erase.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_uint, ctypes.c_uint, ctypes.c_uint, ctypes.c_int]
        self.module.bridge_reqloop_flash_erase.restype = ctypes.c_int
        
        self.module.bridge_reqloop_flash_erase_sector.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_uint, ctypes.c_uint, ctypes.c_uint]
        self.module.bridge_reqloop_flash_erase_sector.restype = ctypes.c_int
        
        self.module.bridge_reqloop_flash_erase_chip.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_uint, ctypes.c_uint]
        self.module.bridge_reqloop_flash_erase_chip.restype = ctypes.c_int
        
        self.module.bridge_reqloop_close.argtypes = [ctypes.c_void_p, ctypes.c_int]

        self.module.bridge_init(config.dump_to_string().encode('utf-8'), verbose)

        #self.module.jtag_shift.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.POINTER(ctypes.c_char_p), ctypes.POINTER(ctypes.c_char_p)]

    def __mount_cable(self):
        if self.cable_name is None:
            raise Exception("Trying to mount cable while no cable was specified")

        if self.cable_name.split('@')[0] in ['ftdi', 'jtag-proxy']:
            self.__mount_ctype_cable()
            pass
        else:
            raise Exception('Unknown cable: ' + self.cable_name)

    def __mount_ctype_cable(self):

        self.cable = Ctype_cable(
            module = self.module,
            config = self.cable_config,
            system_config = self.config
        )

    def get_cable(self):
        if self.cable is None:
            self.__mount_cable()

        return self.cable

    def load_jtag(self, binaries):
        return self.load_default(binaries)

    def load_jtag_hyper(self, binaries):
        raise Exception('JTAG boot is not supported on this target')

    def load_elf(self, binary):
        if self.verbose:
            print ('Loading %s' % binary)

        with open(binary, 'rb') as file:
            elffile = ELFFile(file)

            for segment in elffile.iter_segments():

                if segment['p_type'] == 'PT_LOAD':

                    data = segment.data()
                    addr = segment['p_paddr']
                    size = len(data)

                    if self.verbose:
                        print ('Loading section (base: 0x%x, size: 0x%x)' % (addr, size))

                    self.write(addr, size, data)

                    if segment['p_filesz'] < segment['p_memsz']:
                        addr = segment['p_paddr'] + segment['p_filesz']
                        size = segment['p_memsz'] - segment['p_filesz']
                        print ('Init section to 0 (base: 0x%x, size: 0x%x)' % (addr, size))
                        self.write(
                            addr,
                            size,
                            [0] * size
                        )


            set_pc_addr_config = self.config.get('**/debug_bridge/set_pc_addr')

            if set_pc_addr_config is not None:
                set_pc_offset_config = self.config.get('**/debug_bridge/set_pc_offset')
                entry = elffile.header['e_entry']

                if set_pc_offset_config is not None:
                    entry += set_pc_offset_config.get_int()

                if self.verbose:
                    print ('Setting PC (base: 0x%x, value: 0x%x)' % (set_pc_addr_config.get_int(), entry))

                return self.write_32(set_pc_addr_config.get_int(), entry)

        return 0

    def load(self, binaries=None):
        if binaries is None:
            binaries = self.binaries

        mode = self.config.get_child_str('**/debug_bridge/boot-mode')
        if mode is None:
            mode = 'jtag'

        if mode == 'jtag':
            return self.load_jtag(binaries)
        elif mode == 'jtag_hyper':
            return self.load_jtag_hyper(binaries)
        else:
            return self.load_default(binaries)

    def load_default(self, binaries):
        for binary in binaries:
            if self.load_elf(binary=binary):
                return 1

        return 0

    def start(self):
        start_addr_config = self.config.get('**/debug_bridge/start_addr')
        if start_addr_config is not None:
            if self.verbose:
                print ('Starting (base: 0x%x, value: 0x%x)' % (start_addr_config.get_int(), self.config.get('**/debug_bridge/start_value').get_int()))

            self.write_32(start_addr_config.get_int(), self.config.get('**/debug_bridge/start_value').get_int())
        return 0

    def stop(self):
        stop_addr_config = self.config.get('**/debug_bridge/stop_addr')
        if stop_addr_config is not None:
            self.write_32(stop_addr_config.get_int(), self.config.get('**/debug_bridge/stop_value').get_int()) != 0
        return 0

    def read(self, addr, size):
        return self.get_cable().read(addr, size)

    def write(self, addr, size, buffer):
        return self.get_cable().write(addr, size, buffer)

    def write_int(self, addr, value, size):
        return self.write(addr, size, value.to_bytes(size, byteorder='little'))

    def write_reg_int(self, addr, value, size, device=-1):
        return self.get_cable().reg_write(addr, size, value.to_bytes(size, byteorder='little'), device)

    def write_32(self, addr, value):
        return self.write_int(addr, value, 4)

    def write_16(self, addr, value):
        return self.write_int(addr, value, 2)

    def write_8(self, addr, value):
        return self.write_int(addr, value, 1)

    def read_int(self, addr, size):
        byte_array = None
        for byte in self.read(addr, size):
            if byte_array == None:
                byte_array = byte
            else:
                byte_array += byte
        return int.from_bytes(byte_array, byteorder='little')

    def read_reg_int(self, addr, size, device=-1):
        byte_array = None
        for byte in self.get_cable().reg_read(addr, size, device):
            if byte_array == None:
                byte_array = byte
            else:
                byte_array += byte
        return int.from_bytes(byte_array, byteorder='little')

    def read_32(self, addr):
        return self.read_int(addr, 4)

    def read_16(self, addr):
        return self.read_int(addr, 2)

    def read_8(self, addr):
        return self.read_int(addr, 1)

    def _get_binary_symbol_addr(self, name, binaries=[]):

        binaries = binaries + self.binaries

        for binary in binaries:
            with open(binary, 'rb') as file:
                elf = ELFFile(file)
                for section in elf.iter_sections():
                    if section.header['sh_type'] == 'SHT_SYMTAB':
                        for symbol in section.iter_symbols():
                            if symbol.name == name:
                                t_section=symbol.entry['st_shndx']
                                t_vaddr=symbol.entry['st_value']
                                return t_vaddr
        return 0

    def reset(self):
        self.get_cable().jtag_reset(True)
        self.get_cable().jtag_reset(False)
        self.get_cable().chip_reset(True)
        self.get_cable().chip_reset(False)
        return 0

    def ioloop(self):

        return 0

    def reqloop(self, binaries=[]):

        # First get address of the structure used to communicate between
        # the bridge and the runtime
        addr = self._get_binary_symbol_addr('__rt_debug_struct_ptr', binaries)
        if addr == 0:
            addr = self._get_binary_symbol_addr('debugStruct_ptr', binaries)

        self.reqloop_handle = self.module.bridge_reqloop_open(
            self.get_cable().get_instance(), addr)

        return 0

    def reqloop_close(self, force=False):
        if self.reqloop_handle is not None:
            return self.module.bridge_reqloop_close(self.reqloop_handle, force)
        return 0


    def __flasher_init(self, flasher_init):
        chip = self.config.get('**/board/chip').get('name').get()
        flasher_name = 'flasher-%s' % chip
        flasher_path = os.path.join(os.environ.get('PULP_SDK_INSTALL'), 'bin', flasher_name)

        if flasher_init:
            self.stop()
            # TODO this breaks boot test using bridge, why this is needed ?
            self.load([flasher_path])

        self.reqloop([flasher_path])

        if flasher_init:
            self.start()



    def __flasher_deinit(self):
        self.reqloop_close(force=True)



    def efuse_access(self, flasher_init, is_write, index, value, mask):

        self.__flasher_init(flasher_init)

        print ('efuse access')
        self.module.bridge_reqloop_efuse_access(self.reqloop_handle, is_write, index, value, mask)
        print ('efuse access done')

        self.__flasher_deinit()



    def __alloc_buffer(self, size):

        return self.module.bridge_reqloop_buffer_alloc(self.reqloop_handle, size)



    def eeprom_access(self, flasher_init, itf, cs, is_write,eeprom_addr, filepath):
        self.__flasher_init(flasher_init)

        addr = self.__alloc_buffer(1024)

        with open(filepath, 'rb') as file:
            while True:
                buff = file.read(1024)
                if buff:
                    self.write(addr, len(buff), buff)
                    if self.module.bridge_reqloop_eeprom_access(self.reqloop_handle, itf, cs, True, eeprom_addr, addr, len(buff)):
                        return -1
                    eeprom_addr += 1024
                else:
                    break

        self.__flasher_deinit()

        return 0



    def flash_access(self, flasher_init, type, itf, cs, is_write,flash_addr, size, filepath):
        self.__flasher_init(flasher_init)

        addr = self.__alloc_buffer(1024)

        if is_write:
            with open(filepath, 'rb') as file:
                while True:
                    buff = file.read(1024)
                    if buff:
                        self.write(addr, len(buff), buff)

                        buff_size = len(buff)
                        buff_size = (buff_size + 7) & ~0x7

                        if self.module.bridge_reqloop_flash_access(self.reqloop_handle, type, itf, cs, True, flash_addr, addr, buff_size):
                            return -1
                        flash_addr += 1024
                    else:
                        break
        else:
            with open(filepath, 'wb') as file:
                while size > 0:
                    iter_size = 1024
                    if iter_size > size:
                        iter_size = size
                    if self.module.bridge_reqloop_flash_access(self.reqloop_handle, type, itf, cs, False, flash_addr, addr, iter_size):
                            return -1
                    buff = self.read(addr, iter_size)
                    for elem in buff:
                        file.write(elem)
                    size -= iter_size
                    flash_addr += iter_size

        self.__flasher_deinit()

        return 0




    def flash_erase_sector(self, flasher_init, type, itf, cs, flash_addr):
        self.__flasher_init(flasher_init)

        if self.module.bridge_reqloop_flash_erase_sector(self.reqloop_handle, type, itf, cs, flash_addr):
            return -1

        self.__flasher_deinit()

        return 0




    def flash_erase_chip(self, flasher_init, type, itf, cs):
        self.__flasher_init(flasher_init)

        if self.module.bridge_reqloop_flash_erase_chip(self.reqloop_handle, type, itf, cs):
            return -1

        self.__flasher_deinit()

        return 0


    def flash_erase(self, flasher_init, type, itf, cs, flash_addr, size):
        self.__flasher_init(flasher_init)

        if self.module.bridge_reqloop_flash_erase(self.reqloop_handle, type, itf, cs, flash_addr, size):
            return -1

        self.__flasher_deinit()

        return 0


    def flash(self):
        raise Exception('Flash is not supported on this target')

    def gdb(self, port):
        self.gdb_handle = self.module.gdb_server_open(self.get_cable().get_instance(), port)
        return 0

    def wait(self):
        if self.gdb_handle is not None:
            self.module.gdb_server_close(self.gdb_handle, 0)

        # The wait function returns in case reqloop has been launched
        # as it will check for end of application.
        if self.reqloop_handle is not None:
            return self.module.bridge_reqloop_close(self.reqloop_handle, 0)

        return 0

    def lock(self):
        self.get_cable().lock()

    def unlock(self):
        self.get_cable().unlock()
