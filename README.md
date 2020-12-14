# SDK setup

This is the latest version of the PULP SDK, which is under active development.  The previous (now legacy) version, which is no longer supported, is on the [`v1` branch](https://github.com/pulp-platform/pulp-sdk/tree/v1).

## Getting started

These instructions were developed using a fresh Ubuntu 18.04 Bionic Beaver 64-Bit.

The following packages needed to be installed:

~~~~~shell
sudo apt-get install -y build-essential git libftdi-dev libftdi1 doxygen python3-pip libsdl2-dev curl cmake libusb-1.0-0-dev scons gtkwave libsndfile1-dev rsync autoconf automake texinfo libtool pkg-config libsdl2-ttf-dev
~~~~~

Source the file corresponding to the desired configuration:

~~~~~shell
source configs/pulp-open.sh
~~~~~

At least gcc 4.9.1 is needed. If the default one is not correct, CC and CXX can be set to
point to a correct one.

GAP_RISCV_GCC_TOOLCHAIN must be defined to point to the riscv compiler.

## SDK build

Compile the SDK with this command:

~~~~~shell
make build
~~~~~

## Test execution

Some examples are availaible at https://github.com/GreenWaves-Technologies/pmsis_tests

Then, go to a test, for example pmsis_tests/quick/cluster/fork/, and execute:

~~~~~shell
make clean all run
~~~~~

This will by default execute it on gvsoc, and you can configure the RTL platform with this command:

~~~~~shell
make clean all run platform=rtl
~~~~~

Notice that the environment variable `VSIM_PATH` should be set to the directory where the RTL platform has been built.
This is typically done by sourcing the `setup/vsim.sh` file from the main folder of the RTL platform.

## Getting a single module

You can also use this SDK to just get one module and its dependencies, by doing:

~~~~~shell
make <module name>.all
~~~~~

For example, to get GVSOC, you can execute:

~~~~~shell
make gvsoc.all
~~~~~

Note that you still need to source a config before doing that.
