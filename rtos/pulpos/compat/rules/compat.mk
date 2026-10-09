COMPAT_HOME := $(patsubst %/rules/,%,$(dir $(lastword $(MAKEFILE_LIST))))


platform ?= rtl

# Registering as a module gets -I<mod>/include, VPATH and rules/pulpos/src.mk for free.
PULPOS_MODULES += $(COMPAT_HOME)

# pulp-runtime layout: build<ext>, not build/<ext>. The fault-injection TCLs and
# the transcript grep in the run rules resolve paths relative to it.
ifndef TARGET_BUILD_DIR
TARGET_BUILD_DIR = $(CURDIR)/build$(build_dir_ext)
endif

CONFIG_KERNEL ?= 1
CONFIG_CLUSTER ?= 1
CONFIG_LIBC_MINIMAL ?= 1

# pulp-runtime applications are bare-metal: no filesystem, no UART stdout.
CONFIG_READFS ?= 0
CONFIG_HOSTFS ?= 0
CONFIG_UART ?= 0

# crt0.S's `la t2, main` is the only reference to main in the SDK, so wrapping it
# lets compat interpose the SPMD entry without patching any SDK source.
PULP_LDFLAGS += -Wl,--wrap=main

PULP_CFLAGS += -DPULP_COMPAT=1

# These tests predate GCC 14 making implicit declarations a hard error.
PULP_CFLAGS += -Wno-error=implicit-function-declaration -Wno-error=int-conversion \
               -Wno-error=incompatible-pointer-types -Wno-error=implicit-int

ifdef CONFIG_OPENMP
PULP_CFLAGS += $(PULP_OMP_CFLAGS)
endif

# Accepted and ignored: legacy pulp-sdk-1 knobs still set by many test Makefiles.
# stackSize pulpFc CONFIG_OPT PULP_APP_HOST_SRCS

include $(PULPOS_HOME)/rules/pulpos.mk

# plptest calls `make conf` before building; pulp-runtime defines it as a no-op.
# all/run/run_gui come from default_rules.mk, keyed on platform and CONFIG_NO_FC.
.PHONY: conf
conf:
