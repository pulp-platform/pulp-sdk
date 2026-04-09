// Copyright 2023 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Authors: Cyrill Durrer <cdurrer@iis.ee.ethz.ch>
//          Daniel Keller <dankeller@iis.ee.ethz.ch>
//          Sergio Mazzola <smazzola@iis.ee.ethz.ch>

#include "pmsis.h"

#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

// #include "snrt.h"
#include "konark/konark_cluster_raw_addrmap.h"

#include "konark/hal_datamover.h"
#include "konark/hal_konark.h"

#include "data.h"

// PASSING TESTS (13.03.2026)
// #include "data_5x5_copy.h"
// #include "data_8x8_copy_count.h"
// #include "data_8x11_copy.h"
// #include "data_64x64_copy.h"
// #include "data_65x65_copy.h"
// #include "data_221x199_copy.h"
// #include "data_256x256_copy.h"
// #include "data_128x128_cim64.h"
// #include "data_192x192_cim64.h"
// #include "data_3x3_transp1.h"
// #include "data_8x8_transp1.h"
// #include "data_9x8_transp1.h"
// #include "data_9x9_transp1.h"
// #include "data_60x16_transp1.h"   // MobileViT layer
// #include "data_64x64_transp1.h"
// #include "data_64x65_transp1.h"
// #include "data_65x64_transp1.h"
// #include "data_65x65_transp1.h"
// #include "data_66x66_transp1.h"
// #include "data_66x128_transp1.h"
// #include "data_160x200_transp1.h"
// #include "data_192x192_transp1_rand.h"
// #include "data_31x64_transp2.h"
// #include "data_32x64_transp2.h"
// #include "data_33x64_transp2.h"
// #include "data_64x64_transp2.h"
// #include "data_64x66_transp2.h"
// #include "data_64x130_transp2.h"
// #include "data_66x64_transp2.h"
// #include "data_66x66_transp2.h"
// #include "data_66x128_transp2.h"
// #include "data_92x128_transp2.h"
// #include "data_126x64_transp2.h"
// #include "data_126x126_transp2.h"
// #include "data_128x130_transp2.h"
// #include "data_130x128_transp2.h"
// #include "data_130x130_transp2.h"
// #include "data_160x64_transp2.h"
// #include "data_160x130_transp2.h"
// #include "data_160x160_transp2.h"
// #include "data_160x200_transp2.h"
// #include "data_192x128_transp2.h"
// #include "data_64x68_transp4.h"
// #include "data_66x64_transp4.h"
// #include "data_126x64_transp4.h"
// #include "data_160x200_transp4.h"
// #include "data_192x128_transp4.h"

// PASSING TESTS (08.04.2026)
// #include "data_64x2x64_unfold4_count.h"
// #include "data_64x4x64_unfold4_count.h"
// #include "data_64x8x64_unfold4_count.h"
// #include "data_64x16x64_unfold4.h"
// #include "data_4x2x64_unfold4.h"
// #include "data_4x2x32_unfold4.h"
// #include "data_64x2x32_unfold4.h"
// #include "data_32x32x32_unfold4.h"
// #include "data_64x16x16_unfold4.h"
// #include "data_63x2x64_unfold4.h"
// #include "data_128x2x64_unfold4.h"
// #include "data_144x16x16_unfold4.h"
// #include "data_65x16x16_unfold4.h"
// #include "data_65x2x64_unfold4.h"
// #include "data_72x2x64_unfold4.h"
// #include "data_2x2x128_unfold4.h"
// #include "data_2x128x2_unfold4.h"
// #include "data_65x66x8_unfold4.h"
// #include "data_240x8x8_unfold4.h"    // MobileViT block 3
// #include "data_128x128_cim64.h"

// PASSING TESTS (13.04.2026)
// #include "data_64x16x16_fold4.h"
// #include "data_65x16x16_fold4.h"
// #include "data_129x16x16_fold4.h"
// #include "data_128x16x16_fold4.h"
// #include "data_2x16x16_fold4.h"
// #include "data_2x18x18_fold4.h"
// #include "data_2x64x64_fold4.h"
// #include "data_2x66x126_fold4.h"
// #include "data_240x8x8_fold4.h"

// EXCLUDED FROM AUTOMATED TESTING
// FAILING TESTS
// ToDo: Implement partial tile support for CIM layout conversion
// #include "data_160x160_cim64.h"
// #include "data_4x65_cim64.h"
// #include "data_64x65_cim64.h"

// #include "data_64x32x64_unfold4.h"    // Too big for TCDM

// #include "data_2x120x110_unfold4.h"
// #include "data_2x128x32_unfold4.h"
// #include "data_2x128x64_unfold4.h"

// #include "data_64x4x64_fold4_count.h"


// Allocate output memory space in L1
PI_L1 __attribute__((aligned(32))) uint8_t local_out [SIZE_C * SIZE_M * SIZE_N];

static inline int datamover_compare_int (uint64_t *actual, uint64_t *golden, int total_elements) {
  int errors = 0;
  int len = (int)(total_elements / 8);  // Number of complete uint64_t words
  int remaining_elements = total_elements % 8;
  printf("[DM-INFO] Comparing %d 64b-words + %d 8b-elements:\n", len, remaining_elements);
  printf("[DM-INFO] actual address: %p, golden address: %p\n", (void *)actual, (void *)golden);
  for (int i=0; i<len; i++) {
    uint64_t actual_ = *(actual+i);
    uint64_t golden_ = *(golden+i);
    // printf("  0x%" PRIx64 " (golden) -- 0x%" PRIx64 " (actual) @ %p (%d)\n", golden_, actual_, (void *)(actual+i), i);
    if (actual_ != golden_) {
      printf("ERROR! 0x%" PRIx64 " (golden) @ %p != 0x%" PRIx64 " (actual) @ %p (%d)\n", golden_, (void *)(golden+i), actual_, (void *)(actual+i), i);
      errors ++;
    }
    // else {
    //   printf("OK:    0x%" PRIx64 " (golden) @ %p == 0x%" PRIx64 " (actual) @ %p (%d)\n", golden_, (void *)(golden+i), actual_, (void *)(actual+i), i);
    // }
    // else {
    //   printf("OK:    0x%" PRIx64 " (golden) == 0x%" PRIx64 " (actual) @ %p (%d)\n", golden_, actual_, (void *)(actual+i), i);
    // }
    // #ifdef VERBOSE
    // printf("  0x%" PRIx64 " -- 0x%" PRIx64 " @ %p (%d)\n", golden_, actual_, (void *)(actual+i), i);
    // #endif
  }
  if (remaining_elements > 0) {
    for (int i=0; i<remaining_elements; i++) {
      uint8_t actual_ = *((uint8_t*)actual + len * 8 + i);
      uint8_t golden_ = *((uint8_t*)golden + len * 8 + i);
      // printf("  0x%" PRIx8 " (golden) @ %p == 0x%" PRIx8 " (actual) @ %p (%d)\n", golden_, (void *)((uint8_t*)golden + len * 8 + i), actual_, (void *)((uint8_t*)actual + len * 8 + i), len * 8 + i);
      if (actual_ != golden_) {
        printf("ERROR! 0x%" PRIx8 " (golden) @ %p != 0x%" PRIx8 " (actual) @ %p (%d)\n", golden_, (void *)((uint8_t*)golden + len * 8 + i), actual_, (void *)((uint8_t*)actual + len * 8 + i), len * 8 + i);
        errors ++;
      }
      // else {
      //   printf("OK:    0x%" PRIx8 " (golden) == 0x%" PRIx8 " (actual) @ %p (%d)\n", golden_, actual_, (void *)((uint8_t*)actual + len * 8 + i), len * 8 + i);
      // }
      // #ifdef VERBOSE
      // printf("  0x%" PRIx8 " -- 0x%" PRIx8 " @ %p (%d)\n", golden_, actual_, (void *)((uint8_t*)actual + len * 8 + i), len * 8 + i);
      // #endif
    }
  }
  return errors;
}

static int glob_errors;

int run_test() {
  datamover_status_t datamover_status;
  int errors = 0;
  datamover_transp_mode_t transp_mode = DATAMOVER_TRANSP_NONE;
  uint32_t TOT_SIZE;
  TOT_SIZE = SIZE_C * SIZE_M * SIZE_N;

  printf("Starting Datamover Test...\n");

  switch (TRANSP_MODE) {
    case 1:
      transp_mode = DATAMOVER_TRANSP_1ELEM;
      break;
    case 2:
      transp_mode = DATAMOVER_TRANSP_2ELEM;
      break;
    case 4:
      transp_mode = DATAMOVER_TRANSP_4ELEM;
      break;
    default:
      if (DATAMOVER_MODE == 1 || DATAMOVER_MODE == 3) {
        printf("[DM-ERR] Unknown DATAMOVER_TRANSPOSE_MODE=%d: Must be 1, 2, or 4\n", TRANSP_MODE);
        return -1;
      }
  }

  #if VERBOSE
  printf("[DM-INFO] cluster = %u, core(cluster) = %u, core(global) = %u\n", snrt_cluster_idx(), snrt_cluster_core_idx(), snrt_global_core_idx());
  #endif

  int timeout = 5000000;
  int P = 4; // Patch size for unfold/fold

  printf("golden_in address: %p\n", (void *)golden_in);
  printf("golden_out address: %p\n", (void *)golden_out);
  printf("local_out address: %p\n", (void *)local_out);

  printf("First element of golden_out: 0x%02x @%p\n", golden_out[0], (void *)golden_out);

  printf("[DM-INFO] Test configuration: DATAMOVER_MODE=%d, TRANSP_MODE=%d, CIM_MODE=%d, SIZE_M=%u, SIZE_N=%u, SIZE_C=%u\n", DATAMOVER_MODE, TRANSP_MODE, CIM_MODE, SIZE_M, SIZE_N, SIZE_C);

  // konark_datamover_init();

  switch (DATAMOVER_MODE) {
      case 0:
        printf("[DM-INFO] Starting Datamover COPY operation of %ux%u = %u elements\n", SIZE_M, SIZE_N, TOT_SIZE);
        datamover_status = datamover_copy_blocking(golden_in, (uint8_t*)local_out, SIZE_M, SIZE_N, timeout);
        break;
      case 1:
        printf("[DM-INFO] Starting Datamover TRANSPOSE (%u elements) operation of %ux%u matrix\n", transp_mode, SIZE_M, SIZE_N);
        datamover_status = datamover_transpose_blocking(golden_in, local_out, SIZE_M, SIZE_N, transp_mode, timeout);
        break;
      case 2:
        switch (CIM_MODE) {
          case 0:
            printf("[DM-INFO] Starting Datamover CIM LAYOUT->row-major operation of %ux%u matrix (inner dimension = %u)\n", SIZE_M, SIZE_N, ROW_TILE_SIZE);
            datamover_status = datamover_cim_layout_blocking(golden_in, local_out, SIZE_M, SIZE_N, ROW_TILE_SIZE, timeout);
            break;
          case 1:
            printf("[DM-INFO] Starting Datamover row-major->CIM LAYOUT operation of %ux%u matrix (outer dimension = %u)\n", SIZE_M, SIZE_N, ROW_TILE_SIZE);
            datamover_status = datamover_cim_layout_reverse_blocking(golden_in, local_out, SIZE_M, SIZE_N, ROW_TILE_SIZE, timeout);
            break;
          default:
            printf("[DM-ERR] Unknown CIM_MODE=%d: Must be 0 (row-major->CIM-layout) or 1 (CIM-layout->row-major)\n", CIM_MODE);
            return -1;
        }
        break;
      case 3:
        printf("[DM-INFO] Starting Datamover CIM LAYOUT + TRANSPOSE (%u elements) operation of %ux%u (CONVERTED DIMENSIONS) matrix (CIM inner dimension = %u)\n", transp_mode, SIZE_M, SIZE_N, ROW_TILE_SIZE);
        printf("[DM-INFO] OPERATION 1: CIM-layout to row-major, %ux%u matrix\n", (SIZE_N/ROW_TILE_SIZE), (SIZE_M*ROW_TILE_SIZE));
        datamover_status = datamover_cim_layout_blocking(golden_in, local_out, (SIZE_N/ROW_TILE_SIZE), (SIZE_M*ROW_TILE_SIZE), ROW_TILE_SIZE, timeout);
        printf("[DM-INFO] OPERATION 2: Transpose of %ux%u matrix\n", SIZE_N, SIZE_M);
        datamover_status = datamover_transpose_blocking(local_out, golden_in, SIZE_M, SIZE_N, transp_mode, timeout);    // Use golden_in as temporary buffer
        printf("[DM-INFO] OPERATION 3: Row-major to CIM-layout, %ux%u matrix\n", SIZE_M, SIZE_N);
        datamover_status = datamover_cim_layout_blocking(golden_in, local_out, SIZE_N, SIZE_M, ROW_TILE_SIZE, timeout);
        break;
      case 4:
        printf("[DM-INFO] Starting Datamover UNFOLD operation of %ux%ux%u tensor (CHW) (unfolded dimension = %ux%ux%u)\n", SIZE_C, SIZE_M, SIZE_N, P, SIZE_M*SIZE_N / P, SIZE_C);
        datamover_status = datamover_unfold_blocking(golden_in, local_out, SIZE_C, SIZE_M, SIZE_N, timeout);
        break;
      case 5:
        printf("[DM-INFO] Starting Datamover FOLD operation of %ux%ux%u tensor (CHW) (folded dimension = %ux%ux%u)\n", SIZE_C, SIZE_M, SIZE_N, P, SIZE_M*SIZE_N / P, SIZE_C);
        datamover_status = datamover_fold_blocking(golden_in, local_out, SIZE_C, SIZE_M, SIZE_N, timeout);
        break;
      default:
        printf("[DM-ERR] Unknown DATAMOVER_MODE=%d: Must be 0 (COPY), 1 (TRANSPOSE), 2 (CIM LAYOUT), 3 (CIM LAYOUT TRANSPOSE), 4 (UNFOLD), or 5 (FOLD)\n", DATAMOVER_MODE);
        return -1;
    }

    if (datamover_status != DATAMOVER_OK) {
      printf("[DM-ERR] Datamover operation failed with status %d\n", datamover_status);
      return -1;
    }

    // Disable all HWPE clocks
    // konark_hwpe_disable_all_clk();

    // Verification
    printf("[DM-INFO] Verifying result...\n");
    errors += datamover_compare_int((uint64_t*)local_out, (uint64_t*)golden_out, TOT_SIZE);
    if (errors == 0) {
      printf("[DM-OK] ======= DATAMOVER TEST PASSED =======\n");
      printf("  ***    ***  \n");
      printf(" ** **  ** ** \n");
      printf("  ***    ***  \n");
      printf("      /_      \n");
      printf(" ,          , \n");
      printf("  '-......-'  \n\n");
    } else {
      printf("[DM-ERR] !!!!!!! DATAMOVER TEST FAILED !!!!!!!\n");
      printf("[DM-ERR] mismatches: %d / %u words\n", errors, TOT_SIZE / 8);
    }
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
