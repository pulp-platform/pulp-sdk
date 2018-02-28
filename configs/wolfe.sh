#!/bin/bash

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PULP_CURRENT_CONFIG=wolfe@user_config_file=${scriptDir}/json/wolfe.json

unset PULP_CURRENT_CONFIG_ARGS

source ${scriptDir}/../init.sh
