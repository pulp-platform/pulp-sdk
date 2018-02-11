#!/bin/bash

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PULP_CURRENT_CONFIG=pulpissimo-zeroriscy@user_config_file=${scriptDir}/json/pulpissimo-zeroriscy.json

unset PULP_CURRENT_CONFIG_ARGS

source ${scriptDir}/../init.sh
