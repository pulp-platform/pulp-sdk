# SDK setup

This is the latest version of the PULP SDK, which is under active development. The previous (now legacy) version, which is no longer supported, is on the [`v1` branch](https://github.com/pulp-platform/pulp-sdk/tree/v1).

## Getting started

These instructions were developed using a fresh Ubuntu 18.04 Bionic Beaver 64-Bit.

The following packages needed to be installed:

~~~~~shell
sudo apt-get install -y build-essential git libftdi-dev libftdi1 doxygen python3-pip libsdl2-dev curl cmake libusb-1.0-0-dev scons gtkwave libsndfile1-dev rsync autoconf automake texinfo libtool pkg-config libsdl2-ttf-dev
~~~~~

The SDK also requires the `argcomplete` Python package. You can install it for the local user with:
~~~~~shell
pip install --user argcomplete
~~~~~
Omit `--user` to install at system level instead, which will probably require admin rights.

This version requires PULP toolchain to compile the application exploiting pulp features. PULP toolchain is available at: https://github.com/pulp-platform/pulp-riscv-gnu-toolchain

You can choose also its precompiled version, exploring: https://github.com/pulp-platform/pulp-riscv-gnu-toolchain/releases/tag/v1.0.16

Please, refer to the corresponding README for the installation.

Once PULP toolchain is correctly installed, define the path in which there is toolchain bin folder:

~~~~~shell
export PULP_RISCV_GCC_TOOLCHAIN=<INSTALL_DIR>
~~~~~

Source the file corresponding to the desired configuration:

~~~~~shell
cd pulp-sdk
source configs/pulp-open.sh
~~~~~

At least gcc 4.9.1 is needed. If the default one is not correct, CC and CXX can be set to
point to a correct one. To check if gcc has the right version:

~~~~~shell
gcc --version
~~~~~

Please, refer to official guide to update gcc if is needed.

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

This will by default execute it on gvsoc (platform=gvsoc), and you can configure the RTL platform with this command:

~~~~~shell
make clean all run platform=rtl
~~~~~

Notice that the environment variable `VSIM_PATH` should be set to the directory where the RTL platform has been built.
This is typically done by sourcing the `setup/vsim.sh` file from the main folder of the RTL platform.

## Application: CNNs at the Edge

To run pre-generated real-world networks, such as MobileNetV1:

~~~~~shell
cd applications/MobileNetV1
make clean all run platform=<PLATFORM> CORE=<NUM_CORES>
~~~~~

### Nemo + Dory + Pulp-NN

Our vertical flow allows to deploy optimized QNNs on low-power and low-resources MCUs, starting from a Pytorch model.

#### Nemo

[\[Nemo\]](https://github.com/pulp-platform/nemo) is a framework for Deep Neural Networks layer-wise quantization.
He starts from a common Pytorch project and produces an equivalent quantized model, which well suits the usually integer MCUs.
Its output are a `.onnx` as quantized model and several `.txt` as set of input and weigths of the network, also including the golden activations to checks the output of every network's layer.
Please refer to its README for more details and [\[here\]](https://colab.research.google.com/drive/1qdc__9uZAGk9TzylsH3S8tB73bWcA2cA?usp=sharing#scrollTo=xelcF1jxkAit) you can find a Colab project and a very detailed tutorial on how to get started with Nemo.

#### Dory

[\[Dory\]](https://github.com/pulp-platform/dory) is an automatic tool to generate and directly deploy MLP/CNNs on Pulp family boards, exploiting [\[Pulp-NN\]](https://github.com/pulp-platform/pulp-nn) as optimized back-end.

Dory has a complete and autonomous testsuite, named [\[Dory-Example\]](https://github.com/pulp-platform/dory_examples), which is periodically updated, and please refer to its README for more details.
To generate the code and run one of these examples:

~~~~~shell
cd dory/dory_examples/
python3 network_generate --network_dir <e.g., ./examples/MobileNetV1/>
cd application
make clean all run platform=<PLATFORM> CORE=<NUM_CORES>
~~~~~

where you should choose `CORE=8` if you want to test the network on pulp cluster with all of the eight cores active (by default only 1 is set).

To set up and execute a custom application, firstly, copy your file `network.onnx` and files `out_layer{i}.txt` in a single folder (e.g., `pulp-sdk/application/MyCustomNetwork/`) and then:

~~~~~shell
cd dory/dory_examples/
python3 network_generate --network_dir <pulp-sdk/application/MyCustomNetwork/>
cd application
make clean all run platform=<PLATFORM> CORE=<NUM_CORES>
~~~~~

You can use L1 and L2 memory constraints to specify the amount of memory used inside the application. Please refer to Dory and Dory-example READMEs for more details.
