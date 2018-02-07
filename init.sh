#!/bin/bash -e

add_path() {
    export PATH=${PATH//":$1:"/:}
    export PATH=${PATH/%":$1"/}
    export PATH=${PATH/#"$1:"/}
    export PATH="$1${PATH:+":$PATH"}"
}

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PULP_PROJECT_HOME="$scriptDir"
echo "Configuring Pulp project at $PULP_PROJECT_HOME"
add_path $PULP_PROJECT_HOME/pulp-tools/bin
