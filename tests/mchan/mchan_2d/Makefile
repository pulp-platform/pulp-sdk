APP = test
APP_SRCS = mchan_2d.c
APP_CFLAGS += -O3 -g

ifdef DEBUG_TEST
PULP_CFLAGS += -DDEBUG_TEST
endif

ifdef CYCLE_COUNT
PULP_CFLAGS += -DCYCLE_COUNT
PULP_CFLAGS += -DDEBUG_TEST
endif

include $(RULES_DIR)/pmsis_rules.mk

stimuli:
	python ../gen_stimuli.py 2D