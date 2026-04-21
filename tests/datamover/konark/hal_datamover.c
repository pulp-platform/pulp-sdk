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
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_IN_D0_OFFSET, ((stride & 0xFFFF) << 16) | (len & 0xFFFF));
}

void datamover_in_d1_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_IN_D1_OFFSET, ((stride & 0xFFFF) << 16) | (len & 0xFFFF));
}

void datamover_in_d2_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_IN_D2_OFFSET, ((stride & 0xFFFF) << 16) | (len & 0xFFFF));
}

void datamover_in_d3_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_IN_D3_OFFSET, ((stride & 0xFFFF) << 16) | (len & 0xFFFF));
}

void datamover_out_d0_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_OUT_D0_OFFSET, ((stride & 0xFFFF) << 16) | (len & 0xFFFF));
}

void datamover_out_d1_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_OUT_D1_OFFSET, ((stride & 0xFFFF) << 16) | (len & 0xFFFF));
}

void datamover_out_d2_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_OUT_D2_OFFSET, ((stride & 0xFFFF) << 16) | (len & 0xFFFF));
}

void datamover_out_d3_set(uint32_t stride, uint32_t len) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_OUT_D3_OFFSET, ((stride & 0xFFFF) << 16) | (len & 0xFFFF));
}

void datamover_in_out_d4_stride_set(uint32_t out_stride, uint32_t in_stride) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_IN_OUT_D4_STRIDE_OFFSET, ((out_stride & 0xFFFF) << 16) | (in_stride & 0xFFFF));
}

void datamover_matrix_dim_set(uint32_t tensor_size_n, uint32_t tensor_size_m) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_MATRIX_DIM_OFFSET, ((tensor_size_n & 0xFFFF) << 16) | (tensor_size_m & 0xFFFF));
}

void datamover_channels_set(uint32_t total_elements, uint32_t num_channels) {
  DATAMOVER_HWPE_REG_WRITE(DATAMOVER_REGISTER_OFFSET + DATAMOVER_REG_CHANNELS_OFFSET, ((total_elements & 0x1FFFFF) << 11) | (num_channels & 0x7FF));
}

void datamover_ctrl_engine_set(datamover_mode_t datamover_mode, uint32_t write_dim_en, uint32_t read_dim_en, datamover_transp_mode_t transp_mode) { // dim_en: bit-mask (bit0=d1, bit1=d2, bit2=d3, bit3=d4)
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
  // } while (finished == 0 && --timeout);

  // Wait for all jobs or timeout expiration
  do {
    status = datamover_get_status();    // ToDo: Why STATUS register and not FINISHED register?
  } while (status != 0 && --timeout);

  if (timeout == 0) {
    #if VERBOSE
    printf("[ERROR] datamover_wait_done(): Timeout expired, jobs are stuck.\n");
    #endif
    return DATAMOVER_TO;
  }
  #if VERBOSE
  printf("[DM-HAL] datamover_wait_done: Job(s) finished successfully.\n");
  #endif
  return DATAMOVER_OK;
}

datamover_status_t datamover_copy(uint8_t *src, uint8_t *dst, uint32_t size_m, uint32_t size_n) {
  int acq_to = 1000000;
  int job_id = -1;
  uint32_t total_accesses = (size_m * size_n) / DATAMOVER_BANDWIDTH_ELEMS;  // floored division

  if ((size_m * size_n) % DATAMOVER_BANDWIDTH_ELEMS != 0) {
    total_accesses += 1;    // additional access for remaining elements
  }
  #if VERBOSE
  printf("[DM-HAL] datamover_copy: size_m=%u, size_n=%u, total_elements=%u, BANDWIDTH_ELEMS=%u, total_accesses=%u\n", size_m, size_n, size_m * size_n, DATAMOVER_BANDWIDTH_ELEMS, total_accesses);
  #endif

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
  datamover_matrix_dim_set(size_n, size_m);
  datamover_channels_set(size_m * size_n, 1);
  datamover_ctrl_engine_set(DATAMOVER_COPY, 0, 0, DATAMOVER_TRANSP_NONE);

  datamover_trigger_task();

  return DATAMOVER_OK;
}

datamover_status_t datamover_copy_blocking(uint8_t *src, uint8_t *dst, uint32_t size_m, uint32_t size_n, uint64_t timeout) {
  datamover_status_t status;
  status = datamover_copy(src, dst, size_m, size_n);
  if (status != DATAMOVER_OK) {
    return status;
  }
  status = datamover_wait_done(timeout);
  return status;
}

datamover_status_t datamover_transpose(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, datamover_transp_mode_t transp_mode) {
  int acq_to = 1000000;
  int job_id = -1;

  int m_tiles = size_m % DATAMOVER_BANDWIDTH_ELEMS ? (size_m / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (size_m / DATAMOVER_BANDWIDTH_ELEMS);  // number of tiles in M dimension (rounded up)
  int n_tiles = size_n % DATAMOVER_BANDWIDTH_ELEMS ? (size_n / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (size_n / DATAMOVER_BANDWIDTH_ELEMS);  // number of tiles in N dimension (rounded up)

  while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
  if (acq_to == 0) {
    return DATAMOVER_TO;
  }

  datamover_in_set((uint32_t)matrix_in);
  datamover_out_set((uint32_t)matrix_out);
  datamover_tot_len_set(m_tiles * n_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d0_set(size_n, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d1_set(DATAMOVER_BANDWIDTH_ELEMS, n_tiles);
  datamover_in_d2_set(0, 0);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(size_m * transp_mode, DATAMOVER_BANDWIDTH_ELEMS / transp_mode);
  datamover_out_d1_set(DATAMOVER_BANDWIDTH_ELEMS, m_tiles * transp_mode);
  datamover_out_d2_set(size_m * DATAMOVER_BANDWIDTH_ELEMS, 0);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, 0);
  datamover_matrix_dim_set(size_n, size_m);
  datamover_channels_set(size_m * size_n, 1);
  datamover_ctrl_engine_set(DATAMOVER_TRANSP, 0x3, 0x1, transp_mode);

  datamover_trigger_task();

  return DATAMOVER_OK;
}

datamover_status_t datamover_transpose_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, datamover_transp_mode_t transp_mode, uint64_t timeout) {
  datamover_status_t status;

  status = datamover_transpose(matrix_in, matrix_out, size_m, size_n, transp_mode);
  if (status != DATAMOVER_OK) {
    return status;
  }

  status = datamover_wait_done(timeout);
  return status;
}

void datamover_cim_layout_config_complete_tiles(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size) { // Configure datamover for complete tiles in N dimension
  uint32_t m_tiles = size_m % DATAMOVER_BANDWIDTH_ELEMS ? (size_m / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (size_m / DATAMOVER_BANDWIDTH_ELEMS);  // number of tiles in M dimension (rounded up)
  uint32_t complete_n_tiles = size_n / row_tile_size;  // number of complete tiles in N dimension

  datamover_in_set((uint32_t)matrix_in);
  datamover_out_set((uint32_t)matrix_out);
  datamover_tot_len_set(m_tiles * complete_n_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d0_set(DATAMOVER_BANDWIDTH_ELEMS, row_tile_size / DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d1_set(size_n, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d2_set(row_tile_size, 0);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(DATAMOVER_BANDWIDTH_ELEMS, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_out_d1_set(row_tile_size * size_m, complete_n_tiles);
  datamover_out_d2_set(0, 0);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, 0);
  datamover_matrix_dim_set(complete_n_tiles * DATAMOVER_BANDWIDTH_ELEMS, size_m);
  datamover_channels_set(complete_n_tiles * DATAMOVER_BANDWIDTH_ELEMS * size_m, 1);
  datamover_ctrl_engine_set(DATAMOVER_CIM_LAYOUT, 0x1, 0x3, DATAMOVER_TRANSP_NONE);
}

void datamover_cim_layout_config_leftover_tiles(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size) { // Configure datamover for leftover tiles in N dimension
  uint32_t m_tiles = size_m % DATAMOVER_BANDWIDTH_ELEMS ? (size_m / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (size_m / DATAMOVER_BANDWIDTH_ELEMS);  // number of tiles in M dimension (rounded up)
  uint32_t complete_n_tiles = size_n / row_tile_size;  // number of complete tiles in N dimension
  uint32_t leftover_columns = size_n % DATAMOVER_BANDWIDTH_ELEMS;
  uint8_t *matrix_in_shifted = matrix_in + complete_n_tiles * DATAMOVER_BANDWIDTH_ELEMS;
  uint8_t *matrix_out_shifted = matrix_out + complete_n_tiles * size_m * DATAMOVER_BANDWIDTH_ELEMS;

  datamover_in_set((uint32_t)matrix_in_shifted);
  datamover_out_set((uint32_t)matrix_out_shifted);
  datamover_tot_len_set(m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d0_set(DATAMOVER_BANDWIDTH_ELEMS, row_tile_size / DATAMOVER_BANDWIDTH_ELEMS);    // Unused if row_tile_size == BANDWIDTH_ELEMS
  datamover_in_d1_set(size_n, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d2_set(0, 0);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(leftover_columns, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);  // different stride for leftover columns
  datamover_out_d1_set(0, 0);
  datamover_out_d2_set(0, 0);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, 0);
  datamover_matrix_dim_set(leftover_columns, size_m);
  datamover_channels_set(leftover_columns * size_m, 1);
  datamover_ctrl_engine_set(DATAMOVER_CIM_LAYOUT, 0x0, 0x1, DATAMOVER_TRANSP_NONE);
}

datamover_status_t datamover_cim_layout(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size) {
  // For row-major to A-layout: row_tile_size = CIM inner dimension [elements] (64)
  // For row-major to B-layout: row_tile_size = CIM outer dimension [elements] (8x #CIM)
  // NOTE: Only supports row_tile_size = BANDWIDTH_ELEMS for now
  int acq_to = 1000000;
  int job_id = -1;

  uint32_t complete_n_tiles = size_n / row_tile_size;  // number of complete tiles in N dimension

  // Handle leftover columns (if any)
  uint32_t leftover_columns = size_n % DATAMOVER_BANDWIDTH_ELEMS;
  if(leftover_columns != 0) {
    if(size_n > DATAMOVER_BANDWIDTH_ELEMS) {   // Handle complete tiles
      while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
      if (acq_to == 0) {
        return DATAMOVER_TO;
      }
      datamover_cim_layout_config_complete_tiles(matrix_in, matrix_out, size_m, size_n, row_tile_size);
      datamover_trigger_task();
    }
    // Handle leftover columns
    while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
    if (acq_to == 0) {
      return DATAMOVER_TO;
    }
    datamover_status_t wait_status = datamover_wait_done(5000000);     // ToDo: use second context instead of waiting for completion and reconfiguring datamover for leftover columns
    if (wait_status != DATAMOVER_OK) {
      return wait_status;
    }

    datamover_cim_layout_config_leftover_tiles(matrix_in, matrix_out, size_m, size_n, row_tile_size);
    datamover_trigger_task();
  }
  else {                                      // Aligned matrix: Handle complete tiles only
      while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
      if (acq_to == 0) {
        return DATAMOVER_TO;
      }
      datamover_cim_layout_config_complete_tiles(matrix_in, matrix_out, size_m, size_n, row_tile_size);
      datamover_trigger_task();
  }
  return DATAMOVER_OK;
}

datamover_status_t datamover_cim_layout_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size, uint64_t timeout) {
  datamover_status_t status;

  status = datamover_cim_layout(matrix_in, matrix_out, size_m, size_n, row_tile_size);
  if (status != DATAMOVER_OK) {
    return status;
  }

  status = datamover_wait_done(timeout);
  return status;
}

void datamover_cim_layout_reverse_config_complete_tiles(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size) { // Configure datamover for complete tiles in N dimension
  uint32_t complete_n_tiles = size_n / row_tile_size;  // number of complete tiles in N dimension
  uint32_t cim_layout_m_tiles = (size_m * complete_n_tiles) % DATAMOVER_BANDWIDTH_ELEMS ? ((size_m * complete_n_tiles) / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (size_m * complete_n_tiles) / DATAMOVER_BANDWIDTH_ELEMS;  // number of tiles in M dimension (rounded up)

  datamover_in_set((uint32_t)matrix_in);
  datamover_out_set((uint32_t)matrix_out);
  datamover_tot_len_set(cim_layout_m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d0_set(DATAMOVER_BANDWIDTH_ELEMS, size_m * complete_n_tiles);
  datamover_in_d1_set(0, 0);
  datamover_in_d2_set(0, 0);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(size_n, size_m);
  datamover_out_d1_set(DATAMOVER_BANDWIDTH_ELEMS, complete_n_tiles);
  datamover_out_d2_set(0, 0);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, 0);
  datamover_matrix_dim_set(DATAMOVER_BANDWIDTH_ELEMS, size_m * complete_n_tiles);    // Representing CIM layout instead of original dimensions
  datamover_channels_set(DATAMOVER_BANDWIDTH_ELEMS * size_m * complete_n_tiles, 1);
  datamover_ctrl_engine_set(DATAMOVER_CIM_LAYOUT, 0x1, 0x0, DATAMOVER_TRANSP_NONE);
}

void datamover_cim_layout_reverse_config_leftover_tiles(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size) { // Configure datamover for leftover tiles in N dimension
  uint32_t m_tiles = size_m % DATAMOVER_BANDWIDTH_ELEMS ? (size_m / DATAMOVER_BANDWIDTH_ELEMS) + 1 : (size_m / DATAMOVER_BANDWIDTH_ELEMS);  // number of tiles in M dimension (rounded up)
  uint32_t complete_n_tiles = size_n / row_tile_size;  // number of complete tiles in N dimension
  uint32_t leftover_columns = size_n % DATAMOVER_BANDWIDTH_ELEMS;
  uint8_t *matrix_in_shifted = matrix_in + complete_n_tiles * size_m * DATAMOVER_BANDWIDTH_ELEMS;
  uint8_t *matrix_out_shifted = matrix_out + complete_n_tiles * DATAMOVER_BANDWIDTH_ELEMS;

  datamover_in_set((uint32_t)matrix_in_shifted);
  datamover_out_set((uint32_t)matrix_out_shifted);
  datamover_tot_len_set(m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d0_set(leftover_columns, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d1_set(0, 0);
  datamover_in_d2_set(0, 0);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(size_n, m_tiles * DATAMOVER_BANDWIDTH_ELEMS);  // different stride for leftover columns
  datamover_out_d1_set(0, 0);
  datamover_out_d2_set(0, 0);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, 0);
  datamover_matrix_dim_set(leftover_columns, size_m);
  datamover_channels_set(leftover_columns * size_m, 1);
  datamover_ctrl_engine_set(DATAMOVER_CIM_LAYOUT, 0x0, 0x0, DATAMOVER_TRANSP_NONE);
}

datamover_status_t datamover_cim_layout_reverse(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size) {
  // For A-layout to row-major: row_tile_size = CIM inner dimension [elements] (64)
  // For B-layout to row-major: row_tile_size = CIM outer dimension [elements] (8x #CIM)
  // NOTE: Only supports row_tile_size = BANDWIDTH_ELEMS for now
  int acq_to = 1000000;
  int job_id = -1;

  uint32_t complete_n_tiles = size_n / row_tile_size;  // number of complete tiles in N dimension

  // Handle leftover columns (if any)
  uint32_t leftover_columns = size_n % DATAMOVER_BANDWIDTH_ELEMS;
  if(leftover_columns != 0) {
    if(size_n > DATAMOVER_BANDWIDTH_ELEMS) {   // Handle complete tiles
      while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
      if (acq_to == 0) {
        return DATAMOVER_TO;
      }
      datamover_cim_layout_reverse_config_complete_tiles(matrix_in, matrix_out, size_m, size_n, row_tile_size);
      datamover_trigger_task();
    }
    // Handle leftover columns
    while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
    if (acq_to == 0) {
      return DATAMOVER_TO;
    }
    datamover_status_t wait_status = datamover_wait_done(5000000);     // ToDo: use second context instead of waiting for completion and reconfiguring datamover for leftover columns
    if (wait_status != DATAMOVER_OK) {
      return wait_status;
    }

    datamover_cim_layout_reverse_config_leftover_tiles(matrix_in, matrix_out, size_m, size_n, row_tile_size);
    datamover_trigger_task();
  }
  else {                                      // Aligned matrix: Handle complete tiles only
      while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
      if (acq_to == 0) {
        return DATAMOVER_TO;
      }
      datamover_cim_layout_reverse_config_complete_tiles(matrix_in, matrix_out, size_m, size_n, row_tile_size);
      datamover_trigger_task();
  }
  return DATAMOVER_OK;
}

datamover_status_t datamover_cim_layout_reverse_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size, uint64_t timeout) {
  datamover_status_t status;
  status = datamover_cim_layout_reverse(matrix_in, matrix_out, size_m, size_n, row_tile_size);
  if (status != DATAMOVER_OK) {
    return status;
  }

  status = datamover_wait_done(timeout);
  return status;
}

datamover_status_t datamover_cim_layout_transpose_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size, datamover_transp_mode_t transp_mode, uint64_t timeout) {
  // Performs transposition of a matrix in CIM layout, with input and output in CIM layout, by internally performing the necessary layout conversions and transposition in row-major layout.
  // IMPORTANT: This function uses the input buffer as temporary storage for the transposed matrix in row-major layout. THE ORIGINAL CONTENT OF THE INPUT BUFFER WILL BE OVERWRITTEN!
  datamover_status_t status;
  if (size_n <= row_tile_size && size_m <= row_tile_size) {
    // If the matrix has only one tile in N and M dimensions, no need to perform 3 separate operations, because CIM layout is the same as row-major layout
    #if VERBOSE
    printf("[DM-INFO] Single tile in N and M dimensions, performing direct transpose from CIM-layout to row-major and vice versa, %ux%u matrix\n", size_n, size_m);
    #endif
    status = datamover_transpose_blocking(matrix_in, matrix_out, size_m, size_n, transp_mode, timeout);
  } // ToDo(optional): Add special handling for matrices with only one tile in N dimension (size_n <= row_tile_size) but multiple tiles in M dimension, and the opposite case, omitting the unnecessary layout conversion. Requires either an additional memory space or a copy operation.
  else {
    // Transposition with input and output in CIM layout: 3-phase execution: 1) CIM-layout to row-major, 2) Transpose in row-major, 3) Row-major to CIM-layout
    #if VERBOSE
    printf("[DM-INFO] OPERATION 1: CIM-layout to row-major, %ux%u matrix\n", (size_n/row_tile_size), (size_m*row_tile_size));
    #endif
    status = datamover_cim_layout_reverse_blocking(matrix_in, matrix_out, size_m, size_n, row_tile_size, timeout);
    if (status != DATAMOVER_OK) return status;
    #if VERBOSE
    printf("[DM-INFO] OPERATION 2: Transpose of %ux%u matrix\n", size_m, size_n);
    #endif
    status = datamover_transpose_blocking(matrix_out, matrix_in, size_m, size_n, transp_mode, timeout);
    if (status != DATAMOVER_OK) return status;
    #if VERBOSE
    printf("[DM-INFO] OPERATION 3: Row-major to CIM-layout, %ux%u matrix\n", size_n, size_m);
    #endif
    status = datamover_cim_layout_blocking(matrix_in, matrix_out, size_n, size_m, row_tile_size, timeout);
  }
  return status;
}

datamover_status_t datamover_unfold(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_c, uint32_t size_h, uint32_t size_w) {
  // Converts tensor (C,H,W) to unfolded tensor (P,N,C), patch size P = 2x2 = 4 (other patch dimensions not supported yet)
  // matrix_in: input tensor in (C,H,W) layout
  // matrix_out: output tensor in unfolded (P,N=(H*W)/P,C) layout
  // size_c, size_h, size_w: dimensions of the input tensor
  const int P = 4;      // Patch size (number of elements in a patch), only tested for P=4 (2x2)
  const int side_P = 2; // Patch sidelength
  int acq_to = 1000000;
  int job_id = -1;
  uint32_t c_tiles = (size_c + DATAMOVER_BANDWIDTH_ELEMS - 1) / DATAMOVER_BANDWIDTH_ELEMS;  // number of tiles in C dimension (rounded up)
  uint32_t w_tiles = (size_w + DATAMOVER_BANDWIDTH_ELEMS - 1) / DATAMOVER_BANDWIDTH_ELEMS;  // number of tiles in W dimension (rounded up)

  while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
  if (acq_to == 0) {
    return DATAMOVER_TO;
  }
  datamover_in_set((uint32_t)matrix_in);
  datamover_out_set((uint32_t)matrix_out);
  datamover_tot_len_set(c_tiles * w_tiles * DATAMOVER_BANDWIDTH_ELEMS * size_h);
  datamover_in_d0_set(size_h * size_w, c_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_in_d1_set(DATAMOVER_BANDWIDTH_ELEMS, w_tiles);
  datamover_in_d2_set(size_w, size_h);
  datamover_in_d3_set(0, 0);
  datamover_out_d0_set(size_c * size_h * size_w / P, side_P);
  datamover_out_d1_set(size_c, (w_tiles * DATAMOVER_BANDWIDTH_ELEMS) / side_P);
  datamover_out_d2_set(DATAMOVER_BANDWIDTH_ELEMS, c_tiles);
  datamover_out_d3_set(size_c * size_h * size_w / side_P, side_P);
  datamover_in_out_d4_stride_set(size_c * size_w / side_P, 0);
  datamover_matrix_dim_set(size_w, size_h);
  datamover_channels_set(size_c * size_h * size_w, size_c);
  datamover_ctrl_engine_set(DATAMOVER_UNFOLD, 0xF, 0x3, DATAMOVER_TRANSP_1ELEM);

  datamover_trigger_task();
  return DATAMOVER_OK;
}

datamover_status_t datamover_unfold_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_c, uint32_t size_h, uint32_t size_w, uint64_t timeout) {
  datamover_status_t status;
  status = datamover_unfold(matrix_in, matrix_out, size_c, size_h, size_w);
  if (status != DATAMOVER_OK) {
    return status;
  }
  status = datamover_wait_done(timeout);
  return status;
}

datamover_status_t datamover_fold(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_c, uint32_t size_h, uint32_t size_w) {
  // Converts unfolded tensor (P,N,C) to folded ("normal") tensor (C,H,W), patch size P = 2x2 = 4
  // matrix_in: input tensor in unfolded (P,N=(H*W)/P,C) layout
  // matrix_out: output tensor in folded (C,H,W) layout
  // size_c, size_h, size_w: dimensions of the folded (OUTPUT) tensor
  const int P = 4;      // Patch size (number of elements in a patch), only tested for P=4 (2x2)
  const int side_P = 2; // Patch sidelength
  int acq_to = 1000000;
  int job_id = -1;
  uint32_t c_tiles = (size_c + DATAMOVER_BANDWIDTH_ELEMS - 1) / DATAMOVER_BANDWIDTH_ELEMS;  // number of tiles in C dimension (rounded up)
  uint32_t w_tiles = (size_w + DATAMOVER_BANDWIDTH_ELEMS - 1) / DATAMOVER_BANDWIDTH_ELEMS;  // number of tiles in W dimension (rounded up)

  while ((job_id = datamover_acquire_task()) < 0 && --acq_to) {}
  if (acq_to == 0) {
    return DATAMOVER_TO;
  }
  datamover_in_set((uint32_t)matrix_in);
  datamover_out_set((uint32_t)matrix_out);
  datamover_tot_len_set(c_tiles * w_tiles * DATAMOVER_BANDWIDTH_ELEMS * size_h);
  datamover_in_d0_set(size_c * size_h * size_w / P, side_P);
  datamover_in_d1_set(size_c, (w_tiles * DATAMOVER_BANDWIDTH_ELEMS) / side_P);
  datamover_in_d2_set(DATAMOVER_BANDWIDTH_ELEMS, c_tiles);
  datamover_in_d3_set(size_c * size_h * size_w / side_P, side_P);
  datamover_out_d0_set(size_h * size_w, c_tiles * DATAMOVER_BANDWIDTH_ELEMS);
  datamover_out_d1_set(DATAMOVER_BANDWIDTH_ELEMS, w_tiles);
  datamover_out_d2_set(size_w, size_h);
  datamover_out_d3_set(0, 0);
  datamover_in_out_d4_stride_set(0, size_c * size_w / side_P);
  datamover_matrix_dim_set(size_w, size_h);
  datamover_channels_set(size_c * size_h * size_w, size_c);
  datamover_ctrl_engine_set(DATAMOVER_FOLD, 0x3, 0xF, DATAMOVER_TRANSP_1ELEM);

  datamover_trigger_task();
  return DATAMOVER_OK;
}

datamover_status_t datamover_fold_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_c, uint32_t size_h, uint32_t size_w, uint64_t timeout) {
  datamover_status_t status;
  status = datamover_fold(matrix_in, matrix_out, size_c, size_h, size_w);
  if (status != DATAMOVER_OK) {
    return status;
  }
  status = datamover_wait_done(timeout);
  return status;
}
