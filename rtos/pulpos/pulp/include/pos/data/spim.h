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

#ifndef __POS_DATA_SPIM_H__
#define __POS_DATA_SPIM_H__

#ifndef LANGUAGE_ASSEMBLY

#include "pmsis/task.h"

#define POS_SPIM_UDMA_CMD_SIZE 4

typedef struct
{
    uint32_t *temp_buff;
    uint32_t addr;
    uint32_t size;
    uint32_t end;
} pos_spim_pending_transfert_t;


typedef struct
{
    uint32_t udma_cmd[POS_SPIM_UDMA_CMD_SIZE];
    uint32_t temp_buff[2];
} pos_spim_l2_t;

#if UDMA_VERSION == 4
typedef struct
{
    pi_task_t *pending_copy;
    pi_task_t *waiting_first;
    pi_task_t *waiting_last;
    unsigned int pending_repeat_base;
    unsigned int pending_repeat_callback;
    unsigned int pending_repeat_asm_callback;
    unsigned int pending_repeat_misaligned_size;
    unsigned int pending_repeat_misaligned_addr;
    unsigned int pending_repeat_misaligned_ram_addr;
    unsigned int pending_repeat_misaligned_end;
    unsigned int pending_repeat_misaligned_length;
    unsigned int pending_repeat_misaligned_stride;
    unsigned int pending_repeat_misaligned_2d_size;
    unsigned int pending_chunk_size;
    unsigned int pending_send_cmd;
    unsigned int pending_eot;
    void (*pending_callback)(int event, void *arg);
    uint32_t udma_cmd[POS_SPIM_UDMA_CMD_SIZE];
    int open_count;
    int id;
    pi_task_t task;
    unsigned int pending_repeat_addr;
    unsigned int pending_repeat_dup_addr;
    unsigned int pending_repeat_send;
    unsigned int pending_repeat_flags;
    pos_spim_pending_transfert_t pending_transfers[2];
    int pending_transfer_index;
    int pending_transfer_read_index;
    struct pi_device *pending_repeat_device;
    uint32_t periph_base;
    uint32_t rx_cmd;
    uint32_t tx_cmd;
    int channel;
    int pending_is_auto;
    uint32_t cmd_channel_base;
    uint32_t tx_channel_base;
    uint32_t rx_channel_base;
    uint8_t cmd_channel;
    uint8_t tx_channel;
    uint8_t rx_channel;
} pos_spim_t;
#else
typedef struct
{
    pi_task_t *pending_copy;
    pi_task_t *waiting_first;
    pi_task_t *waiting_last;
    unsigned int pending_repeat_base;
    unsigned int pending_repeat_len;
    unsigned int pending_repeat_callback;
    unsigned int pending_repeat_asm_callback;
    unsigned int pending_repeat_misaligned_size;
    unsigned int pending_repeat_misaligned_addr;
    unsigned int pending_repeat_misaligned_ram_addr;
    unsigned int pending_repeat_misaligned_end;
    unsigned int pending_repeat_misaligned_length;
    unsigned int pending_repeat_misaligned_stride;
    unsigned int pending_repeat_misaligned_2d_size;
    unsigned int pending_chunk_size;
    unsigned int pending_send_cmd;
    unsigned int pending_eot;
    void (*pending_callback)(int event, void *arg);
    uint32_t *udma_cmd;
    int open_count;
    int id;
    pi_task_t task;
    unsigned int pending_repeat_addr;
    unsigned int pending_repeat_dup_addr;
    unsigned int pending_repeat_send;
    unsigned int pending_repeat_flags;
    pos_spim_pending_transfert_t pending_transfers[2];
    int pending_transfer_index;
    int pending_transfer_read_index;
    struct pi_device *pending_repeat_device;
    uint32_t periph_base;
    uint32_t rx_cmd;
    uint32_t tx_cmd;
    int channel;
    int pending_is_auto;
} pos_spim_t;
#endif

#endif

#define POS_SPIM_T_PENDING_COPY      0
#define POS_SPIM_T_WAITING_FIRST     4
#define POS_SPIM_T_WAITING_LAST      8
#define PI_SPIM_T_REPEAT_BASE           12
#define PI_SPIM_T_REPEAT_LEN            16
#define PI_SPIM_T_REPEAT_CALLBACK       20
#define PI_SPIM_T_REPEAT_ASM_CALLBACK   24

#endif
