#!/bin/bash

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PULP_CURRENT_CONFIG=oprecompkw_sfloat@config_file=chips/oprecompkw_sfloat/oprecompkw_sfloat.json

unset PULP_CURRENT_CONFIG_ARGS

if [ -e ${scriptDir}/../init.sh ]; then
    source ${scriptDir}/../init.sh
fi
