/*
 * layer_template_L3.c
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
#include "layerConvBNRelu2L3.h"
#include "pmsis.h"
#include "bsp/fs.h"
#include "bsp/fs/readfs.h"
#include "bsp/flash.h"
#include "bsp/ram.h"
#include "bsp/flash/hyperflash.h"
#include "bsp/ram/hyperram.h"

void __attribute__ ((noinline)) layerConvBNRelu2L3(void *args) 
{
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
  unsigned int outmult =(unsigned int)  real_arg[9];
  unsigned int mult1 = (unsigned int) real_arg[10];
  unsigned int mult2 = (unsigned int) real_arg[11];
  unsigned int out_shift = (unsigned int) real_arg[12];
  char* exec_weights,*transfer_weights;
  char* exec_input,*transfer_input;
  char* exec_output,*transfer_output;
  exec_input = l2_x;
  exec_weights = l2_W;
  exec_output = l2_y;
  // output L3 tiling. Parameters
  char* L2_output_1;
  char* L2_output_2;
  int output_t = 0;
  int output_e = 0;
  pi_cl_ram_req_t buff_req_y1;
  L2_output_1 = l2_y;
  L2_output_2 = l2_y + 65536;
  transfer_output = output_t ? L2_output_2 : L2_output_1;
  exec_output = output_e ? L2_output_2 : L2_output_1;
  // loop over output tiles
  for(int j=0; j<4; j++) 
  {
    int k = 0;
    // execution of L2-L1 layer. Either top, middle or bottom layer.
    pi_cl_team_barrier(0);
    if (j==0)
    {
      unsigned int args[13] = {l3_x,
          l3_y,
          l3_W,
          exec_input,
          l2_x_2,
          dory_get_tile_3d(exec_output, 0, 0, k, 16, 64, 64, 64, 64, 0, 0, 0, 0, 0, 0, 8),
          exec_weights,
          l1_buffer,
          hyperram,
          outmult,
          mult1,
          mult2,
          out_shift};
      layerConvBNRelu2(          args);
    }
    else if (j==(3))
    {
      unsigned int args[13] = {l3_x,
          l3_y,
          l3_W,
          dory_get_tile_3d(exec_input, j, 0, 0, 16, 64, 32, 64, 32, 0, 0,0, 0, 0, 0, 8),
          l2_x_2,
          dory_get_tile_3d(exec_output, 0, 0, k, 16, 64, 64, 64, 64, 0, 0, 0, 0, 0, 0, 8),
          exec_weights,
          l1_buffer,
          hyperram,
          outmult,
          mult1,
          mult2,
          out_shift};
      layerConvBNRelu2(          args);
    }
    else
    {
      unsigned int args[13] = {l3_x,
          l3_y,
          l3_W,
          dory_get_tile_3d(exec_input, j, 0, 0, 16, 64, 32, 64, 32, 0, 0,0, 0, 0, 0, 8),
          l2_x_2,
          dory_get_tile_3d(exec_output, 0, 0, k, 16, 64, 64, 64, 64, 0, 0, 0, 0, 0, 0, 8),
          exec_weights,
          l1_buffer,
          hyperram,
          outmult,
          mult1,
          mult2,
          out_shift};
      layerConvBNRelu2(args);   
      }    
      pi_cl_team_barrier(0);
    if(pi_core_id()==0) 
    {
      // waits for output transfer to be ended
      if (j > 0)
        pi_cl_ram_write_wait(&buff_req_y1);
      pi_cl_ram_write(hyperram, (l3_y + j*65536), transfer_output, 65536, &buff_req_y1);
    }
    // switching parameters
    output_e = !output_e;
    output_t = !output_t;
    exec_output = output_e ? L2_output_2 : L2_output_1;
    transfer_output = output_t ? L2_output_2 : L2_output_1;
  }
  // last wait
  if(pi_core_id()==0) 
  {
    pi_cl_ram_write_wait(&buff_req_y1);
  }
  pi_cl_team_barrier(0);
}
