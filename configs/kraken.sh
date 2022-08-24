#! /usr/bin/env bash


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
export TARGET_CHIP="KRAKEN"
export TARGET_NAME="kraken"
export BOARD_NAME=kraken
export PULP_CURRENT_CONFIG=$BOARD_NAME@config_file=config/$BOARD_NAME.json

export PULPOS_BOARD=kraken
export PULPOS_BOARD_VERSION=kraken
export PULPOS_BOARD_PROFILE=kraken
export PULPOS_TARGET=kraken
export PULPOS_SYSTEM=kraken
export GAPY_TARGET=kraken

export PULPOS_MODULES="$PULP_SDK_HOME/rtos/pulpos/pulp $PULP_SDK_HOME/rtos/pmsis/pmsis_bsp"

export GVSOC_MODULES="$PULP_SDK_HOME/tools/gvsoc/common $PULP_SDK_HOME/tools/gvsoc/pulp/models"

source $PULP_SDK_HOME/configs/common.sh
