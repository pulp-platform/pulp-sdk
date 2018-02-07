SDK installation and setup
==========================

Installation from the tarball
-----------------------------

If the SDK is retrieved as a tarball, it must first be unpacked (the name of the tarball can vary depending on the target): ::

  $ tar xvfj sdk.tar.bz2

Then, the unpacked SDK must be setup by executing this script: ::

  $ cd <unpacked SDK directory>
  $ ./setup.sh

The SDK is then ready to be used. Anytime a new terminal is opened, the environment must be 
setup by sourcing one of the .sh file in the SDK top directory, depending on the desired architecture: ::

  $ source wolfe.sh