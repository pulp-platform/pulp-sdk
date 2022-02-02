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

#ifndef __HAL_IDMA_V1_H__
#define __HAL_IDMA_V1_H__

#include <archi/dma/idma_v1.h>
#include "hal/pulp.h"

#define PLP_DMA_LOC2EXT 0
#define PLP_DMA_EXT2LOC 1

// 2D not yet supported
#define PLP_DMA_1D 0
// #define PLP_DMA_2D 1

#define DMA_MAX_NUM_STREAMS 4

typedef unsigned int dma_ext_t;
typedef unsigned int dma_loc_t;

/** @name High-level DMA memory copy functions
 * The following functions can be used to trigger DMA transfers to copy data between the cluster memory (L1) and another memory outside the cluster (another cluster L1 or L2).
 * Note that they cannot be used from L2 to L2 or from a cluster memory to the same cluster memory. The DMA supports the following features:
 *   - Transfers can be either event-based or irq-based. With event-based transfers the core can call a wait function to block execution until the transfer is done. With irq-based transfers, the completion of the transfer will 
 *     trigger the DMA interrupt. This interrupt cannot be managed with this HAL, but must be managed as any other IRQ.
 *   - The DMA supports 2D transfers which allows transfering a 2D tile in one command. Additional information must then be given to specify the width of the tile and the number of bytes between 2 lines of the tile.
 *   - The event or irq sent at the end of the transfer can be either sent to the core which enqueued the transfer or broadcasted to all cluster cores.
 *   - To identify specific transfers, the DMA provides a counter allocator. All transfers enqueued with the same counter are gathered into a group, on which the core can be stalled until the whole group of transfers is finished.
 *   - All transfers are enqueued into a global FIFO which can stall the core enqueueing the transfer in case it is full. The size of the FIFO depends on the chip but is typically 16 commands.
 */
/**@{*/

/** Memory transfer with event-based completion. 
 * 
  \param   ext     Address in the external memory where to access the data. There is no restriction on memory alignment.
  \param   loc     Address in the cluster memory where to access the data. There is no restriction on memory alignment.
  \param   size    Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \param   ext2loc If 1, the transfer is loading data from external memory and storing to cluster memory. If 0, it is the contrary
  \return          The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int plp_dma_memcpy(dma_ext_t ext, unsigned int loc, unsigned int size, int ext2loc);

/** Cluster memory to external memory transfer with event-based completion. 
 * 
  \param   ext  Address in the external memory where to store the data. There is no restriction on memory alignment.
  \param   loc  Address in the cluster memory where to load the data. There is no restriction on memory alignment.
  \param   size Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \return       The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int plp_dma_l1ToExt(dma_ext_t ext, unsigned int loc, unsigned short size);

/** External memory to cluster memory transfer with event-based completion. 
 * 
  \param   loc  Address in the cluster memory where to store the data. There is no restriction on memory alignment.
  \param   ext  Address in the external memory where to load the data. There is no restriction on memory alignment.
  \param   size Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \return       The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int plp_dma_extToL1(unsigned int loc, dma_ext_t ext, unsigned short size);

/** Memory transfer with irq-based completion. 
 * 
  \param   ext     Address in the external memory where to access the data. There is no restriction on memory alignment.
  \param   loc     Address in the cluster memory where to access the data. There is no restriction on memory alignment.
  \param   size    Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \param   ext2loc If 1, the transfer is loading data from external memory and storing to cluster memory. If 0, it is the contrary
  \return          The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int plp_dma_memcpy_irq(dma_ext_t ext, unsigned int loc, unsigned short size, int ext2loc);

/** Cluster memory to external memory transfer with irq-based completion. 
 * 
  \param   ext  Address in the external memory where to store the data. There is no restriction on memory alignment.
  \param   loc  Address in the cluster memory where to load the data. There is no restriction on memory alignment.
  \param   size Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \return       The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int plp_dma_l1ToExt_irq(dma_ext_t ext, unsigned int loc, unsigned short size);

/** External memory to cluster memory transfer with irq-based completion. 
 * 
  \param   loc  Address in the cluster memory where to store the data. There is no restriction on memory alignment.
  \param   ext  Address in the external memory where to load the data. There is no restriction on memory alignment.
  \param   size Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \return       The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int plp_dma_extToL1_irq(unsigned int loc, dma_ext_t ext, unsigned short size);

//!@}

/** @name DMA wait functions
 */

/** DMA barrier.
 * This blocks the core until no transfer is on-going in the DMA. 
 */
static inline void plp_dma_barrier();

/** DMA wait.
  * This blocks the core until the specified transfer is finished. 
  *
  \param   counter  The counter ID identifying the transfer. This has either been allocated explicitly or returned from an enqueued transfer (e.g. plp_dma_extToL1_2d_irq)
 */
static inline void plp_dma_wait(unsigned int dma_tx_id);

//!@}


/** @name iDMA low-level functions.
  * This can be used instead of the high-level ones in order to have more control over the DMA features. 
 */

/**
 * iDMA configuration generation
 * A standard memcpy will set all of these values to 0.
 *
  \param  decouple   if set to true, there is no longer exactly one AXI write_request issued for
                     every read request. This mode can improve performance of unaligned transfers when crossing
                     the AXI page boundaries.
  \param  deburst    if set, the DMA will split all bursts in single transfers
  \param  serialize  if set, the DMA will only send AX belonging to a given Arbitrary 1D burst request
                     at a time. This is default behavior to prevent deadlocks. Setting `serialize` to
                     zero violates the AXI4+ATOP specification.
  \return            The generated configuration
  */
static inline unsigned int pulp_idma_get_conf(unsigned int decouple, unsigned int deburst, unsigned int serialize);

/**
 * iDMA transfer status
 *
  \param  dma_tx_id  The dma transfer identifier
  \return            transfer status. 1 if complete, 0 if still ongoing or waiting.
  */
static inline unsigned int pulp_idma_tx_cplt(unsigned int dma_tx_id);

/**
 * iDMA memory transfer
 * Launches a standard 1D memory transfer
 *
  \param  dst_addr   The destination address
  \param  src_addr   The source address
  \param  num_bytes  The number bytes
  \return            The dma transfer identifier
  */
static inline unsigned int pulp_idma_memcpy(unsigned int const dst_addr, unsigned int const src_addr, unsigned int num_bytes);

/**
 * iDMA advanced memory transfer
 * Launches a 1D memory transfer with special configuration options
 *
  \param  dst_addr   The destination address
  \param  src_addr   The source address
  \param  num_bytes  The number bytes
  \param  decouple   if set to true, there is no longer exactly one AXI write_request issued for
                     every read request. This mode can improve performance of unaligned transfers when crossing
                     the AXI page boundaries.
  \param  deburst    if set, the DMA will split all bursts in single transfers
  \param  serialize  if set, the DMA will only send AX belonging to a given Arbitrary 1D burst request
                     at a time. This is default behavior to prevent deadlocks. Setting `serialize` to
                     zero violates the AXI4+ATOP specification.
  \return            The dma trasfer identifier
  */
static inline unsigned int pulp_idma_memcpy_advanced(unsigned int const dst_addr, unsigned int const src_addr, unsigned int num_bytes, unsigned int decouple, unsigned int deburst, unsigned int serialize);

/** Return the DMA status.
 * 
  \return             DMA status. 1 means there are still on-going transfers, 0 means nothing is on-going.
  */  
static inline unsigned int plp_dma_status();


//!@}


/// @cond IMPLEM

#if ARCHI_HAS_DMA_DEMUX
#define DMA_ADDR ARCHI_IDMA_DEMUX_ADDR
#else
#define DMA_ADDR ARCHI_IDMA_EXT_ADDR
#endif
#if defined(__riscv__) && !defined(RV_ISA_RV32) && !defined(__LLVM__)
#define DMA_WRITE(value, offset) __builtin_pulp_OffsetedWrite((value), (int *)DMA_ADDR, (offset))
#define DMA_READ(offset) __builtin_pulp_OffsetedRead((int *)DMA_ADDR, (offset))
#else
#define DMA_WRITE(value, offset) pulp_write32(DMA_ADDR + (offset), (value))
#define DMA_READ(offset) pulp_read32(DMA_ADDR + (offset))
#endif

static inline unsigned int pulp_idma_get_conf(unsigned int decouple, unsigned int deburst, unsigned int serialize) {
  unsigned int conf;
#if defined(__riscv__)
  conf = __builtin_bitinsert(0,    decouple,  1, CLUSTER_DMA_FRONTEND_CONF_DECOUPLE_BIT);
  conf = __builtin_bitinsert(conf, deburst,   1, CLUSTER_DMA_FRONTEND_CONF_DEBURST_BIT);
  conf = __builtin_bitinsert(conf, serialize, 1, CLUSTER_DMA_FRONTEND_CONF_SERIALIZE_BIT);
#else
  conf = (((decouple & 0x1)<<CLUSTER_DMA_FRONTEND_CONF_DECOUPLE_BIT) | ((deburst & 0x1)<<CLUSTER_DMA_FRONTEND_CONF_DEBURST_BIT) | ((serialize & 0x1)<<CLUSTER_DMA_FRONTEND_CONF_SERIALIZE_BIT));
#endif
  return conf;
}

static inline unsigned int pulp_idma_tx_cplt(unsigned int dma_tx_id) {
  return (dma_tx_id & 0x0fffffff) <= DMA_READ(CLUSTER_DMA_FRONTEND_DONE_REG_OFFSET + ((dma_tx_id & 0xf0000000) >> 26));
}


static inline unsigned int pulp_idma_memcpy(unsigned int const dst_addr, unsigned int const src_addr, unsigned int num_bytes) {
  DMA_WRITE(src_addr, CLUSTER_DMA_FRONTEND_SRC_ADDR_REG_OFFSET);
  DMA_WRITE(dst_addr, CLUSTER_DMA_FRONTEND_DST_ADDR_REG_OFFSET);
  DMA_WRITE(num_bytes, CLUSTER_DMA_FRONTEND_NUM_BYTES_REG_OFFSET);
  DMA_WRITE(0, CLUSTER_DMA_FRONTEND_CONF_REG_OFFSET);
  asm volatile("" : : : "memory");

  // Launch TX
  unsigned int dma_tx_id = DMA_READ(CLUSTER_DMA_FRONTEND_NEXT_ID_REG_OFFSET);

  return dma_tx_id;
}

static inline unsigned int pulp_idma_memcpy_advanced(unsigned int const dst_addr, unsigned int const src_addr, unsigned int num_bytes, unsigned int decouple, unsigned int deburst, unsigned int serialize) {
  DMA_WRITE(src_addr, CLUSTER_DMA_FRONTEND_SRC_ADDR_REG_OFFSET);
  DMA_WRITE(dst_addr, CLUSTER_DMA_FRONTEND_DST_ADDR_REG_OFFSET);
  DMA_WRITE(num_bytes, CLUSTER_DMA_FRONTEND_NUM_BYTES_REG_OFFSET);
  unsigned int conf = pulp_idma_get_conf(decouple, deburst, serialize);
  DMA_WRITE(conf, CLUSTER_DMA_FRONTEND_CONF_REG_OFFSET);
  asm volatile("" : : : "memory");

  // Launch TX
  unsigned int dma_tx_id = DMA_READ(CLUSTER_DMA_FRONTEND_NEXT_ID_REG_OFFSET);

  return dma_tx_id;
}

static inline unsigned int plp_dma_status() {
  return DMA_READ(CLUSTER_DMA_FRONTEND_STATUS_REG_OFFSET);
}

static inline void plp_dma_wait(unsigned int dma_tx_id) {
  while(!pulp_idma_tx_cplt(dma_tx_id)) {
    eu_evt_maskWaitAndClr(1 << ARCHI_CL_EVT_DMA0);
  }
  return;
}

static inline int plp_dma_memcpy(dma_ext_t ext, unsigned int loc, unsigned int size, int ext2loc) {
  if (ext2loc) {
    return pulp_idma_memcpy(loc, ext, size);
  } else {
    return pulp_idma_memcpy(ext, loc, size);
  }
}

static inline int plp_dma_l1ToExt(dma_ext_t ext, unsigned int loc, unsigned short size) {
  return pulp_idma_memcpy(ext, loc, size);
}

static inline int plp_dma_extToL1(unsigned int loc, dma_ext_t ext, unsigned short size) {
  return pulp_idma_memcpy(loc, ext, size);
}

static inline int plp_dma_memcpy_irq(dma_ext_t ext, unsigned int loc, unsigned short size, int ext2loc) {
  if (ext2loc) {
    return pulp_idma_memcpy(loc, ext, size);
  } else {
    return pulp_idma_memcpy(ext, loc, size);
  }
}

static inline int plp_dma_l1ToExt_irq(dma_ext_t ext, unsigned int loc, unsigned short size) {
  return pulp_idma_memcpy(ext, loc, size);
}

static inline int plp_dma_extToL1_irq(unsigned int loc, dma_ext_t ext, unsigned short size) {
  return pulp_idma_memcpy(loc, ext, size);
}

static inline void plp_dma_barrier() {
  while(plp_dma_status()) {
    eu_evt_maskWaitAndClr(1 << ARCHI_CL_EVT_DMA0);
  }
}

#endif // __HAL_IDMA_V1_H__
