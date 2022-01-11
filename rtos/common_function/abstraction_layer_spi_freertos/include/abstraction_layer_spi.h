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
#include "common_spi.h"

/**================================================================================================
 **                                         DEFINE
 *================================================================================================**/
#define UDMA_CORE_TX_CFG_EN(val)                                                                   \
	(((uint32_t)(((uint32_t)(val)) << UDMA_CORE_TX_CFG_EN_SHIFT)) & UDMA_CORE_TX_CFG_EN_MASK)
#define UDMA_CORE_TX_CFG_DATASIZE(val)                                                             \
	(((uint32_t)(((uint32_t)(val)) << UDMA_CORE_TX_CFG_DATASIZE_SHIFT)) &                      \
	 UDMA_CORE_TX_CFG_DATASIZE_MASK)
#define SPIM_CS_DATA_GET_DRV_DATA(cs_data) (cs_data->drv_data)
     
/**================================================================================================
 **                                         STRUCT
 *================================================================================================**/



/**================================================================================================
 **                                         PROTOTYPE FUNCTION
 *================================================================================================**/
void spim_eot_handler(void *arg);
void spim_tx_handler(void *arg);
void spim_rx_handler(void *arg);
int __pi_spi_open(struct spim_cs_data **cs_data, struct pi_spi_conf *conf);
void __pi_spi_close(struct spim_cs_data *cs_data, struct pi_spi_conf *conf);
void __pi_spim_exec_next_transfer(pi_task_t *task);
void __pi_spi_send_async(struct spim_cs_data *cs_data, void *data, size_t len, pi_spi_flags_e flags, pi_task_t *task);
void __pi_spi_receive_async(struct spim_cs_data *cs_data, void *data, size_t len, pi_spi_flags_e flags, pi_task_t *task);
void __pi_spi_xfer_async(struct spim_cs_data *cs_data, void *tx_data, void *rx_data, size_t len, pi_spi_flags_e flags, pi_task_t *task);
void __pi_spi_receive_async_with_ucode(struct spim_cs_data *cs_data, void *data, size_t len, pi_spi_flags_e flags, int ucode_size, void *ucode, pi_task_t *task);
void __pi_spi_send_async_with_ucode(struct spim_cs_data *cs_data, void *data, size_t len, pi_spi_flags_e flags, int ucode_size, void *ucode, pi_task_t *task);
void __spim_execute_callback(void *arg);
void system_core_clock_update(void);
uint32_t system_core_clock_get(void);
void vApplicationMallocFailedHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void system_init(void);