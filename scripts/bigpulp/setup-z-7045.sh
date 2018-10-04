#!/usr/bin/env bash

set -e

readonly SCRIPT_DIR=$(dirname "$(readlink -f "$0")")

source "$SCRIPT_DIR/../../configs/bigpulp-z-7045.sh"
"$SCRIPT_DIR/setup.inc.sh"
