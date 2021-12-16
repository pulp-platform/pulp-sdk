/*
 * Copyright 2021 GreenWaves Technologies
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

/**================================================================================================
 * *                                           INFO
 *   pulp-sdk/rtos/pulpos/common/rules/pulpos/src.mk
 *   pulp-sdk/rtos/pmsis/pmsis_bsp/rules/pulpos/src.mk
 *	 pulp-sdk/rtos/pulpos/pulp_archi/include/archi/chips/pulp/properties.h
 *	 pulp-sdk/rtos/pulpos/pulp_archi/include/archi/chips/pulp/memory_map.h   
 *
 *================================================================================================**/

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

/*
 * pi_task:
 * data[0] = l2_buf
 * data[1] = size
 * data[2] = flags
 * data[3] = channel
 * data[4] = p_cs_data
 * data[5] = repeat_size
 */

/**================================================================================================
 **                                         Define
 *================================================================================================**/

/* Length of i2c cmd buffer. */
#define __PI_I2C_CMD_BUFF_SIZE (16)
/* Lenght of i2c stop command sequence. */
#define __PI_I2C_STOP_CMD_SIZE (4)
/* Lenght of i2c eot subset of stop command sequence. */
#define __PI_I2C_ONLY_EOT_CMD_SIZE (3)

#if defined(TRACE_I2C)
#define I2C_TRACE(...) printf
#define I2C_TRACE_ERR(...) printf
#else
#define I2C_TRACE(...) ((void)0)
#define I2C_TRACE_ERR(...) ((void)0)
#endif /* TRACE_I2C */

/* Defines for read & write adress access. */
#define ADDRESS_WRITE 0x0
#define ADDRESS_READ 0x1

/* Max length of a i2c request/data buffer. */
#define MAX_SIZE (0xFF)

#define UDMA_EVENT_OFFSET_RX (0)
#define UDMA_EVENT_OFFSET_TX (1)

#define SOC_EVENT_UDMA_I2C_RX(id)                                          \
	((ARCHI_UDMA_I2C_ID(id) << ARCHI_SOC_EVENT_UDMA_NB_CHANNEL_EVT_LOG2) + \
	 UDMA_EVENT_OFFSET_RX)
#define SOC_EVENT_UDMA_I2C_TX(id)                                          \
	((ARCHI_UDMA_I2C_ID(id) << ARCHI_SOC_EVENT_UDMA_NB_CHANNEL_EVT_LOG2) + \
	 UDMA_EVENT_OFFSET_TX)

/**================================================================================================
 **                                         struct
 *================================================================================================**/

typedef enum
{
	RX_CHANNEL = 0,
	TX_CHANNEL = 1,
	COMMAND_CHANNEL = 2
} udma_channel_e;

struct i2c_pending_transfer_s
{
	uint32_t pending_buffer;
	uint32_t pending_size;
	uint32_t pending_repeat;
	uint32_t pending_repeat_size;
	pi_i2c_xfer_flags_e flags;
	int8_t device_id;
	udma_channel_e channel;
};

struct i2c_cs_data_s
{
	struct i2c_cs_data_s *next; /*!< Pointer to next i2c cs data struct. */
	uint8_t device_id;			/*!< I2C interface ID. */
	uint8_t cs;					/*!< Chip select i2c device. */
	uint16_t clk_div;			/*!< Clock divider for the selected i2c chip. */
	uint32_t max_baudrate;		/*!< Max baudrate for the selected i2c chip. */
	struct i2c_itf_data_s *driver_data;
};

struct i2c_itf_data_s
{
	/* Best to use only one queue since both RX & TX can be used at the same time. */
	struct pi_task *buf[2];					/*!< RX + TX */
	struct pi_task *fifo_head;				/*!< Head of SW fifo waiting transfers. */
	struct pi_task *fifo_tail;				/*!< Tail of SW fifo waiting transfers. */
	struct i2c_pending_transfer_s *pending; /*!< RX + TX. */
	uint32_t nb_open;						/*!< Number of devices opened. */
	uint32_t i2c_cmd_index;					/*!< Number of commands in i2c_cmd_seq. */
	/* pi_freq_cb_t i2c_freq_cb;		/\*!< Callback associated to frequency changes. *\/
	 */
	struct i2c_cs_data_s *cs_list;				  /*!< List of i2c associated to this itf. */
	uint8_t i2c_cmd_seq[__PI_I2C_CMD_BUFF_SIZE];  /*!< Command sequence. */
	uint8_t i2c_stop_send;						  /*!< Set if a stop cmd seq should be sent. */
	uint8_t i2c_stop_seq[__PI_I2C_STOP_CMD_SIZE]; /*!< Command STOP sequence. */
	uint8_t i2c_eot_send;						  /*!< Set if a eot cmd seq should be sent. */
	uint8_t *i2c_only_eot_seq;					  /*!< Only EOT sequence part of of STOP sequence */
	uint8_t device_id;							  /*!< I2C interface ID. */
	/* This variable is used to count number of events received to handle EoT sequence. */
	uint8_t nb_events; /*!< Number of events received. */
	pos_udma_channel_t *rx_channel;
	pos_udma_channel_t *tx_channel;
};

/**================================================================================================
 **                                         PROTOTYPE FUNCTION
 *================================================================================================**/
/* Clock divider. */
static uint32_t __pi_i2c_clk_div_get(uint32_t baudrate);

/* Add a cs_data to list of opened devices. */
static void __pi_i2c_cs_data_add(struct i2c_itf_data_s *driver_data, struct i2c_cs_data_s *cs_data);

/* Remove a cs_data from list of opened devices. */
static void __pi_i2c_cs_data_remove(struct i2c_itf_data_s *driver_data,
									struct i2c_cs_data_s *cs_data);

/* Handle a pending transfer after end of previous part of transfer. */
static void __pi_i2c_handle_pending_transfer(struct i2c_itf_data_s *driver_data);

/* Send a stop command sequence. */
static void __pi_i2c_send_stop_cmd(struct i2c_itf_data_s *driver_data);

/* Send a only eot command sequence. */
static void __pi_i2c_send_only_eot_cmd(struct i2c_itf_data_s *driver_data);

/* Check if a HW UDMA slot is free. */
static int32_t __pi_i2c_cb_buf_empty(struct i2c_itf_data_s *driver_data);

/* Enqueue a new task for callback. Currently, there is only a single slot */
static void __pi_i2c_cb_buf_enqueue(struct i2c_itf_data_s *driver_data, struct pi_task *task);

/* Pop a task from callback buffer . */
static struct pi_task *__pi_i2c_cb_buf_pop(struct i2c_itf_data_s *driver_data);

/* Create a new callabck struct with transfer info then enqueue it in SW fifo. */
static void __pi_i2c_task_fifo_enqueue(struct i2c_itf_data_s *driver_data, struct pi_task *task);

/* Pop a callback struct containing a new transfer from SW fifo. */
static struct pi_task *__pi_i2c_task_fifo_pop(struct i2c_itf_data_s *driver_data);

/* Initiate and enqueue a read command sequence. */
static void __pi_i2c_copy_exec_read(struct i2c_itf_data_s *driver_data, struct pi_task *task);

/* Initiate and enqueue a write command sequence. */
static void __pi_i2c_copy_exec_write(struct i2c_itf_data_s *driver_data, struct pi_task *task);

/* Callback to execute when frequency changes. */
static void __pi_i2c_freq_cb(void *args);

/* Init i2c conf struct. */
void __pi_i2c_conf_init(pi_i2c_conf_t *conf);

/* Open i2c device. */
int32_t __pi_i2c_open(struct pi_i2c_conf *conf, struct i2c_cs_data_s **device_data);

/* Close i2c device. */
void __pi_i2c_close(struct i2c_cs_data_s *device_data);

/* Ioctl function. */
void __pi_i2c_ioctl(struct i2c_cs_data_s *device_data, uint32_t cmd, void *arg);

/* Copy in UDMA. */
void __pi_i2c_copy(struct i2c_cs_data_s *cs_data, uint32_t l2_buff, uint32_t length,
				   pi_i2c_xfer_flags_e flags, udma_channel_e channel, struct pi_task *task);

/* Scan i2c bus to detect connected devices. */
int32_t __pi_i2c_detect(struct i2c_cs_data_s *cs_data, struct pi_i2c_conf *conf, uint8_t *rx_data,
						struct pi_task *task);
void pos_i2c_handle_copy(int event, void *arg);
void pos_i2c_create_channel(pos_udma_channel_t *channel, int channel_id, int soc_event);

/**================================================================================================
 **                                         GLOBAL VARIABLE
 *================================================================================================**/
static struct i2c_itf_data_s *g_i2c_itf_data[ARCHI_UDMA_NB_I2C] = {NULL};
static PI_L2 int i2c_channel;
static PI_L2 pos_udma_channel_t i2c_tx_channel;
static PI_L2 pos_udma_channel_t i2c_rx_channel;

/**================================================================================================
 **                                         FUNCTION
 *================================================================================================**/
void pi_i2c_conf_init(pi_i2c_conf_t *conf)
{
	__pi_i2c_conf_init(conf);
}

void pi_i2c_conf_set_slave_addr(struct pi_i2c_conf *conf, uint16_t slave_addr,
								int8_t is_10_bits)
{
	conf->cs = slave_addr;
	conf->is_10_bits = is_10_bits;
}

void pi_i2c_close(struct pi_device *device)
{
	struct i2c_cs_data_s *device_data = (struct i2c_cs_data_s *)device->data;
	if (device_data != NULL)
	{
		I2C_TRACE("Close device id=%d\n", device_data->device_id);
		__pi_i2c_close(device_data);
		device->data = NULL;
	}
}

void pi_i2c_ioctl(struct pi_device *device, uint32_t cmd, void *arg)
{
	struct i2c_cs_data_s *device_data = (struct i2c_cs_data_s *)device->data;
	if (device_data != NULL)
	{
		I2C_TRACE("Ioctl command : %lx, arg %lx\n", cmd, arg);
		__pi_i2c_ioctl(device_data, cmd, arg);
	}
}

int pi_i2c_read(struct pi_device *device, uint8_t *rx_buff, int length, pi_i2c_xfer_flags_e flags)
{
	int status = PI_OK;
	pi_task_t task_block;
	pi_task_block(&task_block);
	pi_i2c_read_async(device, rx_buff, length, flags, &task_block);
	pi_task_wait_on(&task_block);
	pi_task_destroy(&task_block);
	return status;
}

void pi_i2c_read_async(struct pi_device *device, uint8_t *rx_buff, int length,
					   pi_i2c_xfer_flags_e flags, pi_task_t *task)
{
	struct i2c_cs_data_s *device_data = (struct i2c_cs_data_s *)device->data;
	udma_channel_e channel = RX_CHANNEL;
	I2C_TRACE("I2C(%d) : transfer %d %lx %ld %lx, task %lx\n", device_data->device_id, channel,
			  (uint32_t)rx_buff, length, flags, task);
	__pi_i2c_copy(device_data, (uint32_t)rx_buff, length, flags, channel, task);
}

int pi_i2c_write(struct pi_device *device, uint8_t *tx_data, int length, pi_i2c_xfer_flags_e flags)
{
	int status = PI_OK;
	pi_task_t task_block;
	pi_task_block(&task_block);
	pi_i2c_write_async(device, tx_data, length, flags, &task_block);
	pi_task_wait_on(&task_block);
	pi_task_destroy(&task_block);
	return status;
}

void pi_i2c_write_async(struct pi_device *device, uint8_t *tx_data, int length,
						pi_i2c_xfer_flags_e flags, pi_task_t *task)
{
	struct i2c_cs_data_s *device_data = (struct i2c_cs_data_s *)device->data;
	udma_channel_e channel = TX_CHANNEL;
	I2C_TRACE("I2C(%d) : transfer %d %lx %ld %lx, task %lx\n", device_data->device_id, channel,
			  (uint32_t)tx_data, length, flags, task);
	__pi_i2c_copy(device_data, (uint32_t)tx_data, length, flags, channel, task);
}

int pi_i2c_get_request_status(pi_task_t *task)
{
	(void)task;
	return PI_OK;
}

int pi_i2c_detect(struct pi_device *device, struct pi_i2c_conf *conf, uint8_t *rx_data)
{
	int32_t status = -1;
	struct i2c_cs_data_s *cs_data = (struct i2c_cs_data_s *)device->data;
	pi_task_t task_block;
	pi_task_block(&task_block);
	I2C_TRACE("Search device at cs=%x\n", conf->cs);
	__pi_i2c_detect(cs_data, conf, rx_data, &task_block);
	pi_task_wait_on(&task_block);
	pi_task_destroy(&task_block);
	status = (*rx_data == 0x00) ? 0 : -1;
	I2C_TRACE("Search device at cs=%x result=%x\n", conf->cs, status);
	return status;
}

static void __pi_i2c_handle_pending_transfer(struct i2c_itf_data_s *driver_data)
{
	struct i2c_pending_transfer_s *pending = driver_data->pending;
	pending->pending_buffer += pending->pending_repeat;
	pending->pending_repeat_size -= pending->pending_repeat;
	pending->pending_size = pending->pending_repeat;

	if (pending->pending_repeat_size <= pending->pending_repeat)
	{
		pending->pending_repeat = 0;
		pending->pending_size = pending->pending_repeat_size;
		/* Stop bit at the end? */
		driver_data->i2c_stop_send = (pending->flags & PI_I2C_XFER_NO_STOP) ? 0 : 1;
	}
	/* Initiate next part of command sequence. */
	{
		/* Data. */
		uint32_t index = 0;
		driver_data->i2c_cmd_seq[index++] = I2C_CMD_RPT;
		driver_data->i2c_cmd_seq[index++] = pending->pending_size;
		driver_data->i2c_cmd_seq[index++] = I2C_CMD_WR;
	}
	// hal_i2c_enqueue(device_id, driver_data->channel);
	/* TODO: Renqueue next cmd! */
}

static void __pi_i2c_send_stop_cmd(struct i2c_itf_data_s *driver_data)
{
	printf("__pi_i2c_send_stop_cmd\n");
	driver_data->i2c_stop_send = 0;
	driver_data->i2c_eot_send = 0;
	plp_udma_enqueue(UDMA_I2C_CMD_ADDR(driver_data->device_id), (uint32_t)driver_data->i2c_stop_seq,
					 (uint32_t)__PI_I2C_STOP_CMD_SIZE, (UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8));
}

static inline void __pi_irq_handle_end_of_task(pi_task_t *task)
{
	printf("__pi_irq_handle_end_of_task\n");
	switch (task->id)
	{
	case PI_TASK_NONE_ID:
		pi_task_release(task);
		break;

	case PI_TASK_CALLBACK_ID:
		pi_task_push(task);
		break;

	default:
		return;
	}
}

void __pi_i2c_rx_handler(int event, void *arg)
{
	uint32_t evt = (uint32_t)event;
	uint32_t periph_id = (0);

	struct i2c_itf_data_s *driver_data = g_i2c_itf_data[periph_id];
	struct pi_task *task = __pi_i2c_cb_buf_pop(driver_data);
	if (task)
		pos_task_push_locked(task);

	task = __pi_i2c_task_fifo_pop(driver_data);
	if (task)
	{
		/* Enqueue transfer in HW fifo. */
		if (task->data[3] == RX_CHANNEL)
		{
			__pi_i2c_copy_exec_read(driver_data, task);
		}
		else
		{
			__pi_i2c_copy_exec_write(driver_data, task);
		}
	}
}

void __pi_i2c_tx_handler(int event, void *arg)
{
	printf("__pi_i2c_tx_handler\n");
	uint32_t evt = (uint32_t)event;
	uint32_t periph_id = (0);

	struct i2c_itf_data_s *driver_data = g_i2c_itf_data[periph_id];

	if (driver_data->i2c_stop_send)
	{
		__pi_i2c_send_stop_cmd(driver_data);
	}
	else
	{
		struct pi_task *task = __pi_i2c_cb_buf_pop(driver_data);
		if (task)
			pos_task_push_locked(task);

		task = __pi_i2c_task_fifo_pop(driver_data);
		if (task)
		{
			/* Enqueue transfer in HW fifo. */
			if (task->data[3] == RX_CHANNEL)
			{
				__pi_i2c_copy_exec_read(driver_data, task);
			}
			else
			{
				__pi_i2c_copy_exec_write(driver_data, task);
			}
		}
	}
}

static int32_t __pi_i2c_cb_buf_empty(struct i2c_itf_data_s *driver_data)
{
	return driver_data->buf[0] == NULL;
}

static void __pi_i2c_cb_buf_enqueue(struct i2c_itf_data_s *driver_data, struct pi_task *task)
{
	uint32_t irq = hal_irq_disable();
	driver_data->buf[0] = task;
	hal_irq_restore(irq);
}

static struct pi_task *__pi_i2c_cb_buf_pop(struct i2c_itf_data_s *driver_data)
{
	uint32_t irq = hal_irq_disable();
	struct pi_task *task_to_return = NULL;
	task_to_return = driver_data->buf[0];
	/* Free the slot for another transfer. */
	driver_data->buf[0] = NULL;
	hal_irq_restore(irq);
	return task_to_return;
}

static void __pi_i2c_task_fifo_enqueue(struct i2c_itf_data_s *driver_data, struct pi_task *task)
{
	uint32_t irq = hal_irq_disable();
	/* Enqueue transfer in SW fifo. */
	if (driver_data->fifo_head == NULL)
	{
		driver_data->fifo_head = task;
	}
	else
	{
		driver_data->fifo_tail->next = task;
	}
	driver_data->fifo_tail = task;
	hal_irq_restore(irq);
}

static struct pi_task *__pi_i2c_task_fifo_pop(struct i2c_itf_data_s *driver_data)
{
	struct pi_task *task_to_return = NULL;
	uint32_t irq = hal_irq_disable();
	if (driver_data->fifo_head != NULL)
	{
		task_to_return = driver_data->fifo_head;
		driver_data->fifo_head = driver_data->fifo_head->next;
	}
	hal_irq_restore(irq);
	return task_to_return;
}

static uint32_t __pi_i2c_clk_div_get(uint32_t i2c_freq)
{
	/* Clock divided by 4 in HW. */
	uint32_t freq = i2c_freq << 2;
	uint32_t periph_freq = pi_freq_get(PI_FREQ_DOMAIN_PERIPH);
	uint32_t div = (periph_freq + freq - 1) / freq;
	/* Clock divider counts from 0 to clk_div value included. */
	if (div <= 1)
	{
		div = 0;
	}
	else
	{
		div -= 1;
	}
	if (div > 0xFFFF)
	{
		I2C_TRACE_ERR("Error computing clock divier : Fsoc=%ld, Fi2c=%ld\n", periph_freq,
					  i2c_freq);
		return 0xFFFFFFFF;
	}
	return div;
}

static void __pi_i2c_copy_exec_read(struct i2c_itf_data_s *driver_data, struct pi_task *task)
{
	uint32_t index = 0, start_bit = 0, stop_bit = 0;
	uint32_t buffer = task->data[0];
	uint32_t size = task->data[1];
	uint32_t flags = task->data[2];
	uint32_t channel = task->data[3];
	struct i2c_cs_data_s *cs_data = (struct i2c_cs_data_s *)task->data[4];

	if (size == 0)
		return;

	/* Header. */
	driver_data->i2c_cmd_seq[index++] = I2C_CMD_CFG;
	driver_data->i2c_cmd_seq[index++] = ((cs_data->clk_div >> 8) & 0xFF);
	driver_data->i2c_cmd_seq[index++] = (cs_data->clk_div & 0xFF);
	driver_data->i2c_cmd_seq[index++] = I2C_CMD_START;
	driver_data->i2c_cmd_seq[index++] = I2C_CMD_WR;
	driver_data->i2c_cmd_seq[index++] = (cs_data->cs | ADDRESS_READ);

	struct i2c_pending_transfer_s *pending = driver_data->pending;
	if (size > (uint32_t)MAX_SIZE)
	{
		pending->pending_buffer = buffer;
		pending->pending_repeat = (uint32_t)MAX_SIZE;
		pending->pending_repeat_size = size;
		// pending->device_id = driver_data->device_id;
		pending->flags = flags;
		pending->channel = channel;
		size = (uint32_t)MAX_SIZE;
	}
	else
	{
		pending->pending_repeat = 0;
		/* Stop bit at then end? */
		driver_data->i2c_stop_send = (flags & PI_I2C_XFER_NO_STOP) ? 0 : 1;
		driver_data->i2c_eot_send = 1;
	}
	/* Data. */
	if (size > 1)
	{
		driver_data->i2c_cmd_seq[index++] = I2C_CMD_RPT;
		driver_data->i2c_cmd_seq[index++] = size - 1;
		driver_data->i2c_cmd_seq[index++] = I2C_CMD_RD_ACK;
	}
	driver_data->i2c_cmd_seq[index++] = I2C_CMD_RD_NACK;	
	driver_data->rx_channel->pendings[0] = task;

	/* Enqueue in HW fifo. */
	__pi_i2c_cb_buf_enqueue(driver_data, task);

	/* Open RX channel to receive data. */
	plp_udma_enqueue(UDMA_I2C_DATA_ADDR(driver_data->device_id), (uint32_t)buffer, size, (UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8));
	/* Transfer command. */
	plp_udma_enqueue(UDMA_I2C_CMD_ADDR(driver_data->device_id), (uint32_t)driver_data->i2c_cmd_seq, index, (UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8));
}

static void __pi_i2c_copy_exec_write(struct i2c_itf_data_s *driver_data, struct pi_task *task)
{
	uint32_t index = 0, start_bit = 0, stop_bit = 0;
	uint32_t buffer = task->data[0];
	uint32_t size = task->data[1];
	uint32_t flags = task->data[2];
	uint32_t channel = task->data[3];
	struct i2c_cs_data_s *cs_data = (struct i2c_cs_data_s *)task->data[4];
	start_bit = flags & PI_I2C_XFER_NO_START;

	/* Header. */
	driver_data->i2c_cmd_seq[index++] = I2C_CMD_CFG;
	driver_data->i2c_cmd_seq[index++] = ((cs_data->clk_div >> 8) & 0xFF);
	driver_data->i2c_cmd_seq[index++] = (cs_data->clk_div & 0xFF);
	if (!start_bit)
	{
		driver_data->i2c_cmd_seq[index++] = I2C_CMD_START;
		driver_data->i2c_cmd_seq[index++] = I2C_CMD_WR;
		driver_data->i2c_cmd_seq[index++] = (cs_data->cs | ADDRESS_WRITE);
	}
	struct i2c_pending_transfer_s *pending = driver_data->pending;
	if (size > (uint32_t)MAX_SIZE)
	{
		pending->pending_buffer = buffer;
		pending->pending_repeat = (uint32_t)MAX_SIZE;
		pending->pending_repeat_size = size;
		// pending->device_id = driver_data->device_id;
		pending->flags = flags;
		pending->channel = channel;
		size = (uint32_t)MAX_SIZE;
	}
	else
	{
		pending->pending_repeat = 0;
		/* Stop bit at the end? */
		driver_data->i2c_stop_send = (flags & PI_I2C_XFER_NO_STOP) ? 0 : 1;
		driver_data->i2c_eot_send = 1;
	}
	/* Data. */
	if (size > 0)
	{
		driver_data->i2c_cmd_seq[index++] = I2C_CMD_RPT;
		driver_data->i2c_cmd_seq[index++] = size;
		driver_data->i2c_cmd_seq[index++] = I2C_CMD_WR;
	}

	driver_data->tx_channel->pendings[0] = task;

	/* Enqueue in HW fifo. */
	__pi_i2c_cb_buf_enqueue(driver_data, task);

	/* Transfer header. */
	plp_udma_enqueue(UDMA_I2C_CMD_ADDR(driver_data->device_id), (uint32_t)driver_data->i2c_cmd_seq, index, (UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8));
	/* Transfer data. */
	if (size > 0)
		plp_udma_enqueue(UDMA_I2C_CMD_ADDR(driver_data->device_id), (uint32_t)buffer, size, (UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8));
}

static void __pi_i2c_cs_data_add(struct i2c_itf_data_s *driver_data, struct i2c_cs_data_s *cs_data)
{
/**
 	struct i2c_cs_data_s *head = driver_data->cs_list;
 	while (head != NULL)
 	{
		printf("head != NULL\n");
 		head = head->next;
 	}
 	head->next = cs_data;
*/
	cs_data->driver_data = driver_data;
	cs_data->next = driver_data->cs_list;
}

static void __pi_i2c_cs_data_remove(struct i2c_itf_data_s *driver_data,
									struct i2c_cs_data_s *cs_data)
{
	struct i2c_cs_data_s *head = driver_data->cs_list;
	struct i2c_cs_data_s *prev = driver_data->cs_list;
	while ((head != NULL) && (head != cs_data))
	{
		prev = head;
		hal_compiler_barrier();
		head = head->next;
	}
	if (head != NULL)
	{
		prev->next = head->next;
	}
}

static void __pi_i2c_freq_cb(void *args)
{
	uint32_t irq = hal_irq_disable();
	struct i2c_itf_data_s *driver_data = (struct i2c_itf_data_s *)args;
	uint32_t device_id = driver_data->device_id;
	struct i2c_cs_data_s *cs_data = driver_data->cs_list;

	/* Wait until current transfer is done. */
	while (plp_udma_busy(UDMA_I2C_DATA_ADDR(driver_data->device_id)));
	while (plp_udma_busy(UDMA_I2C_CMD_ADDR(driver_data->device_id)));

	/* Update all clock div. */
	while (cs_data != NULL)
	{
		cs_data->clk_div = __pi_i2c_clk_div_get(cs_data->max_baudrate);
		cs_data = cs_data->next;
	}
	hal_irq_restore(irq);
}

static int32_t __pi_i2c_baudrate_set(struct i2c_cs_data_s *cs_data, uint32_t new_baudrate)
{
	cs_data->max_baudrate = new_baudrate;
	uint32_t clk_div = __pi_i2c_clk_div_get(cs_data->max_baudrate);
	if (clk_div == 0xFFFFFFFF)
	{
		I2C_TRACE_ERR("I2C(%d) : error computing clock divider !\n", cs_data->device_id);
		return -14;
	}
	cs_data->clk_div = clk_div;
	return 0;
}

void __pi_i2c_conf_init(pi_i2c_conf_t *conf)
{
	conf->device = PI_DEVICE_I2C_TYPE;
	conf->cs = 0;
	conf->max_baudrate = 200000;
	conf->itf = 0;
	conf->wait_cycles = 1;
	conf->ts_ch = 0;
	conf->ts_evt_id = 0;
}

void pos_i2c_handle_copy(int event, void *arg)
{
	printf("pos_i2c_handle_copy");
	pos_udma_channel_t *channel = arg;
	pi_task_t *pending_0 = channel->pendings[0];
	uint8_t type_channel = pending_0->data[3];
	if (event == 8)
	{
		__pi_i2c_rx_handler(event, &arg);
		pos_task_push_locked(pending_0);
	}
	else if (event == 9)
	{
		__pi_i2c_tx_handler(event, &arg);
		pos_task_push_locked(pending_0);
	}
	else
	{
		printf("error pos_spi_handle_copy\n");
		exit(0);
	}
}

void pos_i2c_create_channel(pos_udma_channel_t *channel, int channel_id, int soc_event)
{
	pos_soc_event_register_callback(soc_event, pos_i2c_handle_copy, (void *)channel);
	channel->pendings[0] = NULL;
	channel->pendings[1] = NULL;
	channel->waitings_first = NULL;
	channel->base = 0;
}

int pi_i2c_open(struct pi_device *device)
{
	int32_t status = -1;
	struct pi_i2c_conf *conf = (struct pi_i2c_conf *)device->config;
	I2C_TRACE("Open device id=%d\n", conf->itf);
	status = __pi_i2c_open(conf, (struct i2c_cs_data_s **)&(device->data));
	I2C_TRACE("Open status : %ld, driver data: %lx\n", status,
		  (struct i2c_cs_data_s *)device->data);
	return status;
}

int32_t __pi_i2c_open(struct pi_i2c_conf *conf, struct i2c_cs_data_s **device_data)
{
	if ((uint8_t)ARCHI_UDMA_NB_I2C < conf->itf) {
		I2C_TRACE_ERR("Error : wrong interface ID, itf=%d !\n", conf->itf);
		return -11;
	}

    unsigned char i2c_id = conf->itf;
	int periph_id = ARCHI_UDMA_I2C_ID(i2c_id); 
	i2c_channel = UDMA_EVENT_ID(periph_id);
    plp_udma_cg_set(plp_udma_cg_get() | (1 << periph_id));
	struct i2c_itf_data_s *driver_data = g_i2c_itf_data[conf->itf];
	if (driver_data == NULL) {
		/* Allocate driver data. */
		driver_data = (struct i2c_itf_data_s *)pi_l2_malloc(sizeof(struct i2c_itf_data_s));
		if (driver_data == NULL) {
			I2C_TRACE_ERR("Driver data alloc failed !\n");
			return -12;
		}
		driver_data->buf[0] = NULL;
		driver_data->fifo_head = NULL;
		driver_data->fifo_tail = NULL;
		driver_data->pending = NULL;
		driver_data->nb_open = 0;
		driver_data->i2c_cmd_index = 0;
		driver_data->cs_list = NULL;
		for (uint32_t i = 0; i < (uint32_t)__PI_I2C_CMD_BUFF_SIZE; i++) {
			driver_data->i2c_cmd_seq[i] = 0;
		}
		driver_data->i2c_stop_send = 0;
		driver_data->i2c_eot_send = 0;
		/* Set up i2c cmd stop sequence. */
		driver_data->i2c_stop_seq[0] = I2C_CMD_STOP;
		driver_data->i2c_stop_seq[1] = I2C_CMD_WAIT;
		driver_data->i2c_stop_seq[2] = conf->wait_cycles > 0xff ? 0xff : conf->wait_cycles;
		driver_data->nb_events = 0;
		driver_data->device_id = conf->itf;
		/* TODO: Attach freq callback. */
		/* pi_freq_callback_init(&(driver_data->i2c_freq_cb), __pi_i2c_freq_cb,
		 * driver_data); */
		/* pi_freq_callback_add(&(driver_data->i2c_freq_cb)); */
		g_i2c_itf_data[conf->itf] = driver_data;

		driver_data->rx_channel = &i2c_rx_channel;
    	driver_data->tx_channel = &i2c_tx_channel;

		/* Set handlers. */
		/* Enable SOC events propagation to FC. */
		if (driver_data->nb_open == 0)
		{
			pos_udma_create_channel(driver_data->rx_channel, UDMA_CHANNEL_ID(periph_id), ARCHI_SOC_EVENT_I2C0_RX);
			pos_udma_create_channel(driver_data->tx_channel, UDMA_CHANNEL_ID(periph_id) + 1, ARCHI_SOC_EVENT_I2C0_TX);
			driver_data->rx_channel->base=i2c_id; //way to save me the spi interface which is associated with the channel
			driver_data->tx_channel->base=i2c_id; //way to save me the spi interface which is associated with the channel
		}

		soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_I2C0_RX);
		soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_I2C0_TX);

		I2C_TRACE("I2C(%d) : driver data init done.\n", driver_data->device_id);
	}

    driver_data->nb_open++;
	struct i2c_cs_data_s *cs_data =
		(struct i2c_cs_data_s *)pi_l2_malloc(sizeof(struct i2c_cs_data_s));
	if (cs_data == NULL) {
		I2C_TRACE_ERR("I2C(%ld) : cs=%d, cs_data alloc failed !\n", driver_data->device_id,
			      conf->cs);
		return -13;
	}
	cs_data->device_id = conf->itf;
	cs_data->cs = conf->cs;
	cs_data->max_baudrate = conf->max_baudrate;
	uint32_t clk_div = __pi_i2c_clk_div_get(cs_data->max_baudrate);
	if (clk_div == 0xFFFFFFFF) {
		pi_l2_free(cs_data, sizeof(struct i2c_cs_data_s));
		I2C_TRACE_ERR("I2C(%d) : error computing clock divider !\n", conf->itf);
		return -14;
	}
	cs_data->clk_div = clk_div;
	cs_data->next = NULL;
    driver_data->cs_list = cs_data;
	__pi_i2c_cs_data_add(driver_data, cs_data);
	I2C_TRACE("I2C(%d) : opened %ld time(s).\n", driver_data->device_id, driver_data->nb_open);
	*device_data = cs_data;
	return 0;
}

void __pi_i2c_close(struct i2c_cs_data_s *device_data)
{
	struct i2c_itf_data_s *driver_data = g_i2c_itf_data[device_data->device_id];
	unsigned char i2c_id = device_data->device_id;
	int periph_id = ARCHI_UDMA_I2C_ID(i2c_id);
	__pi_i2c_cs_data_remove(driver_data, device_data);
	driver_data->nb_open--;
	I2C_TRACE("I2C(%d) : number of opened devices %ld.\n", driver_data->device_id,
			  driver_data->nb_open);
	if (driver_data->nb_open == 0)
	{
		I2C_TRACE("I2C(%d) : closing interface.\n", driver_data->device_id);

		/* TODO:  Remove freq callback. */
		/* pi_freq_callback_remove(&(driver_data->i2c_freq_cb)); */

		/* Clear allocated fifo. */
		pi_l2_free(driver_data->pending, sizeof(struct i2c_pending_transfer_s));
		pi_l2_free(driver_data, sizeof(struct i2c_itf_data_s));

		/* Clear handlers. */
		/* Disable SOC events propagation to FC. */
		soc_eu_fcEventMask_clearEvent(ARCHI_SOC_EVENT_I2C0_RX);
		soc_eu_fcEventMask_clearEvent(ARCHI_SOC_EVENT_I2C0_TX);

		/* Enable UDMA CG. */
		plp_udma_cg_set(plp_udma_cg_get() & ~(1 << periph_id));

		/* Free allocated struct. */
		pi_l2_free(driver_data, sizeof(struct i2c_itf_data_s));
		g_i2c_itf_data[device_data->device_id] = NULL;
	}
	pi_l2_free(device_data, sizeof(struct i2c_cs_data_s));
}

void __pi_i2c_ioctl(struct i2c_cs_data_s *device_data, uint32_t cmd, void *arg)
{
	switch (cmd)
	{
	case PI_I2C_CTRL_SET_MAX_BAUDRATE:
		__pi_i2c_baudrate_set(device_data, (uint32_t)arg);
		break;

	default:
		break;
	}
	return;
}

void __pi_i2c_copy(struct i2c_cs_data_s *cs_data, uint32_t l2_buff, uint32_t length,
				   pi_i2c_xfer_flags_e flags, udma_channel_e channel, struct pi_task *task)
{
	uint32_t irq = hal_irq_disable();
	task->data[0] = l2_buff;
	task->data[1] = length;
	task->data[2] = flags;
	task->data[3] = channel;
	task->data[4] = (uint32_t)cs_data;
	task->next = NULL;
	struct i2c_itf_data_s *driver_data = g_i2c_itf_data[cs_data->device_id];
	int32_t slot_rxtx = __pi_i2c_cb_buf_empty(driver_data);
	/* Both slots should be empty to start a new read transfer. When enqueueing
	 * a new read transfer, RX should be opened first then TX. So if RX is already
	 * in use, then wait for it to finish. */
	if (slot_rxtx == 0)
	{
		/* Enqueue transfer in SW fifo. */
		I2C_TRACE("I2C(%d) : enqueue transfer in SW fifo : channel %d task %lx.\n",
				  driver_data->device_id, task->data[3], task);
		__pi_i2c_task_fifo_enqueue(driver_data, task);
	}
	else
	{
		/* Enqueue transfer in HW fifo. */
		I2C_TRACE("I2C(%d) : enqueue transfer in HW fifo : channel %d task %lx.\n",
				  driver_data->device_id, task->data[3], task);
		if (task->data[3] == RX_CHANNEL)
		{
			__pi_i2c_copy_exec_read(driver_data, task);
		}
		else
		{
			driver_data->tx_channel->pendings[0] = task;
			__pi_i2c_copy_exec_write(driver_data, task);
		}
	}
	hal_irq_restore(irq);
}

int32_t __pi_i2c_detect(struct i2c_cs_data_s *cs_data, struct pi_i2c_conf *conf, uint8_t *rx_data,
						struct pi_task *task)
{
	uint32_t irq = hal_irq_disable();
	if (cs_data->device_id != conf->itf)
	{
		I2C_TRACE_ERR("I2C(%d) : error wrong interfaces %d - %d !\n", cs_data->device_id,
					  conf->itf);
		hal_irq_restore(irq);
		return -11;
	}
	struct i2c_itf_data_s *driver_data = g_i2c_itf_data[cs_data->device_id];
	uint32_t clk_div = __pi_i2c_clk_div_get(conf->max_baudrate);
	if (clk_div == 0xFFFFFFFF)
	{
		I2C_TRACE_ERR("I2C(%d) : error computing clock divider !\n", conf->itf);
		hal_irq_restore(irq);
		return -12;
	}
	uint16_t clkdiv = clk_div;

	task->next = NULL;

	uint32_t index = 0;
	uint32_t buffer = (uint32_t)rx_data;
	uint32_t size = 1;

	/* Header. */
	driver_data->i2c_cmd_seq[index++] = I2C_CMD_CFG;
	driver_data->i2c_cmd_seq[index++] = ((clkdiv >> 8) & 0xFF);
	driver_data->i2c_cmd_seq[index++] = (clkdiv & 0xFF);
	driver_data->i2c_cmd_seq[index++] = I2C_CMD_START;
	driver_data->i2c_cmd_seq[index++] = I2C_CMD_WR;
	driver_data->i2c_cmd_seq[index++] = (conf->cs | ADDRESS_READ);

	struct i2c_pending_transfer_s *pending = driver_data->pending;
	pending->pending_repeat = 0;
	/* Stop bit at then end? */
	driver_data->i2c_stop_send = 1;
	driver_data->i2c_eot_send = 1;

	driver_data->i2c_cmd_seq[index++] = I2C_CMD_RD_NACK;

	/* Enqueue in HW fifo. */
	__pi_i2c_cb_buf_enqueue(driver_data, task);

	/* Open RX channel to receive data. */
	plp_udma_enqueue(UDMA_I2C_DATA_ADDR(driver_data->device_id), (uint32_t)buffer, size, 
	(UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8));
	/* Transfer command. */
	plp_udma_enqueue(UDMA_I2C_CMD_ADDR(driver_data->device_id), (uint32_t)driver_data->i2c_cmd_seq, 
	index, (UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8));

	hal_irq_restore(irq);
	return 0;
}