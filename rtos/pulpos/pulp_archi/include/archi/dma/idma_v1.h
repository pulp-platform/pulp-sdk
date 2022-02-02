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

#ifndef __ARCHI_IDMA_V1_H__
#define __ARCHI_IDMA_V1_H__

// Generated register defines for pulpopen_idma

#ifndef _PULPOPEN_IDMA_REG_DEFS_
#define _PULPOPEN_IDMA_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif
// Register width
#define PULPOPEN_IDMA_PARAM_REG_WIDTH 32

// Source Address
#define PULPOPEN_IDMA_SRC_ADDR_REG_OFFSET 0x0
#define PULPOPEN_IDMA_SRC_ADDR_REG_RESVAL 0x0

// Destination Address
#define PULPOPEN_IDMA_DST_ADDR_REG_OFFSET 0x4
#define PULPOPEN_IDMA_DST_ADDR_REG_RESVAL 0x0

// Number of bytes
#define PULPOPEN_IDMA_NUM_BYTES_REG_OFFSET 0x8
#define PULPOPEN_IDMA_NUM_BYTES_REG_RESVAL 0x0

// Configuration Register for DMA settings
#define PULPOPEN_IDMA_CONF_REG_OFFSET 0xc
#define PULPOPEN_IDMA_CONF_REG_RESVAL 0x0
#define PULPOPEN_IDMA_CONF_DECOUPLE_BIT 0
#define PULPOPEN_IDMA_CONF_DEBURST_BIT 1
#define PULPOPEN_IDMA_CONF_SERIALIZE_BIT 2
#define PULPOPEN_IDMA_CONF_TWOD_BIT 3

// Source Stride
#define PULPOPEN_IDMA_STRIDE_SRC_REG_OFFSET 0x10
#define PULPOPEN_IDMA_STRIDE_SRC_REG_RESVAL 0x0

// Destination Stride
#define PULPOPEN_IDMA_STRIDE_DST_REG_OFFSET 0x14
#define PULPOPEN_IDMA_STRIDE_DST_REG_RESVAL 0x0

// Number of 2D repetitions
#define PULPOPEN_IDMA_NUM_REPETITIONS_REG_OFFSET 0x18
#define PULPOPEN_IDMA_NUM_REPETITIONS_REG_RESVAL 0x0

// DMA Status
#define PULPOPEN_IDMA_STATUS_REG_OFFSET 0x1c
#define PULPOPEN_IDMA_STATUS_REG_RESVAL 0x0
#define PULPOPEN_IDMA_STATUS_BUSY_BIT 0

// Next ID, launches transfer, returns 0 if transfer not set up properly.
#define PULPOPEN_IDMA_NEXT_ID_REG_OFFSET 0x20
#define PULPOPEN_IDMA_NEXT_ID_REG_RESVAL 0x0

// Get ID of finished transactions.
#define PULPOPEN_IDMA_DONE_REG_OFFSET 0x24
#define PULPOPEN_IDMA_DONE_REG_RESVAL 0x0

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _PULPOPEN_IDMA_REG_DEFS_
// End generated register defines for pulpopen_idma

#endif // __ARCHI_IDMA_V1_H__
