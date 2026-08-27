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
export TARGET_CHIP="MEGATRON"
#export TARGET_NAME="pulp"
export TARGET_NAME="megatron"
export BOARD_NAME=megatron

export PULPOS_BOARD=megatron
export PULPOS_BOARD_VERSION=pulp
export PULPOS_BOARD_PROFILE=pulp
export PULPOS_TARGET=megatron
export PULPOS_SYSTEM=pulp
export GAPY_TARGET=megatron
export GAPY_V2_TARGET=megatron

export PULPOS_MODULES="$PULP_SDK_HOME/rtos/pulpos/pulp $PULP_SDK_HOME/rtos/pmsis/pmsis_bsp"

export GAPY_PY_TARGET=Pulp_open_board@megatron.pulp_open_board

source $PULP_SDK_HOME/configs/common.sh
