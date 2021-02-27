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
// func_name                      layerAveragePoolRelu27
// flag_DW                        0
// optional                       AveragePoolRelu
// FLAG_BATCHNORM                 0
// has_bias                       1
// FLAG_RELU                      1
// test_location                  L3
// platform                       GAP8
// chip                           GAP8v3
// type                           char
// nof                            1024
// factor                         1
// g                              1
// nif                            1024
// conv_overlap1                  0
// conv_overlap2                  0
// padding_top                    0
// padding_bottom                 0
// padding_left                   0
// padding_right                  0
// stride                         4
// x_h                            4
// x_w                            4
// x_data_size_byte               8
// x_tile_size_nif                1024
// x_tile_size_h                  4
// x_tile_size_w                  4
// x_tile_size_byte               16384
// x_tile_size_nif_byte           1024
// x_stride_w_byte                4096
// x_stride_c_byte                1024
// y_h                            1
// y_w                            1
// y_data_size_byte               8
// act_dim_bit                    0
// y_tile_size_nof                1024
// y_tile_size_h                  1
// y_tile_size_w                  1
// y_tile_size_byte               1024
// y_stride_w_byte                1024
// y_stride_c_byte                1024
// y_tile_size_nof_byte           1024
// tile_dim_h                     1
// tile_dim_w                     1
// tile_dim_nof                   1
// tile_dim_nif                   1
// fs1                            4
// fs2                            4
// W_data_size_byte               0
// W_tile_size_nof                1024
// b_size_byte                    0
// W_tile_size_nif                1024
// W_tile_size_byte               0
// W_stride_nof_byte              0
// W_stride_hw_byte               0
// W_tile_nif_byte                0
// l2_off_bias                    0
// k_tile_size_byte               0
// lambda_tile_size_byte          0
// k_size_byte                    0
// lambda_size_byte               0
// l1_x_offset                    0
// l1_y_offset                    16388
// x_tile_size_nif_last           1024
// x_tile_size_nif_byte_last      1024
// x_tile_size_h_last             4
// x_tile_size_w_last             4
// y_tile_size_nof_last           1024
// y_tile_size_h_last             1
// y_tile_size_w_last             1
// y_length_nof_byte_last         1024

#include "dory.h"

void  layerAveragePoolRelu27(
  void *args
);
