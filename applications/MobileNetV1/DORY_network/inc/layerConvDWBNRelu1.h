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
// func_name                      layerConvDWBNRelu1
// flag_DW                        1
// optional                       conv
// FLAG_BATCHNORM                 1
// has_bias                       0
// FLAG_RELU                      1
// test_location                  L3
// platform                       GAP8
// chip                           GAPv2
// type                           char
// nof                            32
// factor                         1
// g                              32
// nif                            1
// conv_overlap1                  2
// conv_overlap2                  2
// padding_top                    1
// padding_bottom                 1
// padding_left                   1
// padding_right                  1
// stride                         1
// x_h                            64
// x_w                            64
// x_data_size_byte               8
// x_tile_size_nif                16
// x_tile_size_h                  10
// x_tile_size_w                  64
// x_tile_size_byte               10240
// x_tile_size_nif_byte           16
// x_stride_w_byte                2048
// x_stride_c_byte                32
// y_h                            64
// y_w                            64
// y_data_size_byte               8
// act_dim_bit                    32
// y_tile_size_nof                16
// y_tile_size_h                  8
// y_tile_size_w                  64
// y_tile_size_byte               8192
// y_stride_w_byte                2048
// y_stride_c_byte                32
// y_tile_size_nof_byte           16
// tile_dim_h                     8
// tile_dim_w                     1
// tile_dim_nof                   2
// tile_dim_nif                   2
// fs1                            3
// fs2                            3
// W_data_size_byte               8
// W_tile_size_nof                16
// b_size_byte                    32
// W_tile_size_nif                1
// W_tile_size_byte               144
// W_stride_nof_byte              9
// W_stride_hw_byte               1
// W_tile_nif_byte                1
// l2_off_k                       288
// l2_off_lambda                  416
// k_tile_size_byte               128
// lambda_tile_size_byte          128
// k_size_byte                    128
// lambda_size_byte               128
// k_tile_size_byte_transfer      64
// lambda_tile_size_byte_transfer 64
// l1_x_offset                    0
// l1_y_offset                    20484
// l1_W_offset                    36872
// l1_k_offset                    37164
// l1_lambda_offset               37296
// x_tile_size_nif_last           16
// x_tile_size_nif_byte_last      16
// x_tile_size_h_last             9
// x_tile_size_w_last             64
// W_tile_size_nof_last           16
// W_tile_size_nif_last           1
// W_tile_size_nif_byte_last      1
// y_tile_size_nof_last           16
// y_tile_size_h_last             8
// y_tile_size_w_last             64
// y_length_nof_byte_last         16

#include "dory.h"

void  layerConvDWBNRelu1(
  void *args
);
