/*
 * network.c
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
#include "network.h"
#include "dory.h"
#include "layerConvDWBNRelu11.h"
#include "layerConvDWBNRelu9.h"
#include "layerConvBNRelu4.h"
#include "layerConvDWBNRelu3L3.h"
#include "layerConvBNRelu12.h"
#include "layerAveragePoolRelu27.h"
#include "layerConvBNRelu26L3.h"
#include "layerConvDWBNRelu25.h"
#include "layerConvBNRelu6.h"
#include "layerConvDWBNRelu23.h"
#include "layerConvBNRelu2L3.h"
#include "layerConvBNRelu10.h"
#include "layerConvBNRelu24L3.h"
#include "layerConvDWBNRelu7.h"
#include "layerGemm28_lastL3.h"
#include "layerConvBNRelu14L3.h"
#include "layerConvBNRelu8.h"
#include "layerConvDWBNRelu5.h"
#include "layerConvDWBNRelu1.h"
#include "layerConvBNRelu0.h"
#include "layerConvDWBNRelu13.h"
#include "pmsis.h"
#include "bsp/fs.h"
#include "bsp/fs/readfs.h"
#include "bsp/flash.h"
#include "bsp/ram.h"
#include "bsp/flash/hyperflash.h"
#include "bsp/ram/hyperram.h"

#define FLASH_BUFF_SIZE 128
#define VERBOSE 1

unsigned int PMU_set_voltage(unsigned int Voltage, unsigned int CheckFrequencies)
{
  return 0;
}

// allocation of buffers with parameters needed by the network execution
const char * L3_weights_files[] = {
  "ConvBNRelu0_weights.hex", "ConvDWBNRelu1_weights.hex", "ConvBNRelu2_weights.hex", "ConvDWBNRelu3_weights.hex", "ConvBNRelu4_weights.hex", "ConvDWBNRelu5_weights.hex", "ConvBNRelu6_weights.hex", "ConvDWBNRelu7_weights.hex", "ConvBNRelu8_weights.hex", "ConvDWBNRelu9_weights.hex", "ConvBNRelu10_weights.hex", "ConvDWBNRelu11_weights.hex", "ConvBNRelu12_weights.hex", "ConvDWBNRelu13_weights.hex", "ConvBNRelu14_weights.hex", "ConvDWBNRelu15_weights.hex", "ConvBNRelu16_weights.hex", "ConvDWBNRelu17_weights.hex", "ConvBNRelu18_weights.hex", "ConvDWBNRelu19_weights.hex", "ConvBNRelu20_weights.hex", "ConvDWBNRelu21_weights.hex", "ConvBNRelu22_weights.hex", "ConvDWBNRelu23_weights.hex", "ConvBNRelu24_weights.hex", "ConvDWBNRelu25_weights.hex", "ConvBNRelu26_weights.hex", "Gemm28_weights.hex"
};
int L3_weights_size[28];
static int L3_weights;
static int L3_input;
static int bypass_L3_input;
static int L3_output;
static int bypass_L3_output;
static int activations_input;
static int L3_layers[29] = {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
static int allocate_layer[29] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0};
static int branch_input[29] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int branch_output[29] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int branch_change[29] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int branch_last[29] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int check_weights[29] = {120996, 53504, 303730, 117611, 1103930, 234609, 2236755, 204746, 4266224, 524077, 8552435, 451595, 17038175, 1057921, 33824230, 986273, 34152523, 945904, 28531734, 981254, 27547438, 950986, 27160429, 899062, 70825648, 2086584, 98869725, 0, 130510192};
static int check_weights_dimension[29] = {1120, 544, 2560, 1088, 9216, 2176, 17408, 2176, 34816, 4352, 67584, 4352, 135168, 8704, 133120, 8704, 133120, 8704, 133120, 8704, 133120, 8704, 133120, 8704, 33280, 17408, 8256, 0, 40960};
static int cumulative_weights_dimension[29] = {0, 1120, 1664, 4224, 5312, 14528, 16704, 34112, 36288, 71104, 75456, 143040, 147392, 282560, 291264, 557504, 566208, 832448, 841152, 1107392, 1116096, 1382336, 1391040, 1657280, 1665984, 2198464, 2215872, 3272640, 3272640};
static int check_activations[29] = {4227015, 2593837, 4653816, 3633011, 1319138, 1663743, 2150924, 1086250, 593975, 658359, 660692, 381795, 186363, 281281, 258751, 166779, 255683, 152006, 241858, 141249, 251861, 125971, 262803, 115940, 79579, 28772, 109971, 224275, 13665};
static int check_activations_dimension[29] = {49152, 131072, 131072, 245280, 65536, 131072, 131072, 131072, 32768, 65536, 65536, 65536, 16384, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 8192, 16384, 16384, 16384, 1024};
static int out_mult_vector[29] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 256.0, 0};
static int out_shift_vector[29] = {19, 18, 21, 19, 21, 18, 21, 19, 21, 18, 22, 20, 22, 18, 22, 18, 22, 17, 22, 18, 22, 18, 21, 18, 21, 17, 19, 8, 0};
static int inmul1_vector[29] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int inmul2_vector[29] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int check_activations_out[29] = {2593837, 4653816, 3633011, 1319138, 1663743, 2150924, 1086250, 593975, 658359, 660692, 381795, 186363, 281281, 258751, 166779, 255683, 152006, 241858, 141249, 251861, 125971, 262803, 115940, 79579, 28772, 109971, 224275, 13665, -7285923};
static int check_activations_out_dimension[29] = {131072, 131072, 245280, 65536, 131072, 131072, 131072, 32768, 65536, 65536, 65536, 16384, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 8192, 16384, 16384, 16384, 1024, 4000};
static int layer_with_weights[29] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1};

static uint8_t flashBuffer[FLASH_BUFF_SIZE];

static struct pi_hyperflash_conf flash_conf;
static struct pi_hyper_conf ram_conf;
static struct pi_device ram;


#ifdef VERBOSE
// check for input/output acitvation checksum
static void check_layer(char *output, int check_sum_true, int dim) {
  int checksum = 0;
  char *ptr = (char *) output;
  for(int j=0; j<dim; j++) {
    checksum += ptr[j];
  }

  if(check_sum_true == checksum)
    printf("Checksum in/out Layer :\tOk\n");
  else 
    printf("Checksum in/out Layer :\tFailed [%u vs. %u]\n", checksum, check_sum_true);
}

static void check_layer_last(int *output, int check_sum_true, int dim) {
  int checksum = 0;
  int *ptr = (int *) output;
  for(int j=0; j<(int)(dim/4); j++) {
    checksum += ptr[j];
  }

  if(check_sum_true == checksum)
    printf("Checksum final :\tOk\n");
  else 
    printf("Checksum final :\tFailed [%d vs. %d]\n", checksum, check_sum_true);
}

// check for weight checksum
static void check_layer_weight(char *weight, int check_sum_true, int dim) {
  int checksum = 0;
  char *ptr = (char *) weight;
  for(int j=0; j<dim; j++) {
    checksum += ptr[j];
  }

  if(check_sum_true == checksum)
    printf("Checksum weight/bias Layer :\tOk\n");
  else 
    printf("Checksum weight/bias Layer :\tFailed [%u vs. %u]\n", checksum, check_sum_true);
}
#endif 


// filesystem management functions
void open_filesystem(struct pi_device *flash, struct pi_device *fs)
{
    struct pi_readfs_conf conf;
    struct pi_hyperflash_conf flash_conf;

    /* Init & open flash. */
    pi_hyperflash_conf_init(&flash_conf);
    pi_open_from_conf(flash, &flash_conf);
    if (pi_flash_open(flash))
    {
        printf("Error flash open !\n");
        pmsis_exit(-1);
    }

    /* Open filesystem on flash. */
    pi_readfs_conf_init(&conf);
    conf.fs.flash = flash;
    pi_open_from_conf(fs, &conf);
    if (pi_fs_mount(fs))
    {
        printf("Error FS mounting !\n");
        pmsis_exit(-2);
    }
}

/* Moves the weights and the biases from hyperflash to hyperram */
int network_setup()
{
  pi_task_t task = {0};
  pi_task_block(&task);
  struct pi_device fs;
  struct pi_device flash;
  pi_hyperram_conf_init(&ram_conf);
  open_filesystem(&flash, &fs);
  pi_open_from_conf(&ram, &ram_conf);
  pi_ram_open(&ram);
  pi_fs_file_t *file;
  pi_ram_alloc(&ram, &L3_weights, (uint32_t) 5000000);
  pi_ram_alloc(&ram, &L3_input, (uint32_t) 1000000);
  pi_ram_alloc(&ram, &L3_output, (uint32_t) 1000000);
#ifdef VERBOSE
    printf("\nL3 Buffer alloc initial\t@ %d:\t%s\n", (unsigned int)L3_weights, L3_weights?"Ok":"Failed");
    printf("\nL3 Buffer alloc initial\t@ %d:\t%s\n", (unsigned int)L3_input, L3_input?"Ok":"Failed");
    printf("\nL3 Buffer alloc initial\t@ %d:\t%s\n", (unsigned int)L3_output, L3_output?"Ok":"Failed");
#endif
  unsigned int rdDone = 0;
  int layer_number = 0;
  int sum_weights;
  for (int i=0;i<28;i++)
  {
    if (layer_with_weights[layer_number]==0)
      layer_number +=1;
    file = pi_fs_open(&fs, L3_weights_files[i], 0);
    if (file == NULL)
    {
      printf("file open failed\n");
      return -1;
    }
    L3_weights_size[i] = file->size + rdDone;
    int flashBuffSize = FLASH_BUFF_SIZE * sizeof(char);
    sum_weights = 0;
    while(rdDone < (L3_weights_size[i] / sizeof(char))) 
    { 
      int size = pi_fs_read(file, flashBuffer, flashBuffSize);
      for (int t = 0; t < size; t++)
        sum_weights+=flashBuffer[t];
      pi_ram_write(&ram, L3_weights+rdDone, flashBuffer,size);
      rdDone += size / sizeof(char);
    }
    if (check_weights[layer_number] == sum_weights)
      printf("Layer %-3d: Checksum = %-12d, FLASH %-12d, Check OK\n", layer_number, check_weights[layer_number], sum_weights);
    else
      printf("Layer %-3d: Checksum = %-12d, FLASH %-12d, Check FAILED\n", layer_number, check_weights[layer_number], sum_weights);
    layer_number +=1;
  }
  file = pi_fs_open(&fs, "inputs.hex", 0);
  if (file == NULL)
  {
    printf("file open failed\n");
    return -1;
  }
  activations_input = L3_weights+rdDone;
  rdDone = 0;
  int flashBuffSize = FLASH_BUFF_SIZE * sizeof(char);
  // loop on chunk in file
  while(rdDone < (49152 / sizeof(char))) 
  { 
    // read from HyperFlash
    int size = pi_fs_read(file, flashBuffer, flashBuffSize);
    // write to HyperRam
    pi_ram_write(&ram, activations_input+rdDone, flashBuffer, (uint32_t) size);
    rdDone += size / sizeof(char);
  }
  return 1;
}

// on cluster function execution
void cluster_main(void *arg) 
{
  int *real_arg = (int *) arg;
  network_run((unsigned int) real_arg[0]);
}

// parallelization of the function given the number of cores
void pulp_parallel(void *arg)
{
  pi_cl_team_fork(NUM_CORES, (void *)cluster_main, arg);
}

void network_run_FabricController()
{
  int arg[1];
  arg[0] = (unsigned int) L3_weights_size;
  PMU_set_voltage(1000, 0);
  pi_time_wait_us(10000);
  pi_freq_set(PI_FREQ_DOMAIN_FC, 100000000);
  pi_time_wait_us(10000);
  pi_freq_set(PI_FREQ_DOMAIN_CL, 100000000);
  pi_time_wait_us(10000);
  
  struct pi_device cluster_dev = {0};
  struct pi_cluster_conf conf;
  struct pi_cluster_task cluster_task = {0};
  // task parameters allocation
  pi_cluster_task(&cluster_task, pulp_parallel, arg);
  cluster_task.stack_size = 4096;
  cluster_task.slave_stack_size = 3072;
  // First open the cluster
  pi_cluster_conf_init(&conf);
  conf.id=0;
  pi_open_from_conf(&cluster_dev, &conf);
  if (pi_cluster_open(&cluster_dev))
    return -1;
  // Then offload an entry point, this will get executed on the cluster controller
  pi_cluster_send_task_to_cl(&cluster_dev, &cluster_task);
  // closing of the cluster
  pi_cluster_close(&cluster_dev);
}


int memId;
char* L2_output;
char* L2_input;
char* L2_weights_1;
char* L2_weights_2;
char* L2_buffer_allocation;
char* L2_buffer_tofree_copy;
int L2_buffer_allocation_end;
char *l1_buffer;
uint8_t * bypass_activations;
uint8_t * activation_to_keep;
char *exec_weights, *transfer_weights, *bypass_weights;
int L3_weights_internal;

void network_run(unsigned int L3_weights_size)
{   

/* 
  - initial buffer allocation L2 and L1
  - variable declaration
*/
/* ---------------------------------- */
/* -------- SECTION 0 BEGIN --------- */
/* ---------------------------------- */
  uint16_t out_mult = 0;
  uint16_t out_shift = 0;
  uint16_t inmul1 = 0;
  uint16_t inmul2 = 0;
  int branch_active = 0;
  int branch_keep_active = 0;
  int counter = 0;
  int counter_keep = 0;
  int valid = 0;
  static int keeping = 0;
  static int activation_to_keep_delloced = 0;
  int branch_output_index = 0;
  static int keep_index = 0;
  bypass_activations = 0;
  activation_to_keep = 0;
  int bypass_dimension = 0;
  int bypass_to_dealloc = 0;
  int activation_dimension = 0;
  int d_buffering_weights_t = 0;
  int error_presence = 0;
  int bypass_side = 0;
  int bypass_used_as_out = 0;
  int input_used_as_out = 0;
  int valid_keep = 0;
  int bypass_side_keep = 0;
  int d_buffering_weights_e = 0;
  int d_buffering_inputs = 0;
  int d_buffering_outputs = 0;
  int begin_end_n = 1;
  pi_cl_ram_req_t buff_req1;
  L3_weights_internal = L3_weights;
  transfer_weights = d_buffering_weights_t ? L2_weights_2 : L2_weights_1;
  exec_weights = d_buffering_weights_e ? L2_weights_2 : L2_weights_1;
  bypass_weights = d_buffering_weights_e ? L2_weights_2 : L2_weights_1;
  pi_cl_alloc_req_t alloc_req = {0};
  pi_cl_free_req_t free_req = {0};
  if (pi_core_id()==0)
  {
    pi_cl_l2_malloc((uint32_t) 380000, &alloc_req);
    L2_buffer_allocation = pi_cl_l2_malloc_wait(&alloc_req);
    L2_buffer_tofree_copy = L2_buffer_allocation;
    L2_buffer_allocation_end = L2_buffer_allocation + 380000;
    l1_buffer = pmsis_l1_malloc((uint32_t) 38000);
#ifdef VERBOSE
    printf("\nL2 Buffer alloc initial\t@ 0x%08x:\t%s\n", (unsigned int)L2_buffer_allocation, L2_buffer_allocation?"Ok":"Failed");
    printf("L1 Buffer alloc initial\t@ 0x%08x:\t%s\n\n", (unsigned int)l1_buffer, l1_buffer?"Ok":"Failed");
#endif
  }
/* ---------------------------------- */
/* --------- SECTION 0 END ---------- */ 
/* ---------------------------------- */ 

/* 
  - initial copies from L3 of input
  - copies of weights of first 2 layers
*/
/* ---------------------------------- */
/* -------- SECTION 1 BEGIN --------- */
/* ---------------------------------- */
  if(pi_core_id()==0)
  {
/* 
  - input allocation and copy
*/
    dory_L2_alloc(&L2_buffer_allocation,
      &L2_buffer_allocation_end,
      &L2_input,
      49152,
      begin_end_n // begin is 1, end is 0
      );
    pi_cl_ram_read(&ram, activations_input, L2_input, 49152, &buff_req1);
    pi_cl_ram_read_wait(&buff_req1);
/* 
  - first layer weights allocation and copy
*/
    dory_L2_alloc(&L2_buffer_allocation,
      &L2_buffer_allocation_end,
      &L2_weights_1,
      1120,
      begin_end_n // begin is 1, end is 0
      );
    begin_end_n = !begin_end_n;
    transfer_weights = L2_weights_1;
    exec_weights = L2_weights_1;  
    pi_cl_ram_read(&ram, L3_weights_internal, transfer_weights, 1120, &buff_req1);
    pi_cl_ram_read_wait(&buff_req1);
/* 
  - output of the first layer allocation
*/
    dory_L2_alloc(&L2_buffer_allocation,
      &L2_buffer_allocation_end,
      &L2_output,
      131072,
      begin_end_n // begin is 1, end is 0
      );
/* 
  - second layer weights allocation
*/
    d_buffering_weights_t = !d_buffering_weights_t;
    if(L2_output == NULL) return -1;
    dory_L2_alloc(&L2_buffer_allocation,
      &L2_buffer_allocation_end,
      &L2_weights_2,
      1664- 1120,
      begin_end_n // begin is 1, end is 0
      );
    transfer_weights = d_buffering_weights_t ? L2_weights_2 : L2_weights_1;
    begin_end_n = !begin_end_n;
  }
/* ---------------------------------- */
/* --------- SECTION 1 END ---------- */ 
/* ---------------------------------- */ 
  // perf measurement begin
  int cycle_network_execution = 0;
/* MAIN SECTION
  - for loop over all the layers of the network
  - double buffering using L3
  - check on layers to be executed from L3
  - residual check at the end of each layer
*/
/* ---------------------------------- */
/* -------- SECTION 2 BEGIN --------- */
/* ---------------------------------- */
  for(int i = 0; i < 29; i++)
  {
    if(pi_core_id()==0)
    {
      // copy of weights of next layers:
      // 1. copy only if we have to allocate the weights (hence not weights tiled from L3 and not pooling/add layer)
      // 2. waits before the read if we want to implement a double buffering, after if not. 
      // Waiting based on the fact if layer need or not transfers from L3 memory.
      if(i < 28)
      {
        if (allocate_layer[i+1] == 1)
        {
          if (L3_layers[i-1] == 0 && i > 0)
            pi_cl_ram_read_wait(&buff_req1);
          pi_cl_ram_read(&ram, L3_weights_internal + cumulative_weights_dimension[i+1], transfer_weights, check_weights_dimension[i+1], &buff_req1);
          if (L3_layers[i] == 1)
            pi_cl_ram_read_wait(&buff_req1);
        }
      }
    }
      
#ifdef VERBOSE
    if(pi_core_id()==0)
    {
      if(branch_change[i-1] == 1 && i > 0)
      {
        check_layer(bypass_activations,check_activations[branch_output_index+1],check_activations_dimension[branch_output_index+1]);
      }
      else
      {
        check_layer(L2_input, check_activations[i], check_activations_dimension[i]);
      }
      if(branch_input[i] == 1 && keeping == 1)
      {
        check_layer(activation_to_keep, check_activations[keep_index],check_activations_dimension[keep_index]);
      }
      else if (branch_input[i] == 1 && keeping == 0)
      {
        check_layer(bypass_activations,check_activations[branch_output_index+1],check_activations_dimension[branch_output_index+1]);
      }
    }
#endif  
    out_mult = out_mult_vector[i];
    out_shift = out_shift_vector[i];
    inmul1 = inmul1_vector[i];
    inmul2 = inmul2_vector[i];
    pi_cl_team_barrier(0);
    unsigned int args[13] = {L3_input,
      L3_output,
      L3_weights_internal + cumulative_weights_dimension[i],
      L2_input,
      bypass_activations,
      L2_output,
      exec_weights,
      l1_buffer,
      &ram,
      out_mult,
      inmul1,
      inmul2, 
      out_shift};
    if(branch_last[i] == 1)
    {
      args[0] = bypass_L3_input;
      args[1] = bypass_L3_output;
      args[3] = bypass_activations;
    }
    if(branch_input[i] == 1 && keeping == 1)
    {
      args[4] = activation_to_keep;
    }
    // perf measurement begin
    pi_perf_conf(1<<PI_PERF_CYCLES);          
    pi_perf_reset();                      
    pi_perf_stop();                       
    pi_perf_start();
    switch (i)
    {
      case 0:
        layerConvBNRelu0(args);
        break;
      case 1:
        layerConvDWBNRelu1(args);
        break;
      case 2:
        layerConvBNRelu2L3(args);
        break;
      case 3:
        layerConvDWBNRelu3L3(args);
        break;
      case 4:
        layerConvBNRelu4(args);
        break;
      case 5:
        layerConvDWBNRelu5(args);
        break;
      case 6:
        layerConvBNRelu6(args);
        break;
      case 7:
        layerConvDWBNRelu7(args);
        break;
      case 8:
        layerConvBNRelu8(args);
        break;
      case 9:
        layerConvDWBNRelu9(args);
        break;
      case 10:
        layerConvBNRelu10(args);
        break;
      case 11:
        layerConvDWBNRelu11(args);
        break;
      case 12:
        layerConvBNRelu12(args);
        break;
      case 13:
        layerConvDWBNRelu13(args);
        break;
      case 14:
        layerConvBNRelu14L3(args);
        break;
      case 15:
        layerConvDWBNRelu13(args);
        break;
      case 16:
        layerConvBNRelu14L3(args);
        break;
      case 17:
        layerConvDWBNRelu13(args);
        break;
      case 18:
        layerConvBNRelu14L3(args);
        break;
      case 19:
        layerConvDWBNRelu13(args);
        break;
      case 20:
        layerConvBNRelu14L3(args);
        break;
      case 21:
        layerConvDWBNRelu13(args);
        break;
      case 22:
        layerConvBNRelu14L3(args);
        break;
      case 23:
        layerConvDWBNRelu23(args);
        break;
      case 24:
        layerConvBNRelu24L3(args);
        break;
      case 25:
        layerConvDWBNRelu25(args);
        break;
      case 26:
        layerConvBNRelu26L3(args);
        break;
      case 27:
        layerAveragePoolRelu27(args);
        break;
      case 28:
        layerGemm28_lastL3(args);
        break;
    }
    pi_cl_team_barrier(0);
    // performance measurements: end
    pi_perf_stop();
    int perf_cyc =  pi_perf_read(PI_PERF_CYCLES); 
    cycle_network_execution += perf_cyc;

    // prevents error from compiler
    if (pi_core_id()==0)
    {
      asm volatile("": : :"memory");
      unsigned int temp = L3_input;
      L3_input = L3_output;
      asm volatile("": : :"memory");
      L3_output = temp;
      asm volatile("": : :"memory");
    }

#ifdef VERBOSE
    if(pi_core_id()==0)
    {
      printf("Layer %d ended \n", i);
      if (i < 28)
      {
        check_layer(L2_output, check_activations_out[i], check_activations_out_dimension[i]);
      }
      else
      {
        check_layer_last((int32_t *) L2_output, check_activations_out[i], check_activations_out_dimension[i]);
      }
      if (i==100)
      {    
        check_layer_plus(L2_output,check_activations_out_dimension[i]);
      }
    }    
#endif 
    if(branch_last[i] == 1)
    {
      keep_index = i;
    }
    if (i < 28)
    {
      if(pi_core_id()==0)
      {
        if (branch_input[i] == 1)
        {
          valid = 1;
          valid_keep = 1;
        }

        // deallocation of weights
        if (layer_with_weights[i] == 1)
          dory_L2_free(&L2_buffer_allocation,
            &L2_buffer_allocation_end,
            check_weights_dimension[i],
            begin_end_n // begin is 1, end is 0
            );
        if (layer_with_weights[i+1] == 1)
        {
          d_buffering_weights_e = !d_buffering_weights_e;
          exec_weights = d_buffering_weights_e ? L2_weights_2 : L2_weights_1;
        }
        // deallocation of input if not part of a residual connection
        //IT CAN NOT WORK FOR SOME CASES!!!
        if ((branch_output[i-1] !=1 && branch_change[i-1] != 1) && input_used_as_out!=1 || i==0)
        {
          dory_L2_free(&L2_buffer_allocation,
            &L2_buffer_allocation_end,
            check_activations_dimension[i],
            begin_end_n // begin is 1, end is 0
            );
   
        }
          
        // deallocation of a residual activation previously stored
        if(valid_keep == 1 && keeping == 1 && bypass_side_keep==begin_end_n && bypass_used_as_out!=1)
        {
          dory_L2_free(&L2_buffer_allocation,
            &L2_buffer_allocation_end,
            activation_dimension,
            begin_end_n // begin is 1, end is 0
            );
          counter_keep = 0;
          branch_keep_active = 0;
          keeping = 0;
          activation_to_keep_delloced = 0;
        }
        // MUST MAKE SURE THAT ACTIVATION_TO_KEEP IS NOT INFRONT OF BYPASS AND THAT IT IS
        // SAFE TO DEALLOC BYPASS ACTIVATION. IT'S MOST LIKELY ONLY DONE WHEN ON ADD LAYER
        if (branch_input[i]==1 && bypass_to_dealloc == 1)
        {
          dory_L2_free(&L2_buffer_allocation,
            &L2_buffer_allocation_end,
            bypass_dimension,
            begin_end_n // begin is 1, end is 0
            );
          counter = 0;
          branch_active = 0;
          bypass_to_dealloc = 0;
        }
        // Keep last layer of left side until add layer is encountered.
        if (branch_change[i] == 1)
        {
          activation_to_keep = L2_output;
          activation_dimension = check_activations_dimension[i];
          keeping = 1;
          branch_keep_active = 1;
          activation_to_keep_delloced = 1;
          bypass_side_keep = !begin_end_n; 
          valid_keep = 0;
        }
        if (branch_output[i] == 1)
        {
          bypass_L3_input = L3_input;
          bypass_L3_output = L3_output;
          branch_output_index = i;
          bypass_activations = L2_output;
          bypass_dimension = check_activations_out_dimension[i];
          branch_active = 1;
          bypass_to_dealloc = 1;    
          bypass_side = !begin_end_n;   
          valid = 0;
        }
        L2_input = L2_output;
        // allocation of output feature space
        if (branch_input[i+1]!=1 || (branch_input[i+1]==1 && bypass_side != begin_end_n && keeping == 0))
        {
          dory_L2_alloc(&L2_buffer_allocation,
            &L2_buffer_allocation_end,
            &L2_output,
            check_activations_out_dimension[i+1],
            begin_end_n // begin is 1, end is 0
            );
          input_used_as_out = 0; 
          bypass_used_as_out = 0; 
        }
        else if (keeping == 1) 
        {
          if (bypass_side_keep == begin_end_n)
          {
            L2_output = L2_input;
            input_used_as_out = 1;
          }
          else
          {
            L2_output = activation_to_keep;
            keeping = 0;
          }
        }
        else
        {
          L2_output = bypass_activations;
          bypass_used_as_out = 1;
          bypass_to_dealloc = 0;
        }
        if (i < 27)
        {
          if (branch_input[i+1]==1 && bypass_side_keep == begin_end_n && keeping==1)
            begin_end_n = !begin_end_n;
          // allocation of weights for next next layer, if necessary.
          if (layer_with_weights[i+2] == 1)
          {
            if (d_buffering_weights_e==1)
            {
              dory_L2_alloc(&L2_buffer_allocation,
                &L2_buffer_allocation_end,
                &L2_weights_1,
                check_weights_dimension[i+2],
                begin_end_n // begin is 1, end is 0
                );
            }
            else
            {
              dory_L2_alloc(&L2_buffer_allocation,
                &L2_buffer_allocation_end,
                &L2_weights_2,
                check_weights_dimension[i+2],
                begin_end_n // begin is 1, end is 0
                );
            }
            d_buffering_weights_t = !d_buffering_weights_t;
            transfer_weights = d_buffering_weights_t ? L2_weights_2 : L2_weights_1;
          }
        }
        //switching output and input in the buffer for allocation.
        begin_end_n = !begin_end_n;
      }

    }
  }
/* ---------------------------------- */
/* --------- SECTION 2 END ---------- */
/* ---------------------------------- */

/* ---------------------------------- */
/* -------- SECTION 3 BEGIN --------- */
/* ---------------------------------- */

  int cid = pi_core_id();    
  int MACs = 186400768;
  float perf_MAC =  (float)MACs/cycle_network_execution;
  if (cid == 0)
  {
    printf("\n[%d] : num_cycles: %d\n",cid,cycle_network_execution); 
    printf("[%d] : MACs: %d\n",cid,MACs ); 
    printf("[%d] : MAC/cycle: %f\n",cid,perf_MAC ); 
    printf("[%d] : n. of Cores: %d\n",cid,NUM_CORES); 
  }

  if (pi_core_id()==0)
  {
    pi_cl_l2_free(L2_buffer_tofree_copy, (uint32_t) 380000, &free_req);
    pi_cl_l2_free_wait(&free_req);
    pmsis_l1_malloc_free(l1_buffer, (uint32_t) 38000 );
  }
/* ---------------------------------- */
/* --------- SECTION 3 END ---------- */
/* ---------------------------------- */
}


