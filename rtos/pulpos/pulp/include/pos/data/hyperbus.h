/*
 * Copyright (C) 2020 ETH Zurich and University of Bologna
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
 * Authors: Nazareno Bruschi, Unibo (nazareno.bruschi@unibo.it)
 */

#ifndef __POS_DATA_HYPERBUS_H__
#define __POS_DATA_HYPERBUS_H__

#ifndef LANGUAGE_ASSEMBLY

#include "pmsis/task.h"
#include "pmsis/drivers/hyperbus.h"

typedef struct pos_hyper_s
{
    /* Common setup */
    int *open_count;
    int *channel;
    pos_udma_channel_t *tx_channel;
    pos_udma_channel_t *rx_channel;
    /* Private setup */
    unsigned int baudrate;
    int active;
    pi_hyper_type_e type;
    uint32_t cs;
    signed char hyper_id;
    uint32_t tran_id;
} pos_hyper_t;


#endif

#endif