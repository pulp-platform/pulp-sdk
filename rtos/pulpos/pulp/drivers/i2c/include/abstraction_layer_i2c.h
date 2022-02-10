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
 * @repo           :  pulp-sdk/rtos/pulpos/pulp/drivers/i2c/include
 * @createdOn      :  /01/2022
 * @description    :  PulpOS
 *      			  The driver was tested on a VIP flash memory in RTL, where it was done one
 *                    transfer at a time.
 *                    Multiple concurrent transfers have not been tested. I mean using multiple
 *                    I2C interfaces that do transfers at the same time.
 *-----------------------------------------------------------------------------------------------------------------------**/

/**================================================================================================
 **                                         INCLUDE
 *================================================================================================**/
#include "common_i2c.h"

/**================================================================================================
 **                                         DEFINE
 *================================================================================================**/
/* I2C */
/* Events offsets. */
#define UDMA_EVENT_OFFSET_RX (0)
#define UDMA_EVENT_OFFSET_TX (1)
#define UDMA_EVENT_OFFSET_CMD (2)
#define UDMA_EVENT_OFFSET_EOT (3)

#define MUTEX 1

/* Number of the interrupt to be obtained based on the number of the interface */
#define SOC_EVENT_UDMA_I2C_RX(id)                                          \
	((ARCHI_UDMA_I2C_ID(id) << ARCHI_SOC_EVENT_UDMA_NB_CHANNEL_EVT_LOG2) + \
	 UDMA_EVENT_OFFSET_RX)
#define SOC_EVENT_UDMA_I2C_TX(id)                                          \
	((ARCHI_UDMA_I2C_ID(id) << ARCHI_SOC_EVENT_UDMA_NB_CHANNEL_EVT_LOG2) + \
	 UDMA_EVENT_OFFSET_TX)
#define SOC_EVENT_UDMA_I2C_CMD(id)                                         \
	((ARCHI_UDMA_I2C_ID(id) << ARCHI_SOC_EVENT_UDMA_NB_CHANNEL_EVT_LOG2) + \
	 UDMA_EVENT_OFFSET_CMD)
#define SOC_EVENT_UDMA_I2C_EOT(id)                                         \
	((ARCHI_UDMA_I2C_ID(id) << ARCHI_SOC_EVENT_UDMA_NB_CHANNEL_EVT_LOG2) + \
	 UDMA_EVENT_OFFSET_EOT)
/**================================================================================================
 **                                         STRUCT
 *================================================================================================**/

/**================================================================================================
 **                                         PROTOTYPE FUNCTION
 *================================================================================================**/
/* IRQ handler for tx event */
void __pi_i2c_tx_handler(int event, void *arg);
/* IRQ handler for rx event */
void __pi_i2c_rx_handler(int event, void *arg);
/* IRQ handler for eot event */
void __pi_i2c_eot_handler(int event, void *arg);
/* IRQ handler for cmd event */
void __pi_i2c_cmd_handler(int event, void *arg);
/* Free a task */
void __pi_irq_handle_end_of_task(pi_task_t *task);
/* Copy in UDMA. */
void __pi_i2c_copy(struct i2c_cs_data_s *cs_data, uint32_t l2_buff, uint32_t length, pi_i2c_xfer_flags_e flags, udma_channel_e channel, struct pi_task *task);
/* I create a channel to use uDMA's Tx and RX buffer */
void pos_i2c_create_channel(pos_udma_channel_t *channel, int channel_id, int soc_event);
/* Open i2c device. */
int32_t __pi_i2c_open(struct pi_i2c_conf *conf, struct i2c_cs_data_s **device_data);
/* Close i2c device. */
void __pi_i2c_close(struct i2c_cs_data_s *device_data);
/* API to use the plp_udma_enqueue for tx */
void __pi_i2c_enqueue_tx(uint32_t device_id, uint32_t l2buf, uint32_t size);
/* API to use the plp_udma_enqueue for rx */
void __pi_i2c_enqueue_rx(uint32_t device_id, uint32_t l2buf, uint32_t size);
/* API to use the plp_udma_enqueue for cmd */
void __pi_i2c_enqueue_cmd(uint32_t device_id, uint32_t l2buf, uint32_t size);
/* Initiate and enqueue a read command sequence. */
void __pi_i2c_copy_exec_read(struct i2c_itf_data_s *driver_data, struct pi_task *task);
/* Initiate and enqueue a write command sequence. */
void __pi_i2c_copy_exec_write(struct i2c_itf_data_s *driver_data, struct pi_task *task);
/* Send a eot and stop command sequence. */
void __pi_i2c_send_stop_cmd(struct i2c_itf_data_s *driver_data);
/* Send a only eot command sequence. */
void __pi_i2c_send_only_eot_cmd(struct i2c_itf_data_s *driver_data);
/* Scan i2c bus to detect connected devices. */
int32_t __pi_i2c_detect(struct i2c_cs_data_s *cs_data, struct pi_i2c_conf *conf, uint8_t *rx_data, struct pi_task *task);