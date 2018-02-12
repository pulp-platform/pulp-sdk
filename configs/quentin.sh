#!/bin/bash

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PULP_CURRENT_CONFIG=quentin@user_config_file=${scriptDir}/json/quentin.json

unset PULP_CURRENT_CONFIG_ARGS

source ${scriptDir}/../init.sh