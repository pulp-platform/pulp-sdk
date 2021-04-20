
#ifndef __HAL_RNNACC_V1_H__
#define __HAL_RNNACC_V1_H__

/*
 * Copyright (C) 2021 ETH Zurich and University of Bologna
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
 * Authors:  Geethan Karunaratne, IBM <kar@zurich.ibm.com>
 */

#include "pmsis.h"

/* LOW-LEVEL HAL */
#define RNNACC_ADDR_BASE ARCHI_RNNACC_ADDR
#define RNNACC_ADDR_SPACE 0x00000100

// For all the following functions we use __builtin_pulp_OffsetedWrite and __builtin_pulp_OffsetedRead
// instead of classic load/store because otherwise the compiler is not able to correctly factorize
// the HWME base in case several accesses are done, ending up with twice more code

#if defined(__riscv__) && !defined(RV_ISA_RV32)
#define RNNACC_WRITE(value, offset) __builtin_pulp_OffsetedWrite(value, (int *)RNNACC_ADDR_BASE, offset)
#define RNNACC_READ(offset) __builtin_pulp_OffsetedRead((int *)RNNACC_ADDR_BASE, offset)
#else
#define RNNACC_WRITE(value, offset) pulp_write32(RNNACC_ADDR_BASE + (offset), value)
#define RNNACC_READ(offset) pulp_read32(RNNACC_ADDR_BASE + (offset))
#endif


// Control and generic configuration register layout 
static inline void rnnacc_trigger_job() {
  RNNACC_WRITE(0, RNNACC_TRIGGER);
}
static inline int rnnacc_acquire() {
  return RNNACC_READ(RNNACC_ACQUIRE);
}
static inline int rnnacc_get_finished_jobs() {
  return RNNACC_READ(RNNACC_FINISHED_JOBS);
}
static inline int rnnacc_get_status() {
  return RNNACC_READ(RNNACC_STATUS);
}
static inline int rnnacc_get_running_task_id() {
  return RNNACC_READ(RNNACC_RUNNING_TASK);
}
static inline void rnnacc_soft_clear() {
  RNNACC_WRITE(0,RNNACC_SOFT_CLEAR);
}
static inline int rnnacc_get_state() {
  return RNNACC_READ(RNNACC_CHECK_STATE);
}

// set addresses for matmul computation without bias
static inline void rnnacc_set_matmul( unsigned int start_dst,
                                      unsigned int start_x, unsigned int start_wx,
                                      unsigned int n_input, unsigned int n_output ) {
  RNNACC_WRITE(start_x, RNNACC_ADDR_X);
  RNNACC_WRITE(start_wx, RNNACC_ADDR_WX);
  RNNACC_WRITE(start_dst, RNNACC_ADDR_DST);
  RNNACC_WRITE(n_input, RNNACC_N_INPUT);
  RNNACC_WRITE(n_output, RNNACC_N_OUTPUT);

  RNNACC_WRITE(0x0000, RNNACC_JOB_MODE);

}

// set parameters for matmul computation with bias
static inline void rnnacc_set_biased_matmul( unsigned int start_dst, unsigned int start_bias, 
                                             unsigned int start_x, unsigned int start_wx,
                                             unsigned int n_input, unsigned int n_output ) {
  RNNACC_WRITE(start_x, RNNACC_ADDR_X);
  RNNACC_WRITE(start_wx, RNNACC_ADDR_WX);
  RNNACC_WRITE(start_bias, RNNACC_ADDR_B);
  RNNACC_WRITE(start_dst, RNNACC_ADDR_DST);
  RNNACC_WRITE(n_input, RNNACC_N_INPUT);
  RNNACC_WRITE(n_output, RNNACC_N_OUTPUT);

  RNNACC_WRITE(0x0001, RNNACC_JOB_MODE);

}

// set parameters for lstm gate computation
static inline void rnnacc_set_twomatmul( unsigned int start_dst, unsigned int start_bias, 
                                         unsigned int start_x, unsigned int start_wx, 
                                         unsigned int start_h, unsigned int start_wh,
                                         unsigned int n_input, unsigned int n_output ) {
  RNNACC_WRITE(start_x, RNNACC_ADDR_X);
  RNNACC_WRITE(start_wx, RNNACC_ADDR_WX);
  RNNACC_WRITE(start_h, RNNACC_ADDR_H);
  RNNACC_WRITE(start_wh, RNNACC_ADDR_WH);
  RNNACC_WRITE(start_bias, RNNACC_ADDR_B);
  RNNACC_WRITE(start_dst, RNNACC_ADDR_DST);
  RNNACC_WRITE(n_input, RNNACC_N_INPUT);
  RNNACC_WRITE(n_output, RNNACC_N_OUTPUT);

  RNNACC_WRITE(0x0002, RNNACC_JOB_MODE);

}

// set parameters for lstm gate computation
static inline void rnnacc_set_biased_twomatmul( unsigned int start_dst, unsigned int start_bias, 
                                                unsigned int start_x, unsigned int start_wx, 
                                                unsigned int start_h, unsigned int start_wh,
                                                unsigned int n_input, unsigned int n_output ) {
  RNNACC_WRITE(start_x, RNNACC_ADDR_X);
  RNNACC_WRITE(start_wx, RNNACC_ADDR_WX);
  RNNACC_WRITE(start_h, RNNACC_ADDR_H);
  RNNACC_WRITE(start_wh, RNNACC_ADDR_WH);
  RNNACC_WRITE(start_bias, RNNACC_ADDR_B);
  RNNACC_WRITE(start_dst, RNNACC_ADDR_DST);
  RNNACC_WRITE(n_input, RNNACC_N_INPUT);
  RNNACC_WRITE(n_output, RNNACC_N_OUTPUT);

  RNNACC_WRITE(0x0003, RNNACC_JOB_MODE);

}

// set parameters for lstm gate computation
static inline void rnnacc_set_lstm_gate( unsigned int start_dst, unsigned int start_bias, 
                                         unsigned int start_x, unsigned int start_wx, 
                                         unsigned int start_h, unsigned int start_wh,
                                         unsigned int n_input, unsigned int n_output ) {
  RNNACC_WRITE(start_x, RNNACC_ADDR_X);
  RNNACC_WRITE(start_wx, RNNACC_ADDR_WX);
  RNNACC_WRITE(start_h, RNNACC_ADDR_H);
  RNNACC_WRITE(start_wh, RNNACC_ADDR_WH);
  RNNACC_WRITE(start_bias, RNNACC_ADDR_B);
  RNNACC_WRITE(start_dst, RNNACC_ADDR_DST);
  RNNACC_WRITE(n_input, RNNACC_N_INPUT);
  RNNACC_WRITE(n_output, RNNACC_N_OUTPUT);

  RNNACC_WRITE(0x0007, RNNACC_JOB_MODE);

}

// static inline void rnnacc_submit_plot(unsigned int plot_val) {
//   RNNACC_WRITE(plot_val,RNNACC_SUBMIT_PLOT);  
// }

// static inline void rnnacc_set_job_params(unsigned int start_x,
//                                       unsigned int start_y,
//                                       unsigned int width,
//                                       unsigned int height,
//                                       unsigned int fetch_size,
//                                       unsigned int store_size,
//                                       unsigned int job_line_length,
//                                       unsigned int job_feat_stride,
//                                       unsigned int job_feat_length,
//                                       unsigned int num_of_jobs){

  // RNNACC_WRITE(start_x,RNNACC_J_START_X);
  // RNNACC_WRITE(start_y,RNNACC_J_START_Y);
  // RNNACC_WRITE(width,RNNACC_J_WIDTH);
  // RNNACC_WRITE(height,RNNACC_J_HIGHT);
  // RNNACC_WRITE(fetch_size,RNNACC_FETCH_LENGTH);
  // RNNACC_WRITE(store_size,RNNACC_STORE_LENGTH);
  // RNNACC_WRITE(job_line_length,RNNACC_JOB_LINE_LENGTH);
  // RNNACC_WRITE(job_feat_stride,RNNACC_JOB_FEAT_STRIDE);
  // RNNACC_WRITE(job_feat_length,RNNACC_JOB_FEAT_LENGTH);
  // RNNACC_WRITE(num_of_jobs, RNNACC_NUM_JOBS);
// }

// static inline void rnnacc_set_multi_jobs_param(unsigned int alpha_in_length,
//                                             unsigned int alpha_in_stride,
//                                             unsigned int beta_in_length,
//                                             unsigned int beta_in_stride,
//                                             unsigned int alpha_out_length,
//                                             unsigned int alpha_out_stride,
//                                             unsigned int beta_out_length,
//                                             unsigned int beta_out_stride){
//   RNNACC_WRITE(alpha_in_length, RNNACC_ALPHA_IN_LENGTH);
//   RNNACC_WRITE(alpha_in_stride, RNNACC_ALPHA_IN_STRIDE);
//   RNNACC_WRITE(beta_in_length,  RNNACC_BETA_IN_LENGTH);
//   RNNACC_WRITE(beta_in_stride,  RNNACC_BETA_IN_STRIDE);

//   RNNACC_WRITE(alpha_out_length, RNNACC_ALPHA_OUT_LENGTH);
//   RNNACC_WRITE(alpha_out_stride, RNNACC_ALPHA_OUT_STRIDE);
//   RNNACC_WRITE(beta_out_length,  RNNACC_BETA_OUT_LENGTH);
//   RNNACC_WRITE(beta_out_stride,  RNNACC_BETA_OUT_STRIDE);
// }

// static inline void rnnacc_set_streamer_params(unsigned int src_addr,unsigned int dest_addr,unsigned int src_strd,unsigned int dst_strd) {
//   RNNACC_WRITE(src_addr,RNNACC_J_SRC_ADDR);
//   RNNACC_WRITE(dest_addr,RNNACC_J_DST_ADDR);
//   RNNACC_WRITE(src_strd,RNNACC_J_SRC_STRID);
//   RNNACC_WRITE(dst_strd,RNNACC_J_DST_STRID);
// }

// static inline unsigned int rnnacc_read_val(unsigned int addr_x,unsigned int addr_y) {
//   RNNACC_WRITE(addr_x,RNNACC_PR_ADDR_X);
//   RNNACC_WRITE(addr_y,RNNACC_PR_ADDR_Y);
//   return RNNACC_READ(RNNACC_PR_VAL);
// }


static inline void plp_hwme_enable() {
  *(volatile int*) (ARCHI_SOC_EU_ADDR + (3 << 3)) |=  0xc00;
}

static inline void plp_hwme_disable() {
  *(volatile int*) (ARCHI_SOC_EU_ADDR + (3 << 3)) &= ~0xc00;
}

static inline void plp_hwme_cluster_enable() {
  *(volatile int*) (ARCHI_CLUSTER_CTRL_ADDR + (3 << 3)) |=  0xc00;
}

static inline void plp_hwme_cluster_disable() {
  *(volatile int*) (ARCHI_CLUSTER_CTRL_ADDR + (3 << 3)) &= ~0xc00;
}

#endif 