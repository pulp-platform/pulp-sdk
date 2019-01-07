Configuration
=============

When the SDK is initialized, a default configuration is chosen depending on the script used to initialize the SDK.
This section describes how this default configuration can be customized.



Global customization
--------------------

You can first customize the configuration globally so that it applies to all applications
by defining the following environment variable: ::

  export PULP_CURRENT_CONFIG_ARGS=<config>

A list of configuration items can then be specified, separated by spaces like in the
following example: ::

  export PULP_CURRENT_CONFIG_ARGS="gvsoc/trace=pe0/insn gvsoc/trace=pe1/insn"

Then any application which was already configured with a previous configuration needs to be
reconfigured with the following command execute from the application directory: ::

  make conf



Per-application customization
-----------------------------

You can customize just one application on top of the global configuration by executing this command
from the application directory: ::

  make conf CONFIG_OPT="<config>"

*<config>* should be replaced by the configuration items you want to customize, with the same
syntax as the global customization, like in the following example: ::

  make conf CONFIG_OPT="gvsoc/trace=pe0/insn gvsoc/trace=pe1/insn"

To make this customization permanent for the application, you can put the CONFIG_OPT option directly into the makefile and execute: ::

  make conf



Per-application JSON configuration
----------------------------------

Deeper customizations can also be done at application level by inluding a JSON file in the application in order to customize the global configuration.

This can be done by adding the following variable to the application application, which will give the path to the JSON file: ::

  PULP_USER_CONFIG = $(CURDIR)/config.json

The file can actually be anywhere, as soon as it is found by the makefile, and there can even be several files, for example to select a specific one from the makefile, using make conditional features.

The file follows the JSON syntax and the sections which must be put inside depends on the global configuration (platform, chip, runtime, etc), so the content is documented in other sections.

Here is an example of such a file which adds an hyperflash model, change the boot mode to boot from hyperflash, and change the default padframe profile: ::

  {
    "devices": {
      "hyperram": {
        "type": "hyper",
        "interface": "hyper0",
        "cs": 0
      }
    },

    "runner": {
      "boot-mode": "hyper",
      "flash_type": "hyper"
    },

    "pads": {
      "default_profile": "hyper"
    }
  }



.. _periph_config:

Peripherals customization
-------------------------

When using a simulation platform, the default configuration is usually only activating very few peripherals and
additional peripherals can be added by customizing the default configuration with a specific section in the user JSON configuration.

The devices to be simulated must be specified in the application user JSON configuration. A section with name *devices* must be added, with one sub-section per device, like in the following example: ::

  {
    "devices": {
      "hyperram": {
        "type": "hyper",
        "interface": "hyper0",
        "cs": 0
      }
    }
  }

Each device section must at least have the section *type*, which specifies which device to be simulated. The rest is specific to each device.


Here is the list of supported peripherals.

=================== ==================================================== ===================
Type                Description                                          Supported platforms
=================== ==================================================== ===================
uart_tb             Uart testbench, can be used to                       gvsoc, rtl
                    dump uart TX to file or terminal                        
hyper               Hyperflash/Hyperram model                            gvsoc
=================== ==================================================== ===================



Hyper
.....

This model supports the following parameters

=================== ==================================================== ==================
Name                Description                                          Optional/Mandatory
=================== ==================================================== ==================
interface           Gives the interface where the device is connected.   Mandatory
cs                  Gives the chip select where the device is connected. Mandatory
=================== ==================================================== ==================

Here is an example: ::
  
  {
    "hyperram": {
      "type": "hyper",
      "interface": "hyper0",
      "cs": 0
    }
  }
  