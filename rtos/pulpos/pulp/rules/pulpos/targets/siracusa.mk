PULP_CC = riscv32-unknown-elf-gcc
PULP_AR ?= riscv32-unknown-elf-ar
PULP_LD ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP ?= riscv32-unknown-elf-objdump

CONFIG_NB_CLUSTER_PE ?= 8

PULP_CC_VERSION = $(shell $(PULP_CC) -dumpversion)

ifeq '$(PULP_CC_VERSION)' '9.2.0'
PULP_ARCH_CFLAGS ?=  -march=rv32imc_zfinx_xpulpv3
PULP_ARCH_LDFLAGS ?=  -march=rv32imc_zfinx_xpulpv3
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imc_zfinx_xpulpv3
# PULP_ARCH_CFLAGS ?=  -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
# PULP_ARCH_LDFLAGS ?=  -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
# PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxgap9
else
PULP_ARCH_CFLAGS ?=  -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxgap9
endif

PULP_CFLAGS    += -D__riscv__ -fdata-sections -ffunction-sections -include pos/chips/siracusa/config.h -I$(PULPOS_PULP_HOME)/include/pos/chips/siracusa -I$(PULPOS_PULP_HOME)/drivers/i3c/include -I$(PULPOS_PULP_HOME)/drivers/siracusa_padmux/include -I$(PULP_EXT_LIBS)/include

ifeq '$(CONFIG_OPENMP)' '1'
PULP_CFLAGS    += -fopenmp -mnativeomp
endif

PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULP_EXT_LIBS) -L$(PULPOS_PULP_HOME)/kernel -Tchips/siracusa/link.ld -lgcc

fc/archi=riscv
pe/archi=riscv
pulp_chip=siracusa
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

# HYPER
udma/hyper/version=3
CONFIG_HYPERFLASH=1
CONFIG_HYPERRAM=1
CONFIG_HYPER=1

# I3C
CONFIG_I3C=1

# PAD
CONFIG_PAD=1

# FLL
PULP_SRCS     += kernel/chips/siracusa/pll.c
PULP_SRCS     += kernel/chips/siracusa/soc.c

# I3C driver
PULP_SRCS     += drivers/i3c/src/cdn_print.c
PULP_SRCS     += drivers/i3c/src/command_list.c
PULP_SRCS     += drivers/i3c/src/i3c.c
PULP_SRCS     += drivers/i3c/src/i3c_obj_if.c
PULP_SRCS     += drivers/i3c/src/cps_impl.c

# Padmultiplexing
PULP_SRCS     += drivers/siracusa_padmux/src/siracusa_padctrl.c


include $(PULPOS_HOME)/rules/pulpos/configs/default.mk
include $(PULPOS_HOME)/rules/pulpos/default_rules.mk
