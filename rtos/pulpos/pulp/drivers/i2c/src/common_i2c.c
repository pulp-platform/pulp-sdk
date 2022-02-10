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
 * @repo           :  freertos/drivers or pulp-sdk/rtos/pulpos/pulp/drivers/i2c/src
 * @createdOn      :  /02/2022
 * @description    :  The driver was tested on a VIP eeprom memory in RTL, where it was done one
 *                    transfer at a time.
 *                    Multiple concurrent transfers have not been tested. I mean using multiple
 *                    I2C interfaces that do transfers at the same time.
 * 					  The tests the driver has been tested on are: i2c_scan, i2c_eeprom_async, i2c_eeprom_sync.
 *-----------------------------------------------------------------------------------------------------------------------**/

/**================================================================================================
 **                                         INCLUDE
 *================================================================================================**/
#include "common_i2c.h"

/**================================================================================================
 **                                         FUNCTION
 *================================================================================================**/
uint32_t deactive_irq_i2c(void)
{
#ifdef USE_PULPOS_TEST
	return hal_irq_disable();
#else
	return __disable_irq();
#endif
}

void active_irq_i2c(uint32_t irq)
{
#ifdef USE_PULPOS_TEST
	hal_irq_restore(irq);
#else
	__restore_irq(irq);
#endif
}

void __pi_i2c_wait_transfer(uint32_t device_id){
#ifdef USE_PULPOS_TEST
	while (plp_udma_busy(UDMA_I2C_CMD_ADDR(device_id)));
#else
	while (hal_i2c_busy_get(device_id));
#endif
	
}

void __pi_i2c_handle_pending_transfer(struct i2c_itf_data_s *driver_data)
{
	struct i2c_pending_transfer_s *pending = driver_data->pending;
	pending->pending_buffer += pending->pending_repeat;
	pending->pending_repeat_size -= pending->pending_repeat;
	pending->pending_size = pending->pending_repeat;

	if (pending->pending_repeat_size <= pending->pending_repeat) {
		pending->pending_repeat = 0;
		pending->pending_size = pending->pending_repeat_size;
		/* Stop bit at the end? */
		driver_data->i2c_stop_send = (pending->flags & PI_I2C_XFER_NO_STOP) ? 0 : 1;
	}
	/* Initiate next part of command sequence. */
	{
		/* Data. */
		uint32_t index = 0;
		driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_RPT) << 24) | pending->pending_size;
		driver_data->i2c_cmd_seq[index++] = (((uint32_t)I2C_CMD_WRB) << 24);
	}
	// hal_i2c_enqueue(device_id, driver_data->channel);
	/* TODO: Renqueue next cmd! */
}


int32_t __pi_i2c_cb_buf_empty(struct i2c_itf_data_s *driver_data)
{
	return driver_data->buf[0] == NULL;
}

void __pi_i2c_cb_buf_enqueue(struct i2c_itf_data_s *driver_data, struct pi_task *task)
{
	uint32_t irq = deactive_irq_i2c();
	driver_data->buf[0] = task;
	active_irq_i2c(irq);
}

struct pi_task *__pi_i2c_cb_buf_pop(struct i2c_itf_data_s *driver_data)
{
	uint32_t irq = deactive_irq_i2c();
	struct pi_task *task_to_return = NULL;
	task_to_return = driver_data->buf[0];
	/* Free the slot for another transfer. */
	driver_data->buf[0] = NULL;
	active_irq_i2c(irq);
	return task_to_return;
}

void __pi_i2c_task_fifo_enqueue(struct i2c_itf_data_s *driver_data, struct pi_task *task)
{
	uint32_t irq = deactive_irq_i2c();
	/* Enqueue transfer in SW fifo. */
	if (driver_data->fifo_head == NULL) {
		driver_data->fifo_head = task;
	} else {
		driver_data->fifo_tail->next = task;
	}
	driver_data->fifo_tail = task;
	active_irq_i2c(irq);
}

struct pi_task *__pi_i2c_task_fifo_pop(struct i2c_itf_data_s *driver_data)
{
	struct pi_task *task_to_return = NULL;
	uint32_t irq = deactive_irq_i2c();
	if (driver_data->fifo_head != NULL) {
		task_to_return = driver_data->fifo_head;
		driver_data->fifo_head = driver_data->fifo_head->next;
	}
	active_irq_i2c(irq);
	return task_to_return;
}

uint32_t __pi_i2c_clk_div_get(uint32_t i2c_freq)
{
	/* Clock divided by 4 in HW. */
	uint32_t freq = i2c_freq << 2;
	uint32_t periph_freq = pi_freq_get(PI_FREQ_DOMAIN_PERIPH);
	uint32_t div = (periph_freq + freq - 1) / freq;
	/* Clock divider counts from 0 to clk_div value included. */
	if (div <= 1) {
		div = 0;
	} else {
		div -= 1;
	}
	if (div > 0xFFFF) {
		I2C_TRACE_ERR("Error computing clock divier : Fsoc=%ld, Fi2c=%ld\n", periph_freq,
			      i2c_freq);
		return 0xFFFFFFFF;
	}
	return div;
}

void __pi_i2c_cs_data_add(struct i2c_itf_data_s *driver_data, struct i2c_cs_data_s *cs_data)
{
#ifdef USE_FREERTOS_TEST	
	struct i2c_cs_data_s *head = driver_data->cs_list;
	while (head != NULL) {
		head = head->next;
	}
	head->next = cs_data;
#else
	struct i2c_cs_data_s *head;
	head = cs_data;
    head->next = driver_data->cs_list;
    driver_data->cs_list = head;
#endif
}

void __pi_i2c_cs_data_remove(struct i2c_itf_data_s *driver_data, struct i2c_cs_data_s *cs_data)
{
	struct i2c_cs_data_s *head = driver_data->cs_list;
	struct i2c_cs_data_s *prev = driver_data->cs_list;
#ifdef USE_FREERTOS_TEST	
	while ((head != NULL) && (head != cs_data)) {
		prev = head;
		hal_compiler_barrier();
		head = head->next;
	}
	if (head != NULL) {
		prev->next = head->next;
	}
#else
	int count=0;
	while ((head != NULL) && (head != cs_data)) 
	{
        count++;
		prev = head;
		hal_compiler_barrier();
		head = head->next;
	}
	if (head != NULL) 
	{
		prev->next = head->next;
	}
    if(count==0){
        head=head->next;
        driver_data->cs_list=head;
	}
#endif
}

void __pi_i2c_freq_cb(void *args)
{
	uint32_t irq = deactive_irq_i2c();
	struct i2c_itf_data_s *driver_data = (struct i2c_itf_data_s *)args;
	uint32_t device_id = driver_data->device_id;
	struct i2c_cs_data_s *cs_data = driver_data->cs_list;

	/* Wait until current transfer is done. */
	__pi_i2c_wait_transfer(device_id);

	/* Update all clock div. */
	while (cs_data != NULL) {
		cs_data->clk_div = __pi_i2c_clk_div_get(cs_data->max_baudrate);
		cs_data = cs_data->next;
	}
	active_irq_i2c(irq);
}

int32_t __pi_i2c_baudrate_set(struct i2c_cs_data_s *cs_data, uint32_t new_baudrate)
{
	cs_data->max_baudrate = new_baudrate;
	uint32_t clk_div = __pi_i2c_clk_div_get(cs_data->max_baudrate);
	if (clk_div == 0xFFFFFFFF) {
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

void __pi_i2c_ioctl(struct i2c_cs_data_s *device_data, uint32_t cmd, void *arg)
{
	switch (cmd) {
	case PI_I2C_CTRL_SET_MAX_BAUDRATE:
		__pi_i2c_baudrate_set(device_data, (uint32_t)arg);
		break;
	default:
		break;
	}
	return;
}
