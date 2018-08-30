Virtual platform
================

Usage
-----

The virtual platform must be launched through the common runner called *pulp-run*.

The specified platform must be *vp* (through option *--platform*) and the only mandatory option is *--config-file* in order to give the system configuration to simulate.

As the virtual platform will generate several temporary files, it is also good to launch it from a specific folder or to specify it through the *--dir* option.

Here is an example example: ::

  $ pulp-run --platform=vp --dir=work_dir --config-file=config.json

All the other options for the platform must be given inside the configuration file, inside the *gvsoc* object, as a set of properties. As for other configuration items, these properties can be set directly inside the configuration file (in JSON format), or using the following environment variable: ::

  $ export PULP_CURRENT_CONFIG_ARGS="gvsoc/trace=.*"

Or by giving them to the SDK makefiles (if they are included): ::

  $ make clean all run CONFIG_OPT="gvsoc/trace=.*"


System traces
-------------

The virtual platform allows dumping architecture events to help developers debugging their applications by better showing what is happening in the system.

For example, it can show instructions being executed, DMA transfers, events generated, memory accesses and so on.

This feature can be enabled and configured through the configuration item *gvsoc/trace*. This option takes an argument which specifies a regular expression of the path in the architecture where the traces must be enabled. All components whose path matches the specified one will dump traces. Several paths can be specified by using the option several times. Here is an example that activates instruction traces for core 0 and core 1: ::

  $ make clean all run CONFIG_OPT="gvsoc/trace=pe0/insn gvsoc/trace=pe1/insn"

The trace file should look as follows: ::

  194870000: 19487: [/sys/board/chip/soc/cluster/pe1/insn] M 1c001a96 c.li                a2, 0, 0           a2=00000000 
  194870000: 19487: [/sys/board/chip/soc/cluster/pe0/insn] M 1c001a2c beq                 a1, s4, 76         a1:00000020  s4:00000025

There is usually one line per event, although an event can sometimes takes several lines to display more information.

The number on the left gives the timestamp of the event, in picoseconds, and the one right after the number of cycles. Both are given because different blocks like clusters can have different frequencies.

The second part, which is a string, gives the path in the architecture where the event occurred. This is useful to differentiate blocks of the same kind that generate the same event. This path can also be used with the *gvsoc/trace* option to reduce the number of events.

The third part, which is also a string, is the information dumped by the event, and is totally specific to this event. In our example, the core simulator is just printing information about the instruction that has been executed.

One difficulty is usually to find out which paths should be activated to get the needed information. One method is to dump all the events with *gvsoc/trace=.**, then find out which one are interesting and then put them on the command line. Here are the paths for the main components:

========================================= ===============================
Path                                      Description
========================================= ===============================
/sys/board/chip/soc/cluster/pe0           Processing element, useful to see the IOs made by the core, and the instruction it executes. You can add */iss* to just get instruction events
//sys/board/chip/soc/cluster/event_unit   Hardware synchronizer events, useful for debugging inter-core synchronization mechanisms
/sys/board/chip/soc/cluster/pcache        Shared program cache accesses
/sys/board/chip/soc/cluster/l1_ico        Shared L1 interconnect
/sys/board/chip/soc/cluster/l1/bankX      L1 memory banks (the X should be replaced by the bank number)
/sys/board/chip/soc/l2                    L2 memory accesses
/sys/board/chip/soc/cluster/dma           DMA events
========================================= ===============================

At first, the most interesting traces are the core instruction traces. As they show not only the instructions executed but also the registers accessed, their content and the memory accesses, they are very useful for debugging bugs like memory corruptions.

Here is an example: ::

  4890000: 489: [/sys/board/chip/soc/cluster/pe0/insn] M 1c001252 p.sw  0, 4(a5!)  a5=10000010  a5:1000000c  PA:1000000c

The event information dumped for executed instructions is using the following format: ::

  <address> <instruction> <operands> <operands info>

<address> is the address of the instruction.

<instruction> is the instruction label.

<operands> is the part of the decoded operands.

<operands info> is giving details about the operands values and how they are used.

The latter information is using the following convention:

  - When a register is accessed, its name is displayed followed by *=* if it is written or *:* if it is read. In case it is read and written, the register appears twice. It is followed by its value, which is the new one in case it is written.

  - When a memory access is done, *PA:* is displayed, followed by the address of the access.

  - The order of the statements is following the order on the decoded instruction

The memory accesses which are displayed are particularly interesting for tracking memory corruptions as they can be used to look for accesses to specific locations.


VCD traces
----------

The virtual platform can dump VCD traces which show the state of several components over the time, like the cores PC, the DMA transfers, etc, and thus gives a better overview than the VCD traces.

Configuration
.............

VCD tracing can be activated through the following configuration item: ::

  make conf CONFIG_OPT="gvsoc/event=.*"

Once the platform is run, this will generate a VCD file called *all.vcd* in the build folder.



Display
.......

Any VCD viewer can be used to display the traces. On Linux the free gtkwave viewer can be used. For example to display the PC traces, you can launch it with: ::

  gtkwave <vcdFilePath>

Then click on Search->Search Signal Regexp, enter "pc", click on Select All and Insert, and close the box. You should now see the PC traces in the view, you can zoom out to see the full window.

.. Gtkwave script generation
.. .........................

.. In case gtkwave is used, a script can be automatically generated that will pre-configure all interesting signals. For that the following option must be used: ::

..   $ make clean all run CONFIG_OPT="gvsoc/vcd/active gvsoc/vcd/gtkw"

.. You should see at the beginning of the simulation that says that a script has been generated. Just open it with gtkwave and you should see all signals already configured in the view.

.. This view contains some pre-defined groups that are very useful for quickly opening and closing architectures parts. It also contains a group called debug, that contains more high-level debug information like function name, disassembled instructions and so on.


Application profiling
---------------------

The virtual platform is for now not providing any particular feature in terms of profiling except for hardware performance counters whose most of them are modeled.

To use them, the test should configure and use them as on the real silicon, with the difference that all performance counters are implemented, not only one.

You can have a look at this SDK example to know more about peformance counters: examples/pulp-rt-examples/perf/cluster/direct



Timing models
-------------

Timing models are always active, there is no specific option to set to activate them. They are mainly timing the core model so that the main stalls are modeled. This includes branch penalty, load-use penalty an so on. The rest of the architecture is slightly timed. Remote accesses are assigned a fixed cost and are impacted by bandwidth limitation, although this still not reflect exactly the HW (the bus width may be different). L1 contentions are modeled with no priority. DMA is modeled with bursts, which gets assigned a cost. All UDMA interfaces are finely modeled.



Power models
------------

This is for now a very preliminary work. Power traces can be showed when VCD traces are activated. Each power source is able to register an amount of energy, which is showed inside a VCD trace as a pulse.

For now only the core is registering the energy consumed by an instruction, but all instructions are assigned a fixed cost, which just has an arbitrary value.

A more detailed power report will soon be produced, and power sources added.



Devices
-------

The virtual platform is by default simulating only a stand-alone chip. Device models such as camera, flash or microphones can be connected in order to run full applications.

The devices to be simulated must be specified using the standard SDK feature for customizing peripherals, see :ref:`periph_config`.


GDB
---

To use GDB with the virtual platform, the Pulp debug bridge must be used to connect to the virtual platform
and to open an RSP server where GDB can be connected to communicate with the virtual platform as if the real
chip would be used.

For that, the application must first be configured with the following option: ::

  $ make conf gdb=1

Then the virtual platform must be launched with the usual command: ::

  $ make run

This should start the virtual platform but quickly stop waiting for an incoming connection with the following 
message: ::

  Proxy listening on port 37539

The Pulp debug bridge must then be launched and connected from another terminal with the same SDK configured
in the same way, with the following command: ::

  $ plpbridge --config=<config file path> gdb wait --rsp-port=2345

The config file must be the one generated for the application being debugged. Its path
can be seen on the pulp-run command which is generated when *make run* is executed.

The RSP port can be any port, it just needs to correspond to the one used when opening GDB.

Then from another terminal, GDB must be launched and connected: ::

  $ riscv32-unknown-elf-gdb
  $ (gdb) target remote :2345

The port specified here must be the same as the one specified in the plpbridge command.

There are currently a few GDB issues. First it is using by default the 64bit mode if no binary is specified.
So a binary must always be specified until this is fixed (with option file). Then breakpoints with compressed instructions generate a GDB error. To avoid it, the following gdb property must be set: ::

  $ (gdb) set riscv use_compressed_breakpoint off

There are also a few limitations, watchpoints are not supported.