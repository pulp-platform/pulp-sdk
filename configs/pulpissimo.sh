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
export TARGET_CHIP="PULPISSIMO"
export TARGET_NAME="pulpissimo"
export BOARD_NAME=pulpissimo
export PULP_CURRENT_CONFIG=$BOARD_NAME@config_file=config/$BOARD_NAME.json

export PULPOS_BOARD=pulpissimo
export PULPOS_BOARD_VERSION=pulpissimo
export PULPOS_BOARD_PROFILE=pulpissimo
export PULPOS_TARGET=pulpissimo
export PULPOS_SYSTEM=pulpissimo
export GAPY_TARGET=pulpissimo

export PULPOS_MODULES="$PULP_SDK_HOME/rtos/pulpos/pulp $PULP_SDK_HOME/rtos/pmsis/pmsis_bsp"

export GVSOC_MODULES="$PULP_SDK_HOME/tools/gvsoc/common $PULP_SDK_HOME/tools/gvsoc/pulp/models"

source $PULP_SDK_HOME/configs/common.sh
