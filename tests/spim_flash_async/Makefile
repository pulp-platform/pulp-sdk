APP = test_spi_async
APP_SRCS += test_spi_async.c

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

CONFIG_SPIM = 1

include $(RULES_DIR)/pmsis_rules.mk
