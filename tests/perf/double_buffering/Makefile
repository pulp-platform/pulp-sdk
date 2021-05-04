APP = test
APP_SRCS += test.c
APP_CFLAGS += -O3 -g

CONFIG_HYPERRAM = 1

ifdef DB
APP_CFLAGS += -DDOUBLE_BUFFERING
endif


include $(RULES_DIR)/pmsis_rules.mk
