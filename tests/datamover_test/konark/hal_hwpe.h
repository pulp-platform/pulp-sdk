// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Authors: Daniel Keller <dankeller@iis.ee.ethz.ch>
//          Cyrill Durrer <cdurrer@iis.ee.ethz.ch>
//          Sergio Mazzola <smazzola@iis.ee.ethz.ch>

#ifndef __HAL_HWPE_H__
#define __HAL_HWPE_H__

#include <stdint.h>

///////////
// Archi //
///////////

// Register offsets of HWPE Ctrl (32-bit registers)
#define HWPE_TRIGGER_OFFSET 0*4
#define HWPE_ACQUIRE_OFFSET 1*4
#define HWPE_FINISHED_OFFSET 2*4
#define HWPE_STATUS_OFFSET 3*4
#define HWPE_RUNNING_JOB_OFFSET 4*4
#define HWPE_SOFT_CLEAR_OFFSET 5*4
// #define HWPE_RESERVED_OFFSET 6*4
#define HWPE_SWSYNC_OFFSET 7*4

/////////////
// Defines //
/////////////

#define __HAL_HWPE_VERBOSE 0

#if __HAL_HWPE_VERBOSE
/* Verbose read/write register */
#include "printf.h"
#define __HAL_HWPE_REG_WRITE(base, offset, value) do { \
    *(volatile uint32_t *)(base + offset) = value; \
    printf("__HAL_HWPE_REG_WRITE: Addr 0x%08x <= 0x%08x\n", (uint32_t)(base + offset), (uint32_t)(value)); \
  } while(0)
#define __HAL_HWPE_REG_READ(base, offset) ({ \
    uint32_t read_value = *(volatile uint32_t *)(base + offset); \
    printf("__HAL_HWPE_REG_READ: Addr 0x%08x => 0x%08x\n", (uint32_t)(base + offset), (uint32_t)(read_value)); \
    read_value; \
  })
#else
/* Normal read/write register */
#define __HAL_HWPE_REG_WRITE(base, offset, value) *(volatile uint32_t *)(base + offset) = value
#define __HAL_HWPE_REG_READ(base, offset)         *(volatile uint32_t *)(base + offset)
#endif

////////////////
// Prototypes //
////////////////

// Drivers
void hwpe_task_queue_release_and_run(uint32_t hwpe_base_addr);
void hwpe_task_queue_release(uint32_t hwpe_base_addr);
int hwpe_task_queue_acquire_task(uint32_t hwpe_base_addr);
uint32_t hwpe_finished(uint32_t hwpe_base_addr);
uint32_t hwpe_task_queue_status(uint32_t hwpe_base_addr);
uint8_t hwpe_last_task_id(uint32_t hwpe_base_addr);
void hwpe_soft_clear(uint32_t hwpe_base_addr);

#endif // __HAL_HWPE_H__
