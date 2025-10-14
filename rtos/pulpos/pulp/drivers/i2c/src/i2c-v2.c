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
 * Modify by Nico Orlando (nico.orlando@studio.unibo.it)
 * 
 */
/**-----------------------------------------------------------------------------------------------------------------------
 * ?                                                     ABOUT
 * @author         :  Orlando Nico, GreenWaves Technologies, Robert Balas 
 * @email          :  nico.orlando@studio.unibo.it, balasr@iis.ee.ethz.ch
 * @repo           :  driver
 * @createdOn      :  /01/2022
 * @description    :  FreeRTOS
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
 **                                         DEFINE
 *================================================================================================**/
#define MUTEX 1

/**================================================================================================
 **                                         STRUCT
 *================================================================================================**/

/**================================================================================================
 **                                         GLOBAL VARIABLE
 *================================================================================================**/

/**================================================================================================
 **                                         PROTOTYPE FUNCTION
 *================================================================================================**/
void pi_i2c_conf_init(pi_i2c_conf_t *conf)
{
	__pi_i2c_conf_init(conf);
}

void pi_i2c_conf_set_slave_addr(struct pi_i2c_conf *conf, uint16_t slave_addr, int8_t is_10_bits)
{
	conf->cs = slave_addr;
	conf->is_10_bits = is_10_bits;
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
#if MUTEX
	pi_task_block(&task_block);
	pi_i2c_read_async(device, rx_buff, length, flags, &task_block);
	pi_task_wait_on(&task_block);
	pi_task_destroy(&task_block);
#else
	pi_task_block_no_mutex(&task_block);
	pi_i2c_read_async(device, rx_buff, length, flags, &task_block);
	pi_task_wait_on_no_mutex(&task_block);
#endif
	/* only some udma i2c peripherals support ack detection */
#ifdef CONFIG_UDMA_I2C_ACK
#ifdef USE_FREERTOS_TEST
	struct i2c_cs_data_s *device_data = (struct i2c_cs_data_s *)device->data;
	if (REG_GET(I2C_ACK_NACK, i2c_ack_get(device_data->device_id)))
		status = PI_ERR_I2C_NACK;
#endif
#ifdef USE_PULPOS_TEST
	struct i2c_cs_data_s *device_data = (struct i2c_cs_data_s *)device->data;
	unsigned int udma_i2c_channel_base = hal_udma_channel_base(UDMA_CHANNEL_ID(ARCHI_UDMA_I2C_ID(device_data->device_id)));
	uint32_t ack = pulp_read32(udma_i2c_channel_base+0x38);
	if (ack)
		status = PI_ERR_I2C_NACK;
#endif
#endif
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
#if MUTEX
	pi_task_block(&task_block);
	pi_i2c_write_async(device, tx_data, length, flags, &task_block);
	pi_task_wait_on(&task_block);
	pi_task_destroy(&task_block);
#else
	pi_task_block_no_mutex(&task_block);
	pi_i2c_write_async(device, tx_data, length, flags, &task_block);
	pi_task_wait_on_no_mutex(&task_block);
#endif
	/* only some udma i2c peripherals support ack detection */
#ifdef CONFIG_UDMA_I2C_ACK
#ifdef USE_FREERTOS_TEST
	struct i2c_cs_data_s *device_data = (struct i2c_cs_data_s *)device->data;
	if (REG_GET(I2C_ACK_NACK, i2c_ack_get(device_data->device_id)))
		status = PI_ERR_I2C_NACK;
#endif
#ifdef USE_PULPOS_TEST
	struct i2c_cs_data_s *device_data = (struct i2c_cs_data_s *)device->data;
	unsigned int udma_i2c_channel_base = hal_udma_channel_base(UDMA_CHANNEL_ID(ARCHI_UDMA_I2C_ID(device_data->device_id)));
	uint32_t ack = pulp_read32(udma_i2c_channel_base+0x38);
	if (ack)
		status = PI_ERR_I2C_NACK;
#endif
#endif
	return status;
}

void pi_i2c_write_async(struct pi_device *device, uint8_t *tx_data, int length,
			pi_i2c_xfer_flags_e flags, pi_task_t *task)
{
	struct i2c_cs_data_s *device_data = (struct i2c_cs_data_s *)device->data;
	udma_channel_e channel = COMMAND_CHANNEL;
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

 