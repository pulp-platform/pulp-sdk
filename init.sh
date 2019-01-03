#!/bin/bash -e

add_path() {
    export PATH=${PATH//":$1:"/:}
    export PATH=${PATH/%":$1"/}
    export PATH=${PATH/#"$1:"/}
    export PATH="$1${PATH:+":$PATH"}"
}

add_PYTHONPATH() {
    export PYTHONPATH=${PYTHONPATH//":$1:"/:}
    export PYTHONPATH=${PYTHONPATH/%":$1"/}
    export PYTHONPATH=${PYTHONPATH/#"$1:"/}
    export PYTHONPATH="$1${PYTHONPATH:+":$PYTHONPATH"}"
}

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PULP_PROJECT_HOME="$scriptDir"
echo "Configuring Pulp project at $PULP_PROJECT_HOME"
add_path $PULP_PROJECT_HOME/pulp-tools/bin
add_path $PULP_PROJECT_HOME/pulp-configs/bin
add_PYTHONPATH $PULP_PROJECT_HOME/pulp-tools/python
add_PYTHONPATH $PULP_PROJECT_HOME/pulp-configs/python
add_PYTHONPATH $PULP_PROJECT_HOME/pulp-configs/bin
add_PYTHONPATH $PULP_PROJECT_HOME/json-tools/python
export PULP_CONFIGS_PATH=$PULP_PROJECT_HOME/pulp-configs/configs

