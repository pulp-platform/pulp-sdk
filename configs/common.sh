#! /bin/bash

if [ -n "$PULP_RISCV_GCC_TOOLCHAIN_BASE" ]; then
    export PULP_RISCV_GCC_TOOLCHAIN=$PULP_RISCV_GCC_TOOLCHAIN_BASE/1.3
fi

export PATH="$PULP_RISCV_GCC_TOOLCHAIN/bin":"$PULP_SDK_HOME/tools/bin":$PATH

# keep compatibility with gap_sdk
export GAP_SDK_HOME=$PULP_SDK_HOME
export GAP_RISCV_GCC_TOOLCHAIN=$PULP_RISCV_GCC_TOOLCHAIN

export TARGET_INSTALL_DIR=$PULP_SDK_HOME/install/$TARGET_CHIP
export INSTALL_DIR=$PULP_SDK_HOME/install/workstation
export DEP_DIRS=$INSTALL_DIR
export RULES_DIR=$PULP_SDK_HOME/tools/rules


export PATH="$INSTALL_DIR/bin":$PATH
export LD_LIBRARY_PATH="$INSTALL_DIR/lib":$LD_LIBRARY_PATH
export PYTHONPATH=$INSTALL_DIR/python:$PYTHONPATH


# Gapy
export PATH=$PULP_SDK_HOME/tools/gapy:$PATH
export PYTHONPATH=$PULP_SDK_HOME/tools/gap-configs/python:$PYTHONPATH
export PYTHONPATH=$PULP_SDK_HOME/tools/gapy:$PYTHONPATH
export PULP_CONFIGS_PATH=$PULP_SDK_HOME/tools/gap-configs/configs:$PULP_CONFIGS_PATH

# GVSOC
export PULP_SDK_INSTALL=$INSTALL_DIR
export GVSOC_PATH=$INSTALL_DIR/python
export XTENSOR_INCLUDE_DIR=$PULP_SDK_HOME/ext/xtensor/include
export GVSOC_INC_PATHS="$PULP_SDK_HOME/rtos/pulpos/gap_archi/include/archi/chips/$TARGET_NAME $PULP_SDK_HOME/rtos/pulpos/gap_archi/include $PULP_SDK_HOME/rtos/pulpos/pulp_archi/include"

# FPGA
#export PULP_

# PulpOS 2
export PULPOS_HOME=$PULP_SDK_HOME/rtos/pulpos/common
export PULPOS_PULP_HOME=$PULP_SDK_HOME/rtos/pulpos/pulp
export PULPOS_GAP8_HOME=$PULP_SDK_HOME/rtos/pulpos/gap8
export PULPOS_GAP9_HOME=$PULP_SDK_HOME/rtos/pulpos/gap9
export GAP_PULPOS_ARCHI=$PULP_SDK_HOME/rtos/pulpos/gap_archi
export PULPOS_ARCHI=$PULP_SDK_HOME/rtos/pulpos/pulp_archi
export PULPOS_HAL=$PULP_SDK_HOME/rtos/pulpos/pulp_hal
export PMSIS_API=$PULP_SDK_HOME/rtos/pmsis/pmsis_api
export PULP_EXT_LIBS=$PULP_SDK_HOME/ext_libs
export PULPOS_OPENMP_DIR=$PULPOS_HOME/lib/omp
