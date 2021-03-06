/*
 * mem_controller.c
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

#include "mem_controller.h"
//#define VERBOSE

/* allocation and de-allocation functions for manually manage L2 and L1 memory.
   The allocation in L2 is made in a bidirectional way inside an allocator.
   Based on begin_end parameter, the allocation/free happens in begin/end of the buffer.
*/

void dory_L2_alloc(unsigned int * L2_pointer_input_begin,
              unsigned int * L2_pointer_input_end,
              unsigned int * L2_pointer_output,
              int memory_to_allocate,
              int begin_end_n // begin is 1, end is 0
              )
{
  if (begin_end_n == 1)
  {
    *(L2_pointer_output) = *(L2_pointer_input_begin);
    *(L2_pointer_input_begin) = *(L2_pointer_input_begin) + memory_to_allocate;
  }
  else
  {

    *(L2_pointer_output) = *(L2_pointer_input_end) - memory_to_allocate;
    *(L2_pointer_input_end) = *(L2_pointer_input_end) - memory_to_allocate;    
  }
#ifdef VERBOSE
  printf("L2_pointer_input_begin %d, L2_pointer_input_end %d, L2_pointer_allocated %d with a memory of %d at the begin/end (1/0) %d\n", *L2_pointer_input_begin - 469810000, *L2_pointer_input_end - 469810000, *L2_pointer_output - 469810000, memory_to_allocate, begin_end_n);
  printf("End-in %d\n", *L2_pointer_input_end - *L2_pointer_input_begin);
#endif
}


void dory_L2_free(unsigned int * L2_pointer_input_begin,
            unsigned int * L2_pointer_input_end,
            int memory_to_free,
            int begin_end_n // begin is 1, end is 0
            )
{
  if (begin_end_n == 1)
  {
    *(L2_pointer_input_begin) = *(L2_pointer_input_begin) - memory_to_free;
  }
  else
  {
    *(L2_pointer_input_end) = *(L2_pointer_input_end) + memory_to_free;    
  }
#ifdef VERBOSE
  printf("L2_pointer_input_begin %d, L2_pointer_input_end %d, free a memory of %d at the begin/end (1/0) %d\n", *L2_pointer_input_begin - 469810000, *L2_pointer_input_end - 469810000, memory_to_free, begin_end_n);
  printf("End-in %d\n", *L2_pointer_input_end - *L2_pointer_input_begin);
#endif
}

void dory_L1_alloc(unsigned int * L2_pointer_input_begin,
              unsigned int * L2_pointer_output,
              int memory_to_allocate
              )
{
    *(L2_pointer_output) = *(L2_pointer_input_begin);
    *(L2_pointer_input_begin) = *(L2_pointer_input_begin) + memory_to_allocate;

}


void dory_L1_free(unsigned int * L2_pointer_input_begin,
            int memory_to_free
            )
{
    *(L2_pointer_input_begin) = *(L2_pointer_input_begin) - memory_to_free;

}