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
 * @repo           :  pulp-sdk/?/abstraction_layer_spi_freertos
 * @createdOn      :  29/12/2021
 * @description    :  Abstraction Layer SPI for FreeRTOS
 *-----------------------------------------------------------------------------------------------------------------------**/

/**================================================================================================
 **                                         INCLUDE
 *================================================================================================**/
#include "abstraction_layer_spi.h"

/**================================================================================================
 **                                         GLOBAL VARIABLE
 *================================================================================================**/
struct spim_driver_data *__g_spim_drv_data[UDMA_NB_SPIM] = {0};

/**================================================================================================
 **                                         FUNCTION
 *================================================================================================**/
void __pi_spi_receive_async(struct spim_cs_data *cs_data, void *data, size_t len,
			    pi_spi_flags_e flags, pi_task_t *task)
{
	DBG_PRINTF("...start -> __pi_spi_receive_async...\n");
	// SPIM_CS_DATA_GET_DRV_DATA(cs_data) = (cs_data->drv_data)
	struct spim_driver_data *drv_data = SPIM_CS_DATA_GET_DRV_DATA(cs_data);
	int qspi = (flags & (0x3 << 2)) == PI_SPI_LINES_QUAD;
	// Choose the type of cs mode, see enum "pi_spi_flags_e" to understand better.
	int cs_mode = (flags >> 0) & 0x3;

	int device_id = drv_data->device_id;
	uint32_t cfg = cs_data->cfg;
	DBG_PRINTF("%s:%s:%d: core clock:%" PRIu32 ", baudrate:%" PRIu32 ", div=%" PRIu32
		   ", udma_cmd cfg =%lx, qpi=%d\n",
		   __FILE__, __func__, __LINE__, system_core_clock_get(), cs_data->max_baudrate,
		   system_core_clock_get() / cs_data->max_baudrate, cfg, qspi);

	int buffer_size = (len + 7) >> 3;

	if (len > 8192 * 8) {
		DBG_PRINTF("%s:%s:%d: Transaction splitting unimplemented, too large", __FILE__,
			   __func__, __LINE__);
		abort(); /* TODO: unimplemented transaction splitting */
	}

	DBG_PRINTF("%s:%s:%d: udma_cmd = %p\n", __FILE__, __func__, __LINE__,
		   &(cs_data->udma_cmd[0]));
	uint32_t irq = deactive_irq();

	uint8_t bitsword = 0;
	uint8_t UDMA_CORE_CFG = 0;
	uint32_t byte_align = 0;

	if (cs_data->wordsize == PI_SPI_WORDSIZE_8) {
		bitsword = 8;
		UDMA_CORE_CFG = UDMA_CORE_CFG_DATASIZE_8; // 0x0
		byte_align = (cs_data->wordsize) && cs_data->big_endian;

	} else if (cs_data->wordsize == PI_SPI_WORDSIZE_16) {
		bitsword = 16;
		UDMA_CORE_CFG = UDMA_CORE_CFG_DATASIZE_16; // 0x1
		byte_align = (cs_data->wordsize) && cs_data->big_endian;
	} else {
		bitsword = 32;
		UDMA_CORE_CFG = UDMA_CORE_CFG_DATASIZE_32; // 0x2
		byte_align = (cs_data->wordsize) && cs_data->big_endian;
	}

	DBG_PRINTF("%s:%s:%d: bitsword = %u\n", __FILE__, __func__, __LINE__, bitsword);
	DBG_PRINTF("%s:%s:%d: UDMA_CORE_CFG = %u\n", __FILE__, __func__, __LINE__, UDMA_CORE_CFG);

	if (!drv_data->end_of_transfer) {
		cs_data->udma_cmd[0] = cfg;
		cs_data->udma_cmd[1] = SPI_CMD_SOT(cs_data->cs);
		cs_data->udma_cmd[2] = SPI_CMD_RX_DATA(len / bitsword, SPI_CMD_1_WORD_PER_TRANSF,
						       bitsword, qspi, SPI_CMD_MSB_FIRST);
		cs_data->udma_cmd[3] = SPI_CMD_EOT(1, cs_mode == PI_SPI_CS_KEEP);

		drv_data->end_of_transfer = task;
		drv_data->repeat_transfer = NULL;

		// number of byteword
		uint32_t rx_conf =
			UDMA_CORE_TX_CFG_EN(1) | UDMA_CORE_TX_CFG_DATASIZE(UDMA_CORE_CFG);

		/* receive data stream with 32-bit data size */
		spim_enqueue_channel(SPIM(device_id), (uint32_t)data, buffer_size, rx_conf,
				     RX_CHANNEL);
		// number of byteword
		uint32_t cmd_conf = UDMA_CORE_TX_CFG_EN(1) |
				    UDMA_CORE_TX_CFG_DATASIZE(UDMA_CORE_CFG_DATASIZE_32);
		/* send command stream with 32-bit data size */
		spim_enqueue_channel(SPIM(device_id), (uint32_t)cs_data->udma_cmd,
				     4 * sizeof(uint32_t), cmd_conf, COMMAND_CHANNEL);
		DBG_PRINTF("%s:%s:%d: cmd_conf= %u\n", __FILE__, __func__, __LINE__, cmd_conf);
		DBG_PRINTF("%s:%s:%d: rx_conf = %u\n", __FILE__, __func__, __LINE__, rx_conf);
	} else {
		struct spim_transfer transfer;
		transfer.data = data;
		transfer.flags = flags;
		transfer.len = len;
		transfer.cfg_cmd = cfg;
		transfer.byte_align = byte_align;
		transfer.is_send = 0;
		__pi_spim_drv_fifo_enqueue(cs_data, &transfer, task);
	}
	active_irq(irq);
	DBG_PRINTF("...end -> __pi_spi_receive_async...\n");
}

void __pi_spi_send_async(struct spim_cs_data *cs_data, void *data, size_t len, pi_spi_flags_e flags,
			 pi_task_t *task)
{
	DBG_PRINTF("...start -> __pi_spi_send_async...\n");
	struct spim_driver_data *drv_data = SPIM_CS_DATA_GET_DRV_DATA(cs_data);
	int qspi = (flags & (0x3 << 2)) == PI_SPI_LINES_QUAD;
	// Choose the type of cs mode, see enum "pi_spi_flags_e" to understand better.
	int cs_mode = (flags >> 0) & 0x3;

	// Which SPI interface am I using.
	int device_id = drv_data->device_id;
	uint32_t cfg = cs_data->cfg; // SPI_CMD_CFG(...)
	DBG_PRINTF("%s:%s:%d: core clock:%" PRIu32 ", baudrate:%" PRIu32 ", div=%" PRIu32
		   ", udma_cmd cfg =%lx, qpi=%d\n",
		   __FILE__, __func__, __LINE__, system_core_clock_get(), cs_data->max_baudrate,
		   system_core_clock_get() / cs_data->max_baudrate, cfg, qspi);

	/* buffer size */
	int buffer_size = (len + 7) >> 3;

	if (len > 8192 * 8) {
		DBG_PRINTF("%s:%s:%d: Transaction splitting unimplemented, too large", __FILE__,
			   __func__, __LINE__);
		abort(); /* TODO: unimplemented transaction splitting */
	}

	// Address of the command buffer to be sent to the uDMA
	DBG_PRINTF("%s:%s:%d: udma_cmd = %p\n", __FILE__, __func__, __LINE__,
		   &(cs_data->udma_cmd[0]));
	uint32_t irq = disable_irq();
	/* check if we already have a transfer ongoing */

	uint8_t bitsword = 0;
	uint8_t UDMA_CORE_CFG = 0;
	uint32_t byte_align = 0;
	// uint32_t byte_align = (cs_data->wordsize == PI_SPI_WORDSIZE_32) && cs_data->big_endian;

	if (cs_data->wordsize == PI_SPI_WORDSIZE_8) {
		bitsword = 8;
		UDMA_CORE_CFG = UDMA_CORE_CFG_DATASIZE_8; // 0x0
		byte_align = (cs_data->wordsize) && cs_data->big_endian;

	} else if (cs_data->wordsize == PI_SPI_WORDSIZE_16) {
		bitsword = 16;
		UDMA_CORE_CFG = UDMA_CORE_CFG_DATASIZE_16; // 0x1
		byte_align = (cs_data->wordsize) && cs_data->big_endian;
	} else {
		bitsword = 32;
		UDMA_CORE_CFG = UDMA_CORE_CFG_DATASIZE_32; // 0x2
		byte_align = (cs_data->wordsize) && cs_data->big_endian;
	}

	DBG_PRINTF("%s:%s:%d: bitsword = %u\n", __FILE__, __func__, __LINE__, bitsword);
	DBG_PRINTF("%s:%s:%d: UDMA_CORE_CFG = %u\n", __FILE__, __func__, __LINE__, UDMA_CORE_CFG);
	DBG_PRINTF("%s:%s:%d: device_id = %d\n", __FILE__, __func__, __LINE__, device_id);

	if (!drv_data->end_of_transfer) { /* enqueue the transfer */
		cs_data->udma_cmd[0] = cfg;
		cs_data->udma_cmd[1] = SPI_CMD_SOT(cs_data->cs);
		cs_data->udma_cmd[2] = SPI_CMD_TX_DATA((len / bitsword), SPI_CMD_1_WORD_PER_TRANSF,
						       bitsword, qspi, SPI_CMD_MSB_FIRST);
		cs_data->udma_cmd[3] = SPI_CMD_EOT(1, cs_mode == PI_SPI_CS_KEEP);

		drv_data->end_of_transfer = task;
		drv_data->repeat_transfer = NULL;

		uint32_t cmd_conf = UDMA_CORE_TX_CFG_EN(1) |
				    UDMA_CORE_TX_CFG_DATASIZE(UDMA_CORE_CFG_DATASIZE_32);
		/* send command stream with 32-bit data size */
		spim_enqueue_channel(SPIM(device_id), (uint32_t)cs_data->udma_cmd,
				     4 * sizeof(uint32_t), cmd_conf, COMMAND_CHANNEL);

		uint32_t tx_conf =
			UDMA_CORE_TX_CFG_EN(1) | UDMA_CORE_TX_CFG_DATASIZE(UDMA_CORE_CFG);
		/* send data stream with 32-bit data size */
		spim_enqueue_channel(SPIM(device_id), (uint32_t)data, buffer_size, tx_conf,
				     TX_CHANNEL);
		DBG_PRINTF("%s:%s:%d: cmd_conf = %u\n", __FILE__, __func__, __LINE__, cmd_conf);
		DBG_PRINTF("%s:%s:%d: tx_conf = %u\n", __FILE__, __func__, __LINE__, tx_conf);
	} else { /* a transfer is running, append to pending transfers queue */
		struct spim_transfer transfer;
		transfer.data = data;
		transfer.flags = flags;
		transfer.len = len;
		transfer.cfg_cmd = cfg;
		transfer.byte_align = byte_align;
		transfer.is_send = 1;
		__pi_spim_drv_fifo_enqueue(cs_data, &transfer, task);
	}
	active_irq(irq);
	DBG_PRINTF("...end -> __pi_spi_send_async...\n");
}

int __pi_spi_open(struct spim_cs_data **cs_data, struct pi_spi_conf *conf)
{
	DBG_PRINTF("...start -> pi_spi_open...\n");

	uint32_t irq = deactive_irq();
	/* int status = __pi_spi_open((struct spim_cs_data **)(&device->data),
	 * conf); */

	/* TODO: hacked */
	int status = 0;
	/* TODO: paste beg */

	// disable clock gating for said device
	unsigned char spi_id = conf->itf;
	int periph_id = UDMA_SPIM_ID(spi_id);

	DBG_PRINTF("%s:%s:%d: periph_id = %u\n", __FILE__, __func__, __LINE__, periph_id);

	udma_ctrl_cg_disable(UDMA_SPIM_ID(conf->itf));

	hal_soc_eu_set_fc_mask(SOC_EVENT_UDMA_SPIM_EOT(conf->itf));

	pi_fc_event_handler_set(SOC_EVENT_UDMA_SPIM_EOT(conf->itf), spim_eot_handler);
	pi_fc_event_handler_set(SOC_EVENT_UDMA_SPIM_TX(conf->itf), spim_tx_handler);
	pi_fc_event_handler_set(SOC_EVENT_UDMA_SPIM_RX(conf->itf), spim_rx_handler);

	/*
	** spim_driver_data keeps information for each spi interface.
	*/
	// Take care of driver data
	struct spim_driver_data *drv_data;
	if (__g_spim_drv_data[conf->itf]) {
		drv_data = __g_spim_drv_data[conf->itf];
	} else {

		// Do this to define a node in a list. The list is a dynamic object.
		__g_spim_drv_data[conf->itf] = pi_default_malloc(sizeof(struct spim_driver_data));
		memset(__g_spim_drv_data[conf->itf], 0, sizeof(struct spim_driver_data));
		drv_data = __g_spim_drv_data[conf->itf];
		// Do this to define a node in a list. The list is a dynamic object.
		drv_data->drv_fifo = pi_default_malloc(sizeof(struct spim_drv_fifo));
		memset(drv_data->drv_fifo, 0, sizeof(struct spim_drv_fifo));
		// controllo che il puntatore sia = 0
		if (!drv_data->drv_fifo) {
			active_irq(irq);
			return -1;
		}
		drv_data->device_id = conf->itf;
	}
	/*
	** Number of open SPI interfaces
	*/
	drv_data->nb_open++;

	// Take care of cs data
	*cs_data = __pi_spim_get_cs_data(drv_data, conf->cs);

	if (!*cs_data) { // if (*cs_data == 0)
		uint32_t clk_div = __pi_spi_clk_div_get(conf->max_baudrate);
		// alloc a cs data, need to be in udma reachable ram
		struct spim_cs_data *_cs_data = pi_data_malloc(sizeof(struct spim_cs_data));
		if (_cs_data == NULL) {
			DBG_PRINTF("[%s] _cs_data alloc failed\n", __func__);
			active_irq(irq);
			return -2;
		}
		if (clk_div > 0xFF) {
			DBG_PRINTF("[%s] clk_div, %" PRIu32
				   ", does not fit into 8 bits. SoC frequency too high.\n",
				   __func__, clk_div);
			active_irq(irq);
			return -3;
		}

		memset(_cs_data, 0, sizeof(struct spim_cs_data));
		_cs_data->max_baudrate = conf->max_baudrate;
		_cs_data->polarity = conf->polarity;
		_cs_data->phase = conf->phase;
		_cs_data->big_endian = conf->big_endian;
		_cs_data->wordsize = conf->wordsize;
		_cs_data->cs = conf->cs;
		_cs_data->cfg = SPI_CMD_CFG(clk_div, _cs_data->phase, _cs_data->polarity);
		// _cs_data->cfg = SPI_CMD_CFG(1,0,0);
		*cs_data = _cs_data;
		// I insert a new element in the cs_data list
		__pi_spim_cs_data_add(drv_data, _cs_data);
	}
	DBG_PRINTF("%s:%d\n", __func__, __LINE__);

	/* TODO: paste end */

	active_irq(irq);
	DBG_PRINTF("...end -> pi_spi_open...\n");
	return status;
}

void __pi_spi_close(struct spim_cs_data *cs_data, struct pi_spi_conf *conf)
{
	DBG_PRINTF("...start -> pi_spi_close...\n");

	uint32_t irq = deactive_irq();
	/* TODO: paste beg */
	struct spim_driver_data *drv_data = cs_data->drv_data;
	__pi_spim_cs_data_del(drv_data, cs_data->cs);
	/*
	** Number of open SPI interfaces
	*/
	drv_data->nb_open--;

	if (drv_data->nb_open == 0) {
		/* reactivate clock gating for said device */
		udma_ctrl_cg_enable(UDMA_SPIM_ID(drv_data->device_id));
		hal_soc_eu_clear_fc_mask(SOC_EVENT_UDMA_SPIM_EOT(drv_data->device_id));
		pi_fc_event_handler_clear(SOC_EVENT_UDMA_SPIM_EOT(drv_data->device_id));
		__g_spim_drv_data[drv_data->device_id] = NULL;
		pi_default_free(drv_data->drv_fifo, sizeof(drv_data->drv_fifo));
		pi_default_free(drv_data, sizeof(drv_data));

		active_irq(irq);
		return;
	}
	pi_data_free(cs_data, sizeof(cs_data));
	/* TODO: moved to end return drv_data->nb_open; */
	/* TODO: paste end */

	active_irq(irq);
	DBG_PRINTF("...end -> pi_spi_close...\n");
	return;
}

void spim_eot_handler(void *arg)
{
	DBG_PRINTF("...start -> spim_eot_handler...\n");
	uint32_t event = (uint32_t)arg;
	uint32_t channel = event & 0x1;
	/* TODO: remove is garbage */
	// EOT is simply 22 + id in GAP8
	uint32_t periph_id = (event - SOC_EVENT_UDMA_SPIM_EOT(0));
	DBG_PRINTF("%s:%s:%d periph_id=%u\n", __FILE__, __func__, __LINE__, periph_id);

	struct spim_driver_data *drv_data = __g_spim_drv_data[periph_id];

	if (drv_data->repeat_transfer) {
		DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		// TODO: reenqueue the rest of the transfer
		DBG_PRINTF("Large transfers (>8k) are not implemented yet\n");
		return;
	}
	pi_task_t *task = drv_data->end_of_transfer;
	DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	if (task != NULL) {
		if (task->id == PI_TASK_NONE_ID) {
			DBG_PRINTF("%s:%s:%d release task %p\n", __FILE__, __func__, __LINE__,
				   task);
			pi_task_release(task);
		} else {
			/* TODO: hacked away */
			/* DBG_PRINTF("%s:%d push task %p with id:%x in
			 * sched%p\n", */
			/* 	   __func__, __LINE__, task, task->id, */
			/* 	   default_sched); */
			DBG_PRINTF("%s:%s:%d periph id:%" PRIu32 "\n", __FILE__, __func__, __LINE__,
				   periph_id);
			/* TODO: hacked away */
			/* pmsis_event_push(default_sched, task); */
			DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		}
		drv_data->end_of_transfer = NULL;
	}
#ifdef DEBUG
	else {
		DBG_PRINTF("%s:%s:%d next task %d\n", __FILE__, __func__, __LINE__,
			   task == NULL ? 0 : 1);
	}
#endif
	// I put the next item on the list at the top of the list.
	task = __pi_spim_drv_fifo_pop(drv_data);

	DBG_PRINTF("%s:%s:%d next task %d\n", __FILE__, __func__, __LINE__, task == NULL ? 0 : 1);
	DBG_PRINTF("%s:%s:%d __pi_spim_drv_fifo_pop(%p)\n", __FILE__, __func__, __LINE__, drv_data);
	DBG_PRINTF("%s:%s:%d new task %p\n", __FILE__, __func__, __LINE__, &task);
	if (task) {
		__pi_spim_exec_next_transfer(task);
		DBG_PRINTF("%s:%s:%d __pi_spim_exec_next_transfer(%p)\n", __FILE__, __func__,
			   __LINE__, task);
	}
	DBG_PRINTF("...end -> spim_eot_handler...\n");
}

/* TODO: REMOVE THOSE */
void spim_tx_handler(void *arg)
{ // if we're here, it's cs keep related
	DBG_PRINTF("...start -> spim_tx_handler...\n");
	uint32_t event = (uint32_t)arg;
	uint32_t channel = event & 0x1;
	uint32_t periph_id = (event >> UDMA_CHANNEL_NB_EVENTS_LOG2) - UDMA_SPIM_ID(0);
	hal_soc_eu_clear_fc_mask(SOC_EVENT_UDMA_SPIM_TX(periph_id));
	arg = (void *)(SOC_EVENT_UDMA_SPIM_EOT(0) + periph_id);
	DBG_PRINTF("%s:%s:%d periph_id %" PRIu32 " arg:%p\n", __FILE__, __func__, __LINE__,
		   periph_id, arg);
	spim_eot_handler(arg);
	DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	DBG_PRINTF("...end -> spim_tx_handler...\n");
}

/* TODO: REMOVE THOSE and the handler */
void spim_rx_handler(void *arg)
{ // if we're here, it's cs keep related
	DBG_PRINTF("...start -> spim_rx_handler...\n");
	uint32_t event = (uint32_t)arg;
	uint32_t channel = event & 0x1;
	uint32_t periph_id = (event >> UDMA_CHANNEL_NB_EVENTS_LOG2) - UDMA_SPIM_ID(0);
	hal_soc_eu_clear_fc_mask(SOC_EVENT_UDMA_SPIM_RX(periph_id));
	arg = (void *)(SOC_EVENT_UDMA_SPIM_EOT(0) + periph_id);
	DBG_PRINTF("%s:%s:%d periph_id %" PRIu32 " arg:%p\n", __FILE__, __func__, __LINE__,
		   periph_id, arg);
	spim_eot_handler(arg);
	DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	DBG_PRINTF("...end -> spim_rx_handler...\n");
}

// TODO: prepare pseudo exec for delegate
void __pi_spim_execute_callback(void *arg)
{
	return;
}

void __pi_spim_exec_next_transfer(pi_task_t *task)
{
	DBG_PRINTF("...start -> __pi_spim_exec_next_transfer...\n");
	DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	if (task->data[5] == 1) { // if is send
		// cs data | data buffer | len | flags | end of transfer task
		__pi_spi_send_async((struct spim_cs_data *)task->data[0], (void *)task->data[1],
				    task->data[2], task->data[3], (pi_task_t *)task->data[4]);
	} else if (task->data[5] == 0) {
		// cs data | data buffer | len | flags | end of transfer task
		__pi_spi_receive_async((struct spim_cs_data *)task->data[0], (void *)task->data[1],
				       task->data[2], task->data[3], (pi_task_t *)task->data[4]);
	} else { // task->data[5] contains rx data addr
		// cs data | tx buffer | rx buffer| len | flags | end of
		// transfer task
		__pi_spi_xfer_async((struct spim_cs_data *)task->data[0], (void *)task->data[5],
				    (void *)task->data[1], task->data[2], task->data[3],
				    (pi_task_t *)task->data[4]);
	}
	DBG_PRINTF("...end -> __pi_spim_exec_next_transfer...\n");
}

void __pi_spi_xfer_async(struct spim_cs_data *cs_data, void *tx_data, void *rx_data, size_t len,
			 pi_spi_flags_e flags, pi_task_t *task)
{
	/* TODO: port spi_xfer async */
	abort();
#if 0
    struct spim_driver_data *drv_data = SPIM_CS_DATA_GET_DRV_DATA(cs_data);
    int qspi    = (flags & (0x3 << 2)) == PI_SPI_LINES_QUAD;
    int cs_mode = (flags >> 0) & 0x3;

    int device_id = drv_data->device_id;
    uint32_t cfg = cs_data->cfg;
    DBG_PRINTF("%s:%d: core clock:%"PRIu32", baudrate:%"PRIu32", div=%"PRIu32", udma_cmd cfg =%d\n",
            __func__,__LINE__,system_core_clock_get(),cs_data->max_baudrate,
            system_core_clock_get() / cs_data->max_baudrate,cfg);
    uint32_t byte_align = (cs_data->wordsize == PI_SPI_WORDSIZE_32)
        && cs_data->big_endian;
    int size = (len + 7) >> 3;

    int cmd_id = 0;

    uint32_t irq = deactive_irq();
    if(!drv_data->end_of_transfer)
    {
        cs_data->udma_cmd[0] = cfg;
        cs_data->udma_cmd[1] = SPI_CMD_SOT(cs_data->cs);
        cs_data->udma_cmd[2] = SPI_CMD_FULL_DUPL(len, byte_align);
        drv_data->end_of_transfer = task;
        drv_data->repeat_transfer = NULL;
        if(cs_mode == PI_SPI_CS_AUTO)
        {
            spim_enqueue_channel(SPIM(device_id), (uint32_t)cs_data->udma_cmd,
                    3*(sizeof(uint32_t)), UDMA_CORE_TX_CFG_EN(1),
                    TX_CHANNEL);
            spim_enqueue_channel(SPIM(device_id), (uint32_t)rx_data, size,
                    UDMA_CORE_RX_CFG_EN(1) | (2<<1), RX_CHANNEL);
            spim_enqueue_channel(SPIM(device_id), (uint32_t)tx_data,
                    size, UDMA_CORE_TX_CFG_EN(1),
                    TX_CHANNEL);
            // wait until TX channel is free
            while((hal_read32((void*)&(SPIM(device_id)->udma.tx_cfg))
                        & (1<<5))>>5)
            {
                DBG_PRINTF("%s:%d\n",__func__,__LINE__);
            }
            // send EOT
            cs_data->udma_cmd[3] = SPI_CMD_EOT(1);
            spim_enqueue_channel(SPIM(device_id),
                    (uint32_t)&cs_data->udma_cmd[3], sizeof(uint32_t),
                    UDMA_CORE_TX_CFG_EN(1), TX_CHANNEL);
        }
        else
        {
            spim_enqueue_channel(SPIM(device_id), (uint32_t)cs_data->udma_cmd,
                    3*(sizeof(uint32_t)), UDMA_CORE_TX_CFG_EN(1),
                    TX_CHANNEL);
            // wait until TX channel is free
            while((hal_read32((void*)&(SPIM(device_id)->udma.tx_cfg))
                        & (1<<5))>>5)
            {
                DBG_PRINTF("%s:%d\n",__func__,__LINE__);
            }
            // activate rx event
            hal_soc_eu_set_fc_mask(SOC_EVENT_UDMA_SPIM_RX(device_id));
            // NOTE: both transfers have the same size
            // does not matter which one we wait
            spim_enqueue_channel(SPIM(device_id), (uint32_t)rx_data, size,
                    UDMA_CORE_RX_CFG_EN(1) | (2<<1), RX_CHANNEL);
            spim_enqueue_channel(SPIM(device_id), (uint32_t)tx_data,
                    size, UDMA_CORE_TX_CFG_EN(1),
                    TX_CHANNEL);
        }

    }
    else
    {
        struct spim_transfer transfer;
        transfer.data = rx_data;
        transfer.flags = flags;
        transfer.len = len;
        transfer.cfg_cmd = cfg;
        transfer.byte_align = byte_align;
        transfer.is_send = (uint32_t) tx_data; // sending a pointer means xfer
        __pi_spim_drv_fifo_enqueue(cs_data, &transfer, task);
    }
    active_irq(irq);
#endif
}

void __pi_spi_receive_async_with_ucode(struct spim_cs_data *cs_data, void *data, size_t len,
				       pi_spi_flags_e flags, int ucode_size, void *ucode,
				       pi_task_t *task)
{
	/* TODO: port spi_async with ucode */
	abort();
#if 0
    struct spim_driver_data *drv_data = SPIM_CS_DATA_GET_DRV_DATA(cs_data);
    int qspi    = ((flags >> 2) & 0x3) == ((PI_SPI_LINES_QUAD>>2) & 0x03);
    int cs_mode = (flags >> 0) & 0x3;

    int device_id = drv_data->device_id;
    uint32_t byte_align = (cs_data->wordsize == PI_SPI_WORDSIZE_32)
        && cs_data->big_endian;
    uint32_t cfg = cs_data->cfg;
    DBG_PRINTF("%s:%d: core clock:%d, baudrate:%d, div=%d,  byte_align =%lx, cfg= %lx, qspi=%lx\n",
            __func__,__LINE__,system_core_clock_get(),cs_data->max_baudrate,
            system_core_clock_get() / cs_data->max_baudrate,byte_align,cfg,qspi);
    int size = (len + 7) >> 3;

    int cmd_id = 0;

    uint32_t irq = deactive_irq();
    if(!drv_data->end_of_transfer)
    {
        if(cs_mode != PI_SPI_CS_AUTO)
        {
            hal_soc_eu_set_fc_mask(SOC_EVENT_UDMA_SPIM_RX(device_id));
        }
        drv_data->end_of_transfer = task;
        drv_data->repeat_transfer = NULL;
        if(((0xFFF00000 & (uint32_t)ucode)!= 0x1c000000))
        {
            memcpy(&(cs_data->udma_cmd[0]), ucode, ucode_size);
            spim_enqueue_channel(SPIM(device_id), (uint32_t)data, size,
                    UDMA_CORE_RX_CFG_EN(1) | (2<<1), RX_CHANNEL);
            spim_enqueue_channel(SPIM(device_id), (uint32_t)cs_data->udma_cmd,
                    ucode_size, UDMA_CORE_TX_CFG_EN(1), TX_CHANNEL);
        }
        else
        {
            spim_enqueue_channel(SPIM(device_id), (uint32_t)data, size,
                    UDMA_CORE_RX_CFG_EN(1) | (2<<1), RX_CHANNEL);
            spim_enqueue_channel(SPIM(device_id), (uint32_t)ucode,
                    ucode_size, UDMA_CORE_TX_CFG_EN(1), TX_CHANNEL);
        }
    }
    else
    {
#if 0
        struct spim_transfer transfer;
        transfer.data = data;
        transfer.flags = flags;
        transfer.len = len;
        transfer.cfg_cmd = cfg;
        transfer.byte_align = byte_align;
        transfer.is_send = 0;
        __pi_spim_drv_fifo_enqueue(cs_data, &transfer, task);
#endif
    }
    active_irq(irq);
#endif
}

void __pi_spi_send_async_with_ucode(struct spim_cs_data *cs_data, void *data, size_t len,
				    pi_spi_flags_e flags, int ucode_size, void *ucode,
				    pi_task_t *task)
{
	/* TODO: port spi_async with ucode */
	abort();
#if 0
    struct spim_driver_data *drv_data = SPIM_CS_DATA_GET_DRV_DATA(cs_data);
    int qspi    = ((flags >> 2) & 0x3) == ((PI_SPI_LINES_QUAD>>2) & 0x03);
    int cs_mode = (flags >> 0) & 0x3;

    int device_id = drv_data->device_id;
    uint32_t byte_align = (cs_data->wordsize == PI_SPI_WORDSIZE_32)
        && cs_data->big_endian;
    uint32_t cfg = cs_data->cfg;
    DBG_PRINTF("%s:%d: core clock:%d, baudrate:%d, div=%d,  byte_align =%lx, cfg= %lx, qspi=%lx\n",
            __func__,__LINE__,system_core_clock_get(),cs_data->max_baudrate,
            system_core_clock_get() / cs_data->max_baudrate,byte_align,cfg,qspi);
    int size = (len + 7) >> 3;

    int cmd_id = 0;

    uint32_t irq = deactive_irq();
    if(!drv_data->end_of_transfer)
    {
        if(cs_mode != PI_SPI_CS_AUTO)
        {
            hal_soc_eu_set_fc_mask(SOC_EVENT_UDMA_SPIM_TX(device_id));
        }
        drv_data->end_of_transfer = task;
        drv_data->repeat_transfer = NULL;

        spim_enqueue_channel(SPIM(device_id), (uint32_t)ucode,
                ucode_size, UDMA_CORE_TX_CFG_EN(1), TX_CHANNEL);
        pi_time_wait_us(1000);
        spim_enqueue_channel(SPIM(device_id), (uint32_t)data,
                size, UDMA_CORE_TX_CFG_EN(1), TX_CHANNEL);
        if(cs_mode == PI_SPI_CS_AUTO)
        {
            // wait until channel is free
            while((hal_read32((void*)&(SPIM(device_id)->udma.tx_cfg))
                        & (1<<5))>>5)
            {
                DBG_PRINTF("%s:%d\n",__func__,__LINE__);
            }

            // enqueue EOT
            cs_data->udma_cmd[0] = SPI_CMD_EOT(1);
            spim_enqueue_channel(SPIM(device_id),
                    (uint32_t)&cs_data->udma_cmd[0], 1*(sizeof(uint32_t)),
                    UDMA_CORE_TX_CFG_EN(1), TX_CHANNEL);
        }
    }
    else
    {
#if 0
        struct spim_transfer transfer;
        transfer.data = data;
        transfer.flags = flags;
        transfer.len = len;
        transfer.cfg_cmd = cfg;
        transfer.byte_align = byte_align;
        transfer.is_send = 0;
        __pi_spim_drv_fifo_enqueue(cs_data, &transfer, task);
#endif
    }
    active_irq(irq);
#endif
}

void vApplicationMallocFailedHook(void)
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a
	hook function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task,
	queue, timer or semaphore is created.  It is also called by various
	parts of the demo application.  If heap_1.c or heap_2.c are used, then
	the size of the heap available to pvPortMalloc() is defined by
	configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the
	xPortGetFreeHeapSize() API function can be used to query the size of
	free heap space that remains (although it does not provide information
	on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	printf("error: application malloc failed\n");
	__asm volatile("ebreak");
	for (;;)
		;
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	(void)pcTaskName;
	(void)pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	printf("error: stack overflow\n");
	__asm volatile("ebreak");
	for (;;)
		;
}