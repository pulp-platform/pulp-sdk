#! /bin/bash

if [ -n "$PULP_RISCV_GCC_TOOLCHAIN_BASE" ]; then
    export PULP_RISCV_GCC_TOOLCHAIN=$PULP_RISCV_GCC_TOOLCHAIN_BASE/1.3
fi

if [ -n "$PULP_RISCV_GCC_TOOLCHAIN" ]; then
    export PATH="$PULP_RISCV_GCC_TOOLCHAIN/bin":"$PULP_SDK_HOME/tools/bin":$PATH
fi

if [ -n "$PULP_RISCV_LLVM_TOOLCHAIN" ]; then
    export PATH="$PULP_RISCV_LLVM_TOOLCHAIN/bin":"$PULP_SDK_HOME/tools/bin":$PATH
fi

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
export PYTHONPATH=$PULP_SDK_HOME/tools/gapy:$PYTHONPATH
export PULP_CONFIGS_PATH=$PULP_SDK_HOME/tools/gap-configs/configs:$PULP_CONFIGS_PATH

# Gapy v2 (for gvsoc)
export PATH=$PULP_SDK_HOME/tools/gvsoc/gapy/bin:$PATH

# GVSOC
export GVSOC_MODULES="${GAP_SDK_HOME}/tools/gvsoc/core/models;${GAP_SDK_HOME}/tools/gvsoc/pulp"
export GVSOC_TARGETS="$GAPY_V2_TARGET"


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
