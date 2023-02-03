ifdef XTENSOR_INCLUDE_DIR
IMPLEMENTATIONS += pulp/neureka/neureka
pulp/neureka/neureka_SRCS = pulp/neureka/src/neureka.cpp \
                      pulp/neureka/src/neureka_fsm.cpp \
                      pulp/neureka/src/neureka_stream.cpp \
                      pulp/neureka/src/neureka_regfile.cpp \
                      pulp/neureka/src/neureka_clear.cpp \
                      pulp/neureka/src/neureka_load.cpp \
                      pulp/neureka/src/neureka_streamin.cpp \
                      pulp/neureka/src/neureka_matrixvec.cpp \
                      pulp/neureka/src/neureka_streamout.cpp \
                      pulp/neureka/src/neureka_normquant.cpp \
                      pulp/neureka/src/neureka_index.cpp \
                      pulp/neureka/src/neureka_debug.cpp
pulp/neureka/neureka_CFLAGS = -Ipulp/neureka/include -DXTENSOR_USE_XSIMD -isystem $(XTENSOR_INCLUDE_DIR) -march=native -std=c++14
else
IMPLEMENTATIONS += pulp/neureka/neureka
pulp/neureka/neureka_SRCS = pulp/neureka/src/neureka_empty.cpp
endif
