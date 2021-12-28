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
 * @author         :  Orlando Nico
 * @email          :  nico.orlando@studio.unibo.it
 * @repo           :  pulp-sdk/rtos/pulpos/pulp/drivers/spi
 * @createdOn      :  11/11/2021
 * @description    :  The driver was tested on a VIP flash memory in RTL, where it was done one
 *                    transfer at a time.
 *                    Multiple concurrent transfers have not been tested. I mean using multiple
 *                    SPI interfaces that do transfers at the same time.
 *-----------------------------------------------------------------------------------------------------------------------**/

/**================================================================================================
 * *                                           INFO
 *      Important definitions:
 *	    pulp-sdk/rtos/pulpos/pulp_archi/include/archi/chips/pulp/properties.h
 *	    pulp-sdk/rtos/pulpos/pulp_archi/include/archi/chips/pulp/memory_map.h
 *================================================================================================**/

/**================================================================================================
 **                                         INCLUDE
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


/**================================================================================================
 **                                         DEFINE
 *================================================================================================**/
/**
#define DEBUG 0
 #ifdef DEBUG
 #define DEBUG_PRINTF //////printf
 #define DBG_PRINTF   //////printf
 #else
 #define DEBUG_PRINTF(...) ((void)0)
 #define DBG_PRINTF(...)	  ((void)0)
 #endif
*/
#define DEBUG_PRINTF(...) ((void)0)
#define DBG_PRINTF(...) ((void)0)

/* TODO: remove this glue */

#define SPIM_CS_DATA_GET_DRV_DATA(cs_data) (cs_data->drv_data)

#define NB_SOC_EVENTS (ARCHI_SOC_EVENT_NB_TOTAL)

typedef void (*pi_fc_event_handler_t)(void *arg);

// va bene per Control-Pulp
/*
** #define pi_default_malloc(x)  malloc(x)
** #define pi_default_free(x,y)  free(x)
** #define pi_data_malloc(x)     malloc(x)
** #define pi_data_free(x,y)     free(x)
*/

// Pulp-Open
#define pi_default_malloc(x) pi_l2_malloc(x)
#define pi_default_free(x, y) pi_l2_free(x, y)
#define pi_data_malloc(x) pi_l2_malloc(x)
#define pi_data_free(x, y) pi_l2_free(x, y)

#define UDMA_EVENT_OFFSET_SPI_EOT 3
#define SOC_EVENT_UDMA_SPIM_EOT(id)                                            \
	((ARCHI_UDMA_SPIM_ID(id) << ARCHI_SOC_EVENT_UDMA_NB_CHANNEL_EVT_LOG2) +                   \
	 UDMA_EVENT_OFFSET_SPI_EOT)
/**================================================================================================
 **                                         STRUCT
 *================================================================================================**/

/* Structure holding infos for each chip selects (itf, cs, polarity etc...) */
struct spim_cs_data
{
	struct spim_cs_data *next;
	struct spim_driver_data *drv_data;
	uint32_t cfg;
	uint32_t udma_cmd[8];
	uint32_t max_baudrate;
	uint32_t polarity;
	uint32_t phase;
	uint8_t cs;
	uint8_t wordsize;
	uint8_t big_endian;
};

struct spim_drv_fifo
{
	pi_task_t *fifo_head;
	pi_task_t *fifo_tail;
};


/* Structure holding info for each interfaces
 * most notably the fifo of enqueued transfers and meta to know whether
 * interface is free or not */
struct spim_driver_data
{
	struct spim_drv_fifo *drv_fifo; // does the same task as Dolphine with true and false
	struct spim_cs_data *cs_list;	// list of devices connected to the spi interface
	pi_task_t *repeat_transfer;
	pi_task_t *end_of_transfer; // gli associo un task per sapere se un trasferimento ha finito?
	uint32_t nb_open;
	uint8_t device_id;
	pos_udma_channel_t *rx_channel;
	pos_udma_channel_t *tx_channel;
};



struct spim_transfer
{
	pi_spi_flags_e flags;
	void *data;
	uint32_t len;
	uint32_t cfg_cmd;
	uint32_t byte_align;
	uint32_t is_send;
};

/**================================================================================================
 **                                         PROTOTYPE FUNCTION
 *================================================================================================**/
void __spim_execute_callback(void *arg);
int __pi_spi_open(struct spim_cs_data **cs_data, struct pi_spi_conf *conf);
void __pi_spi_close(struct spim_cs_data *cs_data, struct pi_spi_conf *conf);
static int32_t __pi_spim_drv_fifo_enqueue(struct spim_cs_data *data, struct spim_transfer *transfer, pi_task_t *end_task);
static inline pi_task_t *__pi_spim_drv_fifo_pop(struct spim_driver_data *data);
/* static inline void __pi_spim_exec_transfer(pi_task_t *task); */
void __pi_spi_send_async(struct spim_cs_data *cs_data, void *data, size_t len, pi_spi_flags_e flags, pi_task_t *task);
void __pi_spi_receive_async(struct spim_cs_data *cs_data, void *data, size_t len, pi_spi_flags_e flags, pi_task_t *task);
void __pi_spi_receive_async_with_ucode(struct spim_cs_data *cs_data, void *data, size_t len, pi_spi_flags_e flags, int ucode_size, void *ucode, pi_task_t *task);
void __pi_spi_send_async_with_ucode(struct spim_cs_data *cs_data, void *data, size_t len, pi_spi_flags_e flags, int ucode_size, void *ucode, pi_task_t *task);
void __pi_spi_xfer_async(struct spim_cs_data *cs_data, void *tx_data, void *rx_data, size_t len, pi_spi_flags_e flags, pi_task_t *task);
void system_core_clock_update(void);
uint32_t system_core_clock_get(void);
void pos_spi_handle_copy(int event, void *arg);
void pos_spi_create_channel(pos_udma_channel_t *channel, int channel_id, int soc_event);

/**================================================================================================
 **                                         GLOBAL VARIABLE
 *================================================================================================**/
volatile uint32_t system_core_clock;
static PI_L2 int spi_open_count = 0;
static PI_L2 int spi_channel;
struct spim_driver_data *__g_spim_drv_data[ARCHI_UDMA_NB_SPIM] = {0};
/**================================================================================================
 **                                         FUNCTION
 *================================================================================================**/

void system_core_clock_update(void)
{
	// system_core_clock = pi_fll_get_frequency(FLL_SOC, 0);
	system_core_clock = pi_freq_get(FLL_SOC);
}

uint32_t system_core_clock_get(void)
{
	system_core_clock_update();
	return system_core_clock;
}

uint32_t deactive_irq(void){
	return hal_irq_disable();
}

void active_irq(uint32_t irq){
	hal_irq_restore(irq);
}

static inline uint32_t __pi_spi_get_config(struct spim_cs_data *cs_data)
{
	return cs_data->cfg;
}

// It is used to put an item in the list. The new item is put at the top of the list.
static inline int32_t __pi_spim_drv_fifo_enqueue(struct spim_cs_data *cs_data,
												 struct spim_transfer *transfer,
												 pi_task_t *end_task)
{
	DBG_PRINTF("%s:%s:%d: ...start -> __pi_spim_drv_fifo_enqueue...\n", __FILE__, __func__, __LINE__);
	DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	uint32_t irq = deactive_irq();
	struct spim_driver_data *drv_data = cs_data->drv_data;
	/* Callback args. */
	end_task->data[0] = (uintptr_t)cs_data;
	end_task->data[1] = (uintptr_t)transfer->data;
	end_task->data[2] = (uintptr_t)transfer->len;
	end_task->data[3] = (uintptr_t)transfer->flags;
	end_task->data[4] = (uintptr_t)end_task;
	end_task->data[5] = (uintptr_t)transfer->is_send;
	end_task->next = NULL;
	/* Enqueue transfer in drv fifo. */
	if (drv_data->drv_fifo->fifo_head == NULL)
	{
		/* Empty fifo. */
		drv_data->drv_fifo->fifo_head = end_task;
		drv_data->drv_fifo->fifo_tail = drv_data->drv_fifo->fifo_head;
	}
	else
	{
		/* Enqueue to tail. */
		drv_data->drv_fifo->fifo_tail->next = end_task;
		drv_data->drv_fifo->fifo_tail =
			drv_data->drv_fifo->fifo_tail->next;
	}
	active_irq(irq);
	DBG_PRINTF("%s:%s:%d: ...end -> __pi_spim_drv_fifo_enqueue...\n", __FILE__, __func__, __LINE__);
	return 0;
}

// I put the next item on the list at the top of the list.
static inline pi_task_t *__pi_spim_drv_fifo_pop(struct spim_driver_data *data)
{
	// DBG_PRINTF("%s:%s:%d: \n", __FILE__, __func__, __LINE__);
	DBG_PRINTF("%s:%s:%d: ...start -> __pi_spim_drv_fifo_pop...\n", __FILE__, __func__, __LINE__);
	DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	pi_task_t *task_return = NULL;
	// clean the value in the position 7 of regiter 0x300
	// irq = 1100010000000
	int check_300 = 0;
	asm volatile("csrr %0, 0x300"
				 : "=r"(check_300));
	DBG_PRINTF("%s:%s:%d Value of register 0x300: 0x%x\n", __FILE__, __func__, __LINE__, check_300);
	uint32_t irq = deactive_irq();
	DBG_PRINTF("%s:%s:%d: ...irq = %u...\n", __FILE__, __func__, __LINE__, irq);
	if (data->drv_fifo->fifo_head != NULL)
	{
		task_return = data->drv_fifo->fifo_head;
		data->drv_fifo->fifo_head = data->drv_fifo->fifo_head->next;
		if (data->drv_fifo->fifo_head == NULL)
		{
			data->drv_fifo->fifo_tail = NULL;
		}
	}
	// write in the 0x300 register
	active_irq(irq);
	DBG_PRINTF("%s:%s:%d: ...end -> __pi_spim_drv_fifo_pop...\n", __FILE__, __func__, __LINE__);
	return task_return;
}

// It goes to find a certain structure in the list through the information on the chip select
static inline struct spim_cs_data *__pi_spim_get_cs_data(struct spim_driver_data *drv_data, int cs)
{
	DBG_PRINTF("...start -> __pi_spim_get_cs_data...\n");
	DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	struct spim_cs_data *cs_cur = drv_data->cs_list;
	while (cs_cur != NULL && cs_cur->cs != cs)
	{
		cs_cur = cs_cur->next;
	}
	DBG_PRINTF("...end -> __pi_spim_get_cs_data...\n");
	return cs_cur;
}

static inline void __pi_spim_cs_data_del(struct spim_driver_data *drv_data,
										 int cs)
{
	DBG_PRINTF("...start -> __pi_spim_cs_data_del...\n");
	DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	struct spim_cs_data *cs_cur = drv_data->cs_list;
	struct spim_cs_data *cs_prev = cs_cur;
	while (cs_cur != NULL && cs_cur->cs != cs)
	{
		cs_prev = cs_cur;
		cs_cur = cs_cur->next;
	}
	if (cs_cur)
	{
		cs_prev->next = cs_cur->next;
		cs_cur->next = NULL;
	}
	DBG_PRINTF("...end -> __pi_spim_cs_data_del...\n");
}

static inline void __pi_spim_cs_data_add(struct spim_driver_data *drv_data, struct spim_cs_data *cs_data)
{
	DBG_PRINTF("...start -> __pi_spim_cs_data_add...\n");
	DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	// head insert, most recently allocated should be most recently used
	cs_data->drv_data = drv_data;
	cs_data->next = drv_data->cs_list;
	DBG_PRINTF("...end -> __pi_spim_cs_data_add...\n");
}

static uint32_t __pi_spi_clk_div_get(uint32_t spi_freq)
{
	DBG_PRINTF("...start -> __pi_spi_clk_div_get...\n");
	DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	uint32_t periph_freq = pi_freq_get(PI_FREQ_DOMAIN_PERIPH);
	DBG_PRINTF("%s:%s:%d periph_freq=%u\n", __FILE__, __func__, __LINE__, periph_freq);
	if (spi_freq < periph_freq)
	{
		uint32_t clk_div = 0;
		clk_div = (periph_freq + spi_freq - 1) / spi_freq;
		if (clk_div & 1)
		{
			clk_div += 1;
		}
		/* The SPIM always divide by 2 once we activate the divider,
	   thus increase by 1 in case it is even to not go above the max
	   frequency. */
		clk_div = clk_div >> 1;
		DBG_PRINTF("...end -> __pi_spi_clk_div_get...\n");
		return clk_div;
	}
	DBG_PRINTF("...end -> __pi_spi_clk_div_get...\n");
	return 0;
}

void pi_spi_conf_init(struct pi_spi_conf *conf)
{
	DBG_PRINTF("...start -> pi_spi_conf_init...\n");
	DBG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	conf->wordsize = PI_SPI_WORDSIZE_8;
	conf->big_endian = 0;
	conf->max_baudrate = 10000000;
	conf->cs = -1;
	conf->itf = 0;
	conf->polarity = 0;
	conf->phase = 0;
	DBG_PRINTF("...end -> pi_spi_conf_init...\n");
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
	if (task->data[5] == 1)
	{ // if is send
		// printf("__pi_spim_exec_next_transfer tx %p\n", &task);
		//  cs data | data buffer | len | flags | end of transfer task
		__pi_spi_send_async((struct spim_cs_data *)task->data[0],
							(void *)task->data[1], task->data[2],
							task->data[3], (pi_task_t *)task->data[4]);
	}
	else if (task->data[5] == 0)
	{
		// printf("__pi_spim_exec_next_transfer rx %p\n", &task);
		//  cs data | data buffer | len | flags | end of transfer task
		__pi_spi_receive_async((struct spim_cs_data *)task->data[0],
							   (void *)task->data[1], task->data[2],
							   task->data[3],
							   (pi_task_t *)task->data[4]);
	}
	else
	{ // task->data[5] contains rx data addr
		// cs data | tx buffer | rx buffer| len | flags | end of
		// transfer task
		__pi_spi_xfer_async((struct spim_cs_data *)task->data[0],
							(void *)task->data[5],
							(void *)task->data[1], task->data[2],
							task->data[3], (pi_task_t *)task->data[4]);
	}
	DBG_PRINTF("...end -> __pi_spim_exec_next_transfer...\n");
}

extern struct pmsis_event_kernel_wrap *default_sched;

void __pi_spi_receive_async(struct spim_cs_data *cs_data, void *data,
							size_t len, pi_spi_flags_e flags, pi_task_t *task)
{
	DBG_PRINTF("...start -> __pi_spi_receive_async...\n");
	// SPIM_CS_DATA_GET_DRV_DATA(cs_data) = (cs_data->drv_data)
	struct spim_driver_data *drv_data = SPIM_CS_DATA_GET_DRV_DATA(cs_data);
	int qspi = (flags & (0x3 << 2)) == PI_SPI_LINES_QUAD;
	// Choose the type of cs mode, see enum "pi_spi_flags_e" to understand better.
	int cs_mode = (flags >> 0) & 0x3;

	int device_id = drv_data->device_id;
	task->id = device_id; //i need it for pos_spi_handle_copy
	uint32_t cfg = cs_data->cfg;
	DBG_PRINTF(
		"%s:%s:%d: core clock:%" PRIu32 ", baudrate:%" PRIu32 ", div=%" PRIu32 ", udma_cmd cfg =%lx, qpi=%d\n",
		__FILE__, __func__, __LINE__, system_core_clock_get(),
		cs_data->max_baudrate,
		system_core_clock_get() / cs_data->max_baudrate, cfg, qspi);
	uint32_t byte_align = (cs_data->wordsize == PI_SPI_WORDSIZE_32) &&
						  cs_data->big_endian;
	
	int buffer_size = (len + 7) >> 3;

	if (len > 8192 * 8)
	{
		DBG_PRINTF(
			"%s:%s:%d: Transaction splitting unimplemented, too large",
			__FILE__, __func__, __LINE__);
		abort(); /* TODO: unimplemented transaction splitting */
	}

	DBG_PRINTF("%s:%s:%d: udma_cmd = %p\n", __FILE__, __func__, __LINE__,
			   &(cs_data->udma_cmd[0]));
	uint32_t irq = deactive_irq();

	uint8_t bitsword = 0;
	uint8_t UDMA_CORE_CFG = 0;

	if (cs_data->wordsize == PI_SPI_WORDSIZE_8)
	{
		bitsword = 8;
		UDMA_CORE_CFG = UDMA_CHANNEL_CFG_SIZE_8;
	}
	else if (cs_data->wordsize == PI_SPI_WORDSIZE_16)
	{
		bitsword = 16;
		UDMA_CORE_CFG = UDMA_CHANNEL_CFG_SIZE_16;
	}
	else
	{
		bitsword = 32;
		UDMA_CORE_CFG = UDMA_CHANNEL_CFG_SIZE_32;
	}

	DBG_PRINTF("%s:%s:%d: bitsword = %u\n", __FILE__, __func__, __LINE__, bitsword);
	DBG_PRINTF("%s:%s:%d: UDMA_CORE_CFG = %u\n", __FILE__, __func__, __LINE__, UDMA_CORE_CFG);

	/*
	** If I have no transfer in progress, then I go to set the command buffer
	** and then first I send the buffer where to receive the data and then the write command.
	** However, if I have some transfer in progress, then I put the new transfer
	** in the queue in the fifo and send it as soon as possible.
	*/

	cs_data->udma_cmd[0] = cfg;
	cs_data->udma_cmd[1] = SPI_CMD_SOT(cs_data->cs);
	cs_data->udma_cmd[2] = SPI_CMD_RX_DATA(len / bitsword, SPI_CMD_1_WORD_PER_TRANSF, bitsword, qspi, SPI_CMD_MSB_FIRST);
	cs_data->udma_cmd[3] = SPI_CMD_EOT(1, cs_mode == PI_SPI_CS_KEEP);

	uint32_t cfg_cmd = UDMA_CHANNEL_CFG_SIZE_32;
	uint32_t rx_conf = UDMA_CORE_CFG;
	if (drv_data->rx_channel->pendings[0] == NULL)
	{
		drv_data->rx_channel->pendings[0] = task;
		plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(device_id), (uint32_t)cs_data->udma_cmd, 4 * sizeof(uint32_t), UDMA_CHANNEL_CFG_EN | cfg_cmd);
		plp_udma_enqueue(UDMA_SPIM_RX_ADDR(device_id), (uint32_t)data, buffer_size, UDMA_CHANNEL_CFG_EN | rx_conf);
	}
	else
	{
		if (drv_data->rx_channel->pendings[1] == NULL)
		{
			drv_data->rx_channel->pendings[1] = task;
			plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(device_id), (uint32_t)cs_data->udma_cmd, 4 * sizeof(uint32_t), UDMA_CHANNEL_CFG_EN | cfg_cmd);
			plp_udma_enqueue(UDMA_SPIM_RX_ADDR(device_id), (uint32_t)data, buffer_size, UDMA_CHANNEL_CFG_EN | rx_conf);
		}
		else
		{
			// printf("else rx\n");
			struct spim_transfer transfer;
			transfer.data = data;
			transfer.flags = flags;
			transfer.len = len;
			transfer.cfg_cmd = cfg;
			transfer.byte_align = byte_align;
			transfer.is_send = 0;
			drv_data->rx_channel->waitings_first=task; //i need it for pos_spi_handle_copy
			__pi_spim_drv_fifo_enqueue(cs_data, &transfer, task);
		}
	}
	active_irq(irq);
	DBG_PRINTF("...end -> __pi_spi_receive_async...\n");
}

void __pi_spi_receive_async_with_ucode(struct spim_cs_data *cs_data, void *data,
									   size_t len, pi_spi_flags_e flags,
									   int ucode_size, void *ucode,
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

void __pi_spi_send_async_with_ucode(struct spim_cs_data *cs_data, void *data,
									size_t len, pi_spi_flags_e flags,
									int ucode_size, void *ucode,
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

void __pi_spi_send_async(struct spim_cs_data *cs_data, void *data, size_t len,
						 pi_spi_flags_e flags, pi_task_t *task)
{
	DBG_PRINTF("...start -> __pi_spi_send_async...\n");
	struct spim_driver_data *drv_data = SPIM_CS_DATA_GET_DRV_DATA(cs_data);
	int qspi = (flags & (0x3 << 2)) == PI_SPI_LINES_QUAD;
	int cs_mode = (flags >> 0) & 0x3;
	DBG_PRINTF("%s:%s:%d...task %d, %p...\n", __FILE__, __func__, __LINE__, task == NULL ? 0 : 1, &task);
	int device_id = drv_data->device_id;
	task->id = device_id; //i need it for pos_spi_handle_copy
	uint32_t cfg = cs_data->cfg; // SPI_CMD_CFG(...)
	DBG_PRINTF(
		"%s:%s:%d: core clock:%" PRIu32 ", baudrate:%" PRIu32 ", div=%" PRIu32 ", udma_cmd cfg =%lx, qpi=%d\n",
		__FILE__, __func__, __LINE__, system_core_clock_get(),
		cs_data->max_baudrate,
		system_core_clock_get() / cs_data->max_baudrate, cfg, qspi);
	uint32_t byte_align = (cs_data->wordsize == PI_SPI_WORDSIZE_32) &&
						  cs_data->big_endian;

	/* buffer size */
	int buffer_size = (len + 7) >> 3;

	if (len > 8192 * 8)
	{
		DBG_PRINTF(
			"%s:%s:%d: Transaction splitting unimplemented, too large",
			__FILE__, __func__, __LINE__);
		abort(); /* TODO: unimplemented transaction splitting */
	}

	// Address of the command buffer to be sent to the uDMA
	DBG_PRINTF("%s:%s:%d: udma_cmd = %p\n", __FILE__, __func__, __LINE__,
			   &(cs_data->udma_cmd[0]));
	uint32_t irq = deactive_irq();

	uint8_t bitsword = 0;
	uint8_t UDMA_CORE_CFG = 0;

	if (cs_data->wordsize == PI_SPI_WORDSIZE_8)
	{
		bitsword = 8;
		UDMA_CORE_CFG = UDMA_CHANNEL_CFG_SIZE_8; // 0x0
	}
	else if (cs_data->wordsize == PI_SPI_WORDSIZE_16)
	{
		bitsword = 16;
		UDMA_CORE_CFG = UDMA_CHANNEL_CFG_SIZE_16; // 0x1
	}
	else
	{
		bitsword = 32;
		UDMA_CORE_CFG = UDMA_CHANNEL_CFG_SIZE_32; // 0x2
	}

	DBG_PRINTF("%s:%s:%d: bitsword = %u\n", __FILE__, __func__, __LINE__, bitsword);
	DBG_PRINTF("%s:%s:%d: UDMA_CORE_CFG = %u\n", __FILE__, __func__, __LINE__, UDMA_CORE_CFG);

	/*
	** If I have no transfer in progress, then I go to set the command buffer
	** and then I first send the command and then the data.
	** However, if I have some transfer in progress, then I put the new transfer
	** in the queue in the fifo and send when I receive an EOT interrupt,
	**	in this case the interrupt handler will manage the new transfer.
	*/
	cs_data->udma_cmd[0] = cfg;
	cs_data->udma_cmd[1] = SPI_CMD_SOT(cs_data->cs);
	cs_data->udma_cmd[2] = SPI_CMD_TX_DATA((len / bitsword), SPI_CMD_1_WORD_PER_TRANSF, bitsword, qspi, SPI_CMD_MSB_FIRST);
	cs_data->udma_cmd[3] = SPI_CMD_EOT(1, cs_mode == PI_SPI_CS_KEEP);

	uint32_t cfg_cmd = UDMA_CHANNEL_CFG_SIZE_32;
	uint32_t tx_conf = UDMA_CORE_CFG;

	if (drv_data->tx_channel->pendings[0] == NULL)
	{
		drv_data->tx_channel->pendings[0] = task;
		plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(device_id), (uint32_t)cs_data->udma_cmd, 4 * sizeof(uint32_t), UDMA_CHANNEL_CFG_EN | cfg_cmd);
		plp_udma_enqueue(UDMA_SPIM_TX_ADDR(device_id), (uint32_t)data, buffer_size, UDMA_CHANNEL_CFG_EN | tx_conf);
	}
	else
	{
		if (drv_data->tx_channel->pendings[1] == NULL)
		{
			drv_data->tx_channel->pendings[1] = task;
			plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(device_id), (uint32_t)cs_data->udma_cmd, 4 * sizeof(uint32_t), UDMA_CHANNEL_CFG_EN | cfg_cmd);
			plp_udma_enqueue(UDMA_SPIM_TX_ADDR(device_id), (uint32_t)data, buffer_size, UDMA_CHANNEL_CFG_EN | tx_conf);
		}
		else
		{ 	
			/* a transfer is running, append to pendings transfers queue */
			struct spim_transfer transfer;
			transfer.data = data;
			transfer.flags = flags;
			transfer.len = len;
			transfer.cfg_cmd = cfg;
			transfer.byte_align = byte_align;
			transfer.is_send = 1;
			drv_data->tx_channel->waitings_first=task; //i need it for pos_spi_handle_copy
			__pi_spim_drv_fifo_enqueue(cs_data, &transfer, task);
		}
	}
	active_irq(irq);

	DBG_PRINTF("...end -> __pi_spi_send_async...\n");
}

void __pi_spi_xfer_async(struct spim_cs_data *cs_data, void *tx_data,
						 void *rx_data, size_t len, pi_spi_flags_e flags,
						 pi_task_t *task)
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

void pos_spi_handle_copy(int event, void *arg)
{
	pos_udma_channel_t *channel = arg;
	pi_task_t *pending_1 = channel->pendings[1];
	pi_task_t *pending_0 = channel->pendings[0];
	uint32_t spi_id = channel->base;
	struct spim_driver_data *drv_data = __g_spim_drv_data[spi_id];
	pi_task_t *pending_first = __pi_spim_drv_fifo_pop(drv_data);
	channel->pendings[0] = pending_1;
	channel->pendings[1] = NULL;

	if (pending_first)
	{
		__pi_spim_exec_next_transfer(pending_first);
		pending_first = NULL;
	}

	pos_task_push_locked(pending_0);
}

void pos_spi_create_channel(pos_udma_channel_t *channel, int channel_id, int soc_event)
{
	pos_soc_event_register_callback(soc_event, pos_spi_handle_copy, (void *)channel);
	channel->pendings[0] = NULL;
	channel->pendings[1] = NULL;
	channel->waitings_first = NULL;
	channel->base = 0;
}

int pi_spi_open(struct pi_device *device)
{
	int status = -1;
	status = __pi_spi_open((struct spim_cs_data **)(&device->data), (struct pi_spi_conf *)device->config);
	return status;
}

int __pi_spi_open(struct spim_cs_data **cs_data, struct pi_spi_conf *conf)
{
	int irq = deactive_irq();
	for (int i = 0; i < ARCHI_NB_FLL; i++)
	{
		pos_fll_init(i);
	}

	//struct pi_spi_conf *conf = (struct pi_spi_conf *)device->config;

	unsigned char spi_id = conf->itf;
	int periph_id = ARCHI_UDMA_SPIM_ID(spi_id); 
	spi_channel = UDMA_EVENT_ID(periph_id);
	int cs = conf->cs;
	int status = 0;
	//struct spim_cs_data **cs_data = (struct spim_cs_data **)(&device->data);
	plp_udma_cg_set(plp_udma_cg_get() | (1 << periph_id));

	struct spim_driver_data *drv_data;
	if (__g_spim_drv_data[conf->itf])
	{
		drv_data = __g_spim_drv_data[conf->itf];
	}
	else
	{
		__g_spim_drv_data[conf->itf] = pi_default_malloc(sizeof(struct spim_driver_data));
		memset(__g_spim_drv_data[conf->itf], 0, sizeof(struct spim_driver_data));
		drv_data = __g_spim_drv_data[conf->itf];
		// Do this to define a node in a list. The list is a dynamic object.
		drv_data->drv_fifo = pi_default_malloc(sizeof(struct spim_drv_fifo));
		memset(drv_data->drv_fifo, 0, sizeof(struct spim_drv_fifo));
		// controllo che il puntatore sia = 0
		if (!drv_data->drv_fifo)
		{
			active_irq(irq);
			return -1;
		}
		drv_data->device_id = conf->itf; 
	}
	   
	if (drv_data->nb_open == 0)
	{
		pos_spi_create_channel(drv_data->rx_channel, UDMA_CHANNEL_ID(periph_id), SOC_EVENT_UDMA_SPIM_EOT(drv_data->device_id));
		pos_spi_create_channel(drv_data->tx_channel, UDMA_CHANNEL_ID(periph_id) + 1, SOC_EVENT_UDMA_SPIM_EOT(drv_data->device_id));
		drv_data->rx_channel->base=spi_id; //way to save me the spi interface which is associated with the channel
		drv_data->tx_channel->base=spi_id; //way to save me the spi interface which is associated with the channel
	}

	soc_eu_fcEventMask_setEvent(SOC_EVENT_UDMA_SPIM_EOT(drv_data->device_id));

	drv_data->nb_open++;

	*cs_data = __pi_spim_get_cs_data(drv_data, conf->cs);

	if (!*cs_data)
	{ // if (*cs_data == 0)
		uint32_t clk_div = __pi_spi_clk_div_get(conf->max_baudrate);
		// alloc a cs data, need to be in udma reachable ram
		struct spim_cs_data *_cs_data = pi_data_malloc(sizeof(struct spim_cs_data));
		if (_cs_data == NULL)
		{
			DBG_PRINTF("[%s] _cs_data alloc failed\n", __func__);
			active_irq(irq);
			return -2;
		}
		if (clk_div > 0xFF)
		{
			DBG_PRINTF(
				"[%s] clk_div, %" PRIu32 ", does not fit into 8 bits. SoC frequency too high.\n",
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

	active_irq(irq);

	return status;
}

void pi_spi_close(struct pi_device *device)
{
	__pi_spi_close((struct spim_cs_data *)(device->data), (struct pi_spi_conf *)device->config);
}

void __pi_spi_close(struct spim_cs_data *cs_data, struct pi_spi_conf *conf)
{
	DBG_PRINTF("...start -> pi_spi_close...\n");

	//struct pi_spi_conf *conf = (struct pi_spi_conf *)device->config;
	uint32_t irq = deactive_irq();
	unsigned char spi_id = conf->itf;
	int periph_id = ARCHI_UDMA_SPIM_ID(spi_id);
	int spi_channel = UDMA_EVENT_ID(periph_id);
	/* TODO: paste beg */
	//struct spim_cs_data *cs_data = device->data;
	struct spim_driver_data *drv_data = cs_data->drv_data;
	__pi_spim_cs_data_del(drv_data, cs_data->cs);
	/*
	** Number of open SPI interfaces
	*/
	drv_data->nb_open--;

	/*
	** If you no longer have any SPI interface open,
	** then go clear the memory, with free, and clear the interrupt line.
	*/
	if (drv_data->nb_open == 0)
	{
		/* reactivate clock gating for said device */
		plp_udma_cg_set(plp_udma_cg_get() & ~(1 << periph_id));
		soc_eu_fcEventMask_clearEvent(SOC_EVENT_UDMA_SPIM_EOT(drv_data->device_id));
		// hal_soc_eu_clear_fc_mask( SOC_EVENT_UDMA_SPIM_EOT(drv_data->device_id));
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

void pi_spi_ioctl(struct pi_device *device, uint32_t cmd, void *arg)
{
	/* TODO */
}

void pi_spi_send(struct pi_device *device, void *data, size_t len, pi_spi_flags_e flag)
{
	pi_task_t task_block;
	pi_task_block(&task_block);
	pi_spi_send_async(device, data, len, flag, &task_block);
	pi_task_wait_on(&task_block);
	pi_task_destroy(&task_block);
}

void pi_spi_send_async(struct pi_device *device, void *data, size_t len, pi_spi_flags_e flag, pi_task_t *task)
{
	DEBUG_PRINTF("...start -> pi_spi_send_async...\n");
	__pi_spi_send_async(device->data, data, len, flag, task);
	DEBUG_PRINTF("...end -> pi_spi_send_async...\n");
}

void pi_spi_receive(struct pi_device *device, void *data, size_t len, pi_spi_flags_e flag)
{
	DEBUG_PRINTF("...start -> pi_spi_receive...\n");
	pi_task_t task_block;
	pi_task_block(&task_block);
	DEBUG_PRINTF("%s:%s:%d pi_task_block(%p)\n", __FILE__, __func__, __LINE__, &task_block);
	pi_spi_receive_async(device, data, len, flag, &task_block);
	DEBUG_PRINTF("%s:%s:%d pi_spi_receive_async(device, data, len, flag, &task_block)\n", __FILE__, __func__, __LINE__);
	// This function allows to wait on an event task until the event occurs.
	pi_task_wait_on(&task_block);
	DEBUG_PRINTF("%s:%s:%d pi_task_wait_on(%p)\n", __FILE__, __func__, __LINE__, &task_block);
	pi_task_destroy(&task_block);
	DEBUG_PRINTF("%s:%s:%d pi_task_destroy(%p)\n", __FILE__, __func__, __LINE__, &task_block);
	DEBUG_PRINTF("...end -> pi_spi_receive...\n");
}

void pi_spi_receive_async(struct pi_device *device, void *data, size_t len,
						  pi_spi_flags_e flag, pi_task_t *task)
{
	DEBUG_PRINTF("...start -> pi_spi_receive_async...\n");
	__pi_spi_receive_async(device->data, data, len, flag, task);
	DEBUG_PRINTF("...end -> pi_spi_receive_async...\n");
}

void pi_spi_receive_with_ucode(struct pi_device *device, void *data, size_t len,
							   pi_spi_flags_e flags, int ucode_size,
							   void *ucode)
{
	pi_task_t task_block;
	pi_task_block(&task_block);
	__pi_spi_receive_async_with_ucode(device->data, data, len, flags,
									  ucode_size, ucode, &task_block);
	pi_task_wait_on(&task_block);
	pi_task_destroy(&task_block);
}

void pi_spi_send_with_ucode(struct pi_device *device, void *data, size_t len,
							pi_spi_flags_e flags, int ucode_size, void *ucode)
{
	pi_task_t task_block;
	pi_task_block(&task_block);
	__pi_spi_send_async_with_ucode(device->data, data, len, flags,
								   ucode_size, ucode, &task_block);
	pi_task_wait_on(&task_block);
	pi_task_destroy(&task_block);
}

uint32_t pi_spi_get_config(struct pi_device *device)
{
	return __pi_spi_get_config(device->data);
}

void pi_spi_transfer(struct pi_device *device, void *tx_data, void *rx_data,
					 size_t len, pi_spi_flags_e flag)
{

	/* TODO */
	pi_task_t task_block;
	pi_task_block(&task_block);
	DEBUG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	pi_spi_transfer_async(device, tx_data, rx_data, len, flag, &task_block);
	DEBUG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	pi_task_wait_on(&task_block);
	DEBUG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	pi_task_destroy(&task_block);
	DEBUG_PRINTF("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

void pi_spi_transfer_async(struct pi_device *device, void *tx_data,
						   void *rx_data, size_t len, pi_spi_flags_e flag,
						   pi_task_t *task)
{
	/* TODO */
	__pi_spi_xfer_async(device->data, tx_data, rx_data, len, flag, task);
}
