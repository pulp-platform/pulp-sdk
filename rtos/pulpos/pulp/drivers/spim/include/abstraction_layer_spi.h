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
 * @repo           :  pulp-sdk/rtos/pulpos/pulp/drivers/spim/include
 * @createdOn      :  28/12/2021
 * @description    :  Abstraction Layer SPI for PulpOS
 *-----------------------------------------------------------------------------------------------------------------------**/

/**================================================================================================
 **                                         INCLUDE
 *================================================================================================**/
#include "common_spi.h"

/**================================================================================================
 **                                         DEFINE
 *================================================================================================**/
#define DEBUG_PRINTF(...) ((void)0)
#define DBG_PRINTF(...) ((void)0)
#define SPIM_CS_DATA_GET_DRV_DATA(cs_data) (cs_data->drv_data)
#define NB_SOC_EVENTS (ARCHI_SOC_EVENT_NB_TOTAL)
#define pi_default_malloc(x) pi_l2_malloc(x)
#define pi_default_free(x, y) pi_l2_free(x, y)
#define pi_data_malloc(x) pi_l2_malloc(x)
#define pi_data_free(x, y) pi_l2_free(x, y)
#define UDMA_EVENT_OFFSET_SPI_EOT 3
#define SOC_EVENT_UDMA_SPIM_EOT(id)                                            \
    ((ARCHI_UDMA_SPIM_ID(id) << ARCHI_SOC_EVENT_UDMA_NB_CHANNEL_EVT_LOG2) +                   \
     UDMA_EVENT_OFFSET_SPI_EOT)
typedef void (*pi_fc_event_handler_t)(void *arg);
/**================================================================================================
 **                                         STRUCT
 *================================================================================================**/

/**================================================================================================
 **                                         PROTOTYPE FUNCTION
 *================================================================================================**/
void pos_spi_handle_copy(int event, void *arg);
void pos_spi_create_channel(pos_udma_channel_t *channel, int channel_id, int soc_event);
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
