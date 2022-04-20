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

#include "hal_neureka.h"
#include "inc/neureka_cfg.h"
#include "inc/neureka_infeat.h"
#include "inc/neureka_weights.h"
// #include "inc/neureka_weights_28.h"
#include "inc/neureka_scale.h"
#include "inc/neureka_scale_bias.h"
#include "inc/neureka_scale_shift.h"
#include "inc/neureka_streamin.h"
#include "inc/neureka_outfeat.h"

#define NB_ITER 10

static int glob_errors;

#define WEIGHT_MEM_BASE 0x10400000
#define SRAM_OFFSET 0x00400000
#define MRAM_OFFSET 0x00000000

int run_test() {

  uint8_t* x        = neureka_infeat;
  uint8_t* W        = neureka_weights;
  uint8_t* nq       = neureka_scale;
  uint8_t* nqs      = neureka_scale_shift;
  uint8_t* nqb      = neureka_scale_bias;
  uint8_t* golden_y = neureka_outfeat;
  uint8_t* actual_y = neureka_streamin;

  // enable clock
  NEUREKA_CG_ENABLE();

  // setup HCI
  NEUREKA_SETPRIORITY_NEUREKA();
  NEUREKA_RESET_MAXSTALL();
  NEUREKA_SET_MAXSTALL(8);

  // soft-clear NEUREKA
  NEUREKA_WRITE_CMD(NEUREKA_SOFT_CLEAR, NEUREKA_SOFT_CLEAR_ALL);
  for(volatile int kk=0; kk<10; kk++);

  // program NEUREKA
  NEUREKA_WRITE_REG(NEUREKA_REG_WEIGHTS_PTR,     W);
  printf("_______________________REG0________________________\n");
  NEUREKA_WRITE_REG(NEUREKA_REG_INFEAT_PTR,      x);
  printf("_______________________REG1________________________\n");
  NEUREKA_WRITE_REG(NEUREKA_REG_OUTFEAT_PTR,     actual_y);
  printf("_______________________REG2________________________\n");
  NEUREKA_WRITE_REG(NEUREKA_REG_SCALE_PTR,       nq);
  printf("_______________________REG3________________________\n");
  NEUREKA_WRITE_REG(NEUREKA_REG_SCALE_SHIFT_PTR, nqs);
  printf("_______________________REG4________________________\n");
  NEUREKA_WRITE_REG(NEUREKA_REG_SCALE_BIAS_PTR,  nqb);
  printf("_______________________REG5________________________\n");
  for(int i=6; i<24; i++) {
    NEUREKA_WRITE_REG(i*4, neureka_cfg[i]);
  }
  printf("_______________________REG DONE________________________\n");
  // configure & reset perf counters
  pi_perf_conf(1 << PI_PERF_CYCLES);
  pi_perf_reset();

  // fake register access for trace level -- only GVSOC!
  NEUREKA_WRITE_REG(NEUREKA_SPECIAL_TRACE_REG, NEUREKA_L3_ALL);

  // commit NEUREKA computation
  NEUREKA_WRITE_CMD(NEUREKA_COMMIT_AND_TRIGGER, NEUREKA_TRIGGER_CMD);

  // start perf counter
  pi_perf_start();

  // wait on barrier
  NEUREKA_BARRIER();

  // stop perf counter
  pi_perf_stop();

  // disable clock
  NEUREKA_CG_DISABLE();

  printf("%d cycles\n", pi_perf_read(PI_PERF_CYCLES));

  int errors = neureka_compare_int(actual_y, golden_y, STIM_Y_SIZE/4);
  return errors;
}

static struct pi_cluster_task task[1];
static struct pi_task events[1];

static void pe_entry(void *arg) {
  if(pi_core_id() == 0) {
    glob_errors = run_test();
  }
  pi_cl_team_barrier();
}

static void cluster_entry(void *arg) {
  pi_cl_team_fork(0, pe_entry, 0);
}

static int launch_cluster_task() {
  struct pi_device cluster_dev;
  struct pi_cluster_conf conf;
  struct pi_cluster_task task;

  pi_cluster_conf_init(&conf);
  conf.id = 0;
  glob_errors = 0;

  pi_open_from_conf(&cluster_dev, &conf);
  pi_cluster_open(&cluster_dev);

  pi_cluster_task(&task, cluster_entry, NULL);
  pi_cluster_send_task_to_cl(&cluster_dev, &task);
  pi_cluster_close(&cluster_dev);

  return glob_errors;
}

int test_entry() {
  printf("Starting test\n");
  uint8_t* W        = neureka_weights;
  uint32_t* weight_start_ptr = WEIGHT_MEM_BASE+MRAM_OFFSET; 
  printf("Weight start ptr=%x\n",weight_start_ptr);
  printf("NEUREKA WEIGHTS=%x\n", neureka_weights);
  // memcpy((uint32_t*)neureka_weights,(uint32_t*)neureka_weights,sizeof(neureka_weights)); 
  memcpy(weight_start_ptr,(uint32_t*)neureka_weights,sizeof(neureka_weights)); 
  printf("Memcpy id Done!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  int errors = 0;
  // int errors = launch_cluster_task();
  // if (errors)
  //   printf("Test failure\n");
  // else
  //   printf("Test success\n");
  return errors;
}

void test_kickoff(void *arg) {
  int ret = test_entry();
  pmsis_exit(ret);
}

int main() {
  return pmsis_kickoff((void *)test_kickoff);
}
