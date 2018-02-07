Common issues
-------------

Issue with DPI models with Questasim
....................................

It can happen that the following error is raised by Questasim when it is launched: ::

  Could not compile 'STUB_SYMS_OF_libdpimodels.so'

Some peripherals such as cameras and microphones are modeled using SystemVerilog DPI.

It is possible that the compiler used to compile the DPI models is not compatible with the compiler used by Questasim to finalize the compilation of the DPI models. If such an error happen, forcing the compiler used by Questasim can solve this issue. To do this, define which C++ compiler which should be used: ::

  export QUESTA_CXX=/usr/bin/g++

This will propagate it to Questasim using the option *-dpicpppath*.