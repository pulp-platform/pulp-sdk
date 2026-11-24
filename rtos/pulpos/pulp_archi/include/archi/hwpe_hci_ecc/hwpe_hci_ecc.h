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

// Generated register defines for HCI_ECC_manager

#ifndef _HCI_ECC_MANAGER_REG_DEFS_
#define _HCI_ECC_MANAGER_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif
// Register width
#define HCI_ECC_MANAGER_PARAM_REG_WIDTH 32

// Correctable errors on data caught by ecc on interconnect
#define HCI_ECC_MANAGER_DATA_CORRECTABLE_ERRORS_REG_OFFSET 0x0

// Uncorrectable errors on data caught by ecc on interconnect
#define HCI_ECC_MANAGER_DATA_UNCORRECTABLE_ERRORS_REG_OFFSET 0x4

// Correctable errors caught on metadata by ecc on interconnect
#define HCI_ECC_MANAGER_METADATA_CORRECTABLE_ERRORS_REG_OFFSET 0x8

// Uncorrectable errors caught on metadata by ecc on interconnect
#define HCI_ECC_MANAGER_METADATA_UNCORRECTABLE_ERRORS_REG_OFFSET 0xc

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _HCI_ECC_MANAGER_REG_DEFS_
// End generated register defines for HCI_ECC_manager
