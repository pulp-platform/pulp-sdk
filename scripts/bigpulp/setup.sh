#!/usr/bin/env bash

# Workaround to manually resolve cyclic dependencies during initial SDK setup

# The following commands should work cleanly.
set -e
source configs/bigpulp-z-7045.sh
source configs/platform-hsa.sh
make deps
for m in \
    json-tools \
    pulp-tools \
    pulp-configs \
    pulp-rules \
    archi \
    hal \
    debug-bridge2 \
    debug-bridge; \
do
    plpbuild --m $m checkout build --stdout
done

# Building `pulp-rt` will fail, but this is to be expected.
set +e
plpbuild --m pulp-rt checkout build --stdout
echo 'NOTE: The failure of building `pulp-rt` at this point is known and can be tolerated.'

# Now that the `pulp-rt` headers are installed, we can go ahead and install `archi-host` followed by
# a re-installation of the entire SDK.  At some point, this will fail because two of the runtime
# variants ('tiny' and 'bare') are not aligned with the main runtime, causing the wrong header files
# to be deployed.
plpbuild --m archi-host checkout build --stdout
make all

# We fix this by forcing the `pulp-rt` headers, followed by the final compilation of `libvmm`.
find runtime/pulp-rt/include -type f -exec touch {} +
plpbuild --m pulp-rt build --stdout
plpbuild --m libvmm checkout build --stdout
plpbuild --m libgomp checkout build --stdout
plpbuild --m runner checkout build --stdout

make env
