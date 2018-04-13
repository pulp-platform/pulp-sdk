Board
-----

Board setup
...........

In case one of the pulp board must be used, the USB connection to the board through the FTDI cable must be setup.
Most of the time a rule like the following must be added under /etc/udev/rules.d/, for example in /etc/udev/rules.d/10-ftdi.rules: ::

        ATTR{idVendor}=="15ba", ATTR{idProduct}=="002b", MODE="0666", GROUP="dialout"
        ATTR{idVendor}=="15ba", ATTR{idProduct}=="002a", MODE="0666", GROUP="dialout"

The user should also need to be in the *dialout* group.

These rules are suitable for Olimex ARM-USB-OCD-H  and ARM-USB-TINY-H cables but a different one will be needed
for other cables.


GDB
---

To be able to connect GDB to the board, the debug bridge must open an RSP server on which
GDB will connect.

For that, the application must be configured with the following option: ::

  $ make conf gdb=1

Then when the application is launched, you should see the following message telling that the RSP
server is opened: ::

  $ RSP server opened on port 1234


Then from another terminal, GDB must be launched and connected: ::

  $ riscv32-unknown-elf-gdb
  $ (gdb) target remote :1234

The port specified here must be the same as the one reported earlier.

There are currently a few GDB issues. First it is using by default the 64bit mode if no binary is specified.
So a binary must always be specified until this is fixed (with option file). Then breakpoints with compressed instructions generate a GDB error. To avoid it, the following gdb property must be set: ::

  $ (gdb) set riscv use_compressed_breakpoint off

There are also a few limitations, watchpoints are not supported.

