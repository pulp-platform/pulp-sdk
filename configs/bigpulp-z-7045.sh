#!/bin/bash

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PULP_CURRENT_CONFIG=bigpulp-z-7045@config_file=chips/bigpulp-z-7045/bigpulp-z-7045.json

unset PULP_CURRENT_CONFIG_ARGS

if [ -e ${scriptDir}/../init.sh ]; then
    source ${scriptDir}/../init.sh
fi
