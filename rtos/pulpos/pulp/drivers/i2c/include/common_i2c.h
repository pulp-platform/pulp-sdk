/*
 * Copyright 2020 GreenWaves Technologies
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
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/**-----------------------------------------------------------------------------------------------------------------------
 * ?                                                     ABOUT
 * @author         :  Orlando Nico, GreenWaves Technologies, Robert Balas
 * @email          :  nico.orlando@studio.unibo.it, balasr@iis.ee.ethz.ch
 * @repo           :  freertos/drivers or pulp-sdk/rtos/pulpos/pulp/drivers/i2c/include
 * @createdOn      :  /01/2022
 * @description    :  The driver was tested on a VIP flash memory in RTL, where it was done one
 *                    transfer at a time.
 *                    Multiple concurrent transfers have not been tested. I mean using multiple
 *                    I2C interfaces that do transfers at the same time.
 *-----------------------------------------------------------------------------------------------------------------------**/

/**================================================================================================
 **                                         INCLUDE
 *================================================================================================**/
#ifdef USE_PULPOS_TEST
#include "pmsis.h"
#include <soc.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#endif


#ifdef USE_FREERTOS_TEST
#include "pmsis_types.h"
#include "pmsis_task.h"
#include "implementation_specific_defines.h"
#include "i2c.h"
#include "udma.h"
#include "udma_i2c.h"
#include "pi_errno.h"
#include "udma.h"
#include "pmsis_task.h"
#include "fc_event.h"
#include "freq.h"
#include "debug.h"
#endif

/**================================================================================================
 **                                         DEFINE
 *================================================================================================**/
#ifdef USE_PULPOS_TEST
//function to enable or disable prints
#if defined(TRACE_I2C)
#define I2C_TRACE            printf
#define I2C_TRACE_ERR         printf
#else
#define I2C_TRACE(...)             ((void) 0)
#define I2C_TRACE_ERR(...)         ((void) 0)
#endif  /* TRACE_I2C */
#endif

/* Length of i2c cmd buffer. */
#define __PI_I2C_CMD_BUFF_SIZE (256)
/* Lenght of i2c stop command sequence. */
#define __PI_I2C_STOP_CMD_SIZE (3)
/* Lenght of i2c eot subset of stop command sequence. */
#define __PI_I2C_ONLY_EOT_CMD_SIZE (3)

/* Defines for read & write adress access. */
#define ADDRESS_WRITE 0x0
#define ADDRESS_READ  0x1

/* Max length of a i2c request/data buffer. */
#define MAX_SIZE (0xFF)

/**================================================================================================
 **                                         STRUCT
 *================================================================================================**/
#ifdef USE_PULPOS_TEST
typedef enum
{
	RX_CHANNEL = 0,
	TX_CHANNEL = 1,
	COMMAND_CHANNEL = 2
} udma_channel_e;
#endif

struct i2c_pending_transfer_s {
	uint32_t pending_buffer;
	uint32_t pending_size;
	uint32_t pending_repeat;
	uint32_t pending_repeat_size;
	pi_i2c_xfer_flags_e flags;
	int8_t device_id;
	udma_channel_e channel;
};

struct i2c_cs_data_s {
	uint8_t device_id;	    /*!< I2C interface ID. */
	uint8_t cs;		    /*!< Chip select i2c device. */
	uint16_t clk_div;	    /*!< Clock divider for the selected i2c chip. */
	uint32_t max_baudrate;	    /*!< Max baudrate for the selected i2c chip. */
	struct i2c_cs_data_s *next; /*!< Pointer to next i2c cs data struct. */
};

struct i2c_itf_data_s {
	/* Best to use only one queue since both RX & TX can be used at the same time. */
	struct pi_task *buf[2];			/*!< RX + TX */
	struct pi_task *fifo_head;		/*!< Head of SW fifo waiting transfers. */
	struct pi_task *fifo_tail;		/*!< Tail of SW fifo waiting transfers. */
	struct i2c_pending_transfer_s *pending; /*!< RX + TX. */
	uint32_t nb_open;			/*!< Number of devices opened. */
	uint32_t i2c_cmd_index;			/*!< Number of commands in i2c_cmd_seq. */
	/* pi_freq_cb_t i2c_freq_cb;		/\*!< Callback associated to frequency changes. *\/
	 */
	struct i2c_cs_data_s *cs_list;				   /*!< List of i2c associated to this itf. */
	uint32_t i2c_cmd_seq[__PI_I2C_CMD_BUFF_SIZE];  /*!< Command sequence. */
	uint8_t i2c_stop_send;						   /*!< Set if a stop cmd seq should be sent. */
	uint32_t i2c_stop_seq[__PI_I2C_STOP_CMD_SIZE]; /*!< Command STOP sequence. */
	uint8_t i2c_eot_send;						   /*!< Set if a eot cmd seq should be sent. */
	uint32_t *i2c_only_eot_seq;					   /*!< Only EOT sequence part of of STOP sequence */
	uint8_t device_id;	   /*!< I2C interface ID. */
	/* This variable is used to count number of events received to handle EoT sequence. */
	uint8_t nb_events; /*!< Number of events received. */
#ifdef USE_PULPOS_TEST
	pos_udma_channel_t *rx_channel; 
	pos_udma_channel_t *tx_channel;
#endif
};


/**================================================================================================
 **                                         PROTOTYPE FUNCTION
 *================================================================================================**/
/* function that deactive interrupts for freertos or pulpos */
uint32_t deactive_irq_i2c(void);
/* function that triggers interrupts for freertos or pulpos */
void active_irq_i2c(uint32_t irq);
/* Handle a pending transfer after end of previous part of transfer. */
void __pi_i2c_handle_pending_transfer(struct i2c_itf_data_s *driver_data);
/* Check if a HW UDMA slot is free. */
int32_t __pi_i2c_cb_buf_empty(struct i2c_itf_data_s *driver_data);
/* Enqueue a new task for callback. Currently, there is only a single slot */
void __pi_i2c_cb_buf_enqueue(struct i2c_itf_data_s *driver_data, struct pi_task *task);
/* Pop a task from callback buffer . */
struct pi_task *__pi_i2c_cb_buf_pop(struct i2c_itf_data_s *driver_data);
/* Create a new callabck struct with transfer info then enqueue it in SW fifo. */
void __pi_i2c_task_fifo_enqueue(struct i2c_itf_data_s *driver_data, struct pi_task *task);
/* Pop a callback struct containing a new transfer from SW fifo. */
struct pi_task *__pi_i2c_task_fifo_pop(struct i2c_itf_data_s *driver_data);
/* Clock divider. */
uint32_t __pi_i2c_clk_div_get(uint32_t baudrate);
/* Add a cs_data to list of opened devices. */
void __pi_i2c_cs_data_add(struct i2c_itf_data_s *driver_data, struct i2c_cs_data_s *cs_data);
/* Remove a cs_data from list of opened devices. */
void __pi_i2c_cs_data_remove(struct i2c_itf_data_s *driver_data, struct i2c_cs_data_s *cs_data);
/* Callback to execute when frequency changes. */
void __pi_i2c_freq_cb(void *args);
/* Ioctl function. */
void __pi_i2c_ioctl(struct i2c_cs_data_s *device_data, uint32_t cmd, void *arg);
int32_t __pi_i2c_baudrate_set(struct i2c_cs_data_s *cs_data, uint32_t new_baudrate);
/* Init i2c conf struct. */
void __pi_i2c_conf_init(pi_i2c_conf_t *conf);
void __pi_i2c_wait_transfer(uint32_t device_id);

