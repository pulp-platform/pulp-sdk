USE_PULPOS_TEST=1
#USE_FREERTOS_TEST=1

ifdef USE_PULPOS_TEST
APP = i2c_eeprom_sync
APP_SRCS += i2c_eeprom_sync.c
ifdef USE_CLUSTER
APP_CFLAGS += -DCLUSTER -DNUM_CLUSTER=$(USE_CLUSTER)
ifdef NUM_CORES
APP_CFLAGS += -DNUM_CORES=$(NUM_CORES)
else
APP_CFLAGS += -DNUM_CORES=1
endif
endif
APP_CFLAGS += -Os -g -DUSE_PULPOS_TEST -DCONFIG_UDMA_I2C_EOT -DCONFIG_UDMA_I2C_ACK -DCONFIG_I2C
APP_LDFLAGS += -Os -g
CONFIG_I2C = 1
include $(RULES_DIR)/pmsis_rules.mk
endif

ifdef USE_FREERTOS_TEST
PROJ_ROOT = $(shell git rev-parse --show-toplevel)

# good defaults for many environment variables
include $(PROJ_ROOT)/default_flags.mk

# manually set CFLAGS to disable some warnings (-Wconversion)
CFLAGS = \
	-march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore \
	-fsigned-char -ffunction-sections -fdata-sections \
	-std=gnu11 \
	-Wall -Wextra -Wshadow -Wformat=2 -Wundef \
	-Wno-unused-parameter -Wno-unused-variable \
	-Og -g3 \
	 -D__PULP__=1 -DDEBUG \
        -Wstack-usage=1024 -DUSE_FREERTOS_TEST -DCONFIG_UDMA_I2C_EOT -DCONFIG_UDMA_I2C_ACK

CONFIG_UDMA_I2C_EOT=y
CONFIG_UDMA_I2C_ACK=y
CONFIG_CLUSTER=n
# rtos, pulp and pmsis sources
include $(PROJ_ROOT)/default_srcs.mk

# application name
PROG = i2c_eeprom_sync_sync

# application/user specific code
USER_SRCS = i2c_eeprom_sync.c

# FreeRTOS.h
CPPFLAGS += $(addprefix -I$(USER_DIR)/, ".")

CPPFLAGS += -DportasmHANDLE_INTERRUPT=vSystemIrqHandler
CPPFLAGS += -DUSE_STDIO
## number of error check polls before the tests is conclueded as being successful
CPPFLAGS += -DSTREAM_CHECK_ITERATIONS=2

# Uncomment to disable Additional reigsters (HW Loops)
#CPPFLAGS += -DportasmSKIP_ADDITIONAL_REGISTERS

# compile, simulation and analysis targets
include $(PROJ_ROOT)/default_targets.mk
endif

