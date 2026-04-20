APP = test
APP_SRCS = test.c
APP_CFLAGS = -O3 -g

CONFIG_UART = 1

ifdef USE_GAPY
override runner_args += --target-opt=**/uart_checker/uart_checker/loopback=true
else
override runner_args += --param uart_checker/uart_checker/loopback=true
endif

include $(RULES_DIR)/pmsis_rules.mk
