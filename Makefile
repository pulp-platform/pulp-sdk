SHELL=bash

ifndef PULP_SDK_HOME
    $(error Please source the proper configuration first)
endif

BUILD_DIR ?= $(CURDIR)/build

export BUILD_DIR

include rules/json-tools.mk
include rules/gap-configs.mk
include rules/pulp-debug-bridge.mk
include rules/dpi-models.mk
include rules/gvsoc.mk
include rules/pulpos.mk

build: gvsoc.build.all

clean: gvsoc.clean

all: build
