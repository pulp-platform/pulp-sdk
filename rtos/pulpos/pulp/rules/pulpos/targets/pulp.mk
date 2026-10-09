CONFIG_NB_CLUSTER_PE ?= 8

PULP_LDFLAGS      += 
ifdef USE_CV32E40P
# No xpulp ISA and no __builtin_pulp_*; downstream keys off __cv32e40p__.
PULP_CFLAGS       +=  -D__cv32e40p__
else
PULP_CFLAGS       +=  -D__riscv__
endif

ifneq ($(and $(PULP_RISCV_GCC_TOOLCHAIN),$(PULP_RISCV_LLVM_TOOLCHAIN)),)	
$(error PULP_RISCV_GCC_TOOLCHAIN and PULP_RISCV_LLVM_TOOLCHAIN cannot be set both at the same time)
endif

ifdef PULP_RISCV_GCC_TOOLCHAIN
ifdef USE_CV32E40P

PULP_CV32_PREFIX := $(if $(wildcard $(PULP_RISCV_GCC_TOOLCHAIN)/bin/riscv32-unknown-elf-gcc),riscv32-unknown-elf,riscv64-unknown-elf)
PULP_CV32_GCC    := $(PULP_RISCV_GCC_TOOLCHAIN)/bin/$(PULP_CV32_PREFIX)-gcc

PULP_CV32_MARCH  := -march=rv32imc_xcvalu_xcvbi_xcvbitmanip_xcvhwlp_xcvmac_xcvmem_xcvsimd_xcvelw_zfinx_zhinx  -mabi=ilp32
PULP_CV32_TUNE   := $(shell $(PULP_CV32_GCC) -mtune=cv32e40p -E -x c /dev/null >/dev/null 2>&1 && echo -mtune=cv32e40p)

PULP_ARCH_CFLAGS ?=  $(PULP_CV32_MARCH) $(PULP_CV32_TUNE)
PULP_ARCH_LDFLAGS ?=  $(PULP_CV32_MARCH)
PULP_ARCH_OBJDFLAGS ?=

PULP_CC = $(PULP_CV32_PREFIX)-gcc
PULP_AR ?= $(PULP_CV32_PREFIX)-ar
PULP_LD ?= $(PULP_CV32_PREFIX)-gcc
PULP_OBJDUMP ?= $(PULP_CV32_PREFIX)-objdump

ifeq ($(PULP_CV32_PREFIX),riscv64-unknown-elf)
PULP_LDFLAGS += -L$(dir $(shell $(PULP_CV32_GCC) -march=rv32imc -mabi=ilp32 -print-libgcc-file-name))
endif
else
ifdef CONFIG_NO_FC
PULP_ARCH_CFLAGS ?=  -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE)
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE)
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxgap9
else
PULP_ARCH_CFLAGS ?=  -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxgap9 -mPE=$(CONFIG_NB_CLUSTER_PE) -mFC=1
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxgap9
endif
endif
endif

ifdef PULP_RISCV_LLVM_TOOLCHAIN
ifdef USE_CV32E40P
RISCV_SYSROOT    ?= $(PULP_RISCV_LLVM_TOOLCHAIN)
PULP_CV32_MARCH  := -target riscv32-unknown-elf -march=rv32imc_xcvalu_xcvbi_xcvbitmanip_xcvhwlp_xcvmac_xcvmem_xcvsimd_xcvelw_zfinx  -mabi=ilp32 --sysroot=${RISCV_SYSROOT}

PULP_ARCH_CFLAGS ?=  $(PULP_CV32_MARCH) -isystem ${RISCV_SYSROOT}/include
PULP_ARCH_LDFLAGS ?=  $(PULP_CV32_MARCH) -L${RISCV_SYSROOT}/lib
PULP_ARCH_OBJDFLAGS ?=

PULP_CC = clang
PULP_AR ?= llvm-ar
PULP_LD ?= clang
PULP_OBJDUMP ?= llvm-objdump
else
PULP_ARCH_CFLAGS ?=   -target riscv32-unknown-elf -march=rv32imcxpulpv2 --sysroot=${PULP_RISCV_LLVM_TOOLCHAIN}/riscv32-unknown-elf -ffreestanding
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxpulpv2
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxpulpv2
endif
endif

PULP_CFLAGS    += -fdata-sections -ffunction-sections -include pos/chips/pulp/config.h -I$(PULPOS_PULP_HOME)/include/pos/chips/pulp -I$(PULP_EXT_LIBS)/include
ifeq '$(CONFIG_OPENMP)' '1'
PULP_CFLAGS    += -fopenmp -mnativeomp
endif

ifdef PULP_RISCV_GCC_TOOLCHAIN
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULP_EXT_LIBS) -L$(PULPOS_PULP_HOME)/kernel -Tchips/pulp/link.ld -lgcc
else
PULP_LDFLAGS += -nostartfiles -Wl,--gc-sections -L$(PULP_EXT_LIBS) -L$(PULPOS_PULP_HOME)/kernel -Tchips/pulp/link.ld -fuse-ld=lld --rtlib=compiler-rt -lc
endif

ifndef USE_CV32E40P
PULP_CC = riscv32-unknown-elf-gcc
PULP_AR ?= riscv32-unknown-elf-ar
PULP_LD ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP ?= riscv32-unknown-elf-objdump
endif

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
