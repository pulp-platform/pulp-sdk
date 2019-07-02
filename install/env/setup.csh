#!/bin/csh

alias prepend_path \
  'set arg2="\!:2";  if ($?\!:1 == 0) setenv \!:1 "$arg2";  if ($?\!:1 && $\!:1 !~ {,*:}"$arg2"{:*,}) setenv \!:1 "$arg2":"$\!:1";'

alias append_path \
  'set arg2="\!:2";  if ($?\!:1 == 0) setenv \!:1 "$arg2";  if ($?\!:1 && $\!:1 !~ {,*:}"$arg2"{:*,}) setenv \!:1 "$\!:1":"$arg2";'

echo "Configuring PULP SDK at: $PULP_SDK_HOME"

prepend_path PATH $PULP_SDK_HOME/install/ws/bin
if ( $?LD_LIBRARY_PATH ) then
    prepend_path LD_LIBRARY_PATH $PULP_SDK_HOME/install/ws/lib
else
    setenv LD_LIBRARY_PATH $PULP_SDK_HOME/install/ws/lib
endif

if ( $?PYTHONPATH ) then
    prepend_path PYTHONPATH $PULP_SDK_HOME/install/ws/python
else
    setenv PYTHONPATH $PULP_SDK_HOME/install/ws/python
endif

setenv RULES_DIR $PULP_SDK_HOME/install/rules
setenv PULP_SDK_INSTALL $PULP_SDK_HOME/install
setenv PULP_SDK_WS_INSTALL $PULP_SDK_HOME/install/ws
setenv PULP_CONFIGS_PATH $PULP_SDK_WS_INSTALL/configs
setenv SDK_TILER_PATH $PULP_SDK_HOME/install/auto-tiler
setenv INSTALL_DIR $PULP_SDK_WS_INSTALL
setenv TARGET_INSTALL_DIR $PULP_SDK_INSTALL
