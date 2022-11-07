# GVSOC needs to be given a config file containing all the components to be compiled

set(TARGET_CONFIG_DIR ${CMAKE_BINARY_DIR}/gvsoc/configs)
set(TARGET_CONFIG ${TARGET_CONFIG_DIR}/${TARGET}.config)
file(MAKE_DIRECTORY ${TARGET_CONFIG_DIR})

message(STATUS "Generating GVSOC config to ${TARGET_CONFIG}")

# Gapy is used to extract the list of components out of the specified target
set(GAPY_CMD ${CONFIG_PULP_SDK_HOME}/tools/gapy_v2/bin/gapy --target=${TARGET} --target-dir=${CONFIG_PULP_SDK_HOME}/tools/gvsoc/pulp/targets
    --target-dir=${CONFIG_PULP_SDK_HOME}/tools/gapy/targets --platform=gvsoc
    --component-file=${TARGET_CONFIG} components --py-stack
)

# We generate first the list of components for the specified target
execute_process (
    COMMAND ${GAPY_CMD}
    COMMAND_ERROR_IS_FATAL ANY
)

# And then append extra components for each extra config
# This is needed for GVSOC options which can add components which are not in the
# default config like the testbench
foreach (opt ${TARGET_OPT})
    execute_process (
        COMMAND ${GAPY_CMD} ${opt} --component-file-append
        COMMAND_ERROR_IS_FATAL ANY
    )
endforeach()

set(VP_MODEL_ROOT_DIRS
    "${CONFIG_PULP_SDK_HOME}/tools/gvsoc/common"
    "${CONFIG_PULP_SDK_HOME}/tools/gvsoc/pulp"
)

set(VP_MODEL_INCLUDE_DIRS
    "${CONFIG_PULP_SDK_HOME}/rtos/pulpos/pulp_archi/include"
)

add_subdirectory(common)