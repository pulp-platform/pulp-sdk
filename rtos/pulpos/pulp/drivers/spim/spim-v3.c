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
 *      pulp-sdk/rtos/pulpos/pulp_archi/include/archi/chips/pulp/properties.h
 *      pulp-sdk/rtos/pulpos/pulp_archi/include/archi/chips/pulp/memory_map.h
 *================================================================================================**/

/**================================================================================================
 **                                         INCLUDE
 *================================================================================================**/
#include "abstraction_layer_spi.h"

/**================================================================================================
 **                                         DEFINE
 *================================================================================================**/

/**================================================================================================
 **                                         STRUCT
 *================================================================================================**/

/**================================================================================================
 **                                         PROTOTYPE FUNCTION
 *================================================================================================**/

/**================================================================================================
 **                                         GLOBAL VARIABLE
 *================================================================================================**/

/**================================================================================================
 **                                         FUNCTION
 *================================================================================================**/
uint32_t pi_spi_get_config(struct pi_device *device)
{
    return __pi_spi_get_config(device->data);
}

void pi_spi_conf_init(struct pi_spi_conf *conf)
{
    conf->wordsize = PI_SPI_WORDSIZE_8;
    conf->big_endian = 0;
    conf->max_baudrate = 10000000;
    conf->cs = -1;
    conf->itf = 0;
    conf->polarity = 0;
    conf->phase = 0;
}

int pi_spi_open(struct pi_device *device)
{
    int status = -1;
    status = __pi_spi_open((struct spim_cs_data **)(&device->data), (struct pi_spi_conf *)device->config);
    return status;
}

void pi_spi_close(struct pi_device *device)
{
    __pi_spi_close((struct spim_cs_data *)(device->data), (struct pi_spi_conf *)device->config);
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