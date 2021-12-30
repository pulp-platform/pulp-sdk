/*
 * Copyright (C) 2019 ETH Zurich and University of Bologna
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

/* Driver to control and configure the PULP IRQ (apb_interrupt_control)*/

#include <stdint.h>
#include <assert.h>

#include "pulp_mem_map.h"
#include "io.h"
#include "irq.h"
#include "csr.h"

extern void (*isr_table[32])(void);

/* set interrupt handler for given interrupt id */
void irq_set_handler(int id, void (*handler)(void))
{
	assert (0 <= id < 32);
	isr_table[id] = handler;
}

/* utility functions for PULPs external interrupt controller */
void irq_mask(uint32_t mask)
{
	writew(mask, (uintptr_t)(PULP_FC_IRQ_ADDR + IRQ_REG_MASK_OFFSET));
}

void irq_enable(uint32_t mask)
{
	writew(mask, (uintptr_t)(PULP_FC_IRQ_ADDR + IRQ_REG_MASK_SET_OFFSET));
}

void irq_disable(uint32_t mask)
{
	writew(mask, (uintptr_t)(PULP_FC_IRQ_ADDR + IRQ_REG_MASK_CLEAR_OFFSET));
}

void irq_pend(uint32_t mask)
{
	writew(mask, (uintptr_t)(PULP_FC_IRQ_ADDR + IRQ_REG_INT_SET_OFFSET));
}

void irq_clear(uint32_t mask)
{
	writew(mask, (uintptr_t)(PULP_FC_IRQ_ADDR + IRQ_REG_INT_CLEAR_OFFSET));
}

/* utility functions for the core level interrupt (CLINT) described in the
 * RISC-V privileged specification */

/* enable/disable interrupt globally (MIE bit in mstatus csr) */
uint32_t irq_clint_global_disable()
{
	uint32_t val = csr_read_clear(CSR_MSTATUS, MSTATUS_IE);
	return val;
}

uint32_t irq_clint_global_enable()
{
	uint32_t val = csr_read_set(CSR_MSTATUS, MSTATUS_IE);
	return val;
}

/* enable/disable interrupts selectively (in mie csr) */
uint32_t irq_clint_disable(int32_t mask)
{
	uint32_t val = csr_read_clear(CSR_MIE, mask);
	return val;
}

uint32_t irq_clint_enable(int32_t mask)
{
 	uint32_t val = csr_read_set(CSR_MIE, mask);
	return val;
}


/* TODO: make this a constructor? */
void pulp_irq_init()
{
	/* the debug module could have enabled irq so we disable it during
	 * initialization
	 */
	irq_disable(0xffffffff);
}
