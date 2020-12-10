/*
 * Copyright (C) 2020 ETH Zurich
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
 */


/* 
 * Authors: Germain Haugou, ETH Zurich (germain.haugou@iis.ee.ethz.ch)
 */

#ifndef __POS_DATA_UART_H__
#define __POS_DATA_UART_H__

#ifndef LANGUAGE_ASSEMBLY

#include "pmsis/task.h"
#include "pmsis/drivers/uart.h"

#if defined(UDMA_UART_VERSION) && UDMA_UART_VERSION == 2

typedef struct pos_uart_s
{
    struct pi_uart_conf conf;
    int open_count;
    int rx_channel_id;
    int tx_channel_id;
    pos_udma_channel_t tx_channel;
    pos_udma_channel_t rx_channel;
} pos_uart_t;

#else

typedef struct pos_uart_s
{    
    int open_count;
    int channel;
    unsigned int baudrate;
    int active;
    pos_udma_channel_t tx_channel;
    pos_udma_channel_t rx_channel;
} pos_uart_t;

#endif

struct pi_cl_uart_req_s {
    pi_device_t *device;
    void *buffer;
    size_t size;
    pi_task_t task;
    int done;
    char cid;
};

#endif

#endif
