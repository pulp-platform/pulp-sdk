CONFIG_NB_CLUSTER_PE ?= 8

ifeq '$(CHIP_CORE_ISA)' ''
CHIP_CORE_ISA=pulpv2
endif

PULP_LDFLAGS         +=
ifeq '$(CHIP_CORE_ISA)' 'pulpv2'
PULP_CFLAGS          += -D__riscv__ -D__PULP_TOOLCHAIN__ -DARCHI_CORE_HAS_PULPV2
PULP_ARCH_CFLAGS     ?= -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_LDFLAGS    ?= -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_OBJDFLAGS  ?= -Mmarch=rv32imcxgap9

PULP_CC              ?= riscv32-unknown-elf-gcc
PULP_AR              ?= riscv32-unknown-elf-ar
PULP_LD              ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP         ?= riscv32-unknown-elf-objdump
PULP_NM      ?= riscv32-unknown-elf-nm
else ifeq '$(CHIP_CORE_ISA)' 'pulpnn'
PULP_CFLAGS          += -D__riscv__ -D__PULP_TOOLCHAIN__ -DARCHI_CORE_HAS_PULPV2
PULP_ARCH_CFLAGS     ?= -march=rv32imcxpulpnn -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_LDFLAGS    ?= -march=rv32imcxpulpnn -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_OBJDFLAGS  ?= -Mmarch=rv32imcxpulpnn

PULP_CC              ?= riscv32-unknown-elf-gcc
PULP_AR              ?= riscv32-unknown-elf-ar
PULP_LD              ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP         ?= riscv32-unknown-elf-objdump
PULP_NM      ?= riscv32-unknown-elf-nm
else ifeq '$(CHIP_CORE_ISA)' 'ibex'
PULP_CFLAGS 	     += -D__ibex__ -U__riscv__ -UARCHI_CORE_HAS_PULPV2 -DPLP_NO_BUILTIN
PULP_ARCH_CFLAGS     ?= -march=rv32imc -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_LDFLAGS    ?= -march=rv32imc -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_OBJDFLAGS  ?= -Mmarch=rv32imc

PULP_CC              ?= riscv32-unknown-elf-gcc
PULP_AR              ?= riscv32-unknown-elf-ar
PULP_LD              ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP         ?= riscv32-unknown-elf-objdump
PULP_NM      ?= riscv32-unknown-elf-nm
else ifeq '$(CHIP_CORE_ISA)' 'corev'

endif
PULP_CFLAGS          += -fdata-sections -ffunction-sections -include pos/chips/pulp/config.h -I$(PULPOS_PULP_HOME)/include/pos/chips/pulp -I$(PULP_EXT_LIBS)/include -include $(PULPOS_PULP_HOME)/include/pos/data/irq.h
PULP_OMP_CFLAGS      += -fopenmp -mnativeomp
PULP_LDFLAGS         += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULP_EXT_LIBS) -L$(PULPOS_PULP_HOME)/kernel -T$(PULPOS_PULP_HOME)/kernel/chips/pulp/link.ld -lgcc

fc/archi             = riscv
pe/archi             = riscv
pulp_chip            = pulp
pulp_chip_family     = pulp
cluster/version      = 5
fc_itc/version       = 1
udma/cpi/version     = 1
udma/i2c/version     = 2
soc/fll/version      = 1
udma/i2s/version     = 2
udma/uart/version    = 1
event_unit/version   = 3
perf_counters        = True
fll/version          = 1
#padframe/version    = 1
udma/spim/version    = 3
#gpio/version        = 2
udma/archi           = 3
udma/version         = 3
soc_eu/version       = 2
udma/hyper/version   = 3


# FLL
PULP_SRCS            += kernel/chips/pulp/fll-v$(fll/version).c
PULP_SRCS            += kernel/freq-domains.c
PULP_SRCS            += kernel/chips/pulp/soc.c

PULP_ASM_SRCS        += kernel/chips/pulp/task_asm.S

# SOC EVENT
ifneq '$(soc_eu/version)' ''
ifneq '$(fc_itc/version)' ''
PULP_ASM_SRCS        += kernel/chips/pulp/soc_event_v$(soc_eu/version)_itc.S
else
PULP_ASM_SRCS        += kernel/chips/pulp/soc_event_eu.S
endif
endif


include $(PULPOS_HOME)/rules/pulpos/configs/default.mk
include $(PULPOS_HOME)/rules/pulpos/default_rules.mk
