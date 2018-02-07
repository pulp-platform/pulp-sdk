#!/bin/csh

if ( $?PULP_PROJECT_HOME ) then
    echo "Configuring Pulp project at $PULP_PROJECT_HOME"

    setenv PATH $PULP_PROJECT_HOME/pulp-tools/bin:$PATH
else
    echo "ERROR: PULP_PROJECT_HOME must be defined to the top SDK module directory"
endif
