/*
 * Copyright (C) 2022-2023 ETH Zurich and University of Bologna
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
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Author: Yvan Tortorella  <yvan.tortorella@unibo.it>
 *
 * RedMulE Hardware Abstraction Layer (HAL)
 */

#ifndef __HAL_REDMULE_H__
#define __HAL_REDMULE_H__

#include "inc/tensor_dim.h"
#include "archi_redmule.h"
#include "inc/config.h"

/*
 * 
 * For control, generic configuration register layout,
 * and job-dependent register map, look at redmule_archi.h
 *
 */

#define REDMULE_EVT0           12

/* LOW-LEVEL HAL */
#define REDMULE_ADDR_BASE REDMULE_BASE_ADD
#define REDMULE_ADDR_SPACE 0x00000100

// For all the following functions we use __builtin_pulp_OffsetedWrite and __builtin_pulp_OffsetedRead
// instead of classic load/store because otherwise the compiler is not able to correctly factorize
// the HWPE base in case several accesses are done, ending up with twice more code

#define HWPE_WRITE(value, offset) *(int *)(REDMULE_ADDR_BASE + offset) = value
#define HWPE_READ(offset) *(int *)(REDMULE_ADDR_BASE + offset)

// static inline void hwpe_bytecode_set(unsigned int offs, unsigned int value) {
//   HWPE_WRITE(value, HWPE_BYTECODE+offs);
// }

static inline void redmule_x_add_set (unsigned int value) {
  HWPE_WRITE(value, REDMULE_REG_OFFS + REDMULE_REG_X_PTR);
}

static inline void redmule_w_add_set (unsigned int value) {
  HWPE_WRITE(value, REDMULE_REG_OFFS + REDMULE_REG_W_PTR);
}

static inline void redmule_y_add_set (unsigned int value) {
  HWPE_WRITE(value, REDMULE_REG_OFFS + REDMULE_REG_Y_PTR);
}

static inline void redmule_z_add_set (unsigned int value) {
  HWPE_WRITE(value, REDMULE_REG_OFFS + REDMULE_REG_Z_PTR);
}

static inline void hwpe_trigger_job() {
  HWPE_WRITE(0, REDMULE_TRIGGER);
}

static inline int hwpe_acquire_job() {
  return HWPE_READ(REDMULE_ACQUIRE);
}

static inline unsigned int hwpe_get_status() {
  return HWPE_READ(REDMULE_STATUS);
}

static inline void hwpe_soft_clear() {
  volatile int i;
  HWPE_WRITE(0, REDMULE_SOFT_CLEAR);
}

static inline void hwpe_cg_enable() {
  return;
}

static inline void hwpe_cg_disable() {
  return;
}

void redmule_cfg (uint16_t m_size, uint16_t n_size, uint16_t k_size, uint8_t gemm_ops){
   uint32_t x_iters        = 0;
   uint32_t w_iters        = 0;
   uint32_t leftovers      = 0;
   uint32_t left_params    = 0;
   uint32_t x_d1_stride    = 0;
   uint32_t x_rows_offs    = 0;
   uint32_t w_tot_len      = 0;
   uint32_t w_d1_len       = 0;
   uint32_t w_d0_stride    = 0;
   uint32_t yz_tot_len     = 0;
   uint32_t yz_d0_stride   = 0;
   uint32_t yz_d2_stride   = 0;
   uint32_t tot_x_read     = 0;
   uint32_t x_buffer_slots = 0;
   uint32_t op_selection   = 0;
   uint16_t tot_stores     = 0;
   uint16_t w_rows         = n_size;
   uint16_t depth          = DATA_WIDTH/(ARRAY_HEIGHT*FPFORMAT);
   uint8_t  tile           = ARRAY_HEIGHT*(PIPE_REGS + 1);
   _Bool    x_rows_sub     = 0;
   _Bool    x_cols_sub     = 0;
   _Bool    w_cols_sub     = 0;
   uint16_t x_rows_iter,
            x_rows_iter_tmp,
            w_rows_iter,
            w_rows_iter_tmp;
   uint16_t x_cols_iter,
            x_cols_iter_tmp,
            w_cols_iter,
            w_cols_iter_tmp;
   uint8_t  x_rows_lftovr,
            x_cols_lftovr,
            w_rows_lftovr,
            w_cols_lftovr,
            slots;

   uint32_t x_tot_len;

   // Calculating the number of iterations alng the two dimensions of the X matrix
   x_rows_iter_tmp = m_size/ARRAY_WIDTH;
   x_cols_iter_tmp = n_size/tile;

   // Calculating the number of iterations alng the two dimensions of the W matrix
   w_rows_iter_tmp = w_rows;
   w_cols_iter_tmp = k_size/tile;

   // Calculating the residuals along the input dimensions
   x_rows_lftovr = m_size - (x_rows_iter_tmp*ARRAY_WIDTH);
   x_cols_lftovr = n_size - (x_cols_iter_tmp*tile);

   // Calculating the residuals along the weight dimensions
   w_rows_lftovr = n_size - (ARRAY_HEIGHT*(w_rows/ARRAY_HEIGHT));
   w_cols_lftovr = k_size - (w_cols_iter_tmp*tile);

   if (w_cols_lftovr != 0)
     w_cols_iter = w_cols_iter_tmp + 1;
   else 
     w_cols_iter = w_cols_iter_tmp;

  if (w_rows_lftovr != 0)
    w_rows_iter = w_rows_iter_tmp + ARRAY_HEIGHT - w_rows_lftovr;
  else
    w_rows_iter = w_rows_iter_tmp;

   if (x_cols_lftovr != 0)
     x_cols_iter = x_cols_iter_tmp + 1;
   else 
     x_cols_iter = x_cols_iter_tmp;

   if (x_rows_lftovr != 0)
     x_rows_iter = x_rows_iter_tmp + 1;
   else 
     x_rows_iter = x_rows_iter_tmp;

   if (x_cols_lftovr%depth != 0)
     x_buffer_slots = x_cols_lftovr/depth + 1;
   else
     x_buffer_slots = x_cols_lftovr/depth;

   // Calculating the number of total stores
   tot_stores = x_rows_iter*w_cols_iter;

   // Determining if input matrixes are sub-matrixes
   if (m_size < ARRAY_WIDTH)
     x_rows_sub = 1;
   if (n_size < ARRAY_HEIGHT)
     x_cols_sub = 1;
   if (k_size < tile)
    w_cols_sub = 1;

   // Operation selection
   switch (gemm_ops) {
     case MATMUL:
       op_selection |= (RNE << 29 | RNE << 26 | OP_FMADD << 22 | OP_MINMAX << 18 | SRC_FMT << 15 | DST_FMT << 12) | 0;
     break;
     
     case GEMM:
       op_selection |= (RNE << 29 | RNE << 26 | OP_FMADD << 22 | OP_MINMAX << 18 | SRC_FMT << 15 | DST_FMT << 12) | 1;
     break;
     
     case ADDMAX:
       op_selection |= (RNE << 29 | RTZ << 26 | OP_ADD << 22 | OP_MINMAX << 18 | SRC_FMT << 15 | DST_FMT << 12) | 1;
     break;
     
     case ADDMIN:
       op_selection |= (RNE << 29 | RNE << 26 | OP_ADD << 22 | OP_MINMAX << 18 | SRC_FMT << 15 | DST_FMT << 12) | 1;
     break;
     
     case MULMAX:
       op_selection |= (RNE << 29 | RTZ << 26 | OP_MUL << 22 | OP_MINMAX << 18 | SRC_FMT << 15 | DST_FMT << 12) | 1;
     break;
     
     case MULMIN:
       op_selection |= (RNE << 29 | RNE << 26 | OP_MUL << 22 | OP_MINMAX << 18 | SRC_FMT << 15 | DST_FMT << 12) | 1;
     break;
     
     case MAXMIN:
       op_selection |= (RTZ << 29 | RNE << 26 | OP_MINMAX << 22 | OP_MINMAX << 18 | SRC_FMT << 15 | DST_FMT << 12) | 1;
     break;
     
     case MINMAX:
       op_selection |= (RNE << 29 | RTZ << 26 | OP_MINMAX << 22 | OP_MINMAX << 18 | SRC_FMT << 15 | DST_FMT << 12) | 1;
     break;
   }

   // Storing iterations and residuals in registers
   x_iters      |= x_rows_iter   << 16 | x_cols_iter   << 0;
   w_iters      |= w_rows_iter   << 16 | w_cols_iter   << 0;
   leftovers    |= x_rows_lftovr << 24 | x_cols_lftovr << 16 | w_rows_lftovr << 8  | w_cols_lftovr << 0;
   left_params  |= tot_stores    << 16 | x_rows_sub    << 15 | x_cols_sub    << 14 | w_cols_sub   << 13;
   x_d1_stride   = (FPFORMAT/8)*((DATA_WIDTH/FPFORMAT) * x_cols_iter_tmp + x_cols_lftovr); 
   x_rows_offs   = ARRAY_WIDTH*x_d1_stride;
   w_tot_len     = w_rows_iter*w_cols_iter*x_rows_iter;
   w_d0_stride   = (FPFORMAT/8)*((DATA_WIDTH/FPFORMAT) * w_cols_iter_tmp + w_cols_lftovr);
   yz_tot_len    = ARRAY_WIDTH*x_rows_iter*w_cols_iter;
   yz_d0_stride  = w_d0_stride;
   yz_d2_stride  = ARRAY_WIDTH*w_d0_stride;
   tot_x_read    = x_rows_iter*x_cols_iter*w_cols_iter;

   x_tot_len     = ARRAY_WIDTH * x_rows_iter * x_cols_iter * w_cols_iter;

   // Writing the computations in configuration register
   HWPE_WRITE(x_iters       , REDMULE_REG_OFFS + REDMULE_REG_X_ITER_PTR         );
   HWPE_WRITE(w_iters       , REDMULE_REG_OFFS + REDMULE_REG_W_ITER_PTR         );
   HWPE_WRITE(leftovers     , REDMULE_REG_OFFS + REDMULE_REG_LEFTOVERS_PTR      );
   HWPE_WRITE(left_params   , REDMULE_REG_OFFS + REDMULE_REG_LEFT_PARAMS_PTR    );
   HWPE_WRITE(x_d1_stride   , REDMULE_REG_OFFS + REDMULE_REG_X_D1_STRIDE_PTR    );
   HWPE_WRITE(x_rows_offs   , REDMULE_REG_OFFS + REDMULE_REG_X_ROWS_OFFS_PTR    );
   HWPE_WRITE(tot_x_read    , REDMULE_REG_OFFS + REDMULE_REG_TOT_X_READ_PTR     );
   HWPE_WRITE(x_buffer_slots, REDMULE_REG_OFFS + REDMULE_REG_X_BUFFER_SLOTS_PTR );
   HWPE_WRITE(w_tot_len     , REDMULE_REG_OFFS + REDMULE_REG_W_TOT_LEN_PTR      );
   HWPE_WRITE(w_d0_stride   , REDMULE_REG_OFFS + REDMULE_REG_W_D0_STRIDE_PTR    );
   HWPE_WRITE(yz_tot_len    , REDMULE_REG_OFFS + REDMULE_REG_YZ_TOT_LEN_PTR     );
   HWPE_WRITE(yz_d0_stride  , REDMULE_REG_OFFS + REDMULE_REG_YZ_D0_STRIDE_PTR   );
   HWPE_WRITE(yz_d2_stride  , REDMULE_REG_OFFS + REDMULE_REG_YZ_D2_STRIDE_PTR   );
   HWPE_WRITE(x_tot_len     , REDMULE_REG_OFFS + REDMULE_REG_X_TOT_LEN_PTR      );
   HWPE_WRITE(op_selection  , REDMULE_REG_OFFS + REDMULE_REG_OP_SELECTION       );
}

#endif /* __HAL_REDMULE_H__ */