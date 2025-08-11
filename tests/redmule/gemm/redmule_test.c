#include "pmsis.h"
#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <bsp/bsp.h>
#include <stdlib.h>
#include <math.h>

#include "hal_redmule.h"
#include "inc/w_input.h"
#include "inc/x_input.h"
#include "inc/y_input.h"
#include "inc/z_output.h"
#include "inc/config.h"

#define NB_ITER 1

#define ERROR_THRES 0x05

#if SRC_FMT==FP16
    typedef uint16_t src_fmt_t;
#elif SRC_FMT==FP8
    typedef uint8_t src_fmt_t;
#elif SRC_FMT==FP32
    typedef uint32_t src_fmt_t;
#endif

PI_L1 __attribute__((aligned(32))) src_fmt_t w [N_SIZE * K_SIZE] = W;
PI_L1 __attribute__((aligned(32))) src_fmt_t x [M_SIZE * N_SIZE] = X;
PI_L1 __attribute__((aligned(32))) src_fmt_t y [M_SIZE * K_SIZE] = Y;
PI_L1 __attribute__((aligned(32))) src_fmt_t z [M_SIZE * K_SIZE];

src_fmt_t z_gold [M_SIZE * K_SIZE] = Z;

void check_errors() {
  for (int i = 0; i < M_SIZE; i++) {
    for (int j = 0; j < K_SIZE; j++) {
      if (abs(z_gold[i * K_SIZE + j] - z[i * K_SIZE + j]) >= ERROR_THRES) {
        printf("DATA MISMATCH: (%d,%d),\tdiff: 0x%x\texpected: 0x%x\twas: 0x%x\n", i, j, abs(z_gold[i * K_SIZE + j] - z[i * K_SIZE + j]), z_gold[i * K_SIZE + j], z[i * K_SIZE + j]);
      }
    }
  }
}

static int glob_errors;

int run_test() {
  redmule_cfg(M_SIZE, N_SIZE, K_SIZE, gemm_ops);
  redmule_x_add_set(x);
  redmule_w_add_set(w);
  redmule_z_add_set(z);
  redmule_y_add_set(y);

  pi_perf_conf(1 << PI_PERF_CYCLES);
  pi_perf_reset();

  pi_perf_start();
  hwpe_trigger_job();

  //while(redmule_get_status()) {
    eu_evt_maskWaitAndClr(1<<REDMULE_EVT0);
  //}

  pi_perf_stop();

  printf("%d cycles\n", pi_perf_read(PI_PERF_CYCLES));

  int predicted_cycles = ARRAY_WIDTH + ARRAY_WIDTH + \
                     ((PIPE_REGS + 1) * (N_SIZE + PIPE_REGS)) * \
                     (ceil(((float) M_SIZE / ARRAY_WIDTH)) * ceil(((float) K_SIZE / (ARRAY_HEIGHT * (PIPE_REGS + 1))))) + \
                     1 + ARRAY_HEIGHT * (PIPE_REGS + 1) + ARRAY_WIDTH;

  printf("Predicted cycles: %d\n", predicted_cycles); 

  printf("DIFF: %d\n", pi_perf_read(PI_PERF_CYCLES) - predicted_cycles);

  check_errors();

  return 0;
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
  int errors = launch_cluster_task();
  if (errors)
    printf("Test failure\n");
  else
    printf("Test success\n");
  return errors;
}

void test_kickoff(void *arg) {
  int ret = test_entry();
  pmsis_exit(ret);
}

int main() {
  return pmsis_kickoff((void *)test_kickoff);
}
