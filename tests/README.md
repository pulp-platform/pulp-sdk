# PMSIS TESTS

This is a set of tests with the aim of testing and building custom applications on GVSoC.
Please, before running a test have a look at the specific Makefile and its options. Every tests is executable on GVSoC using the following commands:

~~~~~shell
cd <TEST_FOLDER>
make clean all run <MAKEFILE_OPTIONS>
~~~~~

The procedure below needed to be executed for the first time before try to compile and run an example.

## Up&Run the Environment

### PULP-Toolchain

These instructions were developed using a fresh Ubuntu 18.04.

This version requires PULP toolchain to compile the application exploiting pulp features. PULP toolchain is available at: https://github.com/pulp-platform/pulp-riscv-gnu-toolchain. Below there is only the needed steps to compile the toolchain. For more about the installation, please refer to proper README.

You can choose also its precompiled version, exploring: https://github.com/pulp-platform/pulp-riscv-gnu-toolchain/releases/tag/v1.0.16

If you have chosen the former, this repository uses submodules. You need the --recursive option to fetch the submodules automatically

~~~~~shell
git clone --recursive https://github.com/pulp-platform/pulp-riscv-gnu-toolchain
~~~~~

Alternatively :

~~~~~shell
git clone https://github.com/pulp-platform/pulp-riscv-gnu-toolchain
cd pulp-riscv-gnu-toolchain
git submodule update --init --recursive
~~~~~

Several standard packages are needed to build the toolchain. On Ubuntu,
executing the following command should suffice:

~~~~~shell
sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev
~~~~~

To build the Newlib cross-compiler for all pulp variants, pick an install path. If you choose,
say, `/opt/riscv`, then add `/opt/riscv/bin` to your `PATH` now with:

~~~~~shell
export PATH=/opt/riscv/bin:$PATH
~~~~~

Then, simply
run the following command, where `--prefix` must be the same chose before as install path:

~~~~~shell
./configure --prefix=/opt/riscv --with-arch=rv32imc --with-cmodel=medlow --enable-multilib
make
~~~~~

This will use the multilib support to build the libraries for the various cores (riscy, zeroriscy and so on). The right libraries will be selected depending on which compiler options you use.

### PULP-SDK

These instructions were developed using a fresh Ubuntu 18.04.

First of all, clone the repository.

~~~~~shell
git clone https://github.com/pulp-platform/pulp-sdk
cd pulp-sdk
~~~~~

The following packages needed to be installed:

~~~~~shell
sudo apt-get install -y build-essential git libftdi-dev libftdi1 doxygen python3-pip libsdl2-dev curl cmake libusb-1.0-0-dev scons gtkwave libsndfile1-dev rsync autoconf automake texinfo libtool pkg-config libsdl2-ttf-dev
~~~~~

The SDK also requires the `argcomplete` Python package. You can install it for the local user with:
~~~~~shell
pip install --user argcomplete
~~~~~
Omit `--user` to install at system level instead, which will probably require admin rights.

You may need also `six` and `pyelftools`, and you can repeat the previous commands like like:
~~~~~shell
pip install --user pyelftools six
~~~~~

At least gcc 4.9.1 is needed. If the default one is not correct, CC and CXX can be set to point to a correct one

Now define the path in which there is toolchain bin folder (in this guide /opt/riscv):

~~~~~shell
export PULP_RISCV_GCC_TOOLCHAIN=<INSTALL_DIR>
~~~~~

Source the file corresponding to the desired configuration:

~~~~~shell
source configs/pulp-open.sh
~~~~~

Build the sdk (and compile GVSoC) with:

~~~~~shell
make build
~~~~~

If you would change the target configuration or you would modify GVSoC, you should recompile the sdk (better from a fresh terminal).

To check if the environment is correctly set:

~~~~~shell
cd tests/hello
make clean all run
~~~~~
