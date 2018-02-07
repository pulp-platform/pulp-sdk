Configuration
=============

Usage
-----

When the SDK is initialized, a default configuration is chosen depending on the script used to initialize the SDK.
Then it is possible to customize this configuration through the *CONFIG_OPT* makefile option.

The following section briefly describes the main interesting aspects to customize. For a complete list of possible configuration items, refer to the help returned by the command *make help_config*.

If a specific configuration is required by an application, it is also possible to customize the configuration only for this application by putting the CONFIG_OPT option directly into the makefile.

Peripherals
-----------

The default configuration is usually only activating the processor and so no peripheral is connected. This section documents how to activate specific peripherals.

Camera
......

The camera can be activated with this option: ::

  $ make conf CONFIG_OPT="camera=himax"

The parameter given to the camera option specifies the camera model to be activated. There are 2 camera models available, either himax or ov7670.

It is also possible to specify the input image to the camera by using the following option: ::

  $ make conf CONFIG_OPT="camera=himax camera/image=frame0.img"

The format of this file should be like the following, one pixel per line, in hexadecimal, with 2 digits per color, in RGB order: ::

  E7E3E0
  E6E3E0

