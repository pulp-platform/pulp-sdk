#!/bin/bash

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PULP_CURRENT_CONFIG=multino@user_config_file=${scriptDir}/json/multino.json

unset PULP_CURRENT_CONFIG_ARGS

source ${scriptDir}/../init.sh
