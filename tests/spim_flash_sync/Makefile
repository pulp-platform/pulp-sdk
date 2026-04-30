USE_PULPOS_TEST=1
#USE_FREERTOS_TEST=1

ifdef USE_PULPOS_TEST
APP = test_spi_sync
APP_SRCS += test_spi_sync.c
ifdef USE_CLUSTER
APP_CFLAGS += -DCLUSTER -DNUM_CLUSTER=$(USE_CLUSTER)
ifdef NUM_CORES
APP_CFLAGS += -DNUM_CORES=$(NUM_CORES)
else
APP_CFLAGS += -DNUM_CORES=1
endif
endif
APP_CFLAGS += -Os -g -DUSE_PULPOS_TEST -DCONFIG_SPIM
APP_LDFLAGS += -Os -g
CONFIG_SPIM = 1
include $(RULES_DIR)/pmsis_rules.mk
endif

ifdef USE_FREERTOS_TEST
APP = test_spi_sync
APP_SRCS += test_spi_sync.c
# indicate this repository's root folder
PROJ_ROOT = $(shell git rev-parse --show-toplevel)

# good defaults for many environment variables
include $(PROJ_ROOT)/../freertos/default_flags.mk

# manually set CFLAGS to disable some warnings (-Wconversion)
CFLAGS += \
	-march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore \
	-fsigned-char -ffunction-sections -fdata-sections \
	-std=gnu11 \
	-Wall -Wextra -Wshadow -Wformat=2 -Wundef \
	-Wno-unused-parameter -Wno-unused-variable \
	-Os -g3 \
	-DFEATURE_CLUSTER=0 -D__PULP__=1 -DASYNC=0 \
	-DSPIM_ITF=0 -DSPIM_CS=1 -DUSE_FREERTOS_TEST

# CFLAGS += -fstack-usage -Wstack-usage=1024
# CFLAGS += -DTRACE_SPI -DPI_LOG_LOCAL_LEVEL=5 -DDEBUG

# testbench configuration for spim_verif dpi
DPI_CONFIG=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))rtl_config.json

# disable cluster
CONFIG_CLUSTER=n

# rtos, pulp and pmsis ources
include $(PROJ_ROOT)/../freertos/default_srcs.mk

# application name
PROG = test_spi_sync

# application/user specific code
#USER_SRCS += test_flash_async.c
USER_SRCS = $(APP_SRCS)

# FreeRTOS.h
CPPFLAGS += $(addprefix -I$(USER_DIR)/, ".")

CPPFLAGS += -DportasmHANDLE_INTERRUPT=vSystemIrqHandler

# Uncomment to disable Additional reigsters (HW Loops)
#CPPFLAGS += -DportasmSKIP_ADDITIONAL_REGISTERS

# compile, simulation and analysis targets
include $(PROJ_ROOT)/../freertos/default_targets.mk
endif

