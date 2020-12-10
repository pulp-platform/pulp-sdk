#! /bin/bash


if [  -n "${ZSH_VERSION:-}" ]; then 
	DIR="$(readlink -f -- "${(%):-%x}")"
	DIRNAME="$(dirname $DIR)"
	GAP_SDK_HOME=$(dirname $DIRNAME)
	export GAP_SDK_HOME
	#echo $(dirname "$(readlink -f ${(%):-%N})")
else
	export GAP_SDK_HOME="$(dirname $(dirname "$(readlink -f "${BASH_SOURCE[0]}")"))"
fi

export TARGET_CHIP_FAMILY="PULP"
export TARGET_CHIP="PULP"
export TARGET_NAME="pulp"
export BOARD_NAME=pulp
export PULP_CURRENT_CONFIG=$BOARD_NAME@config_file=config/pulp_rnnext.json

export PULPOS_BOARD=pulp
export PULPOS_BOARD_VERSION=pulp
export PULPOS_BOARD_PROFILE=pulp
export PULPOS_TARGET=pulp
export PULPOS_SYSTEM=pulp
export GAPY_TARGET=pulp_rnnext

source $GAP_SDK_HOME/configs/common.sh
