# PULP SDK build process


## About

This module is the build process for the PULP SDK. It can be used to do everything needed to build the SDK.

There are several ways of building the SDK, depending on how the dependencies are managed:

 - Section *SDK build with independant dependencies build* shows how to compile the SDK and the dependencies separately. This is the simplest way of building the SDK and should be the one used by default.
 - Section *SDK build with dependencies download* shows how to download precompiled packages for the dependencies and then build the SDK using these precompiled packages. This will only work if you have access to the package server.
 - Section *SDK release download* shows how to download everything as precompiled releases. This will only work if you have access to the package server.


## Linux dependencies

Here are the required system dependencies for building the SDK and its dependencies.

### Ubuntu 16.04

Be carefull that this procedure has been validated with a default installation of Ubuntu 16.04, so you may have some issues if one of the dependency like python3 is not the default one coming with Ubuntu 16.04 (python 3.5).

Starting from a fresh Ubuntu 16.04 distribution, here are the commands to be executed to get all required dependencies:

    $ sudo apt install git python3-pip python-pip gawk texinfo libgmp-dev libmpfr-dev libmpc-dev swig3.0 libjpeg-dev lsb-core doxygen python-sphinx sox graphicsmagick-libmagick-dev-compat libsdl2-dev libswitch-perl libftdi1-dev cmake scons libsndfile1-dev
    $ sudo pip3 install artifactory twisted prettytable sqlalchemy pyelftools 'openpyxl==2.6.4' xlsxwriter pyyaml numpy configparser pyvcd
    $ sudo pip2 install configparser

In case you face a problem regarding python dependencies you may also want to check if the version reported by python3 is matching the python version reported by pip:

    $ pip --version
    $ python3 --version

If they differ you can try to install and manage python packages as follows:

    $ sudo python3 -m pip install --upgrade pip
    $ sudo python3 -m pip install artifactory twisted prettytable sqlalchemy pyelftools openpyxl xlsxwriter pyyaml numpy configparser pyvcd

For more recent Ubuntu distributions it is needed to configure the default gcc version to 5 as more recent gcc versions make the build fail. You can get more information about how to set default gcc version here: https://askubuntu.com/questions/26498/how-to-choose-the-default-gcc-and-g-version

In case you are using the debug bridge for example to connect to a board, you also need to install the prerequisites indicated on this page: https://github.com/pulp-platform/pulp-debug-bridge

### Scientific Linux 7.4/7.5/7.6

Starting from a fresh Scientific Linux 7.4/7.5/7.6 distribution, here are the commands to be executed to get all required dependencies:

    $ sudo yum install git python36-pip python36-devel gawk texinfo gmp-devel mpfr-devel libmpc-devel swig libjpeg-turbo-devel redhat-lsb-core doxygen python-sphinx sox GraphicsMagick-devel ImageMagick-devel SDL2-devel perl-Switch libftdi-devel cmake cmake3 scons
    $ sudo python3 -m pip install --upgrade pip
    $ sudo python2 -m pip install --upgrade pip
    $ sudo python3 -m pip install artifactory twisted prettytable sqlalchemy pyelftools openpyxl xlsxwriter pyyaml numpy configparser pyvcd
    $ sudo python2 -m pip install configparser

In case you are using the debug bridge for example to connect to a board, you also need to install the prerequisites indicated on this page: https://github.com/pulp-platform/pulp-debug-bridge

## Standard SDK build

### Dependencies build

Have a look at the dependencies documentation to see how to build them.

You can have a look [here](https://github.com/pulp-platform/pulp-riscv-gnu-toolchain.git) for the toolchain.


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

    $ git clone https://github.com/pulp-platform/pulp-sdk.git -b <sdb branch or tag>

Take the *master* branch of the SDK if you want a stable branch with the latest features. The branch *integration* can be taken to get more recent features but may not be fully stable.

Note that by default the SDK will fetch other modules using HTTPS. In case you prefer
to use SSH, you can define this environment variable:

    $ export PULP_GITHUB_SSH=1

Then go inside the downloaded folder:

    $ cd pulp-sdk

### Target and platform selection

Before building the SDK, the target for which the SDK will be built must be selected by sourcing one of the files
in directory *configs* for example like the following:

    $ source configs/pulpissimo.sh

Once your application is compiled, you will need to run it on a platform. The platform can
be the RTL simulator, FPGA, or virtual platform.
You can configure the platform to be used by sourcing one of the *platform-*.sh* file, like the following:

    $ source configs/platform-rtl.sh

Note that anytime the target file is sourced, you must source again the one for the platform.



Once your application is compiled, you will need to run it on a platform. The platform can
be the RTL simulator, FPGA, or virtual platform.
You can configure the platform to be used by sourcing one of the *platform-*.sh* file, like the following:

```
source configs/platform-rtl.sh
```

Note that anytime the target file is sourced, you must source again the one for the platform.



### SDK build

You can then build the SDK with this command:

    $ make all



### SDK setup

The SDK build should have installed the SDK under `pkg/sdk/dev`.

Once you want to use it to compile and run applications, you first need to setup the SDK by sourcing the *sourceme.sh* file which is inside the installation folder:

    $ source pkg/sdk/dev/sourceme.sh


The next time you open a new terminal, you have to source again the file for the target (i.e. *configs/pulpissimo.sh) and then the file in the SDK installation folder.

After these steps, the SDK is ready to be used, you can have a look at section *Documentation* for more information.

For a quick hello test, you can get some examples here:

    $ git clone https://github.com/pulp-platform/pulp-rt-examples.git

Then you can go to the folder `pulp-rt-examples/hello` and execute:

    $ make clean all run




## SDK build with dependencies download

This section is only relevant if the toolchains are downloaded precompiled. If you don't have access to the artifactory server, follow the other sections to also build the toolchains.

### Package server configuration

To download the dependencies, the access to the package server (Artifactory) must be configured. This is reserved for internal usage, more information can be retrieved on this [project](https://iis-git.ee.ethz.ch/pulp-sw/pulp-sdk-internal).

The build process will try to download packages suitable for the detected Linux distribution. In case this is not suitable, you can force the distribution to be used by defining this environment variable:

    $ export PULP_ARTIFACTORY_DISTRIB=<distrib>

This can currently be set to `CentOS_7` or `Ubuntu_16`.


### SDK sources

First get the sources of the SDK top module:

    $ git clone https://github.com/pulp-platform/pulp-sdk.git -b <sdb branch or tag>


Take the *master* branch of the SDK if you want a stable branch with the latest features. The branch *integration* can be taken to get more recent features but may not be fully stable.

Note that by default the SDK will fetch other modules using SSH which requires uploading
an SSH key. In case you prefer to use HTTPS to avoid that, you can define this environment variable:

    $ export PULP_GITHUB_HTTPS=1

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


### SDK dependencies download

You can download the SDK dependencies with this command:

    $ make deps



### SDK build

You can then build the SDK with this command:

    $ make all



### SDK setup

Finally, to test the SDK package, you have to generate a sourceme file and source it:

    $ make env
    $ source sourceme.sh

Note that for SDK users, only sourcing this file is enough to configure the SDK and compile applications.

Also, the next time you open a new terminal, you can just source the *sourceme.sh* file.

After these steps, the SDK is ready to be used, you can have a look at section *Documentation* for more information.

For a quick hello test, you can get some examples here:

    $ git clone https://github.com/pulp-platform/pulp-rt-examples.git

Then you can go to the folder *pulp-rt-examples/hello* and execute:

    $ make clean all run


### Update

If the SDK has already been built and needs to be updated, the SDK can be rebuilt on top of the previous one.

For that first update the sources:

    $ git pull

Then redo all the commands, including a command to clean the SDK, as some modules do not work well with incremental compilation:

    $ make deps clean all env



After these steps, the new SDK is ready to be used.







## SDK release download

This section is only relevant if all the packages can be downloaded precompiled. If you don't have access to the artifactory server, follow the other sections to build the packages.

### Getting the top repositories

As this method of getting the SDK allows selecting the SDK amongst several releases, you must
first get this top repository which knows which releases are available:

    $ git clone git@github.com:pulp-platform/pulp-sdk-release.git
    $ cd pulp-sdk-release


### Package server configuration

To download the dependencies, the access to the package server (Artifactory) must be configured. This is reserved for internal usage, more information can be retrieved on this [project]( https://iis-git.ee.ethz.ch/pulp-sw/pulp-sdk-internal). Be careful to configure the artifactory credentials using the .wgetrc file as the packages will be downloaded through wget.

The build process will try to download packages suitable for the detected Linux distribution. In case this is not suitable, you can force the distribution to be used by defining this environment variable:

    $ export PULP_ARTIFACTORY_DISTRIB=<distrib>

This can currently be `CentOS_7` or `Ubuntu_16`. If your distribution is not supported, you can try
to download the packages for a distribution which is close to yours.

You can also specify the distribution on the command-line when invoking make:

    $ make list distrib=Ubuntu_16



### SDK and dependency download

You can get the list of available SDKs for the distribution you selected with this command:

    $ make list

Then you can download the SDK you want by executing the following command with the appropriate
SDK version:

    $ make version=<version> get

You should see packages being downloaded through wget. Otherwise there is probably something wrong
with the artifactory server configuration.

Once the SDK is downloaded, you have to source the file indicated in the terminal to setup the downloaded SDK.



### Target and platform selection

Once the SDK is selected, you can get the list of supported targets with this command:

    $ make targets

Before using the SDK, you have to configure it for a specific target by sourcing the file
indicated next to the target you want to select.

You have to do the same for the platform you want to use, and you can get the list of platforms
with this command:

    $ make platforms




## Documentation

The documentation is inside the SDK folder. You can view it by opening the file `<SDK path>/doc/sdk/index.html` in a web browser.

