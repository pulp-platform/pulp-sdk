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

#include "pmsis.h"
#include "cdn_log.h"

void CPS_WriteReg32(volatile uint32_t* address, uint32_t value) {
  pulp_write32(address, value);
}

uint32_t CPS_ReadReg32(volatile uint32_t* address) {
  return pulp_read32(address);
}

void CPS_UncachedWrite32(volatile uint32_t* address, uint32_t value) {
  pulp_write32(address, value);
  __asm__ __volatile__ ("" : : : "memory");
}

uint32_t CPS_UncachedRead32(volatile uint32_t* address) {
  return pulp_read32(address);
}

void assert(int expression) {
  if (expression) {
    DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Assertion failure! expr = %d\n", expression);
  }
}

