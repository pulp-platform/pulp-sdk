APP = test
APP_SRCS += test2.c

ifdef USE_CLUSTER
APP_CFLAGS += -DCLUSTER -DNUM_CLUSTER=$(USE_CLUSTER)
ifdef NUM_CORES
APP_CFLAGS += -DNUM_CORES=$(NUM_CORES)
else
APP_CFLAGS += -DNUM_CORES=1
endif
endif

ifeq '$(platform)' 'rtl'
APP_CFLAGS    += -DRTL_PLATFORM=1
endif

APP_CFLAGS += -O3 -g -DSKIP_PLL_INIT
APP_LDFLAGS += -O3 -g
$(info APP_CFLAGS=${APP_CFLAGS})
include $(RULES_DIR)/pmsis_rules.mk
