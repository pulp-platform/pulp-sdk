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

#include <pmsis.h>
#include <archi/pulp.h>

PI_FC_TINY void *pos_udma_channel_callback[ARCHI_NB_PERIPH*2];
PI_FC_TINY void *pos_udma_channel_callback_arg[ARCHI_NB_PERIPH*2];



#ifndef __USE_ASM_OPTIM__

void pos_udma_handle_copy(int event, void *arg)
{
    pos_udma_channel_t *channel = arg;

    pi_task_t *pending_1 = channel->pendings[1];
    pi_task_t *pending_0 = channel->pendings[0];
    pi_task_t *pending_first = channel->waitings_first;
    channel->pendings[0] = pending_1;

    if (pending_first)
    {
        channel->waitings_first = pending_first->next;
        channel->pendings[1] = pending_first;
        plp_udma_enqueue(channel->base, pending_first->data[0], pending_first->data[1], 0);
    }
    else
    {
        channel->pendings[1] = NULL;
    }

    pos_task_push_locked(pending_0);
}

#else

extern void __rt_udma_handle_copy();

#endif



void pos_udma_create_channel(pos_udma_channel_t *channel, int channel_id, int soc_event)
{
    pos_soc_event_register_callback(soc_event, pos_udma_handle_copy, (void *)channel);
    channel->pendings[0] = NULL;
    channel->pendings[1] = NULL;
    channel->waitings_first = NULL;
    channel->base = hal_udma_channel_base(channel_id);
}



void pos_udma_enqueue(pos_udma_channel_t *channel, pi_task_t *task, uint32_t buffer, uint32_t size, uint32_t cfg)
{
    // A UDMA channel has 2 slots, enqueue the copy to the UDMA if one of them is available, otherwise
    // put the transfer on hold.
    if (channel->pendings[0] == NULL)
    {
        channel->pendings[0] = task;
        plp_udma_enqueue(channel->base, buffer, size, UDMA_CHANNEL_CFG_EN | cfg);
    }
    else if (channel->pendings[1] == NULL)
    {
        channel->pendings[1] = task;
        plp_udma_enqueue(channel->base, buffer, size, UDMA_CHANNEL_CFG_EN | cfg);
    }
    else
    {
        task->data[0] = buffer;
        task->data[1] = size;
        task->data[2] = cfg;

        if (channel->waitings_first == NULL)
            channel->waitings_first = task;
        else
            channel->waitings_last->next = task;

        channel->waitings_last = task;
        task->next = NULL;
    }
}
