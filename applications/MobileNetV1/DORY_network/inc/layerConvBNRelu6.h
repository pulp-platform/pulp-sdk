/*
 * layer_template_h.h
 * Alessio Burrello <alessio.burrello@unibo.it>
 *
 * Copyright (C) 2019-2020 University of Bologna
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

// sdk                            pulp_sdk
// dma_parallelization            8-cores
// optional_type                  8bit
// func_name                      layerConvBNRelu6
// flag_DW                        0
// optional                       conv
// FLAG_BATCHNORM                 1
// has_bias                       0
// FLAG_RELU                      1
// test_location                  L3
// platform                       GAP8
// chip                           GAP8v3
// type                           char
// nof                            128
// factor                         1
// g                              1
// nif                            128
// conv_overlap1                  0
// conv_overlap2                  0
// padding_top                    0
// padding_bottom                 0
// padding_left                   0
// padding_right                  0
// stride                         1
// x_h                            32
// x_w                            32
// x_data_size_byte               8
// x_tile_size_nif                128
// x_tile_size_h                  24
// x_tile_size_w                  2
// x_tile_size_byte               6144
// x_tile_size_nif_byte           128
// x_stride_w_byte                4096
// x_stride_c_byte                128
// y_h                            32
// y_w                            32
// y_data_size_byte               8
// act_dim_bit                    32
// y_tile_size_nof                64
// y_tile_size_h                  24
// y_tile_size_w                  2
// y_tile_size_byte               3072
// y_stride_w_byte                4096
// y_stride_c_byte                128
// y_tile_size_nof_byte           64
// tile_dim_h                     2
// tile_dim_w                     16
// tile_dim_nof                   2
// tile_dim_nif                   1
// fs1                            1
// fs2                            1
// W_data_size_byte               8
// W_tile_size_nof                64
// b_size_byte                    128
// W_tile_size_nif                128
// W_tile_size_byte               8192
// W_stride_nof_byte              128
// W_stride_hw_byte               128
// W_tile_nif_byte                128
// l2_off_k                       16384
// l2_off_lambda                  16896
// k_tile_size_byte               512
// lambda_tile_size_byte          512
// k_size_byte                    512
// lambda_size_byte               512
// k_tile_size_byte_transfer      256
// lambda_tile_size_byte_transfer 256
// l1_x_offset                    0
// l1_y_offset                    12292
// l1_W_offset                    18440
// l1_k_offset                    34828
// l1_lambda_offset               35344
// x_tile_size_nif_last           128
// x_tile_size_nif_byte_last      128
// x_tile_size_h_last             8
// x_tile_size_w_last             2
// W_tile_size_nof_last           64
// W_tile_size_nif_last           128
// W_tile_size_nif_byte_last      128
// y_tile_size_nof_last           64
// y_tile_size_h_last             8
// y_tile_size_w_last             2
// y_length_nof_byte_last         64

#include "dory.h"

void  layerConvBNRelu6(
  void *args
);
