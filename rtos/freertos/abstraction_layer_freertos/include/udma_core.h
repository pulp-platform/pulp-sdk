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

#ifndef __UDMA_CORE_H__
#define __UDMA_CORE_H__

/**
 * Small low level driver for udma core main functionnalities
 */

#include "target.h"
#include "udma.h"
#include "udma_ctrl.h"

#define UDMA_MAX_SIZE_LOG2 (17)
#define UDMA_MAX_SIZE	   (1 << UDMA_MAX_SIZE_LOG2)


static inline void udma_init_device(uint32_t device_id)
{
	/* disable clock gating for device with device_id */
	udma_ctrl_cg_disable(device_id);
}

/*
 * Common uDMA channel enqueue
 */
static inline void udma_enqueue_channel(udma_channel_t *chan, uint32_t addr,
					uint32_t size, uint32_t config)
{
	hal_write32(&chan->saddr, addr);
	hal_write32(&chan->size, size);
	hal_write32(&chan->cfg, config);
}

static inline void udma_channel_clear(udma_channel_t *chan)
{
	/* TODO: adjust macro */
	hal_write32(&(chan->cfg), REG_SET(UDMA_CORE_RX_CFG_CLR, 1));
}

static inline void udma_deinit_device(uint32_t device_id)
{
	/* enable clock gating for device with device_id */
	udma_ctrl_cg_enable(device_id);
}

#endif /* __UDMA_CORE_H__ */
