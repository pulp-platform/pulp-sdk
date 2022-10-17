APP = test
APP_SRCS = test.c
APP_CFLAGS = -O3 -g

CONFIG_UART = 1

override runner_args += --target-opt=**/uart_checker/uart_checker/loopback=true

include $(RULES_DIR)/pmsis_rules.mk
