# PULP SDK build process


## About

This module is the build process for the Pulp SDK. It can be used to do everything needed to build the SDK.

There are several ways of building the SDK, depending on how the dependencies are managed:

 - Section *SDK build with independant dependencies build* shows how to compile the SDK and the dependencies separately. This is the simplest way of building the SDK and should be the one used by default.
 - Section *SDK build with dependencies download* shows how to download precompiled packages for the dependencies and then build the SDK using these precompiled packages. This will only work if you have access to the package server.
 - Section *Full SDK build including dependencies build* shows how to compile everything, SDK plus dependencies, using the SDK build process. This is convenient to let the SDK choose the versions of the dependencies which are suitable for the version of the SDK which was retrieved. It is however only useful to get an environment which is stricly the same used for validating the SDK.


## Linux dependencies

Here are the required system dependencies for building the SDK and its dependencies.

### Ubuntu 16.04

Starting from a fresh Ubuntu 16.04 distribution, here are the commands to be executed to get all required dependencies:

    $ sudo apt install git python3-pip gawk texinfo libgmp-dev libmpfr-dev libmpc-dev swig3.0 libjpeg-dev lsb-core doxygen python-sphinx sox graphicsmagick-libmagick-dev-compat
    # sudo pip3 install artifactory twisted prettytable sqlalchemy pyelftools openpyxl xlsxwriter



## SDK build with independant dependencies build

### Dependencies build

Have a look at the dependencies documentation to see how to build them.

You can have a look here for the toolchain: git@github.com:pulp-platform/pulp-riscv-gnu-toolchain.git


### Dependencies setup

All the dependencies required to build the SDK must be setup through environment variables.

The toolchain must be built separately and the following environment variable should 
point to it:

    $ export PULP_RISCV_GCC_TOOLCHAIN=<path to the folder containing the bin folder of the toolchain>

RTL platforms should also be built separately (see the platform documentation for that) and the following
environment variable must point to the folder where the platform was installed (this example is for pulpissimo):

    $ export VSIM_PATH=<pulpissimo root folder>/sim


### SDK sources

First get the sources of the SDK top module:

    $ git clone git@iis-git.ee.ethz.ch:pulp-sw/pulp-sdk.git -b <sdb branch or tag>


Take the *master* branch of the SDK if you want a stable branch with the latest features. The branch *integration* can be taken to get more recent features but may not be fully stable.

Then go inside the downloaded folder:

    $ cd pulp-sdk

### Target and platform selection

Before building the SDK, the target for which the SDK will be built must be selected by sourcing one of the file
in directory *configs* for example like the following:

    $ source configs/pulpissimo.sh

Once your application is compiled, you will need to run it on a platform. The platform can
be the RTL simulator, the FPGA, the virtual platform or the FPGA.
You can configure the platform to be used by sourcing one of the *platform-*.sh* file, like the following:

    $ source configs/platform-rtl.sh

Note that anytime the target file is sourced, you must source again the one for the platform.



Once your application is compiled, you will need to run it on a platform. The platform can
be the RTL simulator, the FPGA, the virtual platform or the FPGA.
You can configure the platform to be used by sourcing one of the *platform-*.sh* file, like the following:

```
source configs/platform-rtl.sh
```

Note that anytime the target file is sourced, you must source again the one for the platform.



### SDK build

You can then build the SDK with this command:

    $ make all



### SDK setup

The SDK build should have installed the SDK under *pkg/sdk/dev*.

Once you want to use it to compile and run applications, you first need to setup the SDK by sourcing the *sourceme.sh* file which is inside the installation folder:

$ source pkg/sdk/dev/sourceme.sh


The next time you open a new terminal, you have to source again the file for the target (i.e. *configs/pulpissimo.sh) and then the file in the SDK installation folder.

After these steps, the SDK is ready to be used, you can have a look at section *Documentation* for more information.

For a quick hello test, you can get some examples here:

    $ git clone git@github.com:pulp-platform/pulp-rt-examples.git

Then you can go to the folder *pulp-rt-examples/hello* and execute:

    $ make clean all run




## SDK build with dependencies download

This section is only relevant if the toolchains are downloaded precompiled. If you don't have access to the artifactory server, follow the other sections to also build the toolchains.

## Package server configuration

To download the dependencies, the access to the package server (Artifactory) must be configured. This is reserved for internal usage, more information can be retrieved on this project: https://iis-git.ee.ethz.ch/pulp-sw/pulp-sdk-internal

### First build

All the SDK dependencies can be downloaded if they are compatible with the Linux distribution used.

For that first get the top SDK module, after you have configured your ssh key in gitlab:

    $ git clone git@iis-git.ee.ethz.ch:pulp-sw/pulp-sdk.git -b <sdb branch or tag>

Take the *master* branch of the SDK if you want the latest features. However this branch may not be fully stable. The *release* branch can be retrieved to get the latest stable release. Otherwise any other branch or tag. There is always a tag whose name is the one of the SDK tag which is released.

Then go to the module folder and execute:

    $ git submodule update --init

Configure the targets for which the SDK must be built (you can have a look at section *Possible configurations* to see what to set):

    $ export PULP_CURRENT_CONFIG=system=wolfe

Initialize the SDK:

    $ source init.sh

Download the dependencies:

    $ plpbuild --p sdk deps

Checkout the source code:

    $ plpbuild --p sdk checkout

And build the SDK:

    $ plpbuild --p sdk build --stdout

Finally, to test the SDK package, first generate a sourceme file and source it:

    $ plpbuild --p sdk env
    $ source sourceme.sh

Note that for SDK users, only sourcing this file is enough to configure the SDK and compile applications.

Download some tests, and try one of them:

    $ git clone git@iis-git.ee.ethz.ch:pulp-tests/rt-tests.git
    $ cd rt-tests/rt/threads
    $ make clean all run


### Update

If the SDK has already been built and needs to be updated, the SDK can be rebuilt on top of the previous one.

For that first update the sources:

    $ git pull
    $ git submodule update
    $ plpbuild --p sdk checkout

Download the new dependencies:

    $ plpbuild --p sdk deps

Then rebuild everything, including a command to clean the SDK, as some modules do not work well with incremental compilation:

    $ plpbuild --p sdk clean build --stdout

After these steps, the new SDK is ready to be used.



## Full SDK build including dependencies build

### First build

The SDK has some dependencies like the toolchains which must be built before the SDK can be built.
To build everything including all the dependencies, first get the top SDK module, after you have configured your ssh key in gitlab:

    $ git clone git@iis-git.ee.ethz.ch:pulp-sw/pulp-sdk.git -b <sdb branch or tag>

Take the *master* branch of the SDK if you want the latest features. However this branch may not be fully stable. The *release* branch can be retrieved to get the latest stable release. Otherwise any other branch or tag. There is always a tag whose name is the one of the SDK tag which is released.

Then go to the module folder and execute:

    $ git submodule update --init

Configure the targets for which the SDK must be built (you can have a look at section *Possible configurations* to see what to set):

    $ export PULP_CURRENT_CONFIG=system=wolfe

Initialize the SDK:

    $ source init.sh

Checkout the source code:

    $ plpbuild --p sdk checkout --deps

And build everything:

    $ plpbuild --p sdk build --deps --stdout

Finally, to test the SDK package, first generate a sourceme file and source it:

    $ plpbuild --p sdk env
    $ source sourceme.sh

Note that for SDK users, only sourcing this file is enough to configure the SDK and compile applications.

Download some tests, and try one of them:

    $ git clone git@iis-git.ee.ethz.ch:pulp-tests/rt-tests.git
    $ cd rt-tests/rt/threads
    $ make clean all run

### Update

If the SDK has already been built and needs to be updated, the SDK can be rebuilt on top of the previous one.

For that first update the sources:

    $ git pull
    $ git submodule update
    $ plpbuild --p sdk checkout --deps

Then rebuild everything, including a command to clean the SDK, as some modules do not work well with incremental compilation:

    $ plpbuild --p sdk clean build --deps --stdout

After these steps, the new SDK is ready to be used.


## Possible configurations

Here is a list of possible values for the configuration:

    $ export PULP_CURRENT_CONFIG=system=pulpissimo
    $ export PULP_CURRENT_CONFIG=system=pulp
    $ export PULP_CURRENT_CONFIG=system=gap
    $ export PULP_CURRENT_CONFIG=system=vivosoc3

## Documentation

The documentation is inside the SDK. you can see it by openining the file <SDK path>/doc/sdk/index.html in a Web browser.

