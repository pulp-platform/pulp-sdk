"""
Common definitions for flashes.
In particular, it defines a common class that each flash section should inherit from.
The same for each flash.
"""

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

import typing
import json
from collections import OrderedDict
from prettytable import PrettyTable


class FlashSectionProperty():
    """
    Placeholder for flash section properties.

    Attributes
    ----------
    name : str
        Name of the property.
    value : any
        Value of the property.
    description : str
        Description of he property.
    """

    def __init__(self, name: str, value: any, description: str):

        self.name = name
        self.description = description
        self.value = value


class FlashSection():
    """
    Parent class for all flash sections. This provides utility functions for describing the section.
    A section is a logical part of a flash, like a ROM header or a FS.

    Attributes
    ----------
    parent: gapylib.flash.Flash
        Name of the section.
    name : str
        Name of the section.
    id : int
        Id of the section.
    """

    def __init__(self, parent, name: str, section_id: int):
        self.name = name
        self.section_id = section_id
        self.parent = parent
        self.offset = 0
        self.current_offset = 0
        self.content_dict = {}
        self.structs = []
        self.properties = {}


    def declare_property(self, name: str, value: any, description: str):
        """Declare a section property.

        Section properties are used to tell the section to configure the sections. They must be
        declared with this method before they can be overwritten from the flash content or
        command-line flash properties.

        Parameters
        ----------
        name : str
            Name of the property.
        value : any
            Value of the property.
        description : str
            Description of he property.
        """

        if self.properties.get(name) is not None:
            raise RuntimeError(f'Property {name} already declared')

        self.properties[name] = FlashSectionProperty(
            name=name, value=value, description=description
        )


    def add_struct(self, cstruct: any) -> any:
        """Add a structure to the section.

        Returns
        -------
        CStruct
            The structure.
        """
        self.structs.append(cstruct)
        return cstruct


    def set_offset(self, offset: int):
        """Set the start offset of the section.

        The offset is used by the flash to know where the section starts.
        It is also used to implement a flash allocator in the section.

        Parameters
        ----------
        offset : int
            The section offset.
        """
        self.offset = offset
        # Also set the current offset for the offset allocator.
        self.current_offset = offset

    def alloc_offset(self, size: int) -> int:
        """Allocate an offset.

        This returns the current offset and increases it by the speicified size.
        This can be use to allocate some fields in the section and let the flash knows
        the size of the section.

        Parameters
        ----------
        size : int
            The size to allocate.

        Returns
        -------
        int
            The allocated offset.
        """
        current_offset = self.current_offset
        self.current_offset += size
        return current_offset

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
        aligned_offset = (self.current_offset + alignment - 1) & ~(alignment - 1)
        size = aligned_offset - self.current_offset
        self.current_offset = aligned_offset

        return size

    def get_size(self) -> int:
        """Get the section size.

        The returned size is based on the allocated offsets and the requested alignments.

        Returns
        -------
        int
            The section size.
        """
        return self.current_offset - self.offset

    def get_flash(self) -> 'Flash':
        """Get the sflash containing this section.

        Returns
        -------
        gaplib.flash.Flash
            The section name.
        """
        return self.parent

    def get_name(self) -> str:
        """Get the section name.

        Returns
        -------
        str
            The section name.
        """
        return self.name

    def get_id(self) -> int:
        """Get the section ID.

        Returns
        -------
        int
            The section ID.
        """
        return self.section_id

    def get_offset(self) -> int:
        """Get the section offset.

        Returns
        -------
        int
            The section offset.
        """
        return self.offset

    def get_current_offset(self) -> int:
        """Get the current offset.

        Returns
        -------
        int
            The current offset.
        """
        return self.current_offset

    def get_image(self) -> bytes:
        """Dump the content of the section in bynary form to the specified file descriptor.

        Parameters
        ----------
        file_desc
            File descriptor.
        """
        result = bytearray()
        for cstruct in self.structs:
            result += cstruct.pack()

        return result


    def dump_table(self, level: int) -> str:
        """Dump the section as a table.

        This should only be called by the gapy executable and should be overloaded by the section
        to dump the content of the section into a table.

        Parameters
        ----------
        level : int
            The dump level. The content should be displayed only if it is superior or equal to zero.
        """
        result = ''
        for cstruct in self.structs:
            result += cstruct.dump_table(level)

        return result

    def dump_properties(self) -> str:
        """Dump the section properties as a table.
        """
        if len(self.properties) == 0:
            return ''

        table = PrettyTable()
        table.field_names = ["Property name", "Property value", "Property description"]
        for prop in self.properties.values():
            table.add_row([prop.name, prop.value, prop.description])

        table.align = 'l'

        return str(table)


    def set_content(self, offset: int, content_dict: dict):
        """Set the content of the section.

        This can be called to set the default content, and may be overwritten by gapy executable
        if a new content has been specified.
        The section should in this step declare all its internal structure so that the offsets and
        sizes of all sections are known after this step.

        Parameters
        ----------
        offset : int
            Start offset of the section

        content_dict : dict
            Content of the section
        """
        self.set_offset(offset)
        self.content_dict = content_dict

        if self.content_dict.get("properties") is not None:
            for name, value in self.content_dict.get("properties").items():
                if self.properties.get(name) is None:
                    raise RuntimeError(f'Trying to set undefined property: {name}')

                self.properties[name].value = value


    def finalize(self):
        """Finalize the section.

        This can be called to set internal section fields which requires some knowledge of the
        offset or size of other sections.
        The structure of the section should not be changed in this step
        """
        # Nothing to do, should be overloaded by real sections when needed.


    def get_property(self, name: str) -> any:
        """Return the value of a property.

        This can be called to get the value of a property set from the section content.

        Parameters
        ----------
        name : str
            Name of the property

        Returns
        -------
        str
            The property value.
        """
        if self.properties.get(name) is None:
            raise RuntimeError(f'Trying to get undefined property: {name}')

        return self.properties.get(name).value


    def is_empty(self) -> bool:
        """Tell if the section is empty.

        This is used by the target to know if the flash should be updated in auto mode.

        Returns
        -------
        str
            True if the section is empty, False otherwise.
        """
        return True

    def get_partition_type(self)-> int:
        """Return the partition type.

        This information can be used by the partition table as the subtype.
        This method returns an unknown type (0xff) and should be overloaded by real sections.

        Returns
        -------
        int
            The partition type.
        """
        return 0xff


class Flash():
    """
    Parent class for all flashes. This provides utility functions for describing a flash.

    Attributes
    ----------
    target : gapylib.target.Target
        Target containing the flash.

    name : str
        Name of the flash
    size : int
        Size of the flash
    image_name : str
        Image name where the content of the flash will be dumped
    flash_attributes : dict
        Set of attributes describing the flash.
    """

    def __init__(self, target: 'gapylib.target.Target', name: str, size: int, image_name:str=None,
            flash_attributes: dict=None):
        self.size = size
        self.name = name
        self.sections_templates = {}
        self.sections = OrderedDict({})
        self.content_dict = None
        self.content_parsed = False
        self.properties = {}
        self.flash_attributes = {} if flash_attributes is None else flash_attributes
        self.target = target
        if image_name is None:
            self.image_name = name + '.bin'
        else:
            self.image_name = image_name


    def get_name(self) -> str:
        """Return the name of the flash.

        Returns
        -------
        str
            The flash name.
        """
        return self.name


    def get_target(self) -> 'gapylib.target.Target':
        """Return the target containing the flash.

        Returns
        -------
        gapylib.target.Target
            The target.
        """
        return self.target


    def get_image_name(self) -> str:
        """Return the name of the flash image.

        Gapy will dump the binary image of the flash to the image name.

        Returns
        -------
        str
            The image flash name.
        """
        return self.image_name


    def set_properties(self, properties: dict):
        """Set the section properties of this flash.

        This will be used to overwrite flash section properties retrived from its content.

        Parameters
        ----------
        properties : dict
            The flash properties
        """
        self.properties = properties


    def register_section_template(self, template_name: str, section_template: FlashSection):
        """Register a section template.

        A section template is used by a target to define the kind of sections
        which are allowed for the flash.
        The section content can then instantiates section templates.

        Parameters
        ----------
        template_name : dict
            Name of the template

        section_template : FlashSection
            Section template
        """
        self.sections_templates[template_name] = section_template


    def dump_layout(self, level: int):
        """Dump the layout of the flash.

        Parameters
        ----------
        level : int
            Dumping depth
        """
        self.__parse_content()

        print (f'\nLayout for flash: {self.name}')

        # The flash layout is displayed as a table which can embed deeper level
        # table in a cell
        table = PrettyTable()
        names = ["Section offset", "Section name", "Section size"]
        if level > 0:
            names.append("Section content")
        table.field_names = names

        for section in self.sections.values():
            row = [f'0x{section.get_offset():x}', section.get_name(), f'0x{section.get_size():x}']
            if level > 0:
                row.append(str(section.dump_table(level-1)))
            table.add_row(row)

        table.align = 'l'

        print (table)


    def dump_section_properties(self):
        """Dump the section properties of the flash.
        """
        self.__parse_content()

        print (f'Section properties for flash: {self.name}')

        # The flash layout is displayed as a table which can embed deeper level
        # table in a cell
        table = PrettyTable()
        table.field_names = ["Section name", "Section properties"]

        for section in self.sections.values():
            row = [section.get_name(), section.dump_properties()]
            table.add_row(row)

        table.align = 'l'

        print (table)


    def dump_image(self, file_desc: typing.BinaryIO):
        """Dump the content of the flash in binary form to the specified file.

        Parameters
        ----------
        fd
            File descriptor
        """
        file_desc.write(self.get_image())


    def get_image(self, first: int=None, last: int=None) -> bytes:
        """Return the content of the flash.

        Returns
        -------
        bytes
            The flash content.
        first: int
            The index of the first section from which the image must be generated
        last: int
            The index of the last section until which the image must be generated
        """
        result = bytearray()

        self.__parse_content()

        if first is None:
            first = 0

        if last is None:
            last = len(self.sections)

        prev_section = None
        for section in list(self.sections.values())[first:last+1]:
            # Since we need a contiguous image, insert some padding in case we find
            # a hole between 2 sections
            if prev_section is not None:
                padding = section.get_offset() - prev_section.get_offset() - \
                    prev_section.get_size()
                result += bytearray(padding)

            result += section.get_image()
            prev_section = section

        return result


    def get_image_path(self) -> str:
        """Return the path of the file containing the flash image.

        Returns
        -------
        str
            The image file path.
        """
        return self.target.get_abspath(self.get_image_name())


    def set_content(self, content_dict: dict):
        """Set the content of the flash.

        Parameters
        ----------
        content_dict: dict
            Flash content
        """
        # The content is just stored and is parsed when needed to allow overloading it
        self.content_dict = content_dict


    def get_flash_attribute(self, name: str) -> any:
        """Return the value of a flash property.

        This can be called to get the value of a property converning the flash.

        Parameters
        ----------
        name : str
            Name of the property

        Returns
        -------
        str
            The property value.
        """
        return self.flash_attributes.get(name)


    def get_size(self) -> int:
        """Get flash size.

        Returns
        -------
        int
            The flash size.
        """
        return self.size


    def get_sections(self):
        """Get all the sections of the flash.

        Returns
        -------
        list
            The flash sections.
        """
        self.__parse_content()

        return list(self.sections.values())


    def is_empty(self) -> bool:
        """Tell if the flash is empty.

        This is used by the target to know if the flash should be updated in auto mode.

        Returns
        -------
        str
            True if the flash is empty, False otherwise.
        """
        self.__parse_content()
        for section in self.sections.values():
            if not section.is_empty():
                return False

        return True


    def __get_section_template(self, template_name: str) -> FlashSection:
        return self.sections_templates.get(template_name)


    def __parse_content(self):
        # To allow overloading the content, we parse the content only once the first time
        # it is needed
        if not self.content_parsed and self.content_dict is not None:
            self.content_parsed = True

            # Take the properties set from command-line and overwrite the ones from
            # the content
            self.__handle_section_properties()

            # Now create all the sections
            if self.content_dict.get('sections') is not None:
                for content_section in self.content_dict.get('sections'):

                    if content_section.get('name') is None:
                        raise RuntimeError("Section does not have any name:\n" +  \
                            json.dumps(content_section, indent=4))

                    if content_section.get('template') is None:
                        raise RuntimeError("Section does not have any template:\n" + \
                            json.dumps(content_section, indent=4))

                    section_template = self.__get_section_template(content_section.get('template'))

                    if section_template is None:
                        raise RuntimeError("Unknown section template: " + \
                            content_section.get('template'))

                    section = section_template(self, content_section.get('name'),
                        len(self.sections))
                    self.sections[content_section.get('name')] = section


            # And finally set the content of each section and give it its starting offset
            if self.content_dict.get('sections') is not None:
                flash_offset = 0
                for content_section in self.content_dict.get('sections'):
                    section = self.sections[content_section.get('name')]
                    section.set_content(flash_offset, content_section)
                    flash_offset = section.get_offset() + section.get_size()

                    if flash_offset > self.size:
                        raise RuntimeError(
                            f'Section "{section.get_name()}" overflowed flash "{self.name}",'
                            f' flash size is 0x{self.size:x}, current content size is'
                            f' 0x{flash_offset:x}.')


            # And finally set the content of each section and give it its starting offset
            if self.content_dict.get('sections') is not None:
                flash_offset = 0
                for content_section in self.content_dict.get('sections'):
                    section = self.sections[content_section.get('name')]
                    section.finalize()


    def __handle_section_properties(self):
        property_sections = self.properties
        if property_sections is not None:

            for section_name, section_properties in property_sections.items():
                self.__overwrite_section_properties(section_name, section_properties)


    def __get_section_content(self, section_name: str):
        sections = self.content_dict.get('sections')
        if sections is not None:
            for section in sections:
                if section.get('name') == section_name:
                    return section

        return None

    def __overwrite_section_properties(self, section_name: str, section_properties: dict):

        section = self.__get_section_content(section_name)

        if section is None:
            raise RuntimeError(f'Found unknown section while handling command-line flash section'
                f' properties: {section_name}')

        # Go through each property found for the section and set it into the
        # section properties
        for property_desc in section_properties:
            key, value = property_desc
            if section.get('properties') is None:
                section['properties'] = OrderedDict()

            # If the property is a list, append the value to the list
            if section['properties'].get(key) is not None and \
                    isinstance(section['properties'].get(key), list):
                section['properties'].get(key).append(value)
            # Otherwise just set it
            else:
                section['properties'][key] = value
