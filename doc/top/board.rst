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
