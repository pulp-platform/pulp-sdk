ifdef XTENSOR_INCLUDE_DIR
IMPLEMENTATIONS += pulp/redMule/redMule
pulp/redMule/redMule_SRCS = pulp/redMule/src/redMule.cpp \
                      pulp/redMule/src/redMule_fsm.cpp \
                      pulp/redMule/src/redMule_stream.cpp \
                      pulp/redMule/src/redMule_regfile.cpp \
                      pulp/redMule/src/redMule_debug.cpp
pulp/redMule/redMule_CFLAGS = -Ipulp/redMule/include -DXTENSOR_USE_XSIMD -isystem $(XTENSOR_INCLUDE_DIR) -march=native -std=c++14
else
IMPLEMENTATIONS += pulp/redMule/redMule
pulp/redMule/redMule_SRCS = pulp/redMule/src/redMule_empty.cpp
endif
