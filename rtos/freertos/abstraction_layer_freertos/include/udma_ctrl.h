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

#ifndef __UDMA_CTRL_H__
#define __UDMA_CTRL_H__

/**
 * Small low level driver for ctrl side of udma (cg and event forward)
 */

#include "target.h"
#include "memory_map.h"
#include "soc_eu.h"
#include "bits.h"

/** UDMA Global configuration - Register Layout Typedef */
typedef struct {
	volatile uint32_t CG; /**< UDMA_GC clock gating register, offset: 0x0 */
	volatile uint32_t EVTIN; /**< UDMA_GC input event register, offset: 0x04
				  */
} udma_gc_t;


/*! @name UDMA_GC - UDMA event in register, User chooses which events can come
 * to UDMA as reference events, support up to 4 choices */
#define UDMA_GC_EVTIN_CHOICE0_MASK  (0xFFU)
#define UDMA_GC_EVTIN_CHOICE0_SHIFT (0U)

#define UDMA_GC_EVTIN_CHOICE1_MASK  (0xFF00U)
#define UDMA_GC_EVTIN_CHOICE1_SHIFT (8U)

#define UDMA_GC_EVTIN_CHOICE2_MASK  (0xFF0000U)
#define UDMA_GC_EVTIN_CHOICE2_SHIFT (16U)

#define UDMA_GC_EVTIN_CHOICE3_MASK  (0xFF000000)
#define UDMA_GC_EVTIN_CHOICE3_SHIFT (24U)

#define UDMA_GC_EVTIN_MASK(evt_in) (evt_in & 0xFF)
#define UDMA_GC_EVTIN_SHIFT_ID(id) (id * 8)


/* UDMA Global configuration - instance base addresses */
/** Global configuration UDMA base address */
#define UDMA_GC_BASE (UDMA_CTRL_ADDR)
#define UDMA_GC	     ((udma_gc_t *)UDMA_GC_BASE)


static inline uint32_t udma_ctrl_get_clock_gating_register(void)
{
	return hal_read32(&UDMA_GC->CG);
}

static inline uint32_t udma_ctrl_get_event_register(void)
{
	return hal_read32(&UDMA_GC->EVTIN);
}

static inline void udma_ctrl_cg_disable(uint32_t udma_device_id)
{
	hal_or32(&UDMA_GC->CG, 1 << udma_device_id);
}

static inline void udma_ctrl_cg_enable(uint32_t udma_device_id)
{
	hal_and32(&UDMA_GC->CG, ~(1 << udma_device_id));
}

static inline void udma_ctrl_enable_event_forward(uint8_t udma_evt_nb,
						  uint32_t event_id)
{
	/* clear current event */
	hal_and32(&UDMA_GC->EVTIN, ~(UDMA_GC_EVTIN_MASK(0xFF)
				     << UDMA_GC_EVTIN_SHIFT_ID(udma_evt_nb)));
	/* set new event */
	hal_or32(&UDMA_GC->EVTIN, (UDMA_GC_EVTIN_MASK(event_id)
				   << UDMA_GC_EVTIN_SHIFT_ID(udma_evt_nb)));

	/* tell soc eu to forward us the event */
	hal_soc_eu_set_pr_mask(event_id);
}

static inline void udma_ctrl_disable_event_forward(uint8_t udma_evt_nb,
						   uint32_t event_id)
{
	/* clear event from udma forward */
	hal_and32(&UDMA_GC->EVTIN, ~(UDMA_GC_EVTIN_MASK(0xFF)
				     << UDMA_GC_EVTIN_SHIFT_ID(udma_evt_nb)));

	/* tell soc eu to stop forwarding us the event */
	hal_soc_eu_clear_pr_mask(event_id);
}

#endif /* __UDMA_CTRL_H__ */
