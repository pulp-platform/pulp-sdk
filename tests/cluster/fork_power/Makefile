APP = test
APP_SRCS = test.c
APP_CFLAGS += -O3 -g

NB_PE ?= 8

APP_ARCH_CFLAGS ?=  -march=rv32imcxgap9 -mPE=$(NB_PE) -mFC=1
APP_ARCH_LDFLAGS ?=  -march=rv32imcxgap9 -mPE=$(NB_PE) -mFC=1

APP_CFLAGS += -DNB_PE=$(NB_PE)

# This is needed to turn on all power traces
override runner_args += "--event=.*"

include $(RULES_DIR)/pmsis_rules.mk