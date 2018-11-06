#!/usr/bin/env bash

source configs/hero-z-7045.sh
source configs/platform-hsa.sh

# Source file with commands to setup PULP SDK.
scriptdir="$( cd "$(dirname "$0")" ; pwd -P )"
source "$scriptdir/common.sh"
