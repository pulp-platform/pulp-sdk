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
// func_name                      layerGemm28_last
// flag_DW                        0
// optional                       conv
// FLAG_BATCHNORM                 0
// has_bias                       0
// FLAG_RELU                      0
// test_location                  L3
// platform                       GAP8
// chip                           GAP8v3
// type                           char
// nof                            20
// factor                         50
// g                              1
// nif                            1024
// conv_overlap1                  0
// conv_overlap2                  0
// padding_top                    0
// padding_bottom                 0
// padding_left                   0
// padding_right                  0
// stride                         1
// x_h                            1
// x_w                            1
// x_data_size_byte               8
// x_tile_size_nif                1024
// x_tile_size_h                  1
// x_tile_size_w                  1
// x_tile_size_byte               1024
// x_tile_size_nif_byte           1024
// x_stride_w_byte                1024
// x_stride_c_byte                1024
// y_h                            1
// y_w                            1
// y_data_size_byte               32
// act_dim_bit                    32
// y_tile_size_nof                20
// y_tile_size_h                  1
// y_tile_size_w                  1
// y_tile_size_byte               80
// y_stride_w_byte                4000
// y_stride_c_byte                4000
// y_tile_size_nof_byte           80
// tile_dim_h                     1
// tile_dim_w                     1
// tile_dim_nof                   1
// tile_dim_nif                   1
// fs1                            1
// fs2                            1
// W_data_size_byte               8
// W_tile_size_nof                20
// b_size_byte                    20
// W_tile_size_nif                1024
// W_tile_size_byte               20480
// W_stride_nof_byte              1024
// W_stride_hw_byte               1024
// W_tile_nif_byte                1024
// k_tile_size_byte               0
// lambda_tile_size_byte          0
// k_size_byte                    0
// lambda_size_byte               0
// l1_x_offset                    0
// l1_y_offset                    1028
// l1_W_offset                    1112
// x_tile_size_nif_last           1024
// x_tile_size_nif_byte_last      1024
// x_tile_size_h_last             1
// x_tile_size_w_last             1
// W_tile_size_nof_last           20
// W_tile_size_nif_last           1024
// W_tile_size_nif_byte_last      1024
// y_tile_size_nof_last           20
// y_tile_size_h_last             1
// y_tile_size_w_last             1
// y_length_nof_byte_last         80

#include "dory.h"

void  layerGemm28_last(
  void *args
);
