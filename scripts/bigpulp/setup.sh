#!/usr/bin/env bash

source configs/bigpulp-z-7045.sh
source configs/platform-hsa.sh

if [ -z "$PULP_RISCV_GCC_TOOLCHAIN" ]; then
    echo "'PULP_RISCV_GCC_TOOLCHAIN' not defined, trying to download the toolchain" \
        "(internal use only)."
    make deps
fi

# Source file with commands to setup PULP SDK (common with HERO).
scriptdir="$( cd "$(dirname "$0")" ; pwd -P )"
source "$scriptdir/../hero/common.sh"
