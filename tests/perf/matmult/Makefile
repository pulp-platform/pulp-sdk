APP = test
APP_SRCS += test.c
APP_CFLAGS += -O3 -g

ifdef VERBOSE_PERF
APP_CFLAGS += -DSTATS
endif

PULP_ARCH_CFLAGS = -march=rv32imfcxpulpv2 -mFC=1

include $(RULES_DIR)/pmsis_rules.mk
