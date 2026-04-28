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

#include "konark/konark_cluster_raw_addrmap.h"
#include "konark/hal_datamover.h"

#include "data.h"


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
        printf("[DM-ERR] Unknown TRANSPOSE_MODE=%d: Must be 1, 2, or 4\n", TRANSP_MODE);
        return -1;
      }
  }

  #if VERBOSE
   printf("[DM-INFO] cluster = %u, core = %u\n", pi_cluster_id(), pi_core_id());
  #endif

  int timeout = 5000000;
  int P = 4; // Patch size for unfold/fold

  printf("golden_in address: %p\n", (void *)golden_in);
  printf("golden_out address: %p\n", (void *)golden_out);
  printf("local_out address: %p\n", (void *)local_out);

  printf("First element of golden_out: 0x%02x @%p\n", golden_out[0], (void *)golden_out);

  printf("[DM-INFO] Test configuration: DATAMOVER_MODE=%d, TRANSP_MODE=%d, CIM_MODE=%d, SIZE_M=%u, SIZE_N=%u, SIZE_C=%u\n", DATAMOVER_MODE, TRANSP_MODE, CIM_MODE, SIZE_M, SIZE_N, SIZE_C);

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
            printf("[DM-INFO] Starting Datamover row-major->CIM-layout operation of %ux%u matrix (inner dimension = %u)\n", SIZE_M, SIZE_N, ROW_TILE_SIZE);
            datamover_status = datamover_cim_layout_blocking(golden_in, local_out, SIZE_M, SIZE_N, ROW_TILE_SIZE, timeout);
            break;
          case 1:
            printf("[DM-INFO] Starting Datamover CIM-layout->row-major operation of %ux%u matrix (outer dimension = %u)\n", SIZE_M, SIZE_N, ROW_TILE_SIZE);
            datamover_status = datamover_cim_layout_reverse_blocking(golden_in, local_out, SIZE_M, SIZE_N, ROW_TILE_SIZE, timeout);
            break;
          default:
            printf("[DM-ERR] Unknown CIM_MODE=%d: Must be 0 (row-major->CIM-layout) or 1 (CIM-layout->row-major)\n", CIM_MODE);
            return -1;
        }
        break;
      case 3:
        printf("[DM-INFO] Starting Datamover CIM LAYOUT + TRANSPOSE (%u elements) operation of %ux%u (CONVERTED DIMENSIONS) matrix (CIM inner dimension = %u)\n", transp_mode, SIZE_M, SIZE_N, ROW_TILE_SIZE);
        datamover_status = datamover_cim_layout_transpose_blocking(golden_in, local_out, SIZE_M, SIZE_N, ROW_TILE_SIZE, transp_mode, timeout);
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
