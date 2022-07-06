#
# Copyright (C) 2019 GreenWaves Technologies
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

from elftools.elf.elffile import ELFFile
import struct
import subprocess
import hashlib

from errors import FatalError
import traces

import os


def align(offset, alignment):
    return (offset + alignment - 1) & ~(alignment - 1)


class BlockBuffer(bytearray):
    def __init__(self, blockSize = None, *args, **kargs):
        self.blockSize = blockSize
        return super().__init__(self, *args, **kargs)
    
    def __add__(self, obj):
        print("add")
    
    def appendByte(self, value):
        self += struct.pack('B', value)
    
    def appendInt(self, value):
        self += struct.pack('I', value)
    
    def appendLongInt(self, value):
        self += struct.pack('Q', value)
    
    def pad(self, padsize):
        for i in range(0, padsize):
            self.appendByte(255)
    
    def padToOffset(self, offset):
        if (offset < len(self)):
            raise ValueError
        self.pad(offset - len(self))


class BinarySegment(object):
    def __init__(self, base, data):
        self.base = base
        self.data = data
        self.size = len(data)


class Binary(object):
    
    def __init__(self, blockSize = 0, elf = None, encrypt = False, aesKey = None, aesIv = None):
        self.blockSize = blockSize
        self.encrypt = encrypt
        self.aesKey = aesKey
        self.aesIv = aesIv
        
        self.bin = BlockBuffer(blockSize)
        self.elf = elf
        self.segments = []
        
        if elf:
            elffile = ELFFile(elf)
            self.entry = elffile['e_entry']
            
            for segment in elffile.iter_segments():
                if segment['p_type'] == 'PT_LOAD':
                    self.segments.append(BinarySegment(segment['p_paddr'], segment.data()))


class App(Binary):
    MAGIC = b"GApp"
    
    def __init__(self, elf, *args, **kargs):
        super().__init__(elf = elf, *args, **kargs)
    
    def get_crc(self, buff):
        return hashlib.md5(buff).digest()
    
    def __append(self, buffer, padToOffset = None):
        if self.encrypt:
            cmd = 'aes_encode %s %s' % (self.aesKey, self.aesIv)
            
            crc = self.get_crc(buffer)
            buffer += struct.pack('I', crc)
            
            p = subprocess.Popen(cmd, shell = True, stdout = subprocess.PIPE, stdin = subprocess.PIPE)
            out, err = p.communicate(buffer)
            if p.returncode != 0:
                raise Exception(
                    'Error when executing aes_encore to encrypt binary, probably this tool is not available')
            buffer = out
        
        if padToOffset != None:
            self.bin.padToOffset(padToOffset)
        
        self.bin += buffer
    
    def __dumpHeader(self):
        
        traces.info("Generating app header:")
        traces.info("  Nb areas: %d" % len(self.segments))
        
        # First compute areas flash information
        flashOffset = 0
        flashOffset += 4 + 4 + 4 + 4 + 16 * 4 * len(self.segments)
        
        flashOffset = align(flashOffset, 8)
        
        if self.encrypt:
            for segment in self.segments:
                segment.size += 4
        
        index = 0
        for segment in self.segments:
            segment.offset = flashOffset
            flashOffset += segment.size
            flashOffset = align(flashOffset, 8)
            traces.info("  Area %d: offset: 0x%x, base: 0x%x, size: 0x%x" % (
                index, segment.offset, segment.base, segment.size))
            index += 1
        
        flashOffset = align(flashOffset, 8)
        
        # Then write the header containing memory areas declaration
        header = BlockBuffer(blockSize = 8)
        header.appendInt(len(self.segments))
        header.appendInt(self.entry)

        for area in self.segments:
            header.appendInt(area.offset)
            header.appendInt(area.base)
            header.appendInt(area.size)
        
        crc = self.get_crc(header)
        
        self.__append(struct.pack('4s', App.MAGIC))
        self.__append(struct.pack('16s', crc))
        self.__append(header)
    
    def __dumpSegment(self):
        traces.info("Generating app data")
        
        for area in self.segments:
            self.__append(area.data, padToOffset = area.offset)
    
    def dump(self):
        self.__dumpHeader()
        self.__dumpSegment()
        return self.bin


class SSBL(Binary):
    def __init__(self, flashType, encrypt = False, aesKey = None, aesIv = None, *args, **kargs):

        if flashType == 'hyper':
            romBlockSize = 1024
            self.xip_dev = 0
        elif flashType == 'spi':
            if os.environ.get('TARGET_CHIP_FAMILY') in ['PULP', 'GAP8']:
                romBlockSize = 4096
            else:
                romBlockSize = 1024
            self.xip_dev = 1
        elif flashType == 'mram':
            romBlockSize = 1024
            self.xip_dev = 2
        else:
            raise FatalError(
                'Flash type %s not suported. ROM boot loader supports hyper and spi flash type' % flashType)

        self.encrypt = encrypt
        self.aesKey = aesKey
        self.aesIv = aesIv
       
        super().__init__(romBlockSize, encrypt=encrypt, aesKey=aesKey, aesIv=aesIv, *args, **kargs)
    
    def get_crc(self, buff):
        crc = 0xffffffff
        for data in buff:
            crc = crc ^ data
            for i in range(7, -1, -1):
                if crc & 1 == 1:
                    mask = 0xffffffff
                else:
                    mask = 0
                crc2 = crc >> 1
                crc = (crc >> 1) ^ (0xEDB88320 & mask)
        
        return (crc ^ 0xffffffff)
    
    def __append(self, buffer, padToOffset = None):
        if self.encrypt == True:
            traces.info("encrypt with key: %s Iv: %s" % (self.aesKey, self.aesIv))
            cmd = 'aes_encode %s %s' % (self.aesKey, self.aesIv)
            
            crc = self.get_crc(buffer)
            buffer += struct.pack('I', crc)
            
            p = subprocess.Popen(cmd, shell = True, stdout = subprocess.PIPE, stdin = subprocess.PIPE)
            out, err = p.communicate(buffer)
            if p.returncode != 0:
                raise Exception(
                    'Error when executing aes_encore to encrypt binary, probably this tool is not available')
            buffer = out
        
        if padToOffset != None:
            self.bin.padToOffset(padToOffset)
        
        self.bin += buffer
    
    def __dumpHeader(self):
        
        traces.info("Generating boot loader header:")
        traces.info("  Nb areas: %d, entry point 0x%X" % (len(self.segments), self.entry))
        
        # First compute areas flash information
        flashOffset = 0


        if os.environ.get('TARGET_CHIP_FAMILY') == 'GAP9':
            # flash_header_t is 13*int + 1024 bytes AC, 128 for KC
            if os.environ.get('GAP9_OLD_ROM') is not None:
                flashOffset += 4*16 + 16 * 4 * len(self.segments)
            else:
                flashOffset += 4*13 + 1024 + 128*2 + 16 * 4 * len(self.segments)
        else:
            flashOffset += 4 + 4 + 4 + 4 + 16 * 4 * len(self.segments)
        
        crcOffset = flashOffset
        flashOffset += 4
        
        flashOffset = align(flashOffset, self.blockSize)
        
        if self.encrypt:
            for segment in self.segments:
                segment.size += 4
        
        xip_flash_base = 0
        xip_flash_size = 0
        xip_page_size_cmd = 0  # Page size: 0=512B, 1=1KiB, 2=2KiB, ..., 7 = 64KiB
        xip_page_size = 512 << xip_page_size_cmd

        index = 0
        for segment in self.segments:
            if segment.base >= 0x20000000:
                flashOffset = (flashOffset + xip_page_size - 1) & ~(xip_page_size - 1)
                xip_flash_base = flashOffset
                xip_flash_size = segment.size

            segment.nbBlocks = int((segment.size + self.blockSize - 1) / self.blockSize)
            segment.offset = flashOffset
            flashOffset += segment.nbBlocks * self.blockSize
            traces.info("  Area %d: offset: 0x%x, base: 0x%x, size: 0x%x, nbBlocks: %d" % (
                index, segment.offset, segment.base, segment.size, segment.nbBlocks))
                
            index += 1
        
        flashOffset = align(flashOffset, 8)
        self.partitionTableOffset = flashOffset
        
        # Then write the header containing memory areas declaration
        header = BlockBuffer(blockSize = self.blockSize)
        header.appendInt(self.partitionTableOffset)
        header.appendInt(len(self.segments))
        header.appendInt(self.entry)
        # Legacy bootAddr ROM  field, it is no longer used
        header.appendInt(0x1c000000)
        
        xip_dev = self.xip_dev

        xip_vaddr = 0x20000000
        xip_l2_addr = 0x1c190000 - 16*xip_page_size
        xip_l2_nb_pages = 16
        flash_nb_pages = int((xip_flash_size + xip_page_size-1) / xip_page_size)

        if os.environ.get('TARGET_CHIP_FAMILY') == 'GAP9':
            # XIP config
            header.appendInt(xip_dev) # Device
            header.appendInt(xip_vaddr) # Virtual address
            header.appendInt(xip_page_size_cmd) # Page size: 1=1Kbyte
            header.appendInt(xip_flash_base) # Flash base
            header.appendInt(flash_nb_pages) # Flash nb pages
            header.appendInt(xip_l2_addr) # L2 address
            header.appendInt(xip_l2_nb_pages) # L2 nb pages
            header.appendInt(0) # kc_length
            header.appendInt(0) # key_length

            if os.environ.get('GAP9_OLD_ROM') is not None:
                header.appendInt(0) # secure boot
                header.appendInt(0) # for now
            else:
                for i in range(1024):
                    header.appendByte(0) # AC
                for i in range(256):
                    header.appendByte(0) # KC * 2

        for area in self.segments:
            header.appendInt(area.offset)
            header.appendInt(area.base)
            header.appendInt(area.size)
            header.appendInt(area.nbBlocks)
        
        header.padToOffset(crcOffset)
        crc = self.get_crc(header)
        header.appendInt(crc)
        
        self.__append(header)
    
    def __dumpSegment(self):
        traces.info("Generating boot loader data")
        
        for area in self.segments:
            self.__append(area.data, padToOffset = area.offset)
    
    def dump(self):
        if self.elf != None:
            self.__dumpHeader()
            self.__dumpSegment()
            self.bin.padToOffset(self.partitionTableOffset)
        else:
            # In case no boot binary is there, we must have at least the offset of the partition table
            ssbl_size = os.environ.get('GAP_SSBL_SIZE')
            if ssbl_size is None:
                self.partitionTableOffset = 8
            else:
                self.partitionTableOffset = int(ssbl_size)
            self.bin.appendLongInt(self.partitionTableOffset)
        return self.bin
