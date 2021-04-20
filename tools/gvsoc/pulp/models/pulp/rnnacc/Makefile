ifeq '$(rnnacc/version)' '1'
ifdef XTENSOR_INCLUDE_DIR

IMPLEMENTATIONS += pulp/rnnacc/rnnacc_v1_impl
COMPONENTS += pulp/rnnacc/rnnacc_v1

pulp/rnnacc/rnnacc_v1_impl_CFLAGS = -I. -DXTENSOR_USE_XSIMD -isystem $(XTENSOR_INCLUDE_DIR) -I$(XTENSOR_INCLUDE_DIR) -march=native -std=c++14

pulp/rnnacc/rnnacc_v1_impl_SRCS = pulp/rnnacc/rnnacc_v1_impl.cpp \
								  pulp/rnnacc/src/rnnacc_streamer.cpp \
								  pulp/rnnacc/src/rnnacc_regfile.cpp \
								  pulp/rnnacc/src/rnnacc_loads.cpp \
								  pulp/rnnacc/src/rnnacc_stores.cpp \
								  pulp/rnnacc/src/rnnacc_matmul.cpp \
								  pulp/rnnacc/src/rnnacc_fsm.cpp \
								  pulp/rnnacc/src/rnnacc_debug.cpp \
								  pulp/rnnacc/src/rnnacc_clear.cpp

endif
else
IMPLEMENTATIONS += pulp/rnnacc/rnnacc_v1_impl
pulp/rnnacc/rnnacc_v1_impl_SRCS = pulp/rnnacc/src/rnnacc_empty.cpp
endif