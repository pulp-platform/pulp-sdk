
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
#define RNNACC_WRITE(value, offset) __builtin_pulp_OffsetedWriteByte(value, (int *)RNNACC_ADDR_BASE, offset)
#define RNNACC_READ(offset) __builtin_pulp_OffsetedReadByte((int *)RNNACC_ADDR_BASE, offset)
#else
#define RNNACC_WRITE(value, offset) pulp_write32(RNNACC_ADDR_BASE + (offset), value)
#define RNNACC_READ(offset) pulp_read32(RNNACC_ADDR_BASE + (offset))
#define RNNACC_WRITE_B(value, offset) pulp_write8(RNNACC_ADDR_BASE + (offset), value)
#define RNNACC_READ_B(offset) pulp_read8(RNNACC_ADDR_BASE + (offset))
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

#define RNNACC_BARRIER_NOSTATUS()      eu_evt_maskWaitAndClr (1 << RNNACC_EVT0)
#define RNNACC_BARRIER()               do { eu_evt_maskWaitAndClr (1 << RNNACC_EVT0); } while((*(int volatile *)(RNNACC_STATUS)) != 0)
#define RNNACC_BUSYWAIT()              do { } while((*(int volatile *)(RNNACC_STATUS)) != 0)
#define RNNACC_BARRIER_ACQUIRE(job_id) job_id = RNNACC_READ(RNNACC_ACQUIRE); \
                                while(job_id < 0) { eu_evt_maskWaitAndClr (1 << RNNACC_EVT0);job_id = RNNACC_READ(RNNACC_ACQUIRE); };


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
                                             unsigned int start_x, unsigned int start_wx) {
  RNNACC_WRITE(start_x, RNNACC_ADDR_X);
  RNNACC_WRITE(start_wx, RNNACC_ADDR_WX);
  RNNACC_WRITE(start_bias, RNNACC_ADDR_B);
  RNNACC_WRITE(start_dst, RNNACC_ADDR_DST);
  // RNNACC_WRITE(n_input, RNNACC_N_INPUT);
  // RNNACC_WRITE(n_output, RNNACC_N_OUTPUT);
  // RNNACC_WRITE(0x0001, RNNACC_JOB_MODE);
}

// set parameters for matmul computation with bias
static inline void rnnacc_set_biased_matmul_i_tile( unsigned int start_dst, unsigned int start_bias, 
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

static inline void rnnacc_set_biased_matmul_o_tile( unsigned int start_dst, unsigned int start_bias, 
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