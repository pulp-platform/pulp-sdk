/*
 * pooling_layer_template.c
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


#include "layerAveragePoolRelu27.h"
#define VERBOSE_PRINT(...) printf(__VA_ARGS__)
void layerAveragePoolRelu27(
  void *args
) {
  unsigned int *real_arg = (unsigned int *) args;
  unsigned int l3_x =(unsigned int)  real_arg[0];
  unsigned int l3_y =(unsigned int)  real_arg[1];
  unsigned int l3_W =(unsigned int)  real_arg[2];
  unsigned int l2_x =(unsigned int)  real_arg[3];
  unsigned int l2_x_2 =(unsigned int)  real_arg[4];
  unsigned int l2_y =(unsigned int)  real_arg[5];
  unsigned int l2_W =(unsigned int)  real_arg[6];
  unsigned int l1_buffer =(unsigned int)  real_arg[7];
  unsigned int hyperram =(unsigned int)  real_arg[8];
  unsigned int out_mult_in =(unsigned int)  real_arg[9];
  unsigned int inmul1 = (unsigned int) real_arg[10];
  unsigned int inmul2 = (unsigned int) real_arg[11];
  unsigned int out_shift_in = (unsigned int) real_arg[12];
  unsigned int dma_evt;
  int p_r, p_l, p_t, p_b;
  int last_nof_exec;
  int last_nif_exec;
  int last_h_exec;
  int last_w_exec;
  char *x;
  char *y;
  int x_tile_size_nif_exec;
  int x_tile_size_h_exec;
  int x_tile_size_w_exec;
  int y_tile_size_nof;
  int y_tile_size_h;
  int y_tile_size_w;
  int y_tile_size_byte;
  int y_length_h_px;
  int y_length_nof_byte;
  int db_x;
  int db_y;
  int exec_db_x;
  int exec_db_W;
 char *im2col;
  im2col = l1_buffer + 17448;
  dma_evt = mchan_alloc();
  // copy first tiles
  //l2_x has input activations
  dory_dma_memcpy_3d_custom(
  l2_x, // ext
  (l1_buffer + 0) + 0, // loc
  16384, // size: dimension of the buffer
  4096, // stride_1: stride for the 3d copy: if we have to copy on n_features axis, this is the stride to change from first 2D space to the next ones.
  1024, // stride_0: stride to be passed to 2d_copy: the dimension w of the in image
  4,// length_2: how many 2_d copies we need -> the dimension of the tile in n_features direction
  1024, // length_0: legnth of the 1_d copy, the length of tile in w direction
  1, // dir
  &dma_evt // copy
  );
  // wait for x read
  mchan_barrier(dma_evt);
  // tile loop indeces
  int _i_nof_load=0, _i_nif_load=0, _i_h_load=0, _i_w_load=0;
  int _i_nof_exec=0, _i_nif_exec=0, _i_h_exec=0, _i_w_exec=0;

  // double buffering state
  int db_state_x=0;
  int db_state_y=1;
  int db_state_acc_out=1;
  int flag_first_ch_out;

  // last-tile flags
  int last_nof_load = (1 == 1) ? 1 : 0;
  int last_nif_load = (1 == 1) ? 1 : 0;
  int last_h_load = (1 == 1) ? 1 : 0;
  int last_w_load = (1 == 1) ? 1 : 0;
  int iter;
  uint16_t out_mult = out_mult_in;
  uint16_t out_shift = out_shift_in;
  // tile loop nest
  for(iter=0; iter<1*1*1; iter++) {
    // loop nest is nof,h,w,(nif=0)
    _i_w_load += 1;
    if(_i_w_load==1) 
    {
      _i_w_load = 0;
      _i_h_load += 1;
      if(_i_h_load==1) 
      {
        _i_h_load = 0;
        _i_nif_load += 1;
        _i_nof_load += 1;
      }
    }
    if (_i_nof_exec==0)
      flag_first_ch_out = 1;
    else
      flag_first_ch_out = 0;
    // wait for x,W read
    // check if last in any dimension
    last_nof_exec = last_nof_load;
    last_nif_exec = last_nif_load;
    last_h_exec = last_h_load;
    last_w_exec = last_w_load;
    last_nof_load = (_i_nof_load+1 == 1) ? 1 : 0;
    last_nif_load = (_i_nof_load+1 == 1) ? 1 : 0;
    last_h_load = (_i_h_load+1 == 1) ? 1 : 0;
    last_w_load = (_i_w_load+1 == 1) ? 1 : 0;

    // compute double buffering offsets and update db state
    db_x = !db_state_x ? 16384 : 0;
    db_y = !db_state_y ? 1024 : 0;
    exec_db_x = 0;
    db_state_x = ! db_state_x;

    //switch all double buffering offset and y only after that all n_input_features have been analyzed: we need to pass all n_in to produce a single filter_out
    db_state_y = ! db_state_y;
    if(iter<1*1*1-1) 
    {
      y_tile_size_h   = (last_h_load)   ? 1 : 1;
      y_tile_size_w   = (last_w_load)   ? 1 : 1;
    }
    x = (char *) (l1_buffer + 0 + exec_db_x);
    y = (char *) (l1_buffer + 16388 + db_y);
   
    x_tile_size_nif_exec = (last_nif_exec) ? 1024 : 1024;
    x_tile_size_h_exec   = (last_h_exec)   ? 4 : 4;
    x_tile_size_w_exec   = (last_w_exec)   ? 4 : 4;
  
    y_tile_size_nof = (last_nof_exec) ? 1024 : 1024;
    y_tile_size_h   = (last_h_exec)   ? 1 : 1;
    y_tile_size_w   = (last_w_exec)   ? 1 : 1;
    y_tile_size_byte = y_tile_size_nof*y_tile_size_h*y_tile_size_w*8/8;
    y_length_nof_byte = (last_nof_exec)   ? 1024 : 1024;
    p_r = 0;
    p_l = 0;
    p_t = 0;
    p_b = 0;
    if (_i_h_exec == 0)
      p_t = 0;
    if (_i_w_exec == 0)
      p_l = 0;
    if (_i_h_exec == 1-1)
      p_b = 0;
    if (_i_w_exec == 1-1)
      p_r = 0;
    pi_cl_team_barrier(0);
  
// aggiungere padding su tutti i lati, acc_out, and filter asymettric
    pulp_nn_avgpool(
    x,
    x_tile_size_w_exec,
    x_tile_size_h_exec,
    x_tile_size_nif_exec,
    4,
    4,
    p_t,
    4,
    y_tile_size_w,
    y_tile_size_h,
    im2col,
    y,
    0,
    0,
    flag_first_ch_out,
    1,
    out_shift,
    out_mult
    );
    pi_cl_team_barrier(0);
    mchan_barrier(dma_evt);
    // transfering of output to L2
    dory_dma_memcpy_3d_custom(
      dory_get_tile_3d(l2_y, _i_h_exec, _i_w_exec, _i_nof_exec, 1, 1, 1024, 1, 1024, 0, 0, 0, 0, 0, 0, 8), // ext
      (l1_buffer + 16388) + db_y, // loc
      y_tile_size_byte, // size
      1024, // stride_1
      1024, // stride_0
      y_tile_size_h, // length_2
      y_length_nof_byte, // length_0
      0, // dir
      &dma_evt // copy
    );
    // update prev iterators
    _i_nof_exec = _i_nof_load;
    _i_nif_exec = _i_nif_load;
    _i_h_exec = _i_h_load;
    _i_w_exec = _i_w_load;
  }
  // wait for final write
  mchan_barrier(dma_evt);
  mchan_free(dma_evt);

}
