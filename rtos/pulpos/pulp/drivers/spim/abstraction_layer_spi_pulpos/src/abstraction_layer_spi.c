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
 * @repo           :  pulp-sdk/rtos/pulpos/pulp/drivers/spim/abstraction_layer_spi
 * @createdOn      :  11/11/2021
 * @description    :  
 *-----------------------------------------------------------------------------------------------------------------------**/

/**================================================================================================
 **                                         INCLUDE
 *================================================================================================**/
#include "abstraction_layer_spi.h"

/**================================================================================================
 **                                         GLOBAL VARIABLE
 *================================================================================================**/
volatile uint32_t system_core_clock;
PI_L2 int spi_open_count = 0;
PI_L2 int spi_channel;
struct spim_driver_data *__g_spim_drv_data[ARCHI_UDMA_NB_SPIM] = {0};
extern struct pmsis_event_kernel_wrap *default_sched;

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


// TODO: prepare pseudo exec for delegate
void __pi_spim_execute_callback(void *arg)
{
    return;
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

void __pi_spi_xfer_async(struct spim_cs_data *cs_data, void *tx_data,  void *rx_data, size_t len, pi_spi_flags_e flags, pi_task_t *task)
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

void __pi_spi_send_async(struct spim_cs_data *cs_data, void *data, size_t len, pi_spi_flags_e flags, pi_task_t *task)
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
    **  in this case the interrupt handler will manage the new transfer.
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
