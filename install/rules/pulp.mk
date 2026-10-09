# pulp-runtime compatibility entry point.
# pulp-runtime satisfies this same path via a symlink after aliasing
# PULP_SDK_HOME=$PULPRT_HOME, so apps build against whichever runtime is sourced.

include $(PULP_SDK_HOME)/rtos/pulpos/compat/rules/compat.mk
