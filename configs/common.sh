#! /bin/bash

export PULP_SDK_HOME=$GAP_SDK_HOME

if [ -n "$GAP_RISCV_GCC_TOOLCHAIN_BASE" ]; then
    export GAP_RISCV_GCC_TOOLCHAIN=$GAP_RISCV_GCC_TOOLCHAIN_BASE/1.3
fi

export PATH="$GAP_RISCV_GCC_TOOLCHAIN/bin":"$GAP_SDK_HOME/tools/bin":$PATH


export TARGET_INSTALL_DIR=$GAP_SDK_HOME/install/$TARGET_CHIP
export INSTALL_DIR=$GAP_SDK_HOME/install/workstation
export DEP_DIRS=$INSTALL_DIR
export RULES_DIR=$GAP_SDK_HOME/tools/rules


export PATH="$INSTALL_DIR/bin":$PATH
export LD_LIBRARY_PATH="$INSTALL_DIR/lib":$LD_LIBRARY_PATH
export PYTHONPATH=$INSTALL_DIR/python:$PYTHONPATH


# Gapy
export PATH=$GAP_SDK_HOME/tools/gapy:$PATH
export PYTHONPATH=$GAP_SDK_HOME/tools/gap-configs/python:$PYTHONPATH
export PYTHONPATH=$GAP_SDK_HOME/tools/gapy:$PYTHONPATH
export PULP_CONFIGS_PATH=$GAP_SDK_HOME/tools/gap-configs/configs:$PULP_CONFIGS_PATH

# GVSOC
export PULP_RISCV_GCC_TOOLCHAIN=$GAP_RISCV_GCC_TOOLCHAIN
export PULP_SDK_INSTALL=$INSTALL_DIR
export GVSOC_PATH=$INSTALL_DIR/python
export GVSOC_INC_PATHS="$GAP_SDK_HOME/rtos/pulpos/gap_archi/include/archi/chips/$TARGET_NAME $GAP_SDK_HOME/rtos/pulpos/gap_archi/include $GAP_SDK_HOME/rtos/pulpos/pulp_archi/include"

# FPGA
#export PULP_

# PulpOS 2
export PULPOS_HOME=$GAP_SDK_HOME/rtos/pulpos/common
export PULPOS_PULP_HOME=$GAP_SDK_HOME/rtos/pulpos/pulp
export PULPOS_GAP8_HOME=$GAP_SDK_HOME/rtos/pulpos/gap8
export PULPOS_GAP9_HOME=$GAP_SDK_HOME/rtos/pulpos/gap9
export GAP_PULPOS_ARCHI=$GAP_SDK_HOME/rtos/pulpos/gap_archi
export PULPOS_ARCHI=$GAP_SDK_HOME/rtos/pulpos/pulp_archi
export PULPOS_HAL=$GAP_SDK_HOME/rtos/pulpos/pulp_hal
export PMSIS_API=$GAP_SDK_HOME/rtos/pmsis/pmsis_api
