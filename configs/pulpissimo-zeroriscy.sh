#!/bin/bash

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PULP_CURRENT_CONFIG=pulpissimo-zeroriscy@config_file=chips/pulpissimo-zeroriscy/pulpissimo-zeroriscy.json

unset PULP_CURRENT_CONFIG_ARGS

if [ -e ${scriptDir}/../init.sh ]; then
    source ${scriptDir}/../init.sh
fi
