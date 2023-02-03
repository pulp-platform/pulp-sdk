#include <stdio.h>
#include "pmsis.h"
#if defined(RTL_PLATFORM)
    #include "siracusa_padctrl.h"
#endif
#include "stimuli.h"
#include "stdint.h"

#define CORESTATUS 0x10000000

#define NUM_CORES 8
#define IMAGE_SIZE 9 //Image size: SIZExSIZE
#define PATCH_SIZE 3
#define PATCH_STRIDE 1
#define PADDING 0 // No Padding for now

#define NB_RUN 4 //What is NB_RUN?
#define NUM_PATCHES_PER_ROW ((IMAGE_SIZE-(PATCH_SIZE-PATCH_STRIDE))/PATCH_STRIDE)
#define NUM_PATCHES_PER_COLUMN ((IMAGE_SIZE-(PATCH_SIZE-PATCH_STRIDE))/PATCH_STRIDE)
#define NUM_PATCHES_PER_IMAGE NUM_PATCHES_PER_ROW*NUM_PATCHES_PER_COLUMN

#define NUM_ITERATIONS 10

PI_L1 uint16_t min_results_L1[NUM_PATCHES_PER_ROW * NUM_PATCHES_PER_COLUMN];
PI_L1 uint16_t max_results_L1[NUM_PATCHES_PER_ROW * NUM_PATCHES_PER_COLUMN];

PI_L2 uint16_t min_results_L2[NUM_PATCHES_PER_ROW * NUM_PATCHES_PER_COLUMN];
PI_L2 uint16_t max_results_L2[NUM_PATCHES_PER_ROW * NUM_PATCHES_PER_COLUMN];

static int nb_callback_exec=0;

#define __MIN(a, b) ((a <= b)? a : b)

void perf_print_all_fixed() {
  printf("Perf CYCLES: %d\n\r",      pi_perf_read(0)); // Number of cycles
  printf("Perf INSTR: %d\n\r",       pi_perf_read(1)); // Number of executed instructions (including compressed ones)
  printf("Perf CINSTR: %d\n\r",      pi_perf_read(10));// Number of compressed instructions
  printf("Perf LD_STALL: %d\n\r",    pi_perf_read(2)); // Number of load use data hazards. Not the same as bank conflict (see Contention metric)
  printf("Perf JR_STALL: %d\n\r",    pi_perf_read(3)); // Number of jump register hazards
  printf("Perf IMISS: %d\n\r",       pi_perf_read(4)); // Number of additional cycles waiting for instruction fetch (i.e. 2 could mean one i-miss where we had to wait for 2 cycles of 2 misses where we had to wait for 1 cycle each)
  printf("Perf LD: %d\n\r",          pi_perf_read(5)); // Number of load instructions (incl. L2 access)
  printf("Perf ST: %d\n\r",          pi_perf_read(6)); // Number of store instructions
  printf("Perf JUMP: %d\n\r",        pi_perf_read(7)); // Number of unconditional jumps
  printf("Perf BRANCH: %d\n\r",      pi_perf_read(8)); // Number of branche instructions
  printf("Perf BTAKEN: %d\n\r",      pi_perf_read(9)); // Number of branches taken
  if (!hal_is_fc()) {
    printf("Perf Cluster Sync. CYC: %d\n\r", pi_perf_read(11)); // Number cycles waiting on cluster sync primitive
    printf("Perf L2 LD EXT: %d\n\r",      pi_perf_read(12)); // Number of load instructions targeting L2 memory
    printf("Perf L2 ST EXT: %d\n\r",      pi_perf_read(13)); // Number of store instructions targeting L2 memory
    printf("Perf L2 LD EXT CYC: %d\n\r",  pi_perf_read(14)); // Number of cycles spent for loading from L2 memory
    printf("Perf L2 ST EXT CYC: %d\n\r",  pi_perf_read(15)); // Number of cycles spent for storing to L2 memory
    printf("Perf L1 CONT CYC: %d\n\r",  pi_perf_read(16)); // Number of cycles spent due L1 bank conflicts
    printf("Perf FPU Type conflict CYC: %d\n\r",  pi_perf_read(17)); // Number of type FPU conflict stalls (operations with smaller latency cannot be queued behind operations with larger latency).
    printf("Perf FPU CONT CYC : %d\n\r",  pi_perf_read(18)); // Number of cycles spent due to FPU contention
    printf("Perf FPU WB CONT CYC : %d\n\r",  pi_perf_read(18)); // Number of cycles spent due to FPU Write back contention
  }
}

#define PERF_START_ON_CLUSTER \
  if(pi_core_id() == 7) {     \
    pi_perf_conf(0x7FFFFFFF);\
    pi_perf_reset();\
    pi_perf_start();\
  }

#define PERF_STOP_ON_CLUSTER \
  if(pi_core_id() == 7) {            \
    pi_perf_stop(); \
  }

void maxpool_benchmark_vanilla() {
  for (int iter_idx = 0; iter_idx < NUM_ITERATIONS; iter_idx++) {
    for (int patch_idx = 0; patch_idx < NUM_PATCHES_PER_IMAGE; patch_idx++) {
      int patch_tl_corner_x = (patch_idx % NUM_PATCHES_PER_ROW) * PATCH_STRIDE;
      int patch_tl_corner_y = (patch_idx / NUM_PATCHES_PER_ROW) * PATCH_STRIDE;
      // Calculate the top left corner of the current patch (the patches are numbered row-wise with the upper-left corner being patch #0)
      uint16_t *current_pixel = Image_L2 + patch_tl_corner_y * IMAGE_SIZE + patch_tl_corner_x;
      uint16_t min = INT32_MAX;
      uint16_t max = INT32_MIN;
      for (int y_offset = 0; y_offset < PATCH_SIZE; y_offset++ ) {
        for (int x_offset = 0; x_offset < PATCH_SIZE; x_offset++) {
          min = (*current_pixel <= min)? *current_pixel : min;
          max = (*current_pixel >= max)? *current_pixel : max;
          current_pixel++;
        }
        current_pixel += IMAGE_SIZE - PATCH_SIZE;
      }
      //printf("Core ID: %d, patch_x: %d, patch_y: %d, min: %d, max: %d\n\r", core_id, patch_tl_corner_x, patch_tl_corner_y, min, max);
      min_results_L2[patch_idx] = min;
      max_results_L2[patch_idx] = max;
    }
  }
}

void maxpool_benchmark_no_div() {
  for (int iter_idx = 0; iter_idx < NUM_ITERATIONS; iter_idx++) {
    int patch_tl_corner_x = 0;
    int patch_tl_corner_y = 0;
    for (int patch_idx = 0; patch_idx < NUM_PATCHES_PER_IMAGE; patch_idx++) {
      // Calculate the top left corner of the current patch (the patches are numbered row-wise with the upper-left corner being patch #0)
      uint16_t *current_pixel = Image_L2 + patch_tl_corner_y * IMAGE_SIZE + patch_tl_corner_x;
      uint16_t min = INT32_MAX;
      uint16_t max = INT32_MIN;
      for (int y_offset = 0; y_offset < PATCH_SIZE; y_offset++ ) {
        for (int x_offset = 0; x_offset < PATCH_SIZE; x_offset++) {
          min = (*current_pixel <= min)? *current_pixel : min;
          max = (*current_pixel >= max)? *current_pixel : max;
          current_pixel++;
        }
        current_pixel += IMAGE_SIZE - PATCH_SIZE;
      }
      //printf("Core ID: %d, patch_x: %d, patch_y: %d, min: %d, max: %d\n\r", core_id, patch_tl_corner_x, patch_tl_corner_y, min, max);
      min_results_L2[patch_idx] = min;
      max_results_L2[patch_idx] = max;

      // Update patch corner position
      patch_tl_corner_x += PATCH_STRIDE;
      if (patch_tl_corner_x >= NUM_PATCHES_PER_ROW * PATCH_STRIDE) {
        patch_tl_corner_x -= NUM_PATCHES_PER_ROW * PATCH_STRIDE;
        patch_tl_corner_y += PATCH_STRIDE;
      }
    }
  }
}

void maxpool_benchmark_multicore() {
  int myid = pi_core_id();
  int patch_tl_corner_x_start = (myid % NUM_PATCHES_PER_ROW) * PATCH_STRIDE;
  int patch_tl_corner_y_start = (myid / NUM_PATCHES_PER_ROW) * PATCH_STRIDE;
  // Disable loop unrolling for the first 2 loops. Otherwise, we loose some
  // performance since the generated code is much larger and will cause more
  // instruction cache misses. This improves performance quite a bit although
  // the executed code is less efficient due to more branching overhead.
  #pragma GCC unroll 0
  for (int iter_idx = 0; iter_idx < NUM_ITERATIONS+1; iter_idx++) { // +1 for cache warm-up
    // Start performance counter only after first iteration (i-cache need to warm up)
    if (iter_idx == 1) {
      PERF_START_ON_CLUSTER
    }
    int patch_tl_corner_x = patch_tl_corner_x_start;
    int patch_tl_corner_y = patch_tl_corner_y_start;
    #pragma GCC unroll 0
    for (int patch_idx = myid; patch_idx < NUM_PATCHES_PER_IMAGE; patch_idx+= NUM_CORES) {
      // Calculate the top left corner of the current patch (the patches are numbered row-wise with the upper-left corner being patch #0)
      uint16_t *current_pixel = Image_L1 + patch_tl_corner_y * IMAGE_SIZE + patch_tl_corner_x;
      uint16_t min = INT32_MAX;
      uint16_t max = INT32_MIN;
      for (int y_offset = 0; y_offset < PATCH_SIZE; y_offset++ ) {
        for (int x_offset = 0; x_offset < PATCH_SIZE; x_offset++) {
          min = (*current_pixel <= min)? *current_pixel : min;
          max = (*current_pixel >= max)? *current_pixel : max;
          current_pixel++;
        }
        current_pixel += IMAGE_SIZE - PATCH_SIZE;
      }
      min_results_L1[patch_idx] = min;
      max_results_L1[patch_idx] = max;

      patch_tl_corner_x += NUM_CORES*PATCH_STRIDE;
      while (patch_tl_corner_x >= NUM_PATCHES_PER_ROW * PATCH_STRIDE) {
        patch_tl_corner_x -= NUM_PATCHES_PER_ROW * PATCH_STRIDE;
        patch_tl_corner_y += PATCH_STRIDE;
      }
    }
  }
  PERF_STOP_ON_CLUSTER
}


void maxpool_benchmark_simd_3x3() {
  int patch_tl_corner_x = 0;
  int patch_tl_corner_y = 0;

  for (int patch_idx = 0; patch_idx < NUM_PATCHES_PER_IMAGE; patch_idx++) {
    // Calculate the top left corner of the current patch (the patches are numbered row-wise with the upper-left corner being patch #0)
    uint16_t *current_pixel = Image_L2 + patch_tl_corner_y * IMAGE_SIZE + patch_tl_corner_x;
    uint16_t min = UINT16_MAX;
    uint16_t max = 0;

    // Use 16-bit unsigned int SIMD instructions We decompose the 9 pixel
    // min-max operation into a binary tree where each SIMD assembly instruction
    // can perform 2 comparisons simultaneously.
    v2u op_a, op_b;
    v2u min1, min2, min3;
    uint16_t min_final;
    v2u max1, max2, max3;
    uint16_t max_final;

    // Compute min/max of pixel 1,2 (op_a) and 3,4 (op_b)
    op_a = __PACKU2(current_pixel[0], current_pixel[1]);
    op_b = __PACKU2(current_pixel[2], current_pixel[IMAGE_SIZE-PATCH_SIZE]);
    max1 = __MAXU2(op_a, op_b);
    min1 = __MINU2(op_a, op_b);
    // Compute min/max of pixel 5,6 (op_a) and 7,8 (op_b)
    op_a = __PACKU2(current_pixel[IMAGE_SIZE-PATCH_SIZE+1], current_pixel[IMAGE_SIZE-PATCH_SIZE+2]);
    op_b = __PACKU2(current_pixel[2*IMAGE_SIZE-PATCH_SIZE], current_pixel[2*IMAGE_SIZE-PATCH_SIZE+1]);
    max2 = __MAXU2(op_a, op_b);
    min2 = __MINU2(op_a, op_b);
    // Now compute the min maximum of the next higher level in the binary tree (8 elements) with one additional SIMD instruction
    max3 = __MAXU2(max1, max2);
    min3 = __MINU2(min1, min2);
    // Get the min max of the entire tree
    max_final = __MAX(max3[0], max3[1]);
    min_final = __MIN(min3[0], min3[1]);
    // Now compare this value to the final (9th) pixel
    max = __MAX(max_final, current_pixel[2*IMAGE_SIZE-PATCH_SIZE+2]);
    min = __MIN(min_final, current_pixel[2*IMAGE_SIZE-PATCH_SIZE+2]);
    // Save the result
    min_results_L2[patch_idx] = min;
    max_results_L2[patch_idx] = max;

    //printf("Core ID: %d, patch_x: %d, patch_y: %d, min: %d, max: %d\n\r", core_id, patch_tl_corner_x, patch_tl_corner_y, min, max);

    // Update patch corner position
    patch_tl_corner_x += PATCH_STRIDE;
    if (patch_tl_corner_x >= NUM_PATCHES_PER_ROW * PATCH_STRIDE) {
      patch_tl_corner_x -= NUM_PATCHES_PER_ROW * PATCH_STRIDE;
      patch_tl_corner_y += PATCH_STRIDE;
    }
  }
}

void print_result_matrix(uint16_t *results) {
  for (int y = 0; y < NUM_PATCHES_PER_COLUMN; y++) {
    for (int x = 0; x < NUM_PATCHES_PER_ROW; x++) {
      printf("%d, ", results[y*NUM_PATCHES_PER_ROW + x]);
    }
    printf("\n\r");
  }
}
static void cluster_task_callback(void *arg)
{
  nb_callback_exec++;
}

void cluster_entry(void *arg)
{
    pi_cl_team_fork((NUM_CORES), maxpool_benchmark_multicore, 0);
}
// This is a helper function which offloads the provided function (benchmark_func) to the cluster.
// benchmark_func is a C function pointer to a function with zero arguments and an integer return value (0 == success, 0 != error).
void run_benchmark_on_cluster(char* name, void *benchmark_func) {
  printf("Benchmarking function %s\n\r", name);

  struct pi_device cluster_dev;
  struct pi_cluster_conf cl_conf;
  struct pi_cluster_task cl_task;
  struct pi_task task;

  pi_cluster_conf_init(&cl_conf);
  pi_open_from_conf(&cluster_dev, &cl_conf);
  if (pi_cluster_open(&cluster_dev))
  {
    return -1;
  }

  pi_cluster_task(&cl_task, cluster_entry, NULL);
  pi_task_callback(&task, cluster_task_callback, (void *)&task);
  pi_perf_conf(1<<0x7FFFFFFF);
  pi_perf_reset();
  pi_perf_start();
  pi_cluster_send_task_to_cl_async(&cluster_dev, &cl_task, &task);

  while (nb_callback_exec== 0)
  {
    // pi_yield_polling();
    pi_yield();
  }
  pi_perf_stop();
  pi_cluster_close(&cluster_dev);


  // Start benchmark_func on all 8 cluster cores simultaneously
  // cluster_wait keeps the FC waiting in a busy loop until cluster core 0
  // finished execution. The benchmark function should make sure that not only
  // core 0 finished but all 8 cores by using a synchronization barrier
  // (sync_barrier()).
  int execution_time = pi_perf_read(PI_PERF_CYCLES); // Time is measured in unit soc clock ticks (50
                               // MHz clock by default). The timer could also
                               // be configured to use the 32kHz ref clock or
                               // to enable a prescaler for the PLL clock but
                               // this by default we just count in actual soc
                               // clock ticks.
  printf("Overall execution time (from FC point of view) including UART printf: %d ns\n\r", execution_time*20); // one soc tick is 20ns
}

void run_benchmark_on_fc(char* name, void (*benchmark_func)()) {
  printf("Benchmarking function %s\n\r", name);
  // Start timer on fabric controller
  // pi_perf_conf(1<<PI_PERF_CYCLES);
  pi_perf_conf(0x7FFFFFFF);
  // Reset and start FC's performance counters
  pi_perf_reset();
  pi_perf_start();
  // Run the actual benchmark
  benchmark_func();
  // Stop timer and print wallclock time
  pi_perf_stop();
  perf_print_all_fixed();
  int execution_time = pi_perf_read(PI_PERF_CYCLES); // Time is measured in unit soc clock ticks (50
                               // MHz clock by default). The timer could also
                               // be configured to use the 32kHz ref clock or
                               // to enable a prescaler for the PLL clock but
                               // this by default we just count in actual soc
                               // clock ticks.
  printf("Overall execution time (from FC point of view) including UART printf: %d ns\n\r", execution_time*20); // one soc tick is 20ns
}

void print_minmax_results(int l1_version) {
  if (l1_version) {
    printf("Results, Min: \n\r");
    print_result_matrix(min_results_L1);
    printf("Results, Maxn: \n\r");
    print_result_matrix(max_results_L1);
  } else {
    printf("Results, Min: \n\r");
    print_result_matrix(min_results_L2);
    printf("Results, Maxn: \n\r");
    print_result_matrix(max_results_L2);
  }

}

int main() {
  // Check if we are the fabric controller (FC has cluster id != 0 which cluster
  // cores have cluster id 0) We have this check since we will fork main on all
  // cluster cores and don't need to rerun SoC initialization during parallel
  // execution.
  if (pi_cluster_id() != 0){
    // Initialize the SoC

    // Configure IO pads for UART output on FPGA (map UART TX and RX to pads 31 and 32)
    #if defined(RTL_PLATFORM)
      padctrl_mode_set(PAD_GPIO32, PAD_MODE_UART0_RX); 
      padctrl_mode_set(PAD_GPIO33, PAD_MODE_UART0_TX); 
    #endif

    // We need to add a newline character to force the minimal libc printf implementation to flush UART.
    printf("Starting Benchmarks...\n\r");

    // Fork main on all cluster cores
    // The return value is whatever cluster core 0 is returning
    // TODO remove after debugging
    /* maxpool_benchmark_vanilla(0); */
    /* print_minmax_results(); */

    // Benchmark single core variant
    run_benchmark_on_fc("vanilla", maxpool_benchmark_vanilla); 
    run_benchmark_on_fc("optimized_indexing", maxpool_benchmark_no_div);
    run_benchmark_on_fc("simd", maxpool_benchmark_simd_3x3); 
    run_benchmark_on_cluster("optimized_indexing_multicore", maxpool_benchmark_multicore);
    printf("Min results:\n\r");
    print_result_matrix(min_results_L1);
    printf("Max results:\n\r");
    print_result_matrix(max_results_L1);


    // Benchmark optimized single core variant

    // Benchmark naive multi core implementation

    return 0;
  }
}
