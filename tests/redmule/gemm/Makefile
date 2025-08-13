APP = redmule_test
APP_SRCS += redmule_test.c
APP_CFLAGS += -O3 -g -Iinc

override runner_args += --target-property=chip/cluster/redmule=True

#CONFIG_HYPERRAM=1

#PMSIS_OS=pulpos

$(info $(RULES_DIR))

include $(RULES_DIR)/pmsis_rules.mk
