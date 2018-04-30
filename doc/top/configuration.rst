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



Peripherals customization
-------------------------

When using a simulation platform, the default configuration is usually only activating very few peripherals and
additional peripherals can be added by customizing the default configuration.

Customizing peripherals is using a different mechanism compared to the previous section as it needs to modify
several aspects in the configuration. 

In order to add a peripheral to the current configuration, you can modify an application makefile and
add the following line: ::

  override PULP_TEMPLATE_ARGS += <config>

*<config>* should be replaced by the list of peripherals to be added, as well as their parameters.
Peripherals must be separated by spaces and parameters are given inside parenthesis after the peripheral name,
like in the following example: ::

  override PULP_TEMPLATE_ARGS += uart_tb(uart0)

Here is the list of supported peripherals.

=================== =============================== ========================== ================ ===================
Model name          Description                     Parameters                 Example          Supported platforms
=================== =============================== ========================== ================ ===================
uart_tb             Can be used to dump uart TX to   uart_tb(<uart itf name>)  uart_tb(uart0)   gvsoc
                    file or terminal
=================== =============================== ========================== ================ ===================