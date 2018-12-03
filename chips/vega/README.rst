How to install Vega SDK
-----------------------

Execute the following commands: ::

  git clone git@github.com:pulp-platform/pulp-sdk.git
  cd pulp-sdk
  source configs/vega.sh
  make deps all

You need to setup PULP_ARTIFACTORY_USER to be able to download the toolchain.

The "master" branch will be taken, this branch is always stable. Branch "integration" can also be taken to get most recent developments but is not always stable.


How to setup Vega SDK after it has been installed
-------------------------------------------------

Execute the following commands: ::

  cd pulp-sdk
  source sourceme.sh

The path to the RTL platform must be configured with his environment variable: ::

  export VSIM_PATH<directory containing modelsim.ini>



How to update Vega SDK after it has already been installed
----------------------------------------------------------

Execute the following commands: ::
  cd pulp-sdk
  source configs/vega.sh
  git pull
  make deps all

The SDK should be setup again after that.



How to recompile the runtime after one of the file has been modified
--------------------------------------------------------------------

First make sure the SDK is setup.

In case one of the archi, hal or rt file has been modified, execute this command to recompile the whole runtime: ::

  plpbuild --g runtime build --stdout

To be more selective, the set of modules to be recompiled can be given like that: ::

  plpbuild --m pulp-rt --m archi --m hal build --stdout



How to change test configuration
--------------------------------

A test is compiled and launched with a default configuration which can be modified with this command: ::

  make conf gui=1

Then the test can be recompiled and run: ::

  make all run

To come back to the default configuration, execute this: ::

  make conf

To make a configuration permanent, it can be put directly in the makefile.



How to add a file to archi and hal modules
------------------------------------------

The file should be added in the appropriate hierarchy under include.

When archi or hal modules are built, only the files needed for Vega are copied. If a file is added, the SConstruct file must also be updated so that the file is also copied. The easiest at first is to copy it when the vega config is detected (at the end of the SConstruct file).

Then just recompile the whole runtime to make it available to the test.


How to generate an archi file from xls sheet
--------------------------------------------

There is a new tool called regmap which is installed by module pulp-tools. You can
find it there if you need to modify it: pulp-tools/regmap

You can first see on the terminal how it is parsing the xls file with such a command: ::

  regmap --name=timer    --input-xls=doc/TIMER_UNIT_reference.xlsx  --table

The header file can be generated with this command: ::

  regmap --name=timer    --input-xls=doc/TIMER_UNIT_reference.xlsx  --header=include/archi/timer/timer_v2_new.h

Be careful to not replace the old file as it is still used by current current and virtual platform. Instead create a new one and include it from the old file, like it is done for the timer file.

If you modify the tool, be careful to not break it for other formats, it also support JSON as input and output files, and RST as output file.

The produced header file can be pushed to archi module and the command for generating it in the archi Makefile so that we can easily regenerate all the files in case we fix something in the script.