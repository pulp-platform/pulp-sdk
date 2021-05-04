# Makefile
# Alessio Burrello <alessio.burrello@unibo.it>
#
# Copyright (C) 2019-2020 University of Bologna
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

APP = main
APP_SRCS = DORY_network/src/dory.c

APP_SRCS+=DORY_network/src/layerAveragePoolRelu27.c
APP_SRCS+=DORY_network/src/layerConvBNRelu0.c
APP_SRCS+=DORY_network/src/layerConvBNRelu10.c
APP_SRCS+=DORY_network/src/layerConvBNRelu12.c
APP_SRCS+=DORY_network/src/layerConvBNRelu14.c
APP_SRCS+=DORY_network/src/layerConvBNRelu14L3.c
APP_SRCS+=DORY_network/src/layerConvBNRelu2.c
APP_SRCS+=DORY_network/src/layerConvBNRelu24.c
APP_SRCS+=DORY_network/src/layerConvBNRelu24L3.c
APP_SRCS+=DORY_network/src/layerConvBNRelu26.c
APP_SRCS+=DORY_network/src/layerConvBNRelu26L3.c
APP_SRCS+=DORY_network/src/layerConvBNRelu2L3.c
APP_SRCS+=DORY_network/src/layerConvBNRelu4.c
APP_SRCS+=DORY_network/src/layerConvBNRelu6.c
APP_SRCS+=DORY_network/src/layerConvBNRelu8.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu1.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu11.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu13.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu23.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu25.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu3.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu3L3.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu3_p_b.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu3_p_t.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu5.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu7.c
APP_SRCS+=DORY_network/src/layerConvDWBNRelu9.c
APP_SRCS+=DORY_network/src/layerGemm28_last.c
APP_SRCS+=DORY_network/src/layerGemm28_lastL3.c
APP_SRCS+=DORY_network/src/mem_controller.c
APP_SRCS+=DORY_network/src/network.c
APP_SRCS+=DORY_network/src/pulp_nn_add.c
APP_SRCS+=DORY_network/src/pulp_nn_avgpool.c
APP_SRCS+=DORY_network/src/pulp_nn_conv_Co_parallel.c
APP_SRCS+=DORY_network/src/pulp_nn_conv_HoWo_parallel.c
APP_SRCS+=DORY_network/src/pulp_nn_conv_Ho_parallel.c
APP_SRCS+=DORY_network/src/pulp_nn_depthwise_3x3_s1.c
APP_SRCS+=DORY_network/src/pulp_nn_depthwise_generic.c
APP_SRCS+=DORY_network/src/pulp_nn_linear.c
APP_SRCS+=DORY_network/src/pulp_nn_linear_out_32.c
APP_SRCS+=DORY_network/src/pulp_nn_matmul.c
APP_SRCS+=DORY_network/src/pulp_nn_maxpool.c
APP_SRCS+=DORY_network/src/pulp_nn_pointwise_Co_parallel.c
APP_SRCS+=DORY_network/src/pulp_nn_pointwise_HoWo_parallel.c
APP_SRCS+=DORY_network/src/pulp_nn_pointwise_Ho_parallel.c
APP_SRCS+=DORY_network/src/pulp_nn_utils.c
APP_SRCS+=DORY_network/src/test_template.c

ifndef CORE
CORE=1
else
CORE= $(CORE)
endif

APP_CFLAGS += -DNUM_CORES=$(CORE) -IDORY_network/inc -O3 -w -g3 -fno-tree-loop-distribute-patterns -flto
APP_LDFLAGS = -lm -flto

CONFIG_HYPERRAM = 1
CONFIG_HYPERFLASH = 1

FLASH_FILES = DORY_network/ConvBNRelu0_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu1_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu2_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu3_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu4_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu5_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu6_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu7_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu8_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu9_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu10_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu11_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu12_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu13_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu14_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu15_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu16_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu17_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu18_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu19_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu20_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu21_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu22_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu23_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu24_weights.hex
FLASH_FILES += DORY_network/ConvDWBNRelu25_weights.hex
FLASH_FILES += DORY_network/ConvBNRelu26_weights.hex
FLASH_FILES += DORY_network/Gemm28_weights.hex
FLASH_FILES += DORY_network/inputs.hex
READFS_FILES := $(FLASH_FILES)
PLPBRIDGE_FLAGS += -f

include $(RULES_DIR)/pmsis_rules.mk
