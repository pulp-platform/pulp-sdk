CONFIG_NB_CLUSTER_PE ?= 8

PULP_LDFLAGS      += 
PULP_CFLAGS       +=  -D__riscv__

ifneq ($(and $(PULP_RISCV_GCC_TOOLCHAIN),$(PULP_RISCV_LLVM_TOOLCHAIN)),)	
$(error PULP_RISCV_GCC_TOOLCHAIN and PULP_RISCV_LLVM_TOOLCHAIN cannot be set both at the same time)
endif

ifdef PULP_RISCV_GCC_TOOLCHAIN
PULP_ARCH_CFLAGS ?=  -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxgap9
endif

ifdef PULP_RISCV_LLVM_TOOLCHAIN
PULP_ARCH_CFLAGS ?=   -target riscv32-unknown-elf -march=rv32imcxpulpv2 --sysroot=${PULP_RISCV_LLVM_TOOLCHAIN}/riscv32-unknown-elf -ffreestanding
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxpulpv2
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxpulpv2
endif

PULP_CFLAGS    += -fdata-sections -ffunction-sections -include pos/chips/pulp/config.h -I$(PULPOS_PULP_HOME)/include/pos/chips/pulp -I$(PULP_EXT_LIBS)/include
ifeq '$(CONFIG_OPENMP)' '1'
PULP_CFLAGS    += -fopenmp -mnativeomp
endif
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULP_EXT_LIBS) -L$(PULPOS_PULP_HOME)/kernel -Tchips/pulp/link.ld -lgcc

PULP_CC = riscv32-unknown-elf-gcc 
PULP_AR ?= riscv32-unknown-elf-ar
PULP_LD ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP ?= riscv32-unknown-elf-objdump

fc/archi=riscv
pe/archi=riscv
pulp_chip=pulp
pulp_chip_family=pulp
cluster/version=5
fc_itc/version=1
udma/cpi/version=1
udma/i2c/version=2
soc/fll/version=1
udma/i2s/version=2
udma/uart/version=1
event_unit/version=3
perf_counters=True
fll/version=1
#padframe/version=1
udma/spim/version=3
#gpio/version=2
udma/archi=3
udma/version=3
soc_eu/version=2

udma/hyper/version=3


# FLL
PULP_SRCS     += kernel/fll-v$(fll/version).c
PULP_SRCS     += kernel/freq-domains.c
PULP_SRCS     += kernel/chips/pulp/soc.c


include $(PULPOS_HOME)/rules/pulpos/configs/default.mk
include $(PULPOS_HOME)/rules/pulpos/default_rules.mk
