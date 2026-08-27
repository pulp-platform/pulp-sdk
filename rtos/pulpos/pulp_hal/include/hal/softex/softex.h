// Copyright 2023 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Andrea Belano <andrea.belano@studio.unibo.it>
//

#ifndef __HAL_SOFTEX__
#define __HAL_SOFTEX__

#include "hal/pulp.h"
#include "archi/softex/archi_softex.h"

//#define abs(x) (x>0 ? x : -x)

#define SOFTEX_WRITE(value, offset) *(volatile int *)(ARCHI_SOFTEX_ADDR + offset) = value
#define SOFTEX_READ(offset) *(volatile int *)(ARCHI_SOFTEX_ADDR + offset)

static inline void softex_trigger_job() {
    SOFTEX_WRITE(0, SOFTEX_TRIGGER);
}

static inline int softex_acquire_job() {
    return SOFTEX_READ(SOFTEX_ACQUIRE);
}

static inline unsigned int softex_get_status() {
    return SOFTEX_READ(SOFTEX_STATUS);
}

static inline void softex_soft_clear() {
  SOFTEX_WRITE(0, SOFTEX_SOFT_CLEAR);
}

static inline void softex_eu_wait(uint32_t job_id) {
    
    while (softex_get_status() != 0 && SOFTEX_READ(SOFTEX_RUNNING_JOB) == job_id) {
        eu_evt_maskWaitAndClr(1 << ARCHI_CL_EVT_ACC1);
    }
}

static inline void softex_polling_wait(uint32_t job_id) {
    while (softex_get_status() != 0 && SOFTEX_READ(SOFTEX_RUNNING_JOB) == job_id) {
        // busy wait
    }
}

#endif