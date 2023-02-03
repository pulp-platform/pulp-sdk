/*
 * Copyright (C) 2020 ETH Zurich, University of Bologna and GreenWaves Technologies
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

/*
 * Authors:  Francesco Conti <fconti@iis.ee.ethz.ch>
 *           Gianna Paulin <pauling@iis.ee.ethz.ch>
 *           Renzo Andri <andrire@iis.ee.ethz.ch>
 * Main Test Program for the NEUREKA
 */

#include "pmsis.h"
#include "pmsis/cluster/dma/cl_dma.h"
#include <stdint.h>
#include <stdio.h>


#define NB_ITER 10

static int glob_errors;

#define WEIGHT_MEM_BASE 0x10400000
#define SRAM_OFFSET 0x00400000
#define MRAM_OFFSET 0x00000000

#define DEBUG_PRINTF 1
#define SIZE 0x100000

PI_L2 uint8_t data[SIZE];

void initialize_data(int size, int start){
  for(int i=0; i<size; i++)
    data[i] = ((i+start) & 0xFF);
}

int test_entry() {
  initialize_data(SIZE, 0);
  uint32_t* weight_start_ptr = WEIGHT_MEM_BASE+MRAM_OFFSET; 
  uint32_t* weight_end_ptr = WEIGHT_MEM_BASE+MRAM_OFFSET+SIZE-4; 

  for(int i=0; i<8; i++){
    weight_start_ptr = WEIGHT_MEM_BASE+MRAM_OFFSET+i*SIZE;
    weight_end_ptr = WEIGHT_MEM_BASE+MRAM_OFFSET+(i+1)*SIZE-4; 
    #ifdef DEBUG_PRINTF
      printf("***************Before copying****************************\n");;
      printf("data=%x at addr=%x\n", *weight_start_ptr, weight_start_ptr);
      printf("data=%x at addr=%x\n", *weight_end_ptr, weight_end_ptr);
      printf("Copying L2->WMEM weight starting at addr=%x, with size=%x\n", weight_start_ptr, SIZE);
    #endif
    memcpy(weight_start_ptr,(uint32_t*)data,sizeof(data));
    #ifdef DEBUG_PRINTF
      printf("***************After copying****************************\n");;
      printf("data=%x at addr=%x\n", *weight_start_ptr, weight_start_ptr);
      printf("data=%x at addr=%x\n", *weight_end_ptr, weight_end_ptr);
    #endif 
  }

  return 0;
}

void test_kickoff(void *arg) {
  int ret = test_entry();
  pmsis_exit(ret);
}

int main() {
  return pmsis_kickoff((void *)test_kickoff);
}
