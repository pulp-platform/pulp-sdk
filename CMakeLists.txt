cmake_minimum_required(VERSION 3.16.3)

project(sdk)

if(NOT DEFINED CONFIG_PULP_SDK_HOME)
    message(STATUS "Using current directory as GAP SDK Home")
    set(CONFIG_PULP_SDK_HOME ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Use CCache to speed up compilation if possible
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set(CMAKE_C_COMPILER_LAUNCHER   ccache)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif()

add_subdirectory(tools/gvsoc)

## Install pip requirements, note: does not install nntool reqs as building
## will do that
add_custom_target(
    pip_install
    COMMENT "install pip deps from requirements"
    COMMAND pip install --user -r ${CONFIG_PULP_SDK_HOME}/requirements.txt
    COMMAND pip install --user -r ${CONFIG_PULP_SDK_HOME}/tools/gapy_v2/requirements.txt
    COMMAND pip install --user -r ${CONFIG_PULP_SDK_HOME}/tools/plptest/requirements.txt
    )
