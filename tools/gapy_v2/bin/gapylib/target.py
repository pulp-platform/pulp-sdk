"""Provides target class common to all targets which brings common methods and commands."""

#
# Copyright (C) 2022 GreenWaves Technologies
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

#
# Authors: Germain Haugou, GreenWaves Technologies (germain.haugou@greenwaves-technologies.com)
#


import argparse
import os
import importlib
import json
from collections import OrderedDict
import sys
import gapylib.flash


def get_target(target: str) -> 'Target':
    """Returns the class implementing the support for the specified target.

    The target is specified as a python module which is imported from python path.
    It returns the class 'Target' from the imported module.

    Parameters
    ----------
    target : str
        Name of the target. The name must corresponds to a python module.

    Returns
    -------
    class
        The class 'Target' of the imported module.
    """
    try:
        module = importlib.import_module(target)
    except ModuleNotFoundError as exc:
        raise RuntimeError(f'Invalid target specified: {target}') from exc

    return getattr(module, 'Target')


class Target():
    """
    Parent class for all targets, which provides all common functions and commands.

    Attributes
    ----------
    parser : argparse.ArgumentParser
        The parser where to add arguments.
    options : List
        A list of options for the target.
    """

    def __init__(self, parser, options: list=None):

        parser.add_argument("--flash-property", dest="flash_properties", default=[],
            action="append", help="specify the value of a flash property")

        parser.add_argument("--flash-content", dest="flash_contents", default=[], action="append",
            help="specify the path to the JSON file describing the content of the flash")

        parser.add_argument("--flash-layout-level", dest="layout_level", type=int, default=2,
            help="specify the level of the layout when dumping flash layout")

        parser.add_argument("--flash-no-auto", dest="flash_auto", action="store_false",
            help="Flash auto-mode, will force the flash content update only if needed")

        parser.add_argument("--binary", dest = "binary", default = None,
            help = "Binary to execute on the target")

        self.commands = [
            ['commands'    , 'Show the list of available commands'],
            ['targets'     , 'Show the list of available targets'],
            ['image'       , 'Generate the target images needed to run execution'],
            ['flash'       , 'Upload the flash contents to the target'],
            ['flash_layout', 'Dump the layout of the flashes'],
            ['flash_dump_sections', 'Dump each section of each flash memory'],
            ['flash_properties', 'Dump the value of all flash section properties'],
            ['run'         , 'Start execution on the target'],
        ]

        self.target_dirs = []
        self.flashes = {}
        self.work_dir = None
        self.options = options
        self.layout_level = 0
        self.args = None


    def get_abspath(self, relpath: str) -> str:
        """Return the absolute path depending on the working directory.

        If no working directory was specified, the relpath is appended to the current directory.
        Otherwise it is appended to the working directory.

        Parameters
        ----------
        relpath : str
            Relative path of the file.

        Returns
        -------
        str
            The absolute path.
        """
        if os.path.isabs(relpath):
            return relpath

        if self.work_dir is None:
            return os.path.abspath(relpath)

        return os.path.join(self.work_dir, relpath)



    def register_flash(self, flash: gapylib.flash.Flash):
        """Register a flash.

        The flash should inherit from gapylib.flash.Flash.
        This will allow gapy to produce images for this flash.

        Parameters
        ----------
        flash : gapylib.flash.Flash
            The flash to be registered.
        """
        self.flashes[flash.get_name()] = flash


    def get_args(self) -> argparse.ArgumentParser:
        """Return the command-line arguments.

        Returns
        -------
        argparse.ArgumentParser
            The arguments.
        """

        return self.args


    @staticmethod
    def get_file_path(relpath: str) -> str:
        """Return the absolute path of a file.

        Search into the python path for this file and returns its absolute path.
        This can be used to find configuration files next to python modules.

        Parameters
        ----------
        relpath: str
            The file relative path to look for in the python path

        Returns
        -------
        str
            The path.
        """
        for dirpath in sys.path:
            path = os.path.join(dirpath, relpath)
            if os.path.exists(path):
                return path

        return None


    def handle_command(self, cmd: str):
        """Handle a command.

        This should be called only by gapy executable but can be overloaded by the real targets
        in order to add commands or to handle existing commands differently.

        Parameters
        ----------
        cmd : str
            Command name to be handled.
        """
        if cmd == 'commands':
            self.__print_available_commands()

        elif cmd == 'targets':
            self.__display_targets()

        elif cmd == 'flash_layout':
            for flash in self.flashes.values():
                flash.dump_layout(level=self.layout_level)

        elif cmd == 'flash_dump_sections':
            for flash in self.flashes.values():
                flash.dump_sections()

        elif cmd == 'flash_properties':
            for flash in self.flashes.values():
                flash.dump_section_properties()

        elif cmd == 'image':
            for flash in self.flashes.values():
                flash.dump_image()

        elif cmd == 'flash':
            pass

        else:
            raise RuntimeError('Invalid command: ' + cmd)

    def __print_available_commands(self):
        print('Available commands:')

        for command in self.commands:
            print(f'  {command[0]:16s} {command[1]}')


    def append_args(self, parser: argparse.ArgumentParser):
        """Append target specific arguments to gapy command-line.

        This should be called only by gapy executable to register some arguments handled by the
        default target but it can be overloaded in order to add arguments which should only be
        visible if the target is active.

        Parameters
        ----------
        parser : argparse.ArgumentParser
            The parser where to add arguments.
        """


    def parse_args(self, args: any):
        """Handle arguments.

        This should be called only by gapy executable to handle arguments but it can be overloaded
        in order to handle arguments specifically to a target.

        Parameters
        ----------
        args :
            The arguments.
        """

        self.args = args
        self.layout_level = args.layout_level

        # Parse the flash properties so that we can propagate to each flash only its properties
        if len(args.flash_properties) != 0:
            self.__extract_flash_properties(args.flash_properties)

        # Now propagte the flash content to each flash
        for content in args.flash_contents:
            if content.find("@") == -1:
                raise RuntimeError('Invalid flash content (must be of form '
                    '<content path>@<flash name>): ' + content)

            # Content is specified as content_file_path@flash_name
            content_path, flash_name = content.rsplit('@', 1)

            if self.flashes.get(flash_name) is None:
                raise RuntimeError('Invalid flash content, flash is unknown: ' + content)

            try:
                with open(content_path, "rb") as file_desc:
                    content_dict = json.load(file_desc, object_pairs_hook=OrderedDict)

                    self.flashes.get(flash_name).set_content(content_dict)
            except OSError as exc:
                raise RuntimeError('Invalid flash content, got error while opening content file: '
                    + str(exc)) from exc


    def set_target_dirs(self, target_dirs: list):
        """Set the target directories.

        This sets the directories where to look for targets.
        This should be called only by gapy executable.

        Parameters
        ----------
        target_dirs : list
            The directories.
        """

        self.target_dirs = target_dirs


    def set_working_dir(self, working_dir : str):
        """Set the working directory.

        All files produced by gapy will be dumped into this directory.

        Parameters
        ----------
        working_dir : str
            The working directory.
        """
        self.work_dir = working_dir


    def get_working_dir(self) -> str:
        """Return the working directory.

        All files produced by gapy will be dumped into this directory.

        Returns
        -------
        str
            The working directory.
        """
        if self.work_dir is None:
            return os.getcwd()

        return self.work_dir

    def __display_targets(self):
        print ('Available targets:')

        targets = {}

        for path in self.target_dirs:
            for root, __, files in os.walk(path):

                for file in files:
                    if file.endswith('.py') and file != '__init__.py':
                        rel_dir = os.path.relpath(root, path)
                        file_path = os.path.join(rel_dir, file)
                        target_name = file_path.replace('/', '.').replace('.py', '')
                        target = get_target(target_name)([])

                        if targets.get(target_name) is None:
                            targets[target_name] = True

                            print(f'{target_name:16s} {target}')

    def __extract_flash_properties(self, args_properties: list):
        # Since they can be specified in any order on the command line, we first need to
        # sort them out in a dictionary
        properties = {}

        # First parse them and store them in the dictionary
        for property_desc in args_properties:
            # Property format is value@flash:section:property_name
            try:
                value, path = property_desc.rsplit('@', 1)
                flash, section, property_name = path.split(':')
            except Exception as exc:
                raise RuntimeError('Invalid flash property (must be of form '
                    '<value>@<flash:section:property>): ' + property_desc) from exc

            # Temporary hack for gap_sdk to keep compatibility with old gapy
            # Once all the tests are fully switched to new gapy, this can be removed
            if flash == 'target/chip/soc/mram':
                flash = 'mram'

            if properties.get(flash) is None:
                properties[flash] = {}
            if properties[flash].get(section) is None:
                properties[flash][section] = []
            properties[flash][section].append([property_name, value])

        # And then propagate them to the flashes
        for flash_name, flash_properties in properties.items():
            if self.flashes.get(flash_name) is None:
                raise RuntimeError('Invalid flash property, flash is unknown: ' + flash_name)

            self.flashes.get(flash_name).set_properties(flash_properties)
