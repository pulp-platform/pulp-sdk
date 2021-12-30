/*
 * Copyright (C) 2021 ETH Zurich, University of Bologna
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

#ifndef __I2C_PERIPH_H__
#define __I2C_PERIPH_H__


/* ----------------------------------------------------------------------------
   -- I2C Peripheral Access Layer --
   ---------------------------------------------------------------------------- */

/** I2C_Type Register Layout Typedef */
typedef struct i2c {
	udma_channel_t rx; /**< UDMA RX channel struct. */
	udma_channel_t tx; /**< UDMA TX channel struct. */
	/* this is a i2c v3 feature, but we have a v2 driver */
	/* udma_channel_t cmd; /\**< UDMA CMD channel struct. *\/ */
	volatile uint32_t status; /**< Status register. */
	volatile uint32_t setup;  /**< Configuration register. */
	volatile uint32_t ack;  /**< Nack register (optional). */
} i2c_t;


/* ----------------------------------------------------------------------------
   -- I2C Register Bitfield Access --
   ---------------------------------------------------------------------------- */

/*! @name STATUS */
/* I2C bus busy status flag:
  - 1'b0: no transfer on-going
  - 1'b1: transfer on-going */
#define I2C_STATUS_BUSY_MASK  (0x1)
#define I2C_STATUS_BUSY_SHIFT (0)
#define I2C_STATUS_BUSY(val)                                                                       \
	(((uint32_t)(((uint32_t)(val)) << I2C_STATUS_BUSY_SHIFT)) & I2C_STATUS_BUSY_MASK)

/* I2C arbitration lost status flag:
  - 1'b0: no error
  - 1'b1: arbitration lost error */
#define I2C_STATUS_ARB_LOST_MASK  (0x2)
#define I2C_STATUS_ARB_LOST_SHIFT (1)
#define I2C_STATUS_ARB_LOST(val)                                                                   \
	(((uint32_t)(((uint32_t)(val)) << I2C_STATUS_ARB_LOST_SHIFT)) & I2C_STATUS_ARB_LOST_MASK)


/*! @name SETUP */
/* Reset command used to abort the on-going transfer and clear busy and arbitration lost status
 * flags. */
#define I2C_SETUP_DO_RST_MASK  (0x1)
#define I2C_SETUP_DO_RST_SHIFT (0)
#define I2C_SETUP_DO_RST(val)                                                                      \
	(((uint32_t)(((uint32_t)(val)) << I2C_SETUP_DO_RST_SHIFT)) & I2C_SETUP_DO_RST_MASK)


#ifdef CONFIG_UDMA_I2C_ACK
/*! @name NACK */
/* Read out ACK/NACK status of a the i2c slave */
#define I2C_ACK_NACK_MASK  (0x1)
#define I2C_ACK_NACK_SHIFT (0)
#define I2C_ACK_NACK(val)                                                                      \
	(((uint32_t)(((uint32_t)(val)) << I2C_ACK_NACK_SHIFT)) & I2C_ACK_NACK_MASK)

#endif

/* ----------------------------------------------------------------------------
   -- CMD IDs and macros --
   ---------------------------------------------------------------------------- */
#define I2C_CMD_MASK  (0xF0U)
#define I2C_CMD_SHIFT (4U)
#define I2C_CMD_OFFSET	4

#define I2C_CMD_START	(((uint32_t)(((uint32_t)(0x0)) << I2C_CMD_SHIFT)) & I2C_CMD_MASK) // 0x00
#define I2C_CMD_WAIT_EV (((uint32_t)(((uint32_t)(0x1)) << I2C_CMD_SHIFT)) & I2C_CMD_MASK) // 0x10
#define I2C_CMD_STOP	(((uint32_t)(((uint32_t)(0x2)) << I2C_CMD_SHIFT)) & I2C_CMD_MASK) // 0x20
#define I2C_CMD_RD_ACK	(((uint32_t)(((uint32_t)(0x4)) << I2C_CMD_SHIFT)) & I2C_CMD_MASK) // 0x40
#define I2C_CMD_RD_NACK (((uint32_t)(((uint32_t)(0x6)) << I2C_CMD_SHIFT)) & I2C_CMD_MASK) // 0x60
#define I2C_CMD_WR	(((uint32_t)(((uint32_t)(0x8)) << I2C_CMD_SHIFT)) & I2C_CMD_MASK) // 0x80
#define I2C_CMD_WAIT	(((uint32_t)(((uint32_t)(0xA)) << I2C_CMD_SHIFT)) & I2C_CMD_MASK) // 0xA0
#define I2C_CMD_RPT	(((uint32_t)(((uint32_t)(0xC)) << I2C_CMD_SHIFT)) & I2C_CMD_MASK) // 0xC0
#define I2C_CMD_CFG	(((uint32_t)(((uint32_t)(0xE)) << I2C_CMD_SHIFT)) & I2C_CMD_MASK) // 0xE0

#endif /* __I2C_PERIPH_H__ */
