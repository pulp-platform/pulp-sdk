/*
* Copyright (C) 2026 ETH Zurich, University of Bologna, and Fondazione Chips-IT
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
*
* Authors:  Alessandro Nadalini <alessandro.nadalini3@unibo.it>
*/

#ifndef __HAL_PCM_HWPE__
#define __HAL_PCM_HWPE__

#include "hal/pulp.h"
#include "archi/pcm_hwpe/archi_pcm_hwpe.h"

#define PCM_HWPE_WRITE(value, offset) *(volatile int *)(ARCHI_PCM_HWPE_ADDR + offset) = value
#define PCM_HWPE_READ(offset) *(volatile int *)(ARCHI_PCM_HWPE_ADDR + offset)

static inline void pcm_trigger_job() {
    PCM_HWPE_WRITE(0, PCM_HWPE_TRIGGER);
}

static inline int pcm_acquire_job() {
    return PCM_HWPE_READ(PCM_HWPE_ACQUIRE);
}

static inline unsigned int pcm_get_status() {
    return PCM_HWPE_READ(PCM_HWPE_STATUS);
}

static inline void pcm_soft_clear() {
  PCM_HWPE_WRITE(0, PCM_HWPE_SOFT_CLEAR);
}

static inline void pcm_eu_wait(uint32_t job_id) {
    
    while (pcm_get_status() != 0 && PCM_HWPE_READ(PCM_HWPE_RUN_TASK) == job_id) {
        eu_evt_maskWaitAndClr(1 << ARCHI_CL_EVT_ACC0);
    }
}

static inline void pcm_polling_wait(uint32_t job_id) {
    while (pcm_get_status() != 0 && PCM_HWPE_READ(PCM_HWPE_RUN_TASK) == job_id) {
        // busy wait
    }
}

static inline void pcm_hwpe_streamer_config(
    volatile uint32_t   *src_addr, 
    uint32_t            d0_length,
    uint32_t            d0_stride,
    uint32_t            d1_length,
    uint32_t            d1_stride,
    uint32_t            total_length,
    volatile uint32_t   *dst_addr,
    uint32_t            out_d0_length,
    uint32_t            out_d0_stride,
    uint32_t            out_d1_length,
    uint32_t            out_d1_stride,
    uint32_t            out_total_length
){
    PCM_HWPE_WRITE(src_addr, PCM_HWPE_JOB_SRC_ADDR);
    PCM_HWPE_WRITE(d0_length, PCM_HWPE_D0_LENGTH);
    PCM_HWPE_WRITE(d0_stride, PCM_HWPE_D0_STRIDE);
    PCM_HWPE_WRITE(d1_length, PCM_HWPE_D1_LENGTH);
    PCM_HWPE_WRITE(d1_stride, PCM_HWPE_D1_STRIDE);
    PCM_HWPE_WRITE(total_length, PCM_HWPE_TOTAL_LENGTH);
    PCM_HWPE_WRITE(dst_addr, PCM_HWPE_JOB_DST_ADDR);
    PCM_HWPE_WRITE(out_d0_length, PCM_HWPE_OUT_D0_LENGTH);
    PCM_HWPE_WRITE(out_d0_stride, PCM_HWPE_OUT_D0_STRIDE);
    PCM_HWPE_WRITE(out_d1_length, PCM_HWPE_OUT_D1_LENGTH);
    PCM_HWPE_WRITE(out_d1_stride, PCM_HWPE_OUT_D1_STRIDE);
    PCM_HWPE_WRITE(out_total_length, PCM_HWPE_OUT_TOTAL_LENGTH);
}

static inline void pcm_hwpe_configure_matrix(
    uint8_t layer,
    uint8_t sector_0,
    uint8_t sector_1,
    uint8_t sector_2,
    uint8_t sector_3
){
    PCM_HWPE_WRITE(layer, PCM_HWPE_ACT_LAYER);
    PCM_HWPE_WRITE(sector_0, PCM_HWPE_ACT_SECT_0);
    PCM_HWPE_WRITE(sector_1, PCM_HWPE_ACT_SECT_1);
    PCM_HWPE_WRITE(sector_2, PCM_HWPE_ACT_SECT_2);
    PCM_HWPE_WRITE(sector_3, PCM_HWPE_ACT_SECT_3);
}

static inline void pcm_hwpe_configure_job(
    uint32_t num_jobs
){
    PCM_HWPE_WRITE(num_jobs, PCM_HWPE_NUM_JOBS);
}


#endif