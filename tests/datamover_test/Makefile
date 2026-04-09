APP = datamover_test
$(APP)_BUILD_DIR := ./build
APP_SRCS         := ./src/$(APP).c
$(APP)_INCDIRS   := ./data

# Add Konark HAL sources
APP_SRCS         += konark/hal_datamover.c
APP_SRCS         += konark/hal_konark.c
APP_SRCS         += konark/hal_hwpe.c

APP_CFLAGS += -I. -I./data

ifdef USE_CLUSTER
APP_CFLAGS += -DCLUSTER -DNUM_CLUSTER=$(USE_CLUSTER)
ifdef NUM_CORES
APP_CFLAGS += -DNUM_CORES=$(NUM_CORES)
else
APP_CFLAGS += -DNUM_CORES=1
endif
endif

APP_CFLAGS += -Os -g
APP_LDFLAGS += -Os -g

include $(RULES_DIR)/pmsis_rules.mk
