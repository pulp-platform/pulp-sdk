"""Provides utility classes"""

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


from collections import OrderedDict
import struct
from prettytable import PrettyTable

from gapylib.flash import FlashSection



class CStructField():
    """
    Parent class for all kind of CStruct fields.

    Attributes
    ----------
    name : str
        Name of this field.
    size : int
        Size of this field.
    value
        value of this field.
    offset : int
        Offset of this field in the flash.
    """

    def __init__(self, name: str, size: int, value: any, offset: int):
        self.name = name
        self.size = size
        self.value = value
        self.__offset = offset

    def get_offset(self) -> any:
        """Get field offset in flash.

        Returns
        -------
        int
            The offset.
        """
        return self.__offset

    def set(self, value: any):
        """Set field value.

        Set the value of the field.

        Parameters
        ----------
        value
            Field value
        """
        self.value = value


class CStructScalar(CStructField):
    """
    Class for scalar fields.
    """

    def dump_table(self, table: PrettyTable, level: int):
        """Dump the field to a table.

        This declaration is propagated towards the parents so that the runner taking care of
        the flash image generation knows where the flash are.

        Parameters
        ----------
        x : PrettyTable
            Table where the field should be dumped.
        level: int
            Dump level
        """
        if level >= 0:
            table.add_row([f'0x{self.get_offset():x}', self.name, f'0x{self.size:x}',
                f'0x{self.value:x}'])


class CStructArray(CStructField):
    """
    Class for scalar fields.
    """

    def dump_table(self, table: PrettyTable, level: int):
        """Dump the field to a table.

        This declaration is propagated towards the parents so that the runner taking care of
        the flash image generation knows where the flash are.

        Parameters
        ----------
        x : PrettyTable
            Table where the field should be dumped.
        level: int
            Dump level
        """

        # If this is the last dump level, we just dump the main information, otherwise we dump the
        # content in rows of 32 items of 16bits hex numbers
        if level > 0:
            value = ''
            size = len(self.value)
            if size > 1 << 10:
                size = 1 << 10
            index = 0
            while size > 0:
                iter_size = min(32, size)
                line = ''
                for i in range(0, iter_size):
                    line += '%2.2x' % self.value[index]
                    if i % 2 == 1:
                        line += ' '
                    index += 1

                size -= iter_size

                value += line + '\n'

        else:
            value = '-'


        table.add_row(['0x%x' % self.get_offset(), self.name, '0x%x' % self.size, value])


class CStruct():
    """
    Class for gathering CStruct fields together into a common structure.

    Attributes
    ----------
    name : str
        Name of this structure.
    parent : str
        Parent, which is aggregating all structures.
    """

    def __init__(self, name: str, parent: 'CStructParent'):
        self.fields = OrderedDict()
        self.struct = None
        self.format = ''
        self.__size = 0
        self.__name = name
        self.parent = parent
        parent.add_struct(self)

    def get_name(self) -> str:
        """Get sub-section name.

        Returns
        -------
        str
            The sub-section name.
        """
        return self.__name

    def get_offset(self) -> int:
        """Get sub-section offset.

        Returns
        -------
        int
            The sub-section offset.
        """
        # If there is no field, return 0 since we cannot know the offset
        if len(self.fields.keys()) == 0:
            return 0

        # Otherwise return the offset of the first field
        return list(self.fields.values())[0].get_offset()

    def get_size(self) -> int:
        """Get sub-section size.

        Returns
        -------
        int
            The sub-section size.
        """
        return self.__size


    def get_field(self, name: str) -> CStructField:
        """Get field.

        Can be used for fields added with add_field or add_field_array

        Parameters
        ----------
        name : str
            Name of the field

        Returns
        -------
        CStructField
            The field.
        """
        return self.fields.get(name)


    def set_field(self, name: str, value: any):
        """Set a field value.

        Can be used for fields added with add_field or add_field_array

        Parameters
        ----------
        name : str
            Name of the field
        value : any
            Value of the field
        """
        self.fields.get(name).set(value)


    def add_padding(self, name: str, align: int):
        """An empty field for padding.

        Can be used to add empty bytes to align the next field.

        Parameters
        ----------
        name : str
            Name of the field
        align : int
            Alignment of the next field.
        """
        offset = self.parent.get_current_offset()
        size = self.parent.align_offset(align)

        field = CStructArray(name, size, value=b'', offset=offset)
        self.fields[name] = field

        self.format += f'{size}s'
        self.__size += size

        return field



    def add_field(self, name: str, field_format: str) -> CStructField:
        """Add a scalar field.

        The field is added to the structure. The fields are dumped in the order they are added.

        Parameters
        ----------
        name : str
            Name of the field
        field_format: str
            Format of the field, described by struct python package

        Returns
        -------
        CStructField
            The field.
        """
        size = struct.calcsize(field_format)
        offset = self.parent.alloc_offset(size)


        field = CStructScalar(name, size, value=0, offset=offset)
        self.fields[name] = field

        self.format += field_format
        self.__size += size

        return field


    def add_field_array(self, name: str, size: int) -> CStructField:
        """Add an array field.

        The field is added to the structure. The fields are dumped in the order they are added.

        Parameters
        ----------
        name : str
            Name of the field
        size: int
            Size of the array

        Returns
        -------
        CStructField
            The field.
        """
        offset = self.parent.alloc_offset(size)

        field = CStructArray(name, size, value=b'', offset=offset)
        self.fields[name] = field

        self.format += f'{size}s'
        self.__size += size

        return field

    def dump_table(self, level: int) -> str:
        """Dump the structure to a table.

        This will dump all the fields of this structure to the same table.

        Parameters
        ----------
        level: int
            Dump level

        Returns
        -------
        str
            The table.
        """
        table = PrettyTable()
        table.field_names = ["Offset", "Name", "Size", "Value"]
        for field in self.fields.values():
            field.dump_table(table, level)

        table.align = "l"

        return str(table)

    def pack(self) -> bytes:
        """Pack all fields in bynary form.

        This can be used to dump all the fields to a file.

        Returns
        -------
        bytes
            The values of the fields packed into a byte array.
        """
        if self.struct is None:
            self.struct = struct.Struct(self.format)

        values = []
        for field in self.fields.values():
            values.append(field.value)

        return self.struct.pack(*values)


class CStructParent():
    """
    Class for gathering several structures together into a sub-section.

    Attributes
    ----------
    name : str
        Name of this sub-section.
    parent : FlashSection
        Parent, which is aggregating all sub-sections.
    """

    def __init__(self, name: str, parent: FlashSection):
        self.name = name
        self.structs = []
        self.parent = parent
        if parent is not None:
            parent.add_struct(self)


    def align_offset(self, alignment: int) -> int:
        """Align the current offset.

        The current offset is aligned by the specified alignement in bytes.
        This can be used to add some padding.

        Parameters
        ----------
        size : int
            The size to allocate.

        Returns
        -------
        int
            The size allocated to align the offset.
        """
        return self.parent.align_offset(alignment)


    def get_current_offset(self) -> int:
        """Get the current offset.

        Returns
        -------
        int
            The current offset.
        """
        return self.parent.get_current_offset()


    def alloc_offset(self, size: int) -> int:
        """Allocate an offset.

        This is used by structures to assign an offset to each field.

        Parameters
        ----------
        size : int
            Number of bytes to allocate

        Returns
        -------
        int
            The allocated offset.
        """
        return self.parent.alloc_offset(size)


    def add_struct(self, cstruct: CStructField) -> CStructField:
        """Add a structure to the sub-section.

        Returns
        -------
        CStruct
            The structure.
        """
        self.structs.append(cstruct)
        return cstruct


    def dump_table(self, level: int) -> str:
        """Dump the structure to a table.

        This will dump all the structures of this sub-section into a table.

        Parameters
        ----------
        level: int
            Dump level

        Returns
        -------
        str
            The table.
        """
        table = PrettyTable()
        names = ["Offset", "Size", "Name"]
        if level > 0:
            names.append("Content")
        table.field_names = names

        for cstruct in self.structs:
            row = ['0x%x' % cstruct.get_offset(), '0x%x' % cstruct.get_size(), cstruct.get_name()]
            if level > 0:
                row.append(str(cstruct.dump_table(level-1)))
            table.add_row(row)

        table.align = "l"

        return str(table)


    def get_image(self):
        """Dump the content of the sub-section in binary form to the specified file.

        Parameters
        ----------
        fd
            File descriptor
        """
        return self.pack()


    def pack(self) -> bytes:
        """Pack all fields in bynary form.

        This can be used to dump all the fields to a file.

        Returns
        -------
        bytes
            The values of the fields packed into a byte array.
        """
        result = bytearray()
        for cstruct in self.structs:
            result += cstruct.pack()

        return result
