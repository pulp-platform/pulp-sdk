PulpOS
=============

UART output configuration
-------------------------

The runtime is by default using for the printf the easiest device for the configured platform.
It is possible to force the printf to use the UART by configuring the runtime with this command in the application folder: ::

  make conf io=uart CONFIG_OPT="iodevs/uart/baudrate=115200"


Booting from flash
------------------

Some platforms like the board are by default booting through JTAG. It is possible to force the runtime to boot from flash by configuring it with this command: ::

  make conf io=uart CONFIG_OPT="iodevs/uart/baudrate=115200" boot=dev_hyper

When running the command for compiling and running, this will produce an image for the flash and upload it. Running the application will just interact with the boot code to make it boot from flash.
After the board is powered down, it is possible to just run the application as the binary is kept in the flash.