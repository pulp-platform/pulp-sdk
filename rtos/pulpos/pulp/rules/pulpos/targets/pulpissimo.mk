PULP_LDFLAGS      += 
PULP_CFLAGS       +=  -D__riscv__
PULP_ARCH_CFLAGS    ?= -march=rv32imcxgap9 -mFC=1
PULP_ARCH_LDFLAGS   ?= -march=rv32imcxgap9 -mFC=1
PULP_ARCH_OBJDFLAGS ?= -March=rv32imcxgap9
PULP_CFLAGS    += -fdata-sections -ffunction-sections -include pos/chips/pulpissimo/config.h -I$(PULPOS_PULP_HOME)/include/pos/chips/pulpissimo -I$(PULP_EXT_LIBS)/include
ifeq '$(CONFIG_OPENMP)' '1'
PULP_CFLAGS    += -fopenmp -mnativeomp
endif
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULP_EXT_LIBS) -L$(PULPOS_PULP_HOME)/kernel -Tchips/pulpissimo/link.ld -lgcc

PULP_CC = riscv32-unknown-elf-gcc 
PULP_AR ?= riscv32-unknown-elf-ar
PULP_LD ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP ?= riscv32-unknown-elf-objdump

fc/archi=riscv
pulp_chip=pulpissimo
pulp_chip_family=pulp
fc_itc/version=1
udma/cpi/version=1
udma/i2c/version=2
soc/fll/version=1
udma/i2s/version=2
udma/uart/version=1
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
PULP_SRCS     += kernel/chips/pulpissimo/soc.c


include $(PULPOS_HOME)/rules/pulpos/configs/default.mk
include $(PULPOS_HOME)/rules/pulpos/default_rules.mk
