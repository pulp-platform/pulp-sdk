# the MAGICK_ROOT variable can be used to pass an alternative installation
# prefix for the GraphicsMagick library. 
ifdef MAGICK_ROOT
export MAGICK_ROOT := $(realpath $(MAGICK_ROOT))
endif

SHELL=bash

ifndef PULP_SDK_HOME
    $(error Please source the proper configuration first)
endif

BUILD_DIR ?= $(CURDIR)/build

export BUILD_DIR

include rules/json-tools.mk
include rules/gap-configs.mk
include rules/pulp-debug-bridge.mk
include rules/pulpos.mk

CMAKE_FLAGS ?= -j 6

build:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo \
		-DTARGET=$(GAPY_V2_TARGET) \
		-DTARGET_OPT="$(GAPY_NEW_TARGET_OPT)" \
		-DCMAKE_INSTALL_PREFIX=$(PULP_SDK_HOME)/install/workstation
	cmake --build build $(CMAKE_FLAGS)
	cmake --install build


clean:
	rm -rf build install

checkout:
	git submodule update --recursive --init

all: checkout build


.PHONY: build