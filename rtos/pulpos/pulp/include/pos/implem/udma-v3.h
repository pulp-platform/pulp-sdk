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

#ifndef __POS_IMPLEM_UDMA_V3_H__
#define __POS_IMPLEM_UDMA_V3_H__


#include <stdio.h>

static inline void pos_udma_register_channel_callback(unsigned int channel_id, void (*callback)(int, void *), void *arg)
{
    pos_udma_channel_callback[channel_id] = callback;
    pos_udma_channel_callback_arg[channel_id] = arg;
}

void pos_udma_create_channel(pos_udma_channel_t *channel, int channel_id, int soc_event);

void pos_udma_enqueue(pos_udma_channel_t *channel, pi_task_t *task, uint32_t buffer, uint32_t size, uint32_t cfg);

#endif