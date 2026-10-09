/*
 * Copyright (C) 2023 ETH Zurich and University of Bologna
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

#ifndef __HAL_TCDM_SCRUBBER_H__
#define __HAL_TCDM_SCRUBBER_H__

#include "archi/tcdm_scrubber/tcdm_scrubber.h"
#include "archi/pulp.h"

// Get the number of correctable mismatches detected by the ECC decoders during access
static inline unsigned int tcdm_scrubber_get_mismatch_count(unsigned int cid, unsigned int bank_id) {
  return pulp_read32(ARCHI_TCDM_SCRUBBER_GLOBAL_ADDR(cid) + ECC_MANAGER_MISMATCH_COUNT_REG_OFFSET + bank_id*REGISTER_OFFSET);
}

// Set the interval between each access scrubber access to memory banks
static inline void tcdm_scrubber_set_interval(unsigned int cid, unsigned int bank_id, unsigned int value) {
  pulp_write32(ARCHI_TCDM_SCRUBBER_GLOBAL_ADDR(cid) + ECC_MANAGER_SCRUB_INTERVAL_REG_OFFSET + bank_id*REGISTER_OFFSET, value);
}

// Get the number of correctable mismatches detected by the ECC decoders while scrubbing
static inline unsigned int tcdm_scrubber_get_fix_count(unsigned int cid, unsigned int bank_id) {
  return pulp_read32(ARCHI_TCDM_SCRUBBER_GLOBAL_ADDR(cid) + ECC_MANAGER_SCRUB_FIX_COUNT_REG_OFFSET + bank_id*REGISTER_OFFSET);
}

// Set the number of uncorrectable mismatches detected by the ECC decoders while scrubbing
static inline unsigned int tcdm_scrubber_get_uncorrectable_count(unsigned int cid, unsigned int bank_id) {
  return pulp_read32(ARCHI_TCDM_SCRUBBER_GLOBAL_ADDR(cid) + ECC_MANAGER_SCRUB_UNCORRECTABLE_COUNT_REG_OFFSET + bank_id*REGISTER_OFFSET);
}

#endif // __HAL_TCDM_SCRUBBER_H__
