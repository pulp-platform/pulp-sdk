// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Authors: Sergio Mazzola <smazzola@iis.ee.ethz.ch>
//          Cyrill Durrer <cdurrer@iis.ee.ethz.ch>
//          Daniel Keller <dankeller@iis.ee.ethz.ch>
//          Francesco Conti <f.conti@unibo.it>

#include <stdint.h>
#include <stdio.h>

#include "konark/hal_datamover.h"
#include "konark/hal_hwpe.h"

/////////////
// Drivers //
/////////////

/* HWPE mandatory registers */

int datamover_acquire_task(void) {
  return hwpe_task_queue_acquire_task(DATAMOVER_BASE_ADDR);
}

void datamover_trigger_task(void) {
  hwpe_task_queue_release_and_run(DATAMOVER_BASE_ADDR);
}

uint32_t datamover_finished(void) {
  return hwpe_finished(DATAMOVER_BASE_ADDR);
}

uint32_t datamover_get_status(void) {
  return hwpe_task_queue_status(DATAMOVER_BASE_ADDR);
}

void datamover_soft_clear(void) {
  hwpe_soft_clear(DATAMOVER_BASE_ADDR);
}

/* Datamover-specific registers */
//TODO(smazzola): For now only usage of context 0 is supported

void datamover_in_set(uint32_t value) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_IN_PTR_OFFSET, value);
}

void datamover_out_set(uint32_t value) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_OUT_PTR_OFFSET, value);
}

void datamover_tot_len_set(uint32_t value) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_TOT_LEN_OFFSET, value);
}

void datamover_in_d0_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_IN_D0_OFFSET, (stride << 16) | (len & 0xFFFF));
}

void datamover_in_d1_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_IN_D1_OFFSET, (stride << 16) | (len & 0xFFFF));
}

void datamover_in_d2_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_IN_D2_OFFSET, (stride << 16) | (len & 0xFFFF));
}

void datamover_in_d3_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_IN_D3_OFFSET, (stride << 16) | (len & 0xFFFF));
}

void datamover_out_d0_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_OUT_D0_OFFSET, (stride << 16) | (len & 0xFFFF));
}

void datamover_out_d1_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_OUT_D1_OFFSET, (stride << 16) | (len & 0xFFFF));
}

void datamover_out_d2_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_OUT_D2_OFFSET, (stride << 16) | (len & 0xFFFF));
}

void datamover_out_d3_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_OUT_D3_OFFSET, (stride << 16) | (len & 0xFFFF));
}

void datamover_in_out_d4_stride_set(uint32_t out_stride, uint32_t in_stride) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_IN_OUT_D4_STRIDE_OFFSET, (out_stride << 16) | (in_stride & 0xFFFF));
}

void datamover_matrix_dim_set(uint32_t matrix_dim_n, uint32_t matrix_dim_m) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_MATRIX_DIM_OFFSET, ((matrix_dim_n & 0xFFFF) << 16) | (matrix_dim_m & 0xFFFF));
}

void datamover_ctrl_engine_set(datamover_mode_t datamover_mode, uint32_t write_dim_en, uint32_t read_dim_en, datamover_transp_mode_t transp_mode) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_CTRL_ENGINE_OFFSET, ((write_dim_en & 0xF) << 12) | ((read_dim_en & 0xF) << 8) | ((datamover_mode & 0x1F) << 3) | (transp_mode & 0x7));
}

/////////
// HAL //
/////////

datamover_status_t datamover_wait_done(uint64_t timeout) {
  int status = 0;
  int finished = 0;

  // Wait for all jobs or timeout expiration (NOTE: FINISHED register can behave unexpectedly)
  // do {
  //   finished = datamover_finished();
  //   // printf("[DM-HAL] datamover_wait_done: Finished=0x%08x, Timeout=%" PRIu64 "\n", finished, timeout);
  // } while (finished == 0 && --timeout);

  // Wait for all jobs or timeout expiration
  do {
    status = datamover_get_status();    // ToDo: Why STATUS register and not FINISHED register?
  } while (status != 0 && --timeout);

  if (timeout == 0) {
    // #if VERBOSE
    printf("[ERROR] datamover_wait_done(): Timeout expired, jobs are stuck.\n");
    // #endif
    return DATAMOVER_TO;
  }

  printf("[DM-HAL] datamover_wait_done: Job(s) finished successfully"); // (timeout=%" PRIu64 ").\n", timeout);
  return DATAMOVER_OK;
}

datamover_status_t datamover_copy(uint8_t *src, uint8_t *dst, uint32_t dim_m, uint32_t dim_n) {
  int acq_to = 1000000;
  int job_id = -1;

  int total_accesses = (dim_m * dim_n) / DATAMOVER_BANDWIDTH_ELEMS;  // floored division

  if ((dim_m * dim_n) % DATAMOVER_BANDWIDTH_ELEMS != 0) {
    total_accesses += 1;    // additional access for remaining elements
    printf("[DM-HAL] datamover_copy: Matrix size (%u) misaligned with respect to BANDWIDTH_ELEMS (%u).\n", dim_m * dim_n, DATAMOVER_BANDWIDTH_ELEMS);
  }


  printf("[DM-HAL] datamover_copy: dim_m=%u, dim_n=%u, total_elements=%u, BANDWIDTH_ELEMS=%u, total_accesses=%u\n", dim_m, dim_n, dim_m * dim_n, DATAMOVER_BANDWIDTH_ELEMS, total_accesses);

  while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
  if (acq_to == 0) {
    return DATAMOVER_TO;
  }

  datamover_in_set((uint32_t)src);
  datamover_out_set((uint32_t)dst);
  datamover_tot_len_set(total_accesses);
  datamover_in_d0_set(DATAMOVER_BANDWIDTH_ELEMS, total_accesses);
  datamover_in_d1_set(0, 0);
  datamover_in_d2_set(0, 0);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(DATAMOVER_BANDWIDTH_ELEMS, total_accesses);
  datamover_out_d1_set(0, 0);
  datamover_out_d2_set(0, 0);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, 0);
  datamover_matrix_dim_set(dim_n, dim_m);
  datamover_ctrl_engine_set(DATAMOVER_COPY, 0, 0, DATAMOVER_TRANSP_NONE);

  datamover_trigger_task();

  return DATAMOVER_OK;
}

datamover_status_t datamover_copy_blocking(uint8_t *src, uint8_t *dst, uint32_t dim_m, uint32_t dim_n, uint64_t timeout) {
  datamover_status_t status;

  status = datamover_copy(src, dst, dim_m, dim_n);
  if (status != DATAMOVER_OK) {
    return status;
  }

  status = datamover_wait_done(timeout);
  return status;
}

datamover_status_t datamover_transpose(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_m, uint32_t dim_n, datamover_transp_mode_t transp_mode) {
  int acq_to = 1000000;
  int job_id = -1;

  int m_tiles = dim_m % DATAMOVER_BANDWIDTH_ELEMS ? (dim_m / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (dim_m / DATAMOVER_BANDWIDTH_ELEMS);  // number of tiles in M dimension (rounded up)
  int n_tiles = dim_n % DATAMOVER_BANDWIDTH_ELEMS ? (dim_n / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (dim_n / DATAMOVER_BANDWIDTH_ELEMS);  // number of tiles in N dimension (rounded up)

  while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
  if (acq_to == 0) {
    return DATAMOVER_TO;
  }

  datamover_in_set((uint32_t)matrix_in);
  datamover_out_set((uint32_t)matrix_out);
  datamover_tot_len_set(m_tiles * n_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d0_set(dim_n, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d1_set(DATAMOVER_BANDWIDTH_ELEMS, n_tiles);
  datamover_in_d2_set(0, 0);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(dim_m * transp_mode, DATAMOVER_BANDWIDTH_ELEMS / transp_mode);
  datamover_out_d1_set(DATAMOVER_BANDWIDTH_ELEMS, m_tiles * transp_mode);
  datamover_out_d2_set(dim_m * DATAMOVER_BANDWIDTH_ELEMS, 0);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, 0);
  datamover_matrix_dim_set(dim_n, dim_m);
  datamover_ctrl_engine_set(DATAMOVER_TRANSP, 0x3, 0x1, transp_mode);

  datamover_trigger_task();

  return DATAMOVER_OK;
}

datamover_status_t datamover_transpose_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_m, uint32_t dim_n, datamover_transp_mode_t transp_mode, uint64_t timeout) {
  datamover_status_t status;

  status = datamover_transpose(matrix_in, matrix_out, dim_m, dim_n, transp_mode);
  if (status != DATAMOVER_OK) {
    return status;
  }

  status = datamover_wait_done(timeout);
  return status;
}

void datamover_cim_layout_config_complete_tiles(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_m, uint32_t dim_n, uint32_t row_tile_size) { // Configure datamover for complete tiles in N dimension
  uint32_t m_tiles = dim_m % DATAMOVER_BANDWIDTH_ELEMS ? (dim_m / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (dim_m / DATAMOVER_BANDWIDTH_ELEMS);  // number of tiles in M dimension (rounded up)
  uint32_t complete_n_tiles = dim_n / row_tile_size;  // number of complete tiles in N dimension

  datamover_in_set((uint32_t)matrix_in);
  datamover_out_set((uint32_t)matrix_out);
  datamover_tot_len_set(m_tiles * complete_n_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d0_set(DATAMOVER_BANDWIDTH_ELEMS, row_tile_size / DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d1_set(dim_n, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d2_set(row_tile_size, 0);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(DATAMOVER_BANDWIDTH_ELEMS, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_out_d1_set(row_tile_size * dim_m, complete_n_tiles);
  datamover_out_d2_set(0, 0);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, 0);
  datamover_matrix_dim_set(complete_n_tiles * DATAMOVER_BANDWIDTH_ELEMS, dim_m);
  datamover_ctrl_engine_set(DATAMOVER_CIM_LAYOUT, 0x1, 0x3, DATAMOVER_TRANSP_NONE);
}

void datamover_cim_layout_config_leftover_tiles(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_m, uint32_t dim_n, uint32_t row_tile_size) { // Configure datamover for leftover tiles in N dimension
  uint32_t m_tiles = dim_m % DATAMOVER_BANDWIDTH_ELEMS ? (dim_m / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (dim_m / DATAMOVER_BANDWIDTH_ELEMS);  // number of tiles in M dimension (rounded up)
  uint32_t complete_n_tiles = dim_n / row_tile_size;  // number of complete tiles in N dimension
  uint32_t leftover_columns = dim_n % DATAMOVER_BANDWIDTH_ELEMS;
  uint8_t *matrix_in_shifted = matrix_in + complete_n_tiles * DATAMOVER_BANDWIDTH_ELEMS;
  uint8_t *matrix_out_shifted = matrix_out + complete_n_tiles * dim_m * DATAMOVER_BANDWIDTH_ELEMS;

  datamover_in_set((uint32_t)matrix_in_shifted);
  datamover_out_set((uint32_t)matrix_out_shifted);
  datamover_tot_len_set(m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d0_set(DATAMOVER_BANDWIDTH_ELEMS, row_tile_size / DATAMOVER_BANDWIDTH_ELEMS);    // Unused if row_tile_size == BANDWIDTH_ELEMS
  datamover_in_d1_set(dim_n, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d2_set(0, 0);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(leftover_columns, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);  // different stride for leftover columns
  datamover_out_d1_set(0, 0);
  datamover_out_d2_set(0, 0);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, 0);
  datamover_matrix_dim_set(leftover_columns, dim_m);
  datamover_ctrl_engine_set(DATAMOVER_CIM_LAYOUT, 0x0, 0x1, DATAMOVER_TRANSP_NONE);
}

datamover_status_t datamover_cim_layout(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_m, uint32_t dim_n, uint32_t row_tile_size) { // ToDo(cdurrer): rewrite this to handle partial tiles (first execute complete tiles, than leftover columns)
  // For row-major to A-layout: row_tile_size = CIM inner dimension [elements] (64)
  // For row-major to B-layout: row_tile_size = CIM outer dimension [elements] (8x #CIM)
  int acq_to = 1000000;
  int job_id = -1;

  uint32_t complete_n_tiles = dim_n / row_tile_size;  // number of complete tiles in N dimension

  // Handle leftover columns (if any)
  uint32_t leftover_columns = dim_n % DATAMOVER_BANDWIDTH_ELEMS;
  if(leftover_columns != 0) {
    if(dim_n > DATAMOVER_BANDWIDTH_ELEMS) {   // Handle complete tiles
      while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
      if (acq_to == 0) {
        return DATAMOVER_TO;
      }
      datamover_cim_layout_config_complete_tiles(matrix_in, matrix_out, dim_m, dim_n, row_tile_size);
      datamover_trigger_task();
    }
    // Handle leftover columns
    printf("[DM-HAL] datamover_cim_layout: Handling leftover columns: %u\n", leftover_columns);
    while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
    if (acq_to == 0) {
      return DATAMOVER_TO;
    }
    datamover_wait_done(5000000);     // ToDo(cdurrer): use second context instead of waiting for completion and reconfiguring datamover for leftover columns

    datamover_cim_layout_config_leftover_tiles(matrix_in, matrix_out, dim_m, dim_n, row_tile_size);
    datamover_trigger_task();
  }
  else {                                      // Handle complete tiles
      while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
      if (acq_to == 0) {
        return DATAMOVER_TO;
      }
      datamover_cim_layout_config_complete_tiles(matrix_in, matrix_out, dim_m, dim_n, row_tile_size);
      datamover_trigger_task();
  }
  return DATAMOVER_OK;
}

datamover_status_t datamover_cim_layout_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_m, uint32_t dim_n, uint32_t row_tile_size, uint64_t timeout) {
  datamover_status_t status;

  status = datamover_cim_layout(matrix_in, matrix_out, dim_m, dim_n, row_tile_size);
  if (status != DATAMOVER_OK) {
    return status;
  }

  status = datamover_wait_done(timeout);
  return status;
}

void datamover_cim_layout_reverse_config_complete_tiles(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_m, uint32_t dim_n, uint32_t row_tile_size) { // Configure datamover for complete tiles in N dimension
  uint32_t complete_n_tiles = dim_n / row_tile_size;  // number of complete tiles in N dimension
  uint32_t cim_layout_m_tiles = (dim_m * complete_n_tiles) % DATAMOVER_BANDWIDTH_ELEMS ? ((dim_m * complete_n_tiles) / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (dim_m * complete_n_tiles) / DATAMOVER_BANDWIDTH_ELEMS;  // number of tiles in M dimension (rounded up)

  datamover_in_set((uint32_t)matrix_in);
  datamover_out_set((uint32_t)matrix_out);
  datamover_tot_len_set(cim_layout_m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d0_set(DATAMOVER_BANDWIDTH_ELEMS, dim_m * complete_n_tiles);
  datamover_in_d1_set(0, 0);
  datamover_in_d2_set(0, 0);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(dim_n, dim_m);
  datamover_out_d1_set(DATAMOVER_BANDWIDTH_ELEMS, complete_n_tiles);
  datamover_out_d2_set(0, 0);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, 0);
  datamover_matrix_dim_set(DATAMOVER_BANDWIDTH_ELEMS, dim_m * complete_n_tiles);    // Representing CIM layout instead of original dimensions
  datamover_ctrl_engine_set(DATAMOVER_CIM_LAYOUT, 0x1, 0x0, DATAMOVER_TRANSP_NONE);
}

void datamover_cim_layout_reverse_config_leftover_tiles(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_m, uint32_t dim_n, uint32_t row_tile_size) { // Configure datamover for leftover tiles in N dimension
  uint32_t m_tiles = dim_m % DATAMOVER_BANDWIDTH_ELEMS ? (dim_m / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (dim_m / DATAMOVER_BANDWIDTH_ELEMS);  // number of tiles in M dimension (rounded up)
  uint32_t complete_n_tiles = dim_n / row_tile_size;  // number of complete tiles in N dimension
  uint32_t leftover_columns = dim_n % DATAMOVER_BANDWIDTH_ELEMS;
  uint8_t *matrix_in_shifted = matrix_in + complete_n_tiles * dim_m * DATAMOVER_BANDWIDTH_ELEMS;
  uint8_t *matrix_out_shifted = matrix_out + complete_n_tiles * DATAMOVER_BANDWIDTH_ELEMS;

  datamover_in_set((uint32_t)matrix_in_shifted);
  datamover_out_set((uint32_t)matrix_out_shifted);
  datamover_tot_len_set(m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d0_set(leftover_columns, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);    // Unused if row_tile_size == BANDWIDTH_ELEMS
  datamover_in_d1_set(0, 0);
  datamover_in_d2_set(0, 0);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(dim_n, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);  // different stride for leftover columns
  datamover_out_d1_set(0, 0);
  datamover_out_d2_set(0, 0);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, 0);
  datamover_matrix_dim_set(leftover_columns, dim_m);
  datamover_ctrl_engine_set(DATAMOVER_CIM_LAYOUT, 0x0, 0x0, DATAMOVER_TRANSP_NONE);
}

datamover_status_t datamover_cim_layout_reverse(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_m, uint32_t dim_n, uint32_t row_tile_size) { // ToDo(cdurrer): rewrite this to handle partial tiles (first execute complete tiles, than leftover columns)
  // For row-major to A-layout: row_tile_size = CIM inner dimension [elements] (64)
  // For row-major to B-layout: row_tile_size = CIM outer dimension [elements] (8x #CIM)
  // NOTE: Only supports row_tile_size = BANDWIDTH_ELEMS for now
  int acq_to = 1000000;
  int job_id = -1;

  uint32_t complete_n_tiles = dim_n / row_tile_size;  // number of complete tiles in N dimension

  // Handle leftover columns (if any)
  uint32_t leftover_columns = dim_n % DATAMOVER_BANDWIDTH_ELEMS;
  if(leftover_columns != 0) {
    if(dim_n > DATAMOVER_BANDWIDTH_ELEMS) {   // Handle complete tiles
      while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
      if (acq_to == 0) {
        return DATAMOVER_TO;
      }
      datamover_cim_layout_reverse_config_complete_tiles(matrix_in, matrix_out, dim_m, dim_n, row_tile_size);
      datamover_trigger_task();
    }
    // Handle leftover columns
    while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
    if (acq_to == 0) {
      return DATAMOVER_TO;
    }
    datamover_wait_done(5000000);     // ToDo(cdurrer): use second context instead of waiting for completion and reconfiguring datamover for leftover columns

    datamover_cim_layout_reverse_config_leftover_tiles(matrix_in, matrix_out, dim_m, dim_n, row_tile_size);
    datamover_trigger_task();
  }
  else {                                      // Handle complete tiles
      while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
      if (acq_to == 0) {
        return DATAMOVER_TO;
      }
      datamover_cim_layout_reverse_config_complete_tiles(matrix_in, matrix_out, dim_m, dim_n, row_tile_size);
      datamover_trigger_task();
  }
  return DATAMOVER_OK;
}

datamover_status_t datamover_cim_layout_reverse_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_m, uint32_t dim_n, uint32_t row_tile_size, uint64_t timeout) {
  datamover_status_t status;
  status = datamover_cim_layout_reverse(matrix_in, matrix_out, dim_m, dim_n, row_tile_size);
  if (status != DATAMOVER_OK) {
    return status;
  }

  status = datamover_wait_done(timeout);
  return status;
}

datamover_status_t datamover_cim_layout_transpose_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_m, uint32_t dim_n, uint32_t row_tile_size, datamover_transp_mode_t transp_mode, uint64_t timeout) {
  datamover_status_t status;
  if(dim_n <= row_tile_size) {
    // If the matrix has only one tile in N dimension, no need to perform 3 separate operations, because CIM layout is the same as row-major layout
    printf("[DM-INFO] Single tile in N dimension, performing direct transpose from CIM A-layout to row-major and vice versa, %ux%u matrix\n", dim_n, dim_m);
    status = datamover_transpose_blocking(matrix_in, matrix_out, dim_m, dim_n, transp_mode, timeout);
  }
  else {
    // Transposition with input and output in CIM layout: 3-phase execution: 1) CIM A-layout to row-major, 2) Transpose in row-major, 3) Row-major to CIM A-layout
    printf("[DM-INFO] OPERATION 1: CIM A-layout to row-major, %ux%u matrix\n", (dim_n/row_tile_size), (dim_m*row_tile_size));
    status = datamover_cim_layout_reverse_blocking(matrix_in, matrix_out, dim_m, dim_n, row_tile_size, timeout);
    if (status != DATAMOVER_OK) return status;
    printf("[DM-INFO] OPERATION 2: Transpose of %ux%u matrix\n", dim_n, dim_m);
    status = datamover_transpose_blocking(matrix_out, matrix_in, dim_m, dim_n, transp_mode, timeout);
    if (status != DATAMOVER_OK) return status;
    printf("[DM-INFO] OPERATION 3: Row-major to CIM A-layout, %ux%u matrix\n", dim_m, dim_n);
    status = datamover_cim_layout_blocking(matrix_in, matrix_out, dim_m, dim_n, row_tile_size, timeout);
  }
  return status;
}

datamover_status_t datamover_unfold(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_c, uint32_t dim_h, uint32_t dim_w) {
  // ToDo(cdurrer): Implement this function
  // datamover_in_set((uint32_t)matrix_in_shifted);
  // datamover_out_set((uint32_t)matrix_out_shifted);
  // datamover_tot_len_set(m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  // datamover_in_d0_set(leftover_columns, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);    // Unused if row_tile_size == BANDWIDTH_ELEMS
  // datamover_in_d1_set(0, 0);
  // datamover_in_d2_set(0, 0);
  // datamover_in_d3_set(0, 0);
  // datamover_out_d0_set(dim_n, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);  // different stride for leftover columns
  // datamover_out_d1_set(0, 0);
  // datamover_out_d2_set(0, 0);
  // datamover_out_d3_set(0, 0);
  // datamover_in_out_d4_stride_set(0, 0);
  // datamover_matrix_dim_set(leftover_columns, dim_m);
  // datamover_ctrl_engine_set(DATAMOVER_CIM_LAYOUT, 0x0, 0x0, DATAMOVER_TRANSP_NONE);

  return DATAMOVER_ERR;
}

datamover_status_t datamover_unfold_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t dim_c, uint32_t dim_h, uint32_t dim_w, uint64_t timeout) {
  // ToDo(cdurrer): Implement this function
  return DATAMOVER_ERR;
}
