APP = idma_1d
APP_SRCS += idma_1d.c
APP_CFLAGS += -O3 -g

ifdef DEBUG_TEST
PULP_CFLAGS += -DDEBUG_TEST
endif

ifdef CYCLE_COUNT
PULP_CFLAGS += -DCYCLE_COUNT
PULP_CFLAGS += -DDEBUG_TEST
endif

ifdef MULTI_CORE_S
PULP_CFLAGS += -DMULTI_CORE_S
endif

ifdef MULTI_CORE_P
PULP_CFLAGS += -DMULTI_CORE_P
endif

ifdef QUICK_MODE
PULP_CFLAGS += -DQUICK_MODE
endif

include $(RULES_DIR)/pmsis_rules.mk

stimuli:
	python ../gen_stimuli.py 1D
