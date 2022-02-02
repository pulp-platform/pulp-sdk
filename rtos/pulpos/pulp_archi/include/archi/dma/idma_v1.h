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

#if IDMA_VERSION == 1

// Generated register defines for cluster_dma_frontend

#ifndef _CLUSTER_DMA_FRONTEND_REG_DEFS_
#define _CLUSTER_DMA_FRONTEND_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif
// Register width
#define CLUSTER_DMA_FRONTEND_PARAM_REG_WIDTH 32

// Source Address
#define CLUSTER_DMA_FRONTEND_SRC_ADDR_REG_OFFSET 0x0
#define CLUSTER_DMA_FRONTEND_SRC_ADDR_REG_RESVAL 0x0

// Destination Address
#define CLUSTER_DMA_FRONTEND_DST_ADDR_REG_OFFSET 0x4
#define CLUSTER_DMA_FRONTEND_DST_ADDR_REG_RESVAL 0x0

// Number of bytes
#define CLUSTER_DMA_FRONTEND_NUM_BYTES_REG_OFFSET 0x8
#define CLUSTER_DMA_FRONTEND_NUM_BYTES_REG_RESVAL 0x0

// Configuration Register for DMA settings
#define CLUSTER_DMA_FRONTEND_CONF_REG_OFFSET 0xc
#define CLUSTER_DMA_FRONTEND_CONF_REG_RESVAL 0x0
#define CLUSTER_DMA_FRONTEND_CONF_DECOUPLE_BIT 0
#define CLUSTER_DMA_FRONTEND_CONF_DEBURST_BIT 1
#define CLUSTER_DMA_FRONTEND_CONF_SERIALIZE_BIT 2

// DMA Status
#define CLUSTER_DMA_FRONTEND_STATUS_REG_OFFSET 0x10
#define CLUSTER_DMA_FRONTEND_STATUS_REG_RESVAL 0x0
#define CLUSTER_DMA_FRONTEND_STATUS_BUSY_BIT 0

// Next ID, launches transfer, returns 0 if transfer not set up properly.
#define CLUSTER_DMA_FRONTEND_NEXT_ID_REG_OFFSET 0x14
#define CLUSTER_DMA_FRONTEND_NEXT_ID_REG_RESVAL 0x0

// Get ID of finished transactions.
#define CLUSTER_DMA_FRONTEND_DONE_REG_OFFSET 0x18
#define CLUSTER_DMA_FRONTEND_DONE_REG_RESVAL 0x0

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _CLUSTER_DMA_FRONTEND_REG_DEFS_
// End generated register defines for cluster_dma_frontend

#endif // IDMA_VERSION == 1

#endif // __ARCHI_IDMA_V1_H__
