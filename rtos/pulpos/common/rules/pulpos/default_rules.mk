ifndef TARGET_BUILD_DIR
TARGET_BUILD_DIR = $(CURDIR)/build
ifdef build_dir_ext
TARGET_BUILD_DIR := $(TARGET_BUILD_DIR)/$(build_dir_ext)
endif
endif
ifndef VERBOSE
V = @
endif

ifdef VERBOSE
override gapy_args += --config-opt=**/runner/verbose=true
endif

CONFIG_BUILD_DIR=$(TARGET_BUILD_DIR)


ifdef PULP_RISCV_GCC_TOOLCHAIN_CI
PULP_CC := $(PULP_RISCV_GCC_TOOLCHAIN_CI)/bin/$(PULP_CC)
PULP_LD := $(PULP_RISCV_GCC_TOOLCHAIN_CI)/bin/$(PULP_LD)
PULP_AR := $(PULP_RISCV_GCC_TOOLCHAIN_CI)/bin/$(PULP_AR)
else
ifdef PULP_RUNTIME_GCC_TOOLCHAIN
PULP_CC := $(PULP_RUNTIME_GCC_TOOLCHAIN)/bin/$(PULP_CC)
PULP_LD := $(PULP_RUNTIME_GCC_TOOLCHAIN)/bin/$(PULP_LD)
PULP_AR := $(PULP_RUNTIME_GCC_TOOLCHAIN)/bin/$(PULP_AR)
else
ifdef PULP_RISCV_GCC_TOOLCHAIN
PULP_CC := $(PULP_RISCV_GCC_TOOLCHAIN)/bin/$(PULP_CC)
PULP_LD := $(PULP_RISCV_GCC_TOOLCHAIN)/bin/$(PULP_LD)
PULP_AR := $(PULP_RISCV_GCC_TOOLCHAIN)/bin/$(PULP_AR)
PULP_CFLAGS += -fno-jump-tables -fno-tree-loop-distribute-patterns
endif
ifdef PULP_RISCV_LLVM_TOOLCHAIN
PULP_CC := $(PULP_RISCV_LLVM_TOOLCHAIN)/bin/clang
PULP_LD := $(PULP_RISCV_LLVM_TOOLCHAIN)/bin/$(PULP_LD)
PULP_AR := $(PULP_RISCV_LLVM_TOOLCHAIN)/bin/$(PULP_AR)
endif
endif
endif

VPATH = $(PULPOS_HOME) $(PULPOS_MODULES)

include $(PULPOS_HOME)/rules/pulpos/src.mk


ifeq '$(CONFIG_LIBC_MINIMAL)' '1'
PULP_APP_CFLAGS += -I$(PULPOS_HOME)/lib/libc/minimal/include
endif
PULP_APP_CFLAGS += -I$(PULPOS_HOME)/include -I$(PULPOS_HOME)/kernel -I$(PULPOS_ARCHI)/include -I$(PULPOS_HAL)/include -I$(PMSIS_API)/include

PULP_APP_CFLAGS += $(foreach inc,$(PULPOS_MODULES),-I$(inc)/include)

ifdef PULPOS_PLATFORM
platform=$(PULPOS_PLATFORM)
endif


ifndef platform
platform=gvsoc
endif

override disopt ?= -d

PULP_CFLAGS += -D__PULPOS2__

ifeq '$(platform)' 'gvsoc'
PULP_CFLAGS += -D__PLATFORM__=ARCHI_PLATFORM_GVSOC
endif
ifeq '$(platform)' 'board'
PULP_CFLAGS += -D__PLATFORM__=ARCHI_PLATFORM_BOARD
endif
ifeq '$(platform)' 'rtl'
PULP_CFLAGS += -D__PLATFORM__=ARCHI_PLATFORM_RTL
endif
ifeq '$(platform)' 'fpga'
PULP_CFLAGS += -D__PLATFORM__=ARCHI_PLATFORM_FPGA
endif

ifdef io
ifeq '$(io)' 'host'
CONFIG_IO_HOST = 1
endif
ifeq '$(io)' 'uart'
CONFIG_IO_UART = 1
endif
endif


ifdef CONFIG_NB_CLUSTER_PE
PULP_CFLAGS += -DARCHI_CLUSTER_NB_PE=$(CONFIG_NB_CLUSTER_PE)
override config_args += --config-opt=cluster/nb_pe=$(CONFIG_NB_CLUSTER_PE)
endif

ifdef CONFIG_IO_HOST
PULP_CFLAGS += -DPOS_CONFIG_IO_HOST=$(CONFIG_IO_HOST)
endif

ifdef CONFIG_IO_UART
PULP_CFLAGS += -DPOS_CONFIG_IO_UART=$(CONFIG_IO_UART)
endif

ifdef CONFIG_IO_UART_BAUDRATE
PULP_CFLAGS += -DPOS_CONFIG_IO_UART_BAUDRATE=$(CONFIG_IO_UART_BAUDRATE)
endif

ifdef CONFIG_IO_UART_ITF
PULP_CFLAGS += -DPOS_CONFIG_IO_UART_ITF=$(CONFIG_IO_UART_ITF)
endif

ifdef CONFIG_RISCV_GENERIC
PULP_CFLAGS += -D__RISCV_GENERIC__=1
endif

ifdef CONFIG_USE_ASM_OPTIM
PULP_CFLAGS  += -D__USE_ASM_OPTIM__=1
endif

ifdef CONFIG_TRACE_LEVEL
CONFIG_TRACE_LEVEL_INT = 0
ifeq '$(CONFIG_TRACE_LEVEL)' 'FATAL'
CONFIG_TRACE_LEVEL_INT = 1
endif
ifeq '$(CONFIG_TRACE_LEVEL)' 'ERROR'
CONFIG_TRACE_LEVEL_INT = 2
PI_LOG_LOCAL_LEVEL = 1
endif
ifeq '$(CONFIG_TRACE_LEVEL)' 'WARNING'
CONFIG_TRACE_LEVEL_INT = 3
PI_LOG_LOCAL_LEVEL = 2
endif
ifeq '$(CONFIG_TRACE_LEVEL)' 'INFO'
CONFIG_TRACE_LEVEL_INT = 4
PI_LOG_LOCAL_LEVEL = 3
endif
ifeq '$(CONFIG_TRACE_LEVEL)' 'DEBUG'
CONFIG_TRACE_LEVEL_INT = 5
PI_LOG_LOCAL_LEVEL = 4
endif
ifeq '$(CONFIG_TRACE_LEVEL)' 'TRACE'
CONFIG_TRACE_LEVEL_INT = 6
PI_LOG_LOCAL_LEVEL = 5
endif
PULP_CFLAGS += -D__TRACE_LEVEL__=$(CONFIG_TRACE_LEVEL_INT) -DPI_LOG_LOCAL_LEVEL=$(PI_LOG_LOCAL_LEVEL)
endif

ifdef CONFIG_TRACE_ALL
PULP_CFLAGS += -D__TRACE_ALL__=1
endif

ifdef CONFIG_TRACE_SPIM
PULP_CFLAGS += -D__TRACE_SPIM__=1
endif

ifdef CONFIG_TRACE_FREQ
PULP_CFLAGS += -D__TRACE_FREQ__=1
endif

ifdef CONFIG_TRACE_ALLOC
PULP_CFLAGS += -D__TRACE_ALLOC__=1
endif

ifdef CONFIG_TRACE_INIT
PULP_CFLAGS += -D__TRACE_INIT__=1
endif

ifdef CONFIG_TRACE_UART
PULP_CFLAGS += -D__TRACE_UART__=1
endif


#
# RUNNER
#

ifdef gui
override runner_args += --gui
endif

READFS_FLASH ?= flash

override config_args += $(foreach file, $(READFS_FILES), --config-opt=$(READFS_FLASH)/content/partitions/readfs/files=$(file))
override config_args += $(foreach file, $(HOSTFS_FILES), --config-opt=flash/content/partitions/hostfs/files=$(file))


#
# PULP_APPS
#

define declare_app

$(eval PULP_APP_SRCS_$(1) += $(PULP_APP_SRCS) $(PULP_APP_FC_SRCS) $(PULP_SRCS) $(PULP_APP_CL_SRCS) $(PULP_CL_SRCS))
$(eval PULP_APP_ASM_SRCS_$(1) += $(PULP_APP_ASM_SRCS) $(PULP_ASM_SRCS) $(PULP_APP_CL_ASM_SRCS) $(PULP_CL_ASM_SRCS))
$(eval PULP_APP_OBJS_$(1) += $(patsubst %.c,$(TARGET_BUILD_DIR)/$(1)/%.o,$(PULP_APP_SRCS_$(1))))
$(eval PULP_APP_OBJS_$(1) += $(patsubst %.S,$(TARGET_BUILD_DIR)/$(1)/%.o,$(PULP_APP_ASM_SRCS_$(1))))

$(eval PULP_APP_CFLAGS_$(1) += $(PULP_ARCH_CFLAGS) $(PULP_CFLAGS) $(PULP_APP_CFLAGS))
$(eval PULP_APP_LDFLAGS_$(1) += $(PULP_ARCH_LDFLAGS) $(PULP_LDFLAGS) $(PULP_APP_LDFLAGS))

-include $(PULP_APP_OBJS_$(1):.o=.d)

$(TARGET_BUILD_DIR)/$(1)/%.o: %.c
	@echo "CC  $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_CC) -c $$< -o $$@ -MMD -MP $(PULP_APP_CFLAGS_$(1))

$(TARGET_BUILD_DIR)/$(1)/%.o: %.cpp
	@echo "CXX $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_CC) -c $< -o $@ -MMD -MP $(PULP_APP_CFLAGS_$(1))

$(TARGET_BUILD_DIR)/$(1)/%.o: %.S
	@echo "CC  $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_CC) -c $$< -o $$@ -MMD -MP -DLANGUAGE_ASSEMBLY $(PULP_APP_CFLAGS_$(1))

$(TARGET_BUILD_DIR)/$(1)/$(1): $(PULP_APP_OBJS_$(1))
	@echo "LD  $$@"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_LD) -o $$@ $$^ -MMD -MP $(PULP_APP_LDFLAGS_$(1))

$(TARGET_INSTALL_DIR)/bin/$(1): $(TARGET_BUILD_DIR)/$(1)/$(1)
	@echo "CP  $$@"
	$(V)mkdir -p `dirname $$@`
	$(V)cp $$< $$@

TARGETS += $(TARGET_BUILD_DIR)/$(1)/$(1)
INSTALL_TARGETS += $(TARGET_INSTALL_DIR)/bin/$(1)

endef


ifdef PULP_APP
PULP_APPS += $(PULP_APP)
endif

$(foreach app, $(PULP_APPS), $(eval $(call declare_app,$(app))))



define declare_static_lib

$(eval PULP_STATIC_LIB_SRCS_$(1) += $(PULP_STATIC_LIB_SRCS) $(PULP_STATIC_LIB_FC_SRCS)  $(PULP_STATIC_LIB_CL_SRCS) )
$(eval PULP_STATIC_LIB_ASM_SRCS_$(1) += $(PULP_STATIC_LIB_ASM_SRCS) $(PULP_STATIC_LIB_FC_ASM_SRCS)  $(PULP_STATIC_LIB_CL_ASM_SRCS))
$(eval PULP_STATIC_LIB_OBJS_$(1) += $(patsubst %.c,$(TARGET_BUILD_DIR)/$(1)/%.o,$(PULP_STATIC_LIB_SRCS_$(1))))
$(eval PULP_STATIC_LIB_OBJS_$(1) += $(patsubst %.S,$(TARGET_BUILD_DIR)/$(1)/%.o,$(PULP_STATIC_LIB_ASM_SRCS_$(1))))

$(eval PULP_STATIC_LIB_CFLAGS_$(1) += $(PULP_ARCH_CFLAGS) $(PULP_CFLAGS) $(PULP_APP_CFLAGS))

-include $(PULP_STATIC_LIB_OBJS_$(1):.o=.d)

$(TARGET_BUILD_DIR)/$(1)/%.o: %.c
	@echo "CC  $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_CC) -c $$< -o $$@ -MMD -MP $(PULP_STATIC_LIB_CFLAGS_$(1))

$(TARGET_BUILD_DIR)/$(1)/%.o: %.cpp
	@echo "CXX $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_CC) -c $< -o $@ -MMD -MP $(PULP_STATIC_LIB_CFLAGS_$(1))

$(TARGET_BUILD_DIR)/$(1)/%.o: %.S
	@echo "CC  $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_CC) -c $$< -o $$@ -MMD -MP -DLANGUAGE_ASSEMBLY $(PULP_STATIC_LIB_CFLAGS_$(1))

$(TARGET_BUILD_DIR)/lib/lib$(1).a: $(PULP_STATIC_LIB_OBJS_$(1))
	@echo "AR  $$@"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_AR) -r $$@ $$^

STATIC_LIB_TARGETS += $(TARGET_BUILD_DIR)/lib/lib$(1).a

endef


ifdef PULP_STATIC_LIB
PULP_STATIC_LIBS += $(PULP_STATIC_LIB)
endif

$(foreach static_lib, $(PULP_STATIC_LIBS), $(eval $(call declare_static_lib,$(static_lib))))



ifeq '$(platform)' 'gvsoc'
USE_NEW_GAPY=1
endif


conf:

build: $(TARGETS)

ifdef USE_NEW_GAPY

GAPY_TARGET_OPT = --target=$(GAPY_V2_TARGET)

use_gvsoc_target = 0

ifeq '$(platform)' 'gvsoc'
use_gvsoc_target = 1
endif

ifdef GVSOC_COSIM
use_gvsoc_target = 1
endif

ifeq '$(use_gvsoc_target)' '1'
GAPY_TARGET_OPT += --target-dir=$(GVSOC_PULP_SRC_PATH)/targets
endif

GAPY_TARGET_OPT += --target-dir=$(PULP_SDK_HOME)/tools/gapy/targets

override runner_args += --flash-property=boot@flash:rom:boot \
	--flash-property=$(TARGETS)@flash:rom:binary

GAPY_CMD = $(PULP_SDK_HOME)/tools/gapy_v2/bin/gapy $(GAPY_TARGET_OPT) \
	--platform=$(platform) \
	--work-dir=$(TARGET_BUILD_DIR) \
	--binary=$(TARGETS) \
	$(config_args) $(gapy_args) $(runner_args)

image:
	$(GAPY_CMD) image

flash:
	$(GAPY_CMD) flash

exec:
	$(GAPY_CMD) run

run: build
	$(GAPY_CMD) image flash run

else

GAPY = gapy

image:
	gapy $(GAPY_TARGET_OPT) --platform=$(platform) --work-dir=$(TARGET_BUILD_DIR) $(config_args) $(gapy_args) run --image --binary=$(TARGETS) $(runner_args)

flash:
	gapy $(GAPY_TARGET_OPT) --platform=$(platform) --work-dir=$(TARGET_BUILD_DIR) $(config_args) $(gapy_args) run --flash --binary=$(TARGETS) $(runner_args)

run.prepare:
	gapy $(GAPY_TARGET_OPT) --platform=$(platform) --work-dir=$(TARGET_BUILD_DIR) $(config_args) $(gapy_args) run --exec-prepare --binary=$(TARGETS) $(runner_args)

run.exec:
	gapy $(GAPY_TARGET_OPT) --platform=$(platform) --work-dir=$(TARGET_BUILD_DIR) $(config_args) $(gapy_args) run --exec --binary=$(TARGETS) $(runner_args)

run:
	gapy $(GAPY_TARGET_OPT) --platform=$(platform) --work-dir=$(TARGET_BUILD_DIR) $(config_args) $(gapy_args) run --exec-prepare --exec --binary=$(TARGETS) $(runner_args)

endif

all:: build image flash

clean::
	@echo "RM  $(TARGET_BUILD_DIR)"
	$(V)rm -rf $(TARGET_BUILD_DIR)

dis:
	$(PULP_OBJDUMP) $(PULP_ARCH_OBJDFLAGS) $(disopt) $(TARGETS)

build-lib: $(STATIC_LIB_TARGETS)

install-lib: build-lib
	@echo "INSTALL  $(STATIC_LIB_TARGETS)"
	$(V)mkdir -p $(PULP_EXT_LIBS)
	$(V)mkdir -p $(PULP_EXT_LIBS)/include
	$(V)cp $(STATIC_LIB_TARGETS) $(PULP_EXT_LIBS)
	$(V)cp -t $(PULP_EXT_LIBS)/include  $(PULP_STATIC_LIB_HEADERS)

size:
	$(PULPOS_HOME)/bin/pos-size --binary=$(TARGETS) --depth=10

profiler:
	gapy $(GAPY_TARGET_OPT) --platform=$(platform) --work-dir=$(BUILDDIR) $(config_args) $(gapy_args) --config-opt="gvsoc/events/gen_gtkw=false" run --image --flash --exec-prepare --binary=$(BIN) --event=.*@all.bin --event-format=raw $(runner_args)
	cd $(BUILDDIR) && if [ -e all.bin ]; then rm all.bin; fi; mkfifo all.bin
	cd $(BUILDDIR) && export PULP_CONFIG_FILE=$(BUILDDIR)/gvsoc_config.json && profiler $(BUILDDIR) $(BIN) gvsoc_config.json

help:
	@echo "Makefile options:"
	@echo "  CONFIG_TRACE_LEVEL=<level>    Activate traces for the specified level (0=none, 1=fatal, 2=error, 3=warning, 4=info, 5=debug, 6=trace)."
	@echo "  CONFIG_TRACE_ALL=1            Activate all traces. Other traces can be individually activated with CONFIG_TRACE_<NAME>."

.PHONY: image flash exec run dis size help clean all conf build-lib install-lib
