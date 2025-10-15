#! /bin/bash


if [  -n "${ZSH_VERSION:-}" ]; then 
	DIR="$(readlink -f -- "${(%):-%x}")"
	DIRNAME="$(dirname $DIR)"
	PULP_SDK_HOME=$(dirname $DIRNAME)
	export PULP_SDK_HOME
	#echo $(dirname "$(readlink -f ${(%):-%N})")
else
	export PULP_SDK_HOME="$(dirname $(dirname "$(readlink -f "${BASH_SOURCE[0]}")"))"
fi

export TARGET_CHIP_FAMILY="PULP"
export TARGET_CHIP="PULP"
export TARGET_NAME="pulp"
export BOARD_NAME=pulp
export PULP_CURRENT_CONFIG=$BOARD_NAME@config_file=config/$BOARD_NAME.json

export PULPOS_BOARD=pulp
export PULPOS_BOARD_VERSION=pulp
export PULPOS_BOARD_PROFILE=pulp
export PULPOS_TARGET=pulp
export PULPOS_SYSTEM=pulp
export GAPY_TARGET=pulp
export GAPY_V2_TARGET=pulp-open

export PULPOS_MODULES="$PULP_SDK_HOME/rtos/pulpos/pulp $PULP_SDK_HOME/rtos/pmsis/pmsis_bsp"

export GAPY_PY_TARGET=Pulp_open_board@pulp_open.pulp_open_board

source $PULP_SDK_HOME/configs/common.sh
