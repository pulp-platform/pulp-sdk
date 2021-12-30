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

#include "system.h"

#include <FreeRTOS.h>
#include "FreeRTOSConfig.h"

#include "cluster/cl_to_fc_delegate.h"
#include "fll.h"
#include "freq.h"
#include "fc_event.h"
#include "properties.h"
#include "irq.h"
#include "soc_eu.h"
#include "udma_ctrl.h"
#include "udma_uart.h"
#include "uart_periph.h"

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

volatile uint32_t system_core_clock = DEFAULT_SYSTEM_CLOCK;

/* FreeRTOS task handling */
BaseType_t xTaskIncrementTick(void);
void vTaskSwitchContext(void);

/* interrupt handling */
void cl_notify_fc_event_handler(void);
void timer_irq_handler(void);
void undefined_handler(void);
void (*isr_table[32])(void);

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

	/* Hook up isr table. This table is temporary until we figure out how to
	 * do proper vectored interrupts.
	 */
#ifdef CONFIG_CLUSTER
	isr_table[0x2] = cl_notify_fc_event_handler;
#endif
	isr_table[0xa] = timer_irq_handler;
	isr_table[0x1a] = fc_soc_event_handler; // 26

	/* mtvec is set in crt0.S */

	/* deactivate all soc events as they are enabled by default */
	soc_eu_event_init();

	/* Setup soc events handler. */
	/* pi_fc_event_handler_init(FC_SOC_EVENT); */
	pi_fc_event_handler_init(26); /* TODO: FIX THIS */

	/* TODO: I$ enable*/
	/* enable core level interrupt (mie) */
	irq_clint_global_enable();

	/* enable uart as stdio*/
#if CONFIG_STDIO == STDIO_UART
	udma_ctrl_cg_disable(UDMA_UART_ID(STDIO_UART_DEVICE_ID));

	int baudrate = STDIO_UART_BAUDRATE;
	uint32_t div = (pi_freq_get(PI_FREQ_DOMAIN_PERIPH) + baudrate / 2) / baudrate;
	uint32_t val = 0;

	val |= REG_SET(UART_SETUP_TX_ENA, 1);
	val |= REG_SET(UART_SETUP_RX_ENA, 1);
	val |= REG_SET(UART_SETUP_BIT_LENGTH, 0x3); /* 8 bits */
	val |= REG_SET(UART_SETUP_PARITY_ENA, 0);
	val |= REG_SET(UART_SETUP_CLKDIV, div - 1);
	val |= REG_SET(UART_SETUP_POLLING_EN, 1);

	uart_setup_set(UDMA_UART_ID(STDIO_UART_DEVICE_ID), val);
#endif

}

void system_core_clock_update(void)
{
	system_core_clock = pi_fll_get_frequency(FLL_SOC, 0);
}

uint32_t system_core_clock_get(void)
{
	system_core_clock_update();
	return system_core_clock;
}

void timer_irq_handler(void)
{
#warning requires critical section if interrupt nesting is used.

	if (xTaskIncrementTick() != 0) {
		vTaskSwitchContext();
	}
}

void undefined_handler(void)
{
#ifdef __PULP_USE_LIBC
	abort();
#else
	taskDISABLE_INTERRUPTS();
	for (;;)
		;
#endif
}

void vPortSetupTimerInterrupt(void)
{
	extern int timer_irq_init(uint32_t ticks);

	/* No CLINT so use the PULP timer to generate the tick interrupt. */
	/* TODO: configKERNEL_INTERRUPT_PRIORITY - 1 ? */
	timer_irq_init(ARCHI_REF_CLOCK / configTICK_RATE_HZ);
	/* TODO: allow setting interrupt priority (to super high(?)) */
	irq_enable(IRQ_FC_EVT_TIMER0_LO);
}

void vSystemIrqHandler(uint32_t mcause)
{
	extern void (*isr_table[32])(void);
	isr_table[mcause & 0x1f]();
}
