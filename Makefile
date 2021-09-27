SHELL=bash

ifndef PULP_SDK_HOME
    $(error Please source the proper configuration first)
endif

BUILD_DIR ?= $(CURDIR)/build
ARTIFACT_PATH ?= $(CURDIR)/artifact

export BUILD_DIR
export ARTIFACT_PATH

include rules/json-tools.mk
include rules/gap-configs.mk
include rules/pulp-debug-bridge.mk
include rules/dpi-models.mk
include rules/gvsoc.mk
include rules/pulpos.mk

checkout: gvsoc.checkout.all
	git submodule update --init rtos/pulpos/pulp_hal rtos/pulpos/pulp_archi tests/pmsis_tests rtos/pulpos/pulp rtos/pmsis/pmsis_api tools/gapy rtos/pulpos/common/ rtos/pmsis/pmsis_bsp tests/bsp_tests

build: gvsoc.build.all

clean: gvsoc.clean

all: checkout build

artifact: gvsoc.artifact pulpos.artifact
	rsync -avR --exclude=".git*" Makefile rules configs tools/rules $(ARTIFACT_PATH)

	mkdir -p $(ARTIFACT_PATH)/tools/jenkins
	git rev-parse HEAD &> $(ARTIFACT_PATH)/tools/jenkins/sdk_version.txt

.PHONY: artifact