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
 * @repo           :  pulp-sdk/rtos/pulpos/pulp/drivers/spim/src
 * @createdOn      :  28/12/2021
 * @description    :  Common File for Abstraction Layer SPI for PulpOS and FreeRTOS
 *-----------------------------------------------------------------------------------------------------------------------**/

/**================================================================================================
 **                                         INCLUDE
 *================================================================================================**/
#include "common_spi.h"

/**================================================================================================
 **                                         FUNCTION
 *================================================================================================**/
uint32_t deactive_irq(void){
#ifdef USE_PULPOS_TEST
    return hal_irq_disable();
#endif
#ifdef USE_FREERTOS_TEST
    return __disable_irq();
#endif
}

void active_irq(uint32_t irq){
#ifdef USE_PULPOS_TEST 
	hal_irq_restore(irq);
#endif 
#ifdef USE_FREERTOS_TEST
    __restore_irq(irq);
#endif 
}

uint32_t __pi_spi_get_config(struct spim_cs_data *cs_data)
{
	return cs_data->cfg;
}

// It is used to put an item in the list. The new item is put at the top of the list.
int32_t __pi_spim_drv_fifo_enqueue(struct spim_cs_data *cs_data,
												 struct spim_transfer *transfer,
												 pi_task_t *end_task)
{
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
	return 0;
}

pi_task_t *__pi_spim_drv_fifo_pop(struct spim_driver_data *data)
{
	// DBG_PRINTF("%s:%s:%d: \n", __FILE__, __func__, __LINE__);
	pi_task_t *task_return = NULL;
	// clean the value in the position 7 of regiter 0x300
	// irq = 1100010000000
	int check_300 = 0;
	asm volatile("csrr %0, 0x300"
				 : "=r"(check_300));
	uint32_t irq = deactive_irq();
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
	return task_return;
}

struct spim_cs_data *__pi_spim_get_cs_data(struct spim_driver_data *drv_data, int cs)
{
	struct spim_cs_data *cs_cur = drv_data->cs_list;
	while (cs_cur != NULL && cs_cur->cs != cs)
	{
		cs_cur = cs_cur->next;
	}
	return cs_cur;
}

void __pi_spim_cs_data_del(struct spim_driver_data *drv_data,
										 int cs)
{
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
}

void __pi_spim_cs_data_add(struct spim_driver_data *drv_data, struct spim_cs_data *cs_data)
{
	// head insert, most recently allocated should be most recently used
	cs_data->drv_data = drv_data;
	cs_data->next = drv_data->cs_list;
}

uint32_t __pi_spi_clk_div_get(uint32_t spi_freq)
{
	uint32_t periph_freq = pi_freq_get(PI_FREQ_DOMAIN_PERIPH);
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
		return clk_div;
	}
	return 0;
}