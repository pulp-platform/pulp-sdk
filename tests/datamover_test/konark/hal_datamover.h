// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Authors: Sergio Mazzola <smazzola@iis.ee.ethz.ch>
//          Cyrill Durrer <cdurrer@iis.ee.ethz.ch>
//          Daniel Keller <dankeller@iis.ee.ethz.ch>
//          Francesco Conti <f.conti@unibo.it>

#ifndef __HAL_DATAMOVER_H__
#define __HAL_DATAMOVER_H__

#include <stdint.h>

#include "konark_cluster_raw_addrmap.h"

///////////
// Archi //
///////////

#define DATAMOVER_BASE_ADDR KONARK_CLUSTER_ADDRMAP_CLUSTER_HWPE_SUBSYSTEM_HWPE_DATAMOVER_BASE_ADDR

/* Architecture */

#define DATAMOVER_BANDWIDTH  512
#define DATAMOVER_WORD_WIDTH 64
#define DATAMOVER_ELEM_WIDTH 8
#define DATAMOVER_MISALIGNED_ACCESSES 0

#if DATAMOVER_MISALIGNED_ACCESSES
  #define DATAMOVER_BANDWIDTH_ALIGNED (DATAMOVER_BANDWIDTH - DATAMOVER_WORD_WIDTH)
#else
  #define DATAMOVER_BANDWIDTH_ALIGNED (DATAMOVER_BANDWIDTH)
#endif
#define DATAMOVER_BANDWIDTH_ELEMS (DATAMOVER_BANDWIDTH_ALIGNED / DATAMOVER_ELEM_WIDTH)
#define DATAMOVER_WORD_ELEMS      (DATAMOVER_WORD_WIDTH / DATAMOVER_ELEM_WIDTH)

/* Registers */
// To access a register add: DATAMOVER_BASE_ADDR + context offset + register offset

#define DATAMOVER_REGISTER_OFFSET    0x40  // Alias of DATAMOVER_REGISTER_CXT0_OFFS
#define DATAMOVER_REGISTER_CXT0_OFFS 0x80  // Regfile context 0
#define DATAMOVER_REGISTER_CXT1_OFFS 0x120 // Regfile context 1

#define DATAMOVER_REG_IN_PTR_OFFSET           0x00  // Input pointer
#define DATAMOVER_REG_OUT_PTR_OFFSET          0x04  // Output pointer
#define DATAMOVER_REG_TOT_LEN_OFFSET          0x08  // Total length in number of accesses (BW)
#define DATAMOVER_REG_IN_D0_OFFSET            0x0C  // [31:16] in_d0_stride; [15:0] in_d0_len
#define DATAMOVER_REG_IN_D1_OFFSET            0x10  // [31:16] in_d1_stride; [15:0] in_d1_len
#define DATAMOVER_REG_IN_D2_OFFSET            0x14  // [31:16] in_d2_stride; [15:0] in_d2_len
#define DATAMOVER_REG_IN_D3_OFFSET            0x18  // [31:16] in_d3_stride; [15:0] in_d3_len
#define DATAMOVER_REG_OUT_D0_OFFSET           0x1C  // [31:16] out_d0_stride; [15:0] out_d0_len
#define DATAMOVER_REG_OUT_D1_OFFSET           0x20  // [31:16] out_d1_stride; [15:0] out_d1_len
#define DATAMOVER_REG_OUT_D2_OFFSET           0x24  // [31:16] out_d2_stride; [15:0] out_d2_len
#define DATAMOVER_REG_OUT_D3_OFFSET           0x28  // [31:16] out_d3_stride; [15:0] out_d3_len
#define DATAMOVER_REG_IN_OUT_D4_STRIDE_OFFSET 0x2C  // [31:16] out_d4_stride; [15:0] in_d4_stride (d4_len unnecessary due to tot_len)
#define DATAMOVER_REG_MATRIX_DIM_OFFSET       0x30  // [31:16] tensor_size_n; [15:0] tensor_size_m
#define DATAMOVER_REG_CHANNELS_OFFSET         0x34  // [31:11] total_elements = num_channels * size_m * size_n (pre-compute to save HW resources); [10:0] num_channels (for unfolding/folding)
#define DATAMOVER_REG_CTRL_ENGINE_OFFSET      0x38  // [15:12] write_dim_en; [11:8] read_dim_en; [7:3] datamover_mode; [2:0] transp_mode (LSB: 000=none, 001=1 elem, 010=2 elem, 100=4 elem)


///////////
// Types //
///////////
typedef enum {
  DATAMOVER_COPY  = 0x0,
  DATAMOVER_TRANSP = 0x1,
  DATAMOVER_CIM_LAYOUT = 0x2,
  DATAMOVER_CIM_LAYOUT_TRANSPOSE = 0x3,
  DATAMOVER_UNFOLD = 0x4,
  DATAMOVER_FOLD = 0x5
} datamover_mode_t;   // Must match datamover_mode_e in datamover_package.sv
typedef enum {
  DATAMOVER_TRANSP_NONE  = 0x0,
  DATAMOVER_TRANSP_1ELEM = 0x1,
  DATAMOVER_TRANSP_2ELEM = 0x2,
  DATAMOVER_TRANSP_4ELEM = 0x4
} datamover_transp_mode_t;  // Must match transp_mode_e in datamover_package.sv

typedef enum {
    DATAMOVER_OK = 0, // Success
    DATAMOVER_TO,     // Timeout
    DATAMOVER_ERR     // Generic error
} datamover_status_t;

/////////////
// Defines //
/////////////

#if VERBOSE
/* Verbose read/write register */
// #include "printf.h"
#define __HAL_DATAMOVER_REG_WRITE(base, offset, value) do { \
    *(volatile uint32_t *)(base + offset) = value; \
    printf("__HAL_DATAMOVER_REG_WRITE: Addr 0x%08x <= 0x%08x\n", (uint32_t)(base + offset), (uint32_t)(value)); \
  } while(0)
#define __HAL_DATAMOVER_REG_READ(base, offset) ({ \
    uint32_t read_value = *(volatile uint32_t *)(base + offset); \
    printf("__HAL_DATAMOVER_REG_READ: Addr 0x%08x => 0x%08x\n", (uint32_t)(base + offset), (uint32_t)(read_value)); \
    read_value; \
  })
#else
/* Normal read/write register */
#define __HAL_DATAMOVER_REG_WRITE(base, offset, value) *(volatile uint32_t *)(base + offset) = value
#define __HAL_DATAMOVER_REG_READ(base, offset)         *(volatile uint32_t *)(base + offset)
#endif

// DATAMOVER HWPE register access
#define DATAMOVER_HWPE_REG_WRITE(offset, value) __HAL_DATAMOVER_REG_WRITE(DATAMOVER_BASE_ADDR, offset, value)
#define DATAMOVER_HWPE_REG_READ(offset)         __HAL_DATAMOVER_REG_READ(DATAMOVER_BASE_ADDR, offset)

////////////////
// Prototypes //
////////////////

// Drivers
int datamover_acquire_task(void);
void datamover_trigger_task(void);
uint32_t datamover_finished(void);
uint32_t datamover_get_status(void);
void datamover_soft_clear(void);

void datamover_in_set(uint32_t value);
void datamover_out_set(uint32_t value);
void datamover_tot_len_set(uint32_t value);
void datamover_in_d0_set(uint32_t stride, uint32_t len);
void datamover_in_d1_set(uint32_t stride, uint32_t len);
void datamover_in_d2_set(uint32_t stride, uint32_t len);
void datamover_in_d3_set(uint32_t stride, uint32_t len);
void datamover_out_d0_set(uint32_t stride, uint32_t len);
void datamover_out_d1_set(uint32_t stride, uint32_t len);
void datamover_out_d2_set(uint32_t stride, uint32_t len);
void datamover_out_d3_set(uint32_t stride, uint32_t len);
void datamover_in_out_d4_stride_set(uint32_t out_stride, uint32_t in_stride);
void datamover_matrix_dim_set(uint32_t tensor_size_n, uint32_t tensor_size_m);
void datamover_channels_set(uint32_t total_elements, uint32_t num_channels);
void datamover_ctrl_engine_set(datamover_mode_t datamover_mode, uint32_t write_dim_en, uint32_t read_dim_en, datamover_transp_mode_t transp_mode);

// HAL
datamover_status_t datamover_wait_done(uint64_t timeout);
datamover_status_t datamover_copy(uint8_t *src, uint8_t *dst, uint32_t size_m, uint32_t size_n);
datamover_status_t datamover_copy_blocking(uint8_t *src, uint8_t *dst, uint32_t size_m, uint32_t size_n, uint64_t timeout);
datamover_status_t datamover_transpose(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, datamover_transp_mode_t transp_mode);
datamover_status_t datamover_transpose_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, datamover_transp_mode_t transp_mode, uint64_t timeout);
datamover_status_t datamover_cim_layout(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size);
datamover_status_t datamover_cim_layout_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size, uint64_t timeout);
datamover_status_t datamover_cim_layout_reverse(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size);
datamover_status_t datamover_cim_layout_reverse_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size, uint64_t timeout);
datamover_status_t datamover_cim_layout_transpose_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_m, uint32_t size_n, uint32_t row_tile_size, datamover_transp_mode_t transp_mode, uint64_t timeout);
datamover_status_t datamover_unfold(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_c, uint32_t dim_h, uint32_t dim_w);
datamover_status_t datamover_unfold_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_c, uint32_t dim_h, uint32_t dim_w, uint64_t timeout);
datamover_status_t datamover_fold(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_c, uint32_t dim_h, uint32_t dim_w);
datamover_status_t datamover_fold_blocking(uint8_t *matrix_in, uint8_t *matrix_out, uint32_t size_c, uint32_t dim_h, uint32_t dim_w, uint64_t timeout);

#endif // __HAL_DATAMOVER_H__
