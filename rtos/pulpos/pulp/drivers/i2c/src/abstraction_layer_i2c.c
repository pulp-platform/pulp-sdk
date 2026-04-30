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
 * @repo           :  pulp-sdk/rtos/pulpos/pulp/drivers/i2c/src
 * @createdOn      :  /01/2022
 * @description    :  PULP-OS
 * 					  The driver was tested on a VIP flash memory in RTL, where it was done one
 *                    transfer at a time.
 *                    Multiple concurrent transfers have not been tested. I mean using multiple
 *                    I2C interfaces that do transfers at the same time.
 *-----------------------------------------------------------------------------------------------------------------------**/

/**================================================================================================
 **                                         INCLUDE
 *================================================================================================**/
#include "abstraction_layer_i2c.h"
/**================================================================================================
 **                                         GLOBAL VARIABLE
 *================================================================================================**/
struct i2c_itf_data_s *g_i2c_itf_data[ARCHI_UDMA_NB_I2C] = {0};
uint32_t buffer_to_write[MAX_SIZE];
PI_L2 int i2c_channel;
PI_L2 pos_udma_channel_t i2c_rx_channel;
PI_L2 pos_udma_channel_t i2c_tx_channel;

/**================================================================================================
 **                                         FUNCTION
 *================================================================================================**/
#ifdef CONFIG_UDMA_I2C_EOT
/* Some UDMA v2 peripherals support end of transfer signalling. In that case we
 * signal the callback that we are done when we get this EOT information. The
 * regular UDMA v2 says its done when its udma fifos are empty but this might
 * not coincide with when the i2c signalling has finished. This is important
 * when you try to detect slave ACK/NACKs. */
void __pi_i2c_eot_handler(int event, uint8_t itf)
{
	I2C_TRACE("__pi_i2c_eot_handler\n");
	uint32_t evt = (uint32_t)event;
	int periph_id = itf;

	struct i2c_itf_data_s *driver_data = g_i2c_itf_data[periph_id];

	struct pi_task *task = __pi_i2c_cb_buf_pop(driver_data);
	if (task)
		__pi_irq_handle_end_of_task(task);

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
#endif

void __pi_i2c_rx_handler(int event, uint8_t itf)
{
	I2C_TRACE("rx event \n");
}

void __pi_i2c_cmd_handler(int event, uint8_t itf)
{
	uint32_t evt = (uint32_t)event;
	int periph_id = itf;

	struct i2c_itf_data_s *driver_data = g_i2c_itf_data[periph_id];
	/*
	 * In case of a read command sequence, TX ends first then wait on RX.
	 * Until then, no other transaction should occur.
	 */
	/* Pending transfer. */
	if (driver_data->pending->pending_repeat)
	{
		/* FIXME: not implemented */
		__pi_i2c_handle_pending_transfer(driver_data);
	}
	else if (driver_data->i2c_stop_send)
	{
		__pi_i2c_send_stop_cmd(driver_data);
#ifdef CONFIG_UDMA_I2C_EOT
	}
	else if (driver_data->i2c_eot_send)
	{
		__pi_i2c_send_only_eot_cmd(driver_data);
#else
	}
	else
	{
		struct pi_task *task = __pi_i2c_cb_buf_pop(driver_data);
		if (task)
			__pi_irq_handle_end_of_task(task);

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
#endif
	}
}

void __pi_i2c_tx_handler(int event, uint8_t itf)
{
	I2C_TRACE("tx event \n");
}

void __pi_irq_handle_end_of_task(pi_task_t *task)
{
	I2C_TRACE("__pi_irq_handle_end_of_task\n");
	switch (task->id)
	{
	case PI_TASK_NONE_ID:
		pos_task_push_locked(task);
		break;
	case PI_TASK_CALLBACK_ID:
		pi_task_push(task);
		break;
	default:
		return;
	}
}

void pos_i2c_handle_copy(int event, void *arg)
{
	I2C_TRACE("event %d\n", event);
	uint32_t itf = (event >> ARCHI_SOC_EVENT_UDMA_NB_CHANNEL_EVT_LOG2) - ARCHI_UDMA_I2C_ID(0);

	if (event == SOC_EVENT_UDMA_I2C_RX(itf))
	{
		__pi_i2c_rx_handler(event, itf);
	}
	else if (event == SOC_EVENT_UDMA_I2C_TX(itf))
	{
		__pi_i2c_tx_handler(event, itf);
	}
	else if (event == SOC_EVENT_UDMA_I2C_CMD(itf))
	{
		__pi_i2c_cmd_handler(event, itf);
	}
	else if (event == SOC_EVENT_UDMA_I2C_EOT(itf))
	{
		__pi_i2c_eot_handler(event, itf);
	}
	else
	{
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

int32_t __pi_i2c_open(struct pi_i2c_conf *conf, struct i2c_cs_data_s **device_data)
{
	if ((uint8_t)ARCHI_UDMA_NB_I2C < conf->itf)
	{
		I2C_TRACE_ERR("Error : wrong interface ID, itf=%d !\n", conf->itf);
		return -11;
	}

	unsigned char i2c_id = conf->itf;
	int periph_id = ARCHI_UDMA_I2C_ID(i2c_id);
	i2c_channel = UDMA_EVENT_ID(periph_id);

	struct i2c_itf_data_s *driver_data = g_i2c_itf_data[conf->itf];
	if (driver_data == NULL)
	{
		/* Allocate driver data. */
		driver_data = (struct i2c_itf_data_s *)pi_l2_malloc(sizeof(struct i2c_itf_data_s));
		if (driver_data == NULL)
		{
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
		for (uint32_t i = 0; i < (uint32_t)__PI_I2C_CMD_BUFF_SIZE; i++)
		{
			driver_data->i2c_cmd_seq[i] = 0;
		}
		driver_data->i2c_stop_send = 0;
		driver_data->i2c_eot_send = 0;
		/* Set up i2c cmd stop sequence. */
		driver_data->i2c_stop_seq[0] = (((uint32_t)I2C_CMD_STOP) << 24);
		driver_data->i2c_stop_seq[1] = (((uint32_t)I2C_CMD_WAIT) << 24) | (conf->wait_cycles > 0xff ? 0xff : conf->wait_cycles);
#ifdef CONFIG_UDMA_I2C_EOT
		driver_data->i2c_stop_seq[2] = (((uint32_t)I2C_CMD_EOT) << 24);
		driver_data->i2c_only_eot_seq = &driver_data->i2c_stop_seq[1];
#endif
		driver_data->nb_events = 0;
		driver_data->device_id = conf->itf;
		/* TODO: Attach freq callback. */
		/* pi_freq_callback_init(&(driver_data->i2c_freq_cb), __pi_i2c_freq_cb,
		 * driver_data); */
		/* pi_freq_callback_add(&(driver_data->i2c_freq_cb)); */
		g_i2c_itf_data[conf->itf] = driver_data;

		/* Set handlers. */
		/* Enable SOC events propagation to FC. */

		/* Disable UDMA CG. */
		plp_udma_cg_set(plp_udma_cg_get() | (1 << ARCHI_UDMA_I2C_ID(conf->itf)));

		driver_data->rx_channel = &i2c_rx_channel;
		driver_data->tx_channel = &i2c_tx_channel;

		if (driver_data->nb_open == 0)
		{
			pos_i2c_create_channel(driver_data->rx_channel, UDMA_CHANNEL_ID(ARCHI_UDMA_I2C_ID(conf->itf)), SOC_EVENT_UDMA_I2C_RX(driver_data->device_id));
			pos_i2c_create_channel(driver_data->tx_channel, UDMA_CHANNEL_ID(ARCHI_UDMA_I2C_ID(conf->itf)) + 1, SOC_EVENT_UDMA_I2C_CMD(driver_data->device_id));
#ifdef CONFIG_UDMA_I2C_EOT
			pos_i2c_create_channel(driver_data->tx_channel, UDMA_CHANNEL_ID(ARCHI_UDMA_I2C_ID(conf->itf)) + 2, SOC_EVENT_UDMA_I2C_EOT(driver_data->device_id));
#endif
			driver_data->rx_channel->base = i2c_id; // way to save me the spi interface which is associated with the channel
			driver_data->tx_channel->base = i2c_id; // way to save me the spi interface which is associated with the channel
		}

		soc_eu_fcEventMask_setEvent(SOC_EVENT_UDMA_I2C_RX(driver_data->device_id));	
		soc_eu_fcEventMask_setEvent(SOC_EVENT_UDMA_I2C_CMD(driver_data->device_id));
#ifdef CONFIG_UDMA_I2C_EOT
		soc_eu_fcEventMask_setEvent(SOC_EVENT_UDMA_I2C_EOT(driver_data->device_id));
#endif
		I2C_TRACE("I2C(%d) : driver data init done.\n", driver_data->device_id);
	}

	struct i2c_cs_data_s *cs_data =
		(struct i2c_cs_data_s *)pi_l2_malloc(sizeof(struct i2c_cs_data_s));
	if (cs_data == NULL)
	{
		I2C_TRACE_ERR("I2C(%ld) : cs=%d, cs_data alloc failed !\n", driver_data->device_id,
					  conf->cs);
		return -13;
	}
	cs_data->device_id = conf->itf;
	cs_data->cs = conf->cs;
	cs_data->max_baudrate = conf->max_baudrate;
	uint32_t clk_div = __pi_i2c_clk_div_get(cs_data->max_baudrate);
	if (clk_div == 0xFFFFFFFF)
	{
		pi_l2_free(cs_data, sizeof(struct i2c_cs_data_s));
		I2C_TRACE_ERR("I2C(%d) : error computing clock divider !\n", conf->itf);
		return -14;
	}
	cs_data->clk_div = clk_div;
	cs_data->next = NULL;
	__pi_i2c_cs_data_add(driver_data, cs_data);
	driver_data->nb_open++;
	I2C_TRACE("I2C(%d) : opened %ld time(s).\n", driver_data->device_id, driver_data->nb_open);
	*device_data = cs_data;
	return 0;
}

void __pi_i2c_close(struct i2c_cs_data_s *device_data)
{
	struct i2c_itf_data_s *driver_data = g_i2c_itf_data[device_data->device_id];
	driver_data->nb_open--;
	I2C_TRACE("I2C(%d) : number of opened devices %ld.\n", driver_data->device_id,
			  driver_data->nb_open);
	if (driver_data->nb_open == 0)
	{
		I2C_TRACE("I2C(%d) : closing interface.\n", driver_data->device_id);

		/* Clear handlers. */
		/* Disable SOC events propagation to FC. */
		soc_eu_fcEventMask_clearEvent(SOC_EVENT_UDMA_I2C_RX(driver_data->device_id));
		soc_eu_fcEventMask_clearEvent(SOC_EVENT_UDMA_I2C_EOT(driver_data->device_id));
		soc_eu_fcEventMask_clearEvent(SOC_EVENT_UDMA_I2C_CMD(driver_data->device_id));

		/* Enable UDMA CG. */
		plp_udma_cg_set(plp_udma_cg_get() & ~(1 << ARCHI_UDMA_I2C_ID(driver_data->device_id)));
		
		/* Free allocated struct. */
		pi_l2_free(driver_data->pending, sizeof(struct i2c_pending_transfer_s));
		pi_l2_free(driver_data, sizeof(struct i2c_itf_data_s));
		g_i2c_itf_data[device_data->device_id] = NULL;
	}
	__pi_i2c_cs_data_remove(driver_data, device_data);
	pi_l2_free(device_data, sizeof(struct i2c_cs_data_s));
}

void __pi_i2c_enqueue_tx(uint32_t device_id, uint32_t l2buf, uint32_t size)
{
	/* Enqueue l2 buffer & start transfer. */
	plp_udma_enqueue(UDMA_I2C_TX_ADDR(device_id), l2buf, size, (UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8));
}

void __pi_i2c_enqueue_cmd(uint32_t device_id, uint32_t l2buf, uint32_t size)
{
	/* Enqueue l2 buffer & start transfer. */
	plp_udma_enqueue(UDMA_I2C_CMD_ADDR(device_id), l2buf, size * 4, (UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_32));
}

void __pi_i2c_enqueue_rx(uint32_t device_id, uint32_t l2buf, uint32_t size)
{
	/* Enqueue l2 buffer & start transfer. */
	plp_udma_enqueue(UDMA_I2C_DATA_ADDR(device_id), l2buf, size, (UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8));
}

void __pi_i2c_copy_exec_read(struct i2c_itf_data_s *driver_data, struct pi_task *task)
{
	uint32_t index = 0, start_bit = 0, stop_bit = 0;
	uint32_t buffer = task->data[0];
	uint32_t size = task->data[1];
	uint32_t flags = task->data[2];
	uint32_t channel = task->data[3];
	int size_full = size + 3;
	struct i2c_cs_data_s *cs_data = (struct i2c_cs_data_s *)task->data[4];
	driver_data->rx_channel->pendings[0]=task;
	if (size == 0)
		return;

	/* Header. */

	driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_CFG) << 24) | ((cs_data->clk_div >> 8) & 0xFF) | (cs_data->clk_div & 0xFF);
	driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_START) << 24);
	driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_WRB) << 24) | (cs_data->cs | ADDRESS_READ);

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
		for(index = 3; index < (size_full-1); index++)
		{
			driver_data->i2c_cmd_seq[index] = (((uint32_t)I2C_CMD_RD_ACK) << 24);
		}
		driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_RD_NACK) << 24);
	}

	/* Enqueue in HW fifo. */
	__pi_i2c_cb_buf_enqueue(driver_data, task);

	/* Open RX channel to receive data. */
	__pi_i2c_enqueue_rx(driver_data->device_id, buffer, size);
	/* Transfer command. */
	__pi_i2c_enqueue_cmd(driver_data->device_id, (uint32_t)driver_data->i2c_cmd_seq, index);
}

void __pi_i2c_copy_exec_write(struct i2c_itf_data_s *driver_data, struct pi_task *task)
{
	uint32_t index = 0, start_bit = 0, stop_bit = 0;
	uint32_t buffer = task->data[0];
	uint32_t size = task->data[1];
	uint32_t flags = task->data[2];
	uint32_t channel = task->data[3];
	uint8_t *buffer_copy = (uint8_t *)task->data[0];
	struct i2c_cs_data_s *cs_data = (struct i2c_cs_data_s *)task->data[4];
	driver_data->tx_channel->pendings[0]=task;
	start_bit = flags & PI_I2C_XFER_NO_START;

	for(int j=0; j<MAX_SIZE; j++)
	{
		buffer_to_write[j]=0;
	}

	/* Header. */
	driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_CFG) << 24) | ((cs_data->clk_div >> 8) & 0xFF) | (cs_data->clk_div & 0xFF);
	if (!start_bit)
	{
		driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_START) << 24);
		driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_WRB) << 24) | (cs_data->cs | ADDRESS_WRITE);
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
		for(int j=0; j<size; j++)
		{
			buffer_to_write[j] = ((((uint32_t)I2C_CMD_WRB) << 24) | buffer_copy[j]);
		}
	}
	
	/* Enqueue in HW fifo. */
	__pi_i2c_cb_buf_enqueue(driver_data, task);

	/* Transfer header. */
	__pi_i2c_enqueue_cmd(driver_data->device_id, (uint32_t)driver_data->i2c_cmd_seq, index);
	/* Transfer data. */
	if (size > 0)
		__pi_i2c_enqueue_cmd(driver_data->device_id, (uint32_t)buffer_to_write, size);
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
	task->data[5] = cs_data->device_id; 
	task->id = PI_TASK_NONE_ID;
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
			__pi_i2c_copy_exec_write(driver_data, task);
		}
	}
	hal_irq_restore(irq);
}

void __pi_i2c_send_stop_cmd(struct i2c_itf_data_s *driver_data)
{
	I2C_TRACE("__pi_i2c_send_stop_cmd\n");
	driver_data->i2c_stop_send = 0;
	driver_data->i2c_eot_send = 0;
	__pi_i2c_enqueue_cmd(driver_data->device_id, (uint32_t)driver_data->i2c_stop_seq, (uint32_t)__PI_I2C_STOP_CMD_SIZE);
}

void __pi_i2c_send_only_eot_cmd(struct i2c_itf_data_s *driver_data)
{
	driver_data->i2c_eot_send = 0;
	__pi_i2c_enqueue_cmd(driver_data->device_id, (uint32_t)driver_data->i2c_only_eot_seq, (uint32_t)__PI_I2C_ONLY_EOT_CMD_SIZE);
}

int32_t __pi_i2c_detect(struct i2c_cs_data_s *cs_data, struct pi_i2c_conf *conf, uint8_t *rx_data, struct pi_task *task)
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
	driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_CFG) << 24) | ((clkdiv >> 8) & 0xFF) | (clkdiv & 0xFF);
	driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_START) << 24);
	driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_WRB) << 24) | (conf->cs | ADDRESS_READ);

	struct i2c_pending_transfer_s *pending = driver_data->pending;
	pending->pending_repeat = 0;
	/* Stop bit at then end? */
	driver_data->i2c_stop_send = 1;
	driver_data->i2c_eot_send = 1;

	driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_RD_NACK) << 24);

	/* Enqueue in HW fifo. */
	__pi_i2c_cb_buf_enqueue(driver_data, task);

	/* Open RX channel to receive data. */
	__pi_i2c_enqueue_rx(driver_data->device_id, buffer, size);
	/* Transfer command. */
	__pi_i2c_enqueue_cmd(driver_data->device_id, (uint32_t)driver_data->i2c_cmd_seq, index);

	hal_irq_restore(irq);
	return 0;
}