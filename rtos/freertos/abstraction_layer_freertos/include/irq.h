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
 */

/* Interrupt controller definitions */
/* Author: Robert Balas (balasr@iis.ee.ethz.ch)
 *         Germain Haugou (germain.haugou@iis.ee.ethz.ch)
 */

#ifndef __IRQ_H__
#define __IRQ_H__

#include "io.h"
#include "bits.h"
#include "memory_map.h"
#include "riscv.h"

#define IRQ_REG_MASK_OFFSET	  0x000
#define IRQ_REG_MASK_SET_OFFSET	  0x004
#define IRQ_REG_MASK_CLEAR_OFFSET 0x008
#define IRQ_REG_INT_OFFSET	  0x00C
#define IRQ_REG_INT_SET_OFFSET	  0x010
#define IRQ_REG_INT_CLEAR_OFFSET  0x014
#define IRQ_REG_ACK_OFFSET	  0x018
#define IRQ_REG_ACK_SET_OFFSET	  0x01C
#define IRQ_REG_ACK_CLEAR_OFFSET  0x020
#define IRQ_REG_FIFO_OFFSET	  0x024

/* Interrupt line masks: these interrupts directly go to the core (after being
 * serialized as reqest + id). We refer to these interrupts with the prefix IRQ.
 * Events on the other we strictly use to refer to "interrupts/events" that are
 * signaled through (muxed) EU SoC interrupts (IRQ_FC_EVT_SOC_EVT) and need
 * additional handling by the user through the Event Unit.
 */
#define IRQ_FC_EVT_SW0	      BIT(0)
#define IRQ_FC_EVT_SW1	      BIT(1)
#define IRQ_FC_EVT_SW2	      BIT(2)
#define IRQ_FC_EVT_SW3	      BIT(3)
#define IRQ_FC_EVT_SW4	      BIT(4)
#define IRQ_FC_EVT_SW5	      BIT(5)
#define IRQ_FC_EVT_SW6	      BIT(6)
#define IRQ_FC_EVT_SW7	      BIT(7)
#define IRQ_FC_EVT_DMA_PE_EVT BIT(8)
#define IRQ_FC_EVT_DMA_PE_IRQ BIT(9)
#define IRQ_FC_EVT_TIMER0_LO  BIT(10)
#define IRQ_FC_EVT_TIMER0_HI  BIT(11)
#define IRQ_FC_EVT_PF	      BIT(12)
#define IRQ_FC_EVT_CLK_REF    BIT(14)
#define IRQ_FC_EVT_GPIO	      BIT(15)

/* is not in PULPissimo */
/*#define IRQ_FC_EVT_RTC		   16 */

#define IRQ_FC_EVT_ADV_TIMER0 BIT(17)
#define IRQ_FC_EVT_ADV_TIMER1 BIT(18)
#define IRQ_FC_EVT_ADV_TIMER2 BIT(19)
#define IRQ_FC_EVT_ADV_TIMER3 BIT(20)

/* is not in PULPissimo */
/* #define IRQ_FC_EVT_CLUSTER_NOT_BUSY 21 */
/* #define IRQ_FC_EVT_CLUSTER_POK	   22 */
/* #define IRQ_FC_EVT_CLUSTER_CG_OK	   23 */
/* #define IRQ_FC_EVT_PICL_OK     24 */
/* #define IRQ_FC_EVT_SCU_OK      25 */

/*
 * SoC event unit events: Many events get implicitely muxed into this interrupt.
 * A user that gets such an interrupt has to check the event unit's registers to
 * see what happened
 */
#define IRQ_FC_EVT_SOC_EVT BIT(26)
/*
 * Event queue error: If we don't process event unit events quickly enough
 * internal fifos can overflow and we get this error interrupt
 */
#define IRQ_FC_EVT_QUIRQE_ERROR BIT(29)
/* High priority peripheral events: these are hardcoded to directly go to the
 * core using a dedicated interrupt line
 */
#define IRQ_FC_EVT_PERIPH0 BIT(30)
#define IRQ_FC_EVT_PERIPH1 BIT(31)

/* TODO: doc */
void irq_set_handler(int id, void (*handler)(void));
void irq_mask(uint32_t mask);
void irq_enable(uint32_t mask);
void irq_disable(uint32_t mask);
void irq_pend(uint32_t mask);
void irq_clear(uint32_t mask);
uint32_t irq_clint_global_disable();
uint32_t irq_clint_global_enable();
uint32_t irq_clint_disable(int32_t mask);
uint32_t irq_clint_enable(int32_t mask);
void pulp_irq_init();

/** Interrupt Number Definitions */
#define NUMBER_OF_INT_VECTORS                                                  \
	32 /**< Number of interrupts in the Vector table */

typedef enum {
    //FC_NOTIFY_CLUSTER_EVENT = 0,   /**< Software event interrupt */
	CLUSTER_NOTIFY_FC_EVENT = 1,   /**< Software event interrupt */
	FC_SW_NOTIFY_BRIDGE_EVENT = 2, /**< Software event interrupt */
	FC_SW_NOTIFY_EVENT = 3,	       /**< Software event interrupt */
	CLUSTER_NOTIFY_FC_IRQN = 4,    /**< Software event interrupt */
	/* DMA_SW_IRQN = 6, */
	PENDSV_IRQN = 7, /**< Software event U -> M PendSV interrupt */

	/* Device specific interrupts */
	DMA_EVT_IRQN = 8,    /**< DMA event interrupt */
	DMA_IRQN = 9,	     /**< DMA interrupt */
	FC_TIMER0_IRQN = 10, /**< FC timer0 event interrupt */
	SYSTICK_IRQN = 10,   /**< PULP U -> M System Tick Interrupt */
	FC_TIMER1_IRQN = 11, /**< FC timer1 interrupt */

	/* misc */
	FC_CLK_REF_EVENT = 14, /**< Reference clock edge event */
	FC_GPIO_EVENT = 15,    /**< GPIO event */

	/* advanced timer events */
	FC_ADV_TIMER0_EVENT = 17, /**< Advanced Timer 0 event */
	FC_ADV_TIMER1_EVENT = 18, /**< Advanced Timer 1 event */
	FC_ADV_TIMER2_EVENT = 19, /**< Advanced Timer 2 event */
	FC_ADV_TIMER3_EVENT = 20, /**< Advanced Timer 3 event */

	/* CLUSTER_NOT_BUSY_EVENT       = 21, */
	/* CLUSTER_POK_EVENT            = 22, */
	/* CLUSTER_CG_OK_EVENT          = 23, */

	/* PICL_OK_EVENT                = 24, */
	/* SCU_OK_EVENT                 = 25, */

	FC_SOC_EVENT = 26, /**< Event unit new event */

	FC_QUEUE_ERROR_EVENT = 29, /**< Event unit queue overflow event */

	FC_HP_EVENT1 = 30,
	FC_HP_EVENT0 = 31
} simple_irqn_e;

/**
  \brief  Structure type to access the simple interrupt controller.
 */
typedef struct {
	volatile uint32_t MASK; /**< Interrupt Controller mask register, offset:
				   0x00 */
	volatile uint32_t MASK_SET; /**< Interrupt Controller mask set register,
				       offset: 0x04 */
	volatile uint32_t MASK_CLEAR; /**< Interrupt Controller mask clear
					 register, offset: 0x08 */
	volatile uint32_t IRQ; /**< Interrupt Controller irq register, offset:
				  0x0C */
	volatile uint32_t IRQ_SET;   /**< Interrupt Controller irq set register,
					offset: 0x10 */
	volatile uint32_t IRQ_CLEAR; /**< Interrupt Controller irq clear
					register, offset: 0x14 */
	volatile uint32_t ACK; /**< Interrupt Controller ack register, offset:
				  0x18 */
	volatile uint32_t ACK_SET;   /**< Interrupt Controller ack set register,
					offset: 0x1C */
	volatile uint32_t ACK_CLEAR; /**< Interrupt Controller ack clear
					register, offset: 0x20 */
	volatile uint32_t FIFO;	     /**< Interrupt Controller soc event fifo
					register, offset: 0x24 */
} simple_irq_t;

#define IRQ_REG_MASK_OFFSET	  0x000
#define IRQ_REG_MASK_SET_OFFSET	  0x004
#define IRQ_REG_MASK_CLEAR_OFFSET 0x008
#define IRQ_REG_INT_OFFSET	  0x00C
#define IRQ_REG_INT_SET_OFFSET	  0x010
#define IRQ_REG_INT_CLEAR_OFFSET  0x014
#define IRQ_REG_ACK_OFFSET	  0x018
#define IRQ_REG_ACK_SET_OFFSET	  0x01C
#define IRQ_REG_ACK_CLEAR_OFFSET  0x020
#define IRQ_REG_FIFO_OFFSET	  0x024

/* Interrupt line masks: these interrupts directly go to the core (after being
 * serialized as reqest + id). We refer to these interrupts with the prefix IRQ.
 * Events on the other we strictly use to refer to "interrupts/events" that are
 * signaled through (muxed) EU SoC interrupts (IRQ_FC_EVT_SOC_EVT) and need
 * additional handling by the user through the Event Unit.
 */
#define IRQ_FC_EVT_SW0	      BIT(0)
#define IRQ_FC_EVT_SW1	      BIT(1)
#define IRQ_FC_EVT_SW2	      BIT(2)
#define IRQ_FC_EVT_SW3	      BIT(3)
#define IRQ_FC_EVT_SW4	      BIT(4)
#define IRQ_FC_EVT_SW5	      BIT(5)
#define IRQ_FC_EVT_SW6	      BIT(6)
#define IRQ_FC_EVT_SW7	      BIT(7)
#define IRQ_FC_EVT_DMA_PE_EVT BIT(8)
#define IRQ_FC_EVT_DMA_PE_IRQ BIT(9)
#define IRQ_FC_EVT_TIMER0_LO  BIT(10)
#define IRQ_FC_EVT_TIMER0_HI  BIT(11)
#define IRQ_FC_EVT_PF	      BIT(12)
#define IRQ_FC_EVT_CLK_REF    BIT(14)
#define IRQ_FC_EVT_GPIO	      BIT(15)
/* doesn't exist in pulp */
/*#define IRQ_FC_EVT_RTC		   16 */
#define IRQ_FC_EVT_ADV_TIMER0 BIT(17)
#define IRQ_FC_EVT_ADV_TIMER1 BIT(18)
#define IRQ_FC_EVT_ADV_TIMER2 BIT(19)
#define IRQ_FC_EVT_ADV_TIMER3 BIT(20)
/* doesn't exist in pulp */
/* #define IRQ_FC_EVT_CLUSTER_NOT_BUSY 21 */
/* #define IRQ_FC_EVT_CLUSTER_POK	   22 */
/* #define IRQ_FC_EVT_CLUSTER_CG_OK	   23 */
/* #define IRQ_FC_EVT_PICL_OK     24 */
/* #define IRQ_FC_EVT_SCU_OK      25 */
/*
 * SoC event unit events: Many events get implicitely muxed into this interrupt.
 * A user that gets such an interrupt has to check the event unit's registers to
 * see what happened
 */
#define IRQ_FC_EVT_SOC_EVT BIT(26)
/*
 * Event queue error: If we don't process event unit events quickly enough
 * internal fifos can overflow and we get this error interrupt
 */
#define IRQ_FC_EVT_QUIRQE_ERROR BIT(29)
/* High priority peripheral events: these are hardcoded to directly go to the
 * core using a dedicated interrupt line
 */
#define IRQ_FC_EVT_PERIPH0 BIT(30)
#define IRQ_FC_EVT_PERIPH1 BIT(31)


#define SIMPLE_IRQ                                                             \
	((simple_irq_t *)FC_IRQ_ADDR) /*!< Simple irq configuration struct */

/**
  \brief   Enable Interrupt
  \details Enables a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */
static inline void __irq_enable(simple_irqn_e IRQn)
{
	/* U mode does not has the right */
	/* NVIC->MASK_SET = (1UL << IRQn); */
	writew(1UL << IRQn, (uintptr_t)(FC_IRQ_ADDR + IRQ_REG_MASK_SET_OFFSET));
}

/**
  \brief   Get Interrupt Enable status
  \details Returns a device specific interrupt enable status from the NVIC
  interrupt controller. \param [in]      IRQn  Device specific interrupt number.
  \return             0  Interrupt is not enabled.
  \return             1  Interrupt is enabled.
  \note    IRQn must not be negative.
 */
static inline uint32_t __irq_get_enable(simple_irqn_e IRQn)
{
	/* U mode does not has the right */
	/* return ((uint32_t)((NVIC->MASK_IRQ & (1UL << IRQn)) ? 1UL : 0UL)); */
	uint32_t mask = readw((uintptr_t)(FC_IRQ_ADDR + IRQ_REG_MASK_OFFSET));
	return (mask >> IRQn) & 1;
}


/**
  \brief   Disable Interrupt
  \details Disables a device specific interrupt in the NVIC interrupt
  controller. \param [in]      IRQn  Device specific interrupt number. \note
  IRQn must not be negative.
 */
static inline void __irq_disable(simple_irqn_e IRQn)
{
	/* U mode does not has the right */
	/* NVIC->MASK_IRQ_AND = (1UL << IRQn); */
	writew(1UL << IRQn,
	       (uintptr_t)(FC_IRQ_ADDR + IRQ_REG_MASK_CLEAR_OFFSET));
}


/**
  \brief   Get Pending Interrupt
  \details Reads the NVIC pending register and returns the pending bit for the
  specified device specific interrupt. \param [in]      IRQn  Device specific
  interrupt number. \return             0  Interrupt status is not pending.
  \return             1  Interrupt status is pending.
  \note    IRQn must not be negative.
 */
static inline uint32_t __irq_get_pending(simple_irqn_e IRQn)
{
	/* return(0U); */
	uint32_t pending = readw((uintptr_t)(FC_IRQ_ADDR + IRQ_REG_INT_OFFSET));
	return (pending >> IRQn) & 1;
}


/**
  \brief   Set Pending Interrupt
  \details Sets the pending bit of a device specific interrupt in the NVIC
  pending register. \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */
static inline void __irq_set_pending(simple_irqn_e IRQn)
{
	writew(1UL << IRQn, (uintptr_t)(FC_IRQ_ADDR + IRQ_REG_INT_SET_OFFSET));
}


/**
  \brief   Clear Pending Interrupt
  \details Clears the pending bit of a device specific interrupt in the NVIC
  pending register. \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */
static inline void __irq_clear_pending(simple_irqn_e IRQn)
{
	writew(1UL << IRQn,
	       (uintptr_t)(FC_IRQ_ADDR + IRQ_REG_INT_CLEAR_OFFSET));
}


/**
  \brief   Get Active Interrupt
  \details Reads the active register in the NVIC and returns the active bit for
  the device specific interrupt. \param [in]      IRQn  Device specific
  interrupt number. \return             0  Interrupt status is not active.
  \return             1  Interrupt status is active.
  \note    IRQn must not be negative.
 */
static inline uint32_t __irq_get_active(simple_irqn_e IRQn)
{
	assert(0);
	return 0;
}

static inline uint32_t __irq_forge_it_vect(uint32_t ItBaseAddr,
					   uint32_t ItIndex, uint32_t ItHandler)

{
	assert(0);

	return 0;
}

/**
  \brief   Set Interrupt Vector
  \details Sets an interrupt vector in SRAM based interrupt vector table.
	   The interrupt number can be positive to specify a device specific
  interrupt, or negative to specify a processor exception. VTOR must been
  relocated to SRAM before. \param [in]   IRQn      Interrupt number \param [in]
  vector    Address of interrupt handler function
 */
static inline void __irq_set_vector(simple_irqn_e IRQn, uint32_t vector)
{
	assert(0);
}

/**
  \brief   Get Interrupt Vector
  \details Reads an interrupt vector from interrupt vector table.
	   The interrupt number can be positive to specify a device specific
  interrupt, or negative to specify a processor exception. \param [in]   IRQn
  Interrupt number. \return                 Address of interrupt handler
  function
 */
static inline uint32_t __irq_get_vector(simple_irqn_e IRQn)
{
	assert(0);
}


/**
  \brief   System Reset
  \details Initiates a system reset request to reset the MCU.
 */
static inline void __irq_system_reset(void)
{
	assert(0);
}

#endif /* __IRQ_H__ */
