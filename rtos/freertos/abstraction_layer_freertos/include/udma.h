/*
 * Copyright (C) 2019 ETH Zurich, University of Bologna
 * and GreenWaves Technologies
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

#ifndef __UDMA_H__
#define __UDMA_H__

#include <stdint.h>

/** UDMA_CORE_Type Register Layout Typedef */
/* TODO: refactor this really not good code */
typedef struct {
	volatile uint32_t saddr; /**< RX/TX/CMD Channel uDMA transfer address of
				    associated buffer */
	volatile uint32_t size;	 /**< RX/TX/CMD Channel uDMA transfer size of
				    buffer */
	volatile uint32_t cfg;	 /**< RX/TX/CMD Channel uDMA transfer
				    configuration */
	volatile uint32_t initcfg; /**< Not used. */
} udma_channel_t;


typedef enum {
	RX_CHANNEL = 0,
	TX_CHANNEL = 1,
	COMMAND_CHANNEL = 2
} udma_channel_e;


/**< Offset for RX part */
#define UDMA_CHANNEL_RX_OFFSET      0x00

/**< Offset for TX part */
#define UDMA_CHANNEL_TX_OFFSET      0x10

/**< Offset for peripheral specific part */
#define UDMA_CHANNEL_CUSTOM_OFFSET  0x20

/**< RX/TX/CMD Channel uDMA transfer address of associated buffer */
#define UDMA_CHANNEL_SADDR_OFFSET        (0x0)

/**< RX/TX/CMD Channel uDMA transfer size of buffer */
#define UDMA_CHANNEL_SIZE_OFFSET         (0x4)

/**< RX/TX/CMD Channel uDMA transfer configuration */
#define UDMA_CHANNEL_CFG_OFFSET          (0x8)

/**< Not used. */
#define UDMA_CHANNEL_INTCFG_OFFSET       (0xC)


/*! @name RX_SADDR */
/* Configure pointer to memory buffer:
  - Read: value of the pointer until transfer is over. Else returns 0
  - Write: set Address Pointer to memory buffer start address */
#define UDMA_CORE_RX_SADDR_RX_SADDR_MASK  (0x1fffff)
#define UDMA_CORE_RX_SADDR_RX_SADDR_SHIFT (0)

/* Reserved/Not used. */
#define UDMA_CORE_RX_SADDR_RESERVED_0_MASK  (0xffe00000)
#define UDMA_CORE_RX_SADDR_RESERVED_0_SHIFT (21)


/*! @name RX_SIZE */
/* Buffer size in byte. (128kBytes maximum)
  - Read: buffer size left
  - Write: set buffer size */
#define UDMA_CORE_RX_SIZE_RX_SIZE_MASK	(0xfffff)
#define UDMA_CORE_RX_SIZE_RX_SIZE_SHIFT (0)

/* Reserved/Not used. */
#define UDMA_CORE_RX_SIZE_RESERVED_0_MASK  (0xfff00000)
#define UDMA_CORE_RX_SIZE_RESERVED_0_SHIFT (20)


/*! @name RX_CFG */
/* Channel continuous mode:
  - 1'b0: disable
  - 1'b1: enable
  At the end of the buffer the uDMA reloads the address and size and starts a
  new transfer. */
#define UDMA_CORE_RX_CFG_CONTINOUS_MASK	 (0x1)
#define UDMA_CORE_RX_CFG_CONTINOUS_SHIFT (0)

/* Channel transfer size used to increment uDMA buffer address pointer:
  - 2'b00: +1 (8 bits)
  - 2'b01: +2 (16 bits)
  - 2'b10: +4 (32 bits)
  - 2'b11: +0 */
#define UDMA_CORE_RX_CFG_DATASIZE_MASK	(0x6)
#define UDMA_CORE_RX_CFG_DATASIZE_SHIFT (1)

/* Reserved/Not used. */
#define UDMA_CORE_RX_CFG_RESERVED_0_MASK  (0x8)
#define UDMA_CORE_RX_CFG_RESERVED_0_SHIFT (3)

/* Channel enable and start transfer:
  - 1'b0: disable
  - 1'b1: enable
  This signal is used also to queue a transfer if one is already ongoing. */
#define UDMA_CORE_RX_CFG_EN_MASK  (0x10)
#define UDMA_CORE_RX_CFG_EN_SHIFT (4)

/* Transfer pending in queue status flag:
  - 1'b0: no pending transfer in the queue
  - 1'b1: pending transfer in the queue */
#define UDMA_CORE_RX_CFG_PENDING_MASK  (0x20)
#define UDMA_CORE_RX_CFG_PENDING_SHIFT (5)

/* Channel clear and stop transfer:
  - 1'b0: disable
  - 1'b1: stop and clear the on-going transfer */
#define UDMA_CORE_RX_CFG_CLR_MASK  (0x40)
#define UDMA_CORE_RX_CFG_CLR_SHIFT (6)

/* Reserved/Not used. */
#define UDMA_CORE_RX_CFG_RESERVED_1_MASK  (0xffffff80)
#define UDMA_CORE_RX_CFG_RESERVED_1_SHIFT (7)


/*! @name RX_INITCFG */
/* Reserved/Not used. */
#define UDMA_CORE_RX_INITCFG_RESERVED_0_MASK  (0xffffffff)
#define UDMA_CORE_RX_INITCFG_RESERVED_0_SHIFT (0)


/*! @name TX_SADDR */
/* Configure pointer to memory buffer:
  - Read: value of the pointer until transfer is over. Else returns 0
  - Write: set Address Pointer to memory buffer start address */
#define UDMA_CORE_TX_SADDR_TX_SADDR_MASK  (0x1fffff)
#define UDMA_CORE_TX_SADDR_TX_SADDR_SHIFT (0)

/* Reserved/Not used. */
#define UDMA_CORE_TX_SADDR_RESERVED_0_MASK  (0xffe00000)
#define UDMA_CORE_TX_SADDR_RESERVED_0_SHIFT (21)


/*! @name TX_SIZE */
/* Buffer size in byte. (128kBytes maximum)
  - Read: buffer size left
  - Write: set buffer size */
#define UDMA_CORE_TX_SIZE_TX_SIZE_MASK	(0xfffff)
#define UDMA_CORE_TX_SIZE_TX_SIZE_SHIFT (0)

/* Reserved/Not used. */
#define UDMA_CORE_TX_SIZE_RESERVED_0_MASK  (0xfff00000)
#define UDMA_CORE_TX_SIZE_RESERVED_0_SHIFT (20)


/*! @name TX_CFG */
/* Channel continuous mode:
  - 1'b0: disable
  - 1'b1: enable
  At the end of the buffer the uDMA reloads the address and size and starts a
  new transfer. */
#define UDMA_CORE_TX_CFG_CONTINOUS_MASK	 (0x1)
#define UDMA_CORE_TX_CFG_CONTINOUS_SHIFT (0)

/* Channel transfer size used to increment uDMA buffer address pointer:
  - 2'b00: +1 (8 bits)
  - 2'b01: +2 (16 bits)
  - 2'b10: +4 (32 bits)
  - 2'b11: +0 */
#define UDMA_CORE_TX_CFG_DATASIZE_MASK	(0x6)
#define UDMA_CORE_TX_CFG_DATASIZE_SHIFT (1)

/* Reserved/Not used. */
#define UDMA_CORE_TX_CFG_RESERVED_0_MASK  (0x8)
#define UDMA_CORE_TX_CFG_RESERVED_0_SHIFT (3)

/* Channel enable and start transfer:
  - 1'b0: disable
  - 1'b1: enable
  This signal is used also to queue a transfer if one is already ongoing. */
#define UDMA_CORE_TX_CFG_EN_MASK  (0x10)
#define UDMA_CORE_TX_CFG_EN_SHIFT (4)

/* Transfer pending in queue status flag:
  - 1'b0: no pending transfer in the queue
  - 1'b1: pending transfer in the queue */
#define UDMA_CORE_TX_CFG_PENDING_MASK  (0x20)
#define UDMA_CORE_TX_CFG_PENDING_SHIFT (5)

/* Channel clear and stop transfer:
  - 1'b0: disable
  - 1'b1: stop and clear the on-going transfer */
#define UDMA_CORE_TX_CFG_CLR_MASK  (0x40)
#define UDMA_CORE_TX_CFG_CLR_SHIFT (6)

/* Reserved/Not used. */
#define UDMA_CORE_TX_CFG_RESERVED_1_MASK  (0xffffff80)
#define UDMA_CORE_TX_CFG_RESERVED_1_SHIFT (7)

/*! @name TX_INITCFG */
/* Reserved/Not used. */
#define UDMA_CORE_TX_INITCFG_RESERVED_0_MASK  (0xffffffff)
#define UDMA_CORE_TX_INITCFG_RESERVED_0_SHIFT (0)


#define UDMA_CORE_CFG_DATASIZE_8  (0x0)
#define UDMA_CORE_CFG_DATASIZE_16 (0x1)
#define UDMA_CORE_CFG_DATASIZE_32 (0x2)

/* TODO: remove this legacy glue code */
#define UDMA_CORE_TX_CFG_EN(val)                                               \
	(((uint32_t)(((uint32_t)(val)) << UDMA_CORE_TX_CFG_EN_SHIFT)) &        \
	 UDMA_CORE_TX_CFG_EN_MASK)
#define UDMA_CORE_TX_CFG_DATASIZE(val)                                         \
	(((uint32_t)(((uint32_t)(val)) << UDMA_CORE_TX_CFG_DATASIZE_SHIFT)) &  \
	 UDMA_CORE_TX_CFG_DATASIZE_MASK)
#define UDMA_CORE_RX_CFG_EN(val)                                               \
	(((uint32_t)(((uint32_t)(val)) << UDMA_CORE_RX_CFG_EN_SHIFT)) &        \
	 UDMA_CORE_RX_CFG_EN_MASK)

#endif /* __UDMA_H__ */
