/*
 * Copyright (C) 2024 ETH Zurich and University of Bologna
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

#ifndef __HAL_HWPE_HCI_ECC_H__
#define __HAL_HWPE_HCI_ECC_H__

#include "archi/hwpe_hci_ecc/hwpe_hci_ecc.h"
#include "archi/pulp.h"

// Get the number of correctable errors on data detected by the ECC decoders on the interconnect
static inline unsigned int hwpe_hci_ecc_get_data_correctable_count(unsigned int cid) {
  return pulp_read32(ARCHI_HWPE_HCI_ECC_GLOBAL_ADDR(cid) + HCI_ECC_MANAGER_DATA_CORRECTABLE_ERRORS_REG_OFFSET);
}

// Get the number of uncorrectable errors on data detected by the ECC decoders on the interconnect
static inline unsigned int hwpe_hci_ecc_get_data_uncorrectable_count(unsigned int cid) {
  return pulp_read32(ARCHI_HWPE_HCI_ECC_GLOBAL_ADDR(cid) + HCI_ECC_MANAGER_DATA_UNCORRECTABLE_ERRORS_REG_OFFSET);
}

// Get the number of correctable errors on metadata detected by the ECC decoders on the interconnect
static inline unsigned int hwpe_hci_ecc_get_meta_correctable_count(unsigned int cid) {
  return pulp_read32(ARCHI_HWPE_HCI_ECC_GLOBAL_ADDR(cid) + HCI_ECC_MANAGER_METADATA_CORRECTABLE_ERRORS_REG_OFFSET);
}

// Get the number of uncorrectable errors on metadata detected by the ECC decoders on the interconnect
static inline unsigned int hwpe_hci_ecc_get_meta_uncorrectable_count(unsigned int cid) {
  return pulp_read32(ARCHI_HWPE_HCI_ECC_GLOBAL_ADDR(cid) + HCI_ECC_MANAGER_METADATA_UNCORRECTABLE_ERRORS_REG_OFFSET);
}

#endif // __HAL_HWPE_HCI_ECC_H__
