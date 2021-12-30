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
 * @repo           :  pulp-sdk/rtos/pulpos/pulp/drivers/spim/common
 * @createdOn      :  28/12/2021
 * @description    :  Common File for Abstraction Layer SPI for PulpOS and FreeRTOS
 *-----------------------------------------------------------------------------------------------------------------------**/

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
#ifdef DEBUG
#define DEBUG_PRINTF printf
#define DBG_PRINTF   printf
#else
#define DEBUG_PRINTF(...) ((void)0)
#define DBG_PRINTF(...)	  ((void)0)
#endif /* DEBUG */

/**================================================================================================
 **                                         STRUCT
 *================================================================================================**/
struct spim_drv_fifo
{
	pi_task_t *fifo_head;
	pi_task_t *fifo_tail;
};

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
uint32_t __pi_spi_get_config(struct spim_cs_data *cs_data);
int32_t __pi_spim_drv_fifo_enqueue(struct spim_cs_data *cs_data, struct spim_transfer *transfer, pi_task_t *end_task);
pi_task_t *__pi_spim_drv_fifo_pop(struct spim_driver_data *data);
struct spim_cs_data *__pi_spim_get_cs_data(struct spim_driver_data *drv_data, int cs);
void __pi_spim_cs_data_del(struct spim_driver_data *drv_data, int cs);
void __pi_spim_cs_data_add(struct spim_driver_data *drv_data, struct spim_cs_data *cs_data);
uint32_t __pi_spi_clk_div_get(uint32_t spi_freq);
uint32_t deactive_irq(void);
void active_irq(uint32_t irq);