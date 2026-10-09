// Generated register defines for idma_reg32_3d

// Copyright information found in source file:
// Copyright 2023 ETH Zurich and University of Bologna.

// Licensing information found in source file:
// 
// SPDX-License-Identifier: SHL-0.51

#ifndef __INCLUDE_ARCHI_IDMA_V2_H__
#define __INCLUDE_ARCHI_IDMA_V2_H__

#ifndef LANGUAGE_ASSEMBLY

#include <stdint.h>
#include "archi/utils.h"

#endif

#ifdef __cplusplus
extern "C" {
#endif
// Number of dimensions available
#define IDMA_REG32_3D_PARAM_NUM_DIMS 3

// Register width
#define IDMA_REG32_3D_PARAM_REG_WIDTH 32

// Configuration Register for DMA settings
#define IDMA_REG32_3D_CONF_REG_OFFSET 0x0
#define IDMA_REG32_3D_CONF_DECOUPLE_AW_BIT 0
#define IDMA_REG32_3D_CONF_DECOUPLE_RW_BIT 1
#define IDMA_REG32_3D_CONF_SRC_REDUCE_LEN_BIT 2
#define IDMA_REG32_3D_CONF_DST_REDUCE_LEN_BIT 3
#define IDMA_REG32_3D_CONF_SRC_MAX_LLEN_MASK 0x7
#define IDMA_REG32_3D_CONF_SRC_MAX_LLEN_OFFSET 4
#define IDMA_REG32_3D_CONF_SRC_MAX_LLEN_FIELD \
  ((bitfield_field32_t) { .mask = IDMA_REG32_3D_CONF_SRC_MAX_LLEN_MASK, .index = IDMA_REG32_3D_CONF_SRC_MAX_LLEN_OFFSET })
#define IDMA_REG32_3D_CONF_DST_MAX_LLEN_MASK 0x7
#define IDMA_REG32_3D_CONF_DST_MAX_LLEN_OFFSET 7
#define IDMA_REG32_3D_CONF_DST_MAX_LLEN_FIELD \
  ((bitfield_field32_t) { .mask = IDMA_REG32_3D_CONF_DST_MAX_LLEN_MASK, .index = IDMA_REG32_3D_CONF_DST_MAX_LLEN_OFFSET })
#define IDMA_REG32_3D_CONF_ENABLE_ND_MASK 0x3
#define IDMA_REG32_3D_CONF_ENABLE_ND_OFFSET 10
#define IDMA_REG32_3D_CONF_ENABLE_ND_FIELD \
  ((bitfield_field32_t) { .mask = IDMA_REG32_3D_CONF_ENABLE_ND_MASK, .index = IDMA_REG32_3D_CONF_ENABLE_ND_OFFSET })
#define IDMA_REG32_3D_CONF_SRC_PROTOCOL_MASK 0x7
#define IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET 12
#define IDMA_REG32_3D_CONF_SRC_PROTOCOL_FIELD \
  ((bitfield_field32_t) { .mask = IDMA_REG32_3D_CONF_SRC_PROTOCOL_MASK, .index = IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET })
#define IDMA_REG32_3D_CONF_DST_PROTOCOL_MASK 0x7
#define IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET 15
#define IDMA_REG32_3D_CONF_DST_PROTOCOL_FIELD \
  ((bitfield_field32_t) { .mask = IDMA_REG32_3D_CONF_DST_PROTOCOL_MASK, .index = IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET })

// DMA Status
#define IDMA_REG32_3D_STATUS_0_REG_OFFSET 0x4
#define IDMA_REG32_3D_STATUS_0_BUSY_0_MASK 0x3ff
#define IDMA_REG32_3D_STATUS_0_BUSY_0_OFFSET 0
#define IDMA_REG32_3D_STATUS_0_BUSY_0_FIELD \
  ((bitfield_field32_t) { .mask = IDMA_REG32_3D_STATUS_0_BUSY_0_MASK, .index = IDMA_REG32_3D_STATUS_0_BUSY_0_OFFSET })

// DMA Status
#define IDMA_REG32_3D_STATUS_1_REG_OFFSET 0x8
#define IDMA_REG32_3D_STATUS_1_BUSY_1_MASK 0x3ff
#define IDMA_REG32_3D_STATUS_1_BUSY_1_OFFSET 0
#define IDMA_REG32_3D_STATUS_1_BUSY_1_FIELD \
  ((bitfield_field32_t) { .mask = IDMA_REG32_3D_STATUS_1_BUSY_1_MASK, .index = IDMA_REG32_3D_STATUS_1_BUSY_1_OFFSET })

// Next ID, launches transfer, returns 0 if transfer not set up properly.
#define IDMA_REG32_3D_NEXT_ID_0_REG_OFFSET 0xc

// Next ID, launches transfer, returns 0 if transfer not set up properly.
#define IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET 0x10

// Get ID of finished transactions.
#define IDMA_REG32_3D_DONE_ID_0_REG_OFFSET 0x14

// Get ID of finished transactions.
#define IDMA_REG32_3D_DONE_ID_1_REG_OFFSET 0x18

// Low destination address
#define IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET 0xd0

// Low source address
#define IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET 0xd8

// Low transfer length in byte
#define IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET 0xe0

// Low destination stride dimension 2
#define IDMA_REG32_3D_DST_STRIDE_2_LOW_REG_OFFSET 0xe8

// Low source stride dimension 2
#define IDMA_REG32_3D_SRC_STRIDE_2_LOW_REG_OFFSET 0xf0

// Low number of repetitions dimension 2
#define IDMA_REG32_3D_REPS_2_LOW_REG_OFFSET 0xf8

// Low destination stride dimension 3
#define IDMA_REG32_3D_DST_STRIDE_3_LOW_REG_OFFSET 0x100

// Low source stride dimension 3
#define IDMA_REG32_3D_SRC_STRIDE_3_LOW_REG_OFFSET 0x108

// Low number of repetitions dimension 3
#define IDMA_REG32_3D_REPS_3_LOW_REG_OFFSET 0x110

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _IDMA_REG32_3D_REG_DEFS_
// End generated register defines for idma_reg32_3d