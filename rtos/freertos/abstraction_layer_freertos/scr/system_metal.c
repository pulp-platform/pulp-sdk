/*
 * Copyright 2020 ETH Zurich
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
 * Author: Robert Balas (balasr@iis.ee.ethz.ch)
 */

#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include "system_metal_conf.h"

#include "fll.h"
#include "irq.h"
#include "soc_eu.h"

#ifndef DISABLE_WDOG
#define DISABLE_WDOG 1
#endif

/* test some assumptions we make about compiler settings */
static_assert(sizeof(uintptr_t) == 4,
	      "uintptr_t is not 4 bytes. Make sure you are using -mabi=ilp32*");

/* Allocate heap to special section. Note that we have no references in the
 * whole program to this variable (since its just here to allocate space in the
 * section for our heap), so when using LTO it will be removed. We force it to
 * stay with the "used" attribute
 */
__attribute__((section(".heap"), used)) uint8_t ucHeap[configTOTAL_HEAP_SIZE];

/* Inform linker script about .heap section size. Note: GNU ld seems to
 * internally represent integers with the bfd_vma type, that is a type that can
 * contain memory addresses (typdefd to some int type depending on the
 * architecture). uint32_t seems to me the most fitting candidate for rv32.
 */
uint32_t __heap_size = configTOTAL_HEAP_SIZE;

uint32_t volatile system_core_clock = DEFAULT_SYSTEM_CLOCK;

/**
 * Board init code. Always call this before anything else.
 */
void system_init(void)
{
	/* init flls */
	for (int i = 0; i < ARCHI_NB_FLL; i++) {
		pi_fll_init(i, 0);
	}

	/* make sure irq (itc) is a good state */
	pulp_irq_init();

	/* mtvec is set in crt0.S */

	/* deactivate all soc events as they are enabled by default */
	soc_eu_event_init();

	/* TODO: enable uart */
	/* TODO: I$ enable*/
	/* enable core level interrupt (mie) */
	irq_clint_global_enable();
}

//
void system_core_clock_update()
{
	system_core_clock = pi_fll_get_frequency(FLL_SOC, 0);
}

uint32_t system_core_clock_get(void)
{
	system_core_clock_update();
	return system_core_clock;
}
