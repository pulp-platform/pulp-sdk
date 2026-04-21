// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Authors: Daniel Keller <dankeller@iis.ee.ethz.ch>
//          Cyrill Durrer <cdurrer@iis.ee.ethz.ch>
//          Sergio Mazzola <smazzola@iis.ee.ethz.ch>

#include <stdint.h>

#include "konark/hal_hwpe.h"

/////////////
// Drivers //
/////////////

// Access to HWPE mandatory registers

void hwpe_task_queue_release_and_run(uint32_t hwpe_base_addr) {
  __HAL_HWPE_REG_WRITE(hwpe_base_addr, HWPE_TRIGGER_OFFSET, 0);
}

void hwpe_task_queue_release(uint32_t hwpe_base_addr) {
  __HAL_HWPE_REG_WRITE(hwpe_base_addr, HWPE_TRIGGER_OFFSET, 1);
}

int hwpe_task_queue_acquire_task(uint32_t hwpe_base_addr) {
  int read_value = (int)__HAL_HWPE_REG_READ(hwpe_base_addr, HWPE_ACQUIRE_OFFSET);
  return (int)read_value;
}

uint32_t hwpe_finished(uint32_t hwpe_base_addr) {
  return (uint32_t)__HAL_HWPE_REG_READ(hwpe_base_addr, HWPE_FINISHED_OFFSET);
}

uint32_t hwpe_task_queue_status(uint32_t hwpe_base_addr) {
  return (uint32_t)__HAL_HWPE_REG_READ(hwpe_base_addr, HWPE_STATUS_OFFSET);
}

uint8_t hwpe_last_task_id(uint32_t hwpe_base_addr) {
  return (uint8_t)__HAL_HWPE_REG_READ(hwpe_base_addr, HWPE_RUNNING_JOB_OFFSET);
}

void hwpe_soft_clear(uint32_t hwpe_base_addr) {
  __HAL_HWPE_REG_WRITE(hwpe_base_addr, HWPE_SOFT_CLEAR_OFFSET, 0);
  // for (volatile int i = 0; i < 10; i++); // ToDo: Is this necessary to ensure the soft clear takes effect before next commands?
}
