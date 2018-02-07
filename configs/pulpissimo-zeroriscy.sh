#!/bin/bash

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PULP_CURRENT_CONFIG=pulpissimo@user_config_file=${scriptDir}/json/pulpissimo-zeroriscy.json

source ${scriptDir}/../init.sh