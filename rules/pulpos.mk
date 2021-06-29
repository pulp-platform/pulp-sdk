MODULES = rtos/pulpos/common rtos/pmsis/pmsis_api rtos/pmsis/pmsis_bsp

ifeq '$(TARGET_CHIP_FAMILY)' 'PULP'
MODULES += rtos/pulpos/pulp rtos/pulpos/pulp_archi rtos/pulpos/pulp_hal
endif

ifndef VERBOSE
V = @
endif


define declare_runtime_lib

$(eval PULP_RUNTIME_LIB_SRCS_$(1) += $(PULP_RUNTIME_LIB_SRCS))
$(eval PULP_RUNTIME_LIB_ASM_SRCS_$(1) += $(PULP_RUNTIME_LIB_ASM_SRCS))
$(eval PULP_RUNTIME_LIB_OBJS_$(1) += $(patsubst %.c,$(PULP_LIBS_DIR)/$(1)/%.o,$(PULP_RUNTIME_LIB_SRCS_$(1))))
$(eval PULP_RUNTIME_LIB_OBJS_$(1) += $(patsubst %.S,$(PULP_LIBS_DIR)/$(1)/%.o,$(PULP_RUNTIME_LIB_ASM_SRCS_$(1))))

$(eval PULP_RUNTIME_LIB_CFLAGS_$(1) += $(PULP_RUNTIME_LIB_CFLAGS))

-include $(PULP_RUNTIME_LIB_OBJS_$(1):.o=.d)

$(PULP_LIBS_DIR)/$(1)/%.o: %.c
	@echo "CC  $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_RUNTIME_LIB_CC) -c $$< -o $$@ -MMD -MP $(PULP_RUNTIME_LIB_CFLAGS_$(1))

$(PULP_LIBS_DIR)/$(1)/%.o: %.cpp
	@echo "CXX $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_RUNTIME_LIB_CC) -c $< -o $@ -MMD -MP $(PULP_RUNTIME_LIB_CFLAGS_$(1))

$(PULP_LIBS_DIR)/$(1)/%.o: %.S
	@echo "CC  $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_RUNTIME_LIB_CC) -c $$< -o $$@ -MMD -MP -DLANGUAGE_ASSEMBLY $(PULP_RUNTIME_LIB_CFLAGS_$(1))

$(PULP_LIBS_DIR)/lib$(1).a: $(PULP_RUNTIME_LIB_OBJS_$(1))
	@echo "AR  $$@"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_RUNTIME_LIB_AR) -rcs $$@ $$^

RUNTIME_LIB_TARGETS += $(PULP_LIBS_DIR)/lib$(1).a

endef

# PULP-SPECIFIC RUNTIME LIBRARY. Generates a .a for basic runtime to launch an hello world on PULP
ifeq '$(TARGET_CHIP_FAMILY)' 'PULP'

ifeq '$(CHIP_CORE_ISA)' ''
CHIP_CORE_ISA=pulpv2
endif

ifeq '$(CHIP_CORE_ISA)' 'pulpv2'
PULP_RUNTIME_LIB_CC = riscv32-unknown-elf-gcc
PULP_RUNTIME_LIB_AR = riscv32-unknown-elf-ar
PULP_RUNTIME_LIB_CFLAGS = -march=rv32imcxgap9 -DARCHI_CORE_HAS_PULPV2
PULP_RUNTIME_LIB_LDFLAGS = -march=rv32imcxgap9
else ifeq '$(CHIP_CORE_ISA)' 'corev'
PULP_RUNTIME_LIB_CC = riscv32-corev-elf-gcc
PULP_RUNTIME_LIB_AR = riscv32-corev-elf-ar
PULP_RUNTIME_LIB_CFLAGS = -march=rv32imfc_xcorev -DARCHI_HAS_COREV -DPLP_NO_BUILTIN
PULP_RUNTIME_LIB_LDFLAGS = -march=rv32imfc_xcorev
endif

# from pulp.mk
runtime_lib/udma/uart/version=1
runtime_lib/event_unit/version=3
runtime_lib/fll/version=1
runtime_lib/udma/archi=3
runtime_lib/soc_eu/version=2

PULP_RUNTIME_LIB_SRCS = $(PULPOS_HOME)/kernel/fll-v$(runtime_lib/fll/version).c $(PULPOS_HOME)/kernel/freq-domains.c $(PULPOS_PULP_HOME)/kernel/chips/pulp/soc.c $(PULPOS_HOME)/lib/libc/minimal/io.c \
	$(PULPOS_HOME)/lib/libc/minimal/fprintf.c $(PULPOS_HOME)/lib/libc/minimal/prf.c $(PULPOS_HOME)/lib/libc/minimal/sprintf.c $(PULPOS_HOME)/lib/libc/minimal/semihost.c $(PULPOS_HOME)/kernel/init.c \
	$(PULPOS_HOME)/kernel/kernel.c $(PULPOS_HOME)/kernel/device.c $(PULPOS_HOME)/kernel/task.c $(PULPOS_HOME)/kernel/alloc.c $(PULPOS_HOME)/kernel/alloc_pool.c $(PULPOS_HOME)/kernel/irq.c $(PULPOS_HOME)/kernel/soc_event.c \
	$(PULPOS_HOME)/kernel/log.c $(PULPOS_HOME)/kernel/time.c $(PULPOS_PULP_HOME)/drivers/uart/uart-v$(runtime_lib/udma/uart/version).c $(PULPOS_PULP_HOME)/drivers/udma/udma-v$(runtime_lib/udma/archi).c $(PULPOS_PULP_HOME)/drivers/cluster/cluster.c \
	$(PULP_SDK_HOME)/rtos/pmsis/pmsis_bsp/bsp/pulp.c $(PULP_SDK_HOME)/rtos/pmsis/pmsis_bsp/fs/read_fs/read_fs.c $(PULP_SDK_HOME)/rtos/pmsis/pmsis_bsp/fs/host_fs/semihost.c $(PULP_SDK_HOME)/rtos/pmsis/pmsis_bsp/fs/host_fs/host_fs.c $(PULP_SDK_HOME)/rtos/pmsis/pmsis_bsp/fs/fs.c

PULP_RUNTIME_LIB_ASM_SRCS = $(PULPOS_HOME)/kernel/crt0.S $(PULPOS_HOME)/kernel/irq_asm.S $(PULPOS_HOME)/kernel/task_asm.S $(PULPOS_HOME)/kernel/time_asm.S $(PULPOS_HOME)/kernel/soc_event_v$(runtime_lib/soc_eu/version)_itc.S \
	$(PULPOS_PULP_HOME)/drivers/cluster/pe-eu-v$(runtime_lib/event_unit/version).S

PULP_RUNTIME_LIB_CFLAGS += -MMD -MP -D__riscv__ -fdata-sections -ffunction-sections -include pos/chips/pulp/config.h \
	-I$(PULP_SDK_HOME)/rtos/pulpos/pulp/include/pos/chips/pulp -I$(PULP_SDK_HOME)/ext_libs/include \
	-DCONFIG_PULP -DCONFIG_BOARD_VERSION_PULP -DCONFIG_PROFILE_PULP -D__CONFIG_UDMA__ -fno-jump-tables \
	-fno-tree-loop-distribute-patterns -D__PULPOS2__ -D__PLATFORM__=ARCHI_PLATFORM_GVSOC -DARCHI_CLUSTER_NB_PE=8 \
	-DPOS_CONFIG_IO_UART=0 -DPOS_CONFIG_IO_UART_BAUDRATE=115200 -DPOS_CONFIG_IO_UART_ITF=0 -D__TRACE_LEVEL__=3 -DPI_LOG_LOCAL_LEVEL=2 \
	-I$(PULP_SDK_HOME)/rtos/pulpos/common/lib/libc/minimal/include -I$(PULP_SDK_HOME)/rtos/pulpos/common/include \
	-I$(PULP_SDK_HOME)/rtos/pulpos/common/kernel -I$(PULP_SDK_HOME)/rtos/pulpos/pulp_archi/include \
	-I$(PULP_SDK_HOME)/rtos/pulpos/pulp_hal/include -I$(PULP_SDK_HOME)/rtos/pmsis/pmsis_api/include -I$(PULP_SDK_HOME)/rtos/pmsis/pmsis_bsp/include -I$(PULP_SDK_HOME)/rtos/pulpos/pulp/include

PULP_RUNTIME_LIB_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections \
	-L$(PULP_SDK_HOME)/ext_libs -L$(PULP_SDK_HOME)/rtos/pulpos/pulp/kernel -Tchips/pulp/link.ld -lgcc

endif

PULP_RUNTIME_LIBS += basic-runtime

$(foreach runtime_lib, $(PULP_RUNTIME_LIBS), $(eval $(call declare_runtime_lib,$(runtime_lib))))


pulpos.checkout.deps:

pulpos.checkout:
	git submodule update --init $(MODULES)

pulpos.checkout.all: pulpos.checkout pulpos.checkout.deps

pulpos.build.deps:

pulpos.build:
	
pulpos.clean:

pulpos.src:
	make -C tools/pulpos src INSTALL_DIR=$(ARTIFACT_PATH)/tools/pulpos

pulpos.build.all: pulpos.build.deps pulpos.build

pulpos.all: pulpos.checkout.all pulpos.build.all

pulpos.artifact:
	rsync -avR --exclude=".git*" --exclude=__pycache__ rtos/pulpos/common rtos/pulpos/pulp rtos/pulpos/pulp_archi rtos/pulpos/pulp_hal rtos/pmsis $(ARTIFACT_PATH)

pulpos.libs: pulpos.build.libs

pulpos.build.libs: $(RUNTIME_LIB_TARGETS)
	$(V)rm -rf $(PULP_LIBS_DIR)/$(PULP_RUNTIME_LIBS)
