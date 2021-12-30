/*
 * Copyright (C) 2019 ETH Zurich, University of Bologna and GreenWaves
 * Technologies
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

#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "properties.h"

/* Events offsets. */
#define UDMA_EVENT_OFFSET_RX (0)
#define UDMA_EVENT_OFFSET_TX (1)

#define UDMA_EVENT_OFFSET_SPI_CMD (2)
#define UDMA_EVENT_OFFSET_SPI_EOT (3)

/* Number of events per peripheral. */
#define UDMA_CHANNEL_NB_EVENTS_LOG2 (2)
#define UDMA_CHANNEL_NB_EVENTS	    (1 << UDMA_CHANNEL_NB_EVENTS_LOG2)

/* Number of SW events. */
#define NB_SW_EVENTS (8)

/*! @brief FC events (aka IRQ lines)*/
#define FC_IRQ_SW_EVT(id) (id & (NB_SW_EVENTS - 1))
#define FC_EVENT_SW(id)	 (id & (NB_SW_EVENTS - 1))
#define FC_EVENT_DMA_EVT (8)
#define FC_EVENT_DMA	 (9)
#define FC_EVENT_TIMER0	 (10) /* Timer low.  */
/* #define SYSTICK_IRQN                    FC_EVENT_TIMER0 */
#define FC_EVENT_TIMER1 (11) /* Timer high. */
/* #define FC_EVENT_EU_HWCE                (12) */

/*
 * SoC event unit events: Many events get implicitely muxed into this interrupt.
 * A user that gets such an interrupt has to check the event unit's registers to
 * see what happened
 */
#define FC_EVENT_SOC_EVENT (27)
/* #define FC_EVENT_MPU_ERROR              (28) */
/*
 * Event queue error: If we don't process event unit events quickly enough
 * internal fifos can overflow and we get this error interrupt
 */
#define FC_EVENT_FC_QUEUE_ERROR (29)
#define FC_EVENT_HP0		(30)
#define FC_EVENT_HP1		(31)

/*! @name SoC events  */
/*! @brief Number of FC_Events. */
#define SOC_EU_NB_FC_EVENTS (168)

/*! @brief UDMA events */
/* SPIM */
#define SOC_EVENT_UDMA_SPIM_RX(id)                                             \
	((UDMA_SPIM_ID(id) << UDMA_CHANNEL_NB_EVENTS_LOG2) +                   \
	 UDMA_EVENT_OFFSET_RX)
#define SOC_EVENT_UDMA_SPIM_TX(id)                                             \
	((UDMA_SPIM_ID(id) << UDMA_CHANNEL_NB_EVENTS_LOG2) +                   \
	 UDMA_EVENT_OFFSET_TX)
#define SOC_EVENT_UDMA_SPIM_CMD(id)                                            \
	((UDMA_SPIM_ID(id) << UDMA_CHANNEL_NB_EVENTS_LOG2) +                   \
	 UDMA_EVENT_OFFSET_SPI_CMD)
#define SOC_EVENT_UDMA_SPIM_EOT(id)                                            \
	((UDMA_SPIM_ID(id) << UDMA_CHANNEL_NB_EVENTS_LOG2) +                   \
	 UDMA_EVENT_OFFSET_SPI_EOT)
/* HYPER */
/* #define SOC_EVENT_UDMA_HYPER_RX(id)     ((UDMA_HYPER_ID(id) <<
 * UDMA_CHANNEL_NB_EVENTS_LOG2) + UDMA_EVENT_OFFSET_RX) */
/* #define SOC_EVENT_UDMA_HYPER_TX(id)     ((UDMA_HYPER_ID(id) <<
 * UDMA_CHANNEL_NB_EVENTS_LOG2) + UDMA_EVENT_OFFSET_TX) */
/* UART */
#define SOC_EVENT_UDMA_UART_RX(id)                                             \
	((UDMA_UART_ID(id) << UDMA_CHANNEL_NB_EVENTS_LOG2) +                   \
	 UDMA_EVENT_OFFSET_RX)
#define SOC_EVENT_UDMA_UART_TX(id)                                             \
	((UDMA_UART_ID(id) << UDMA_CHANNEL_NB_EVENTS_LOG2) +                   \
	 UDMA_EVENT_OFFSET_TX)
/* I2C */
#define SOC_EVENT_UDMA_I2C_RX(id)                                              \
	((UDMA_I2C_ID(id) << UDMA_CHANNEL_NB_EVENTS_LOG2) +                    \
	 UDMA_EVENT_OFFSET_RX)
#define SOC_EVENT_UDMA_I2C_TX(id)                                              \
	((UDMA_I2C_ID(id) << UDMA_CHANNEL_NB_EVENTS_LOG2) +                    \
	 UDMA_EVENT_OFFSET_TX)
/* DMACPY */
/* #define SOC_EVENT_UDMA_DMACPY_RX(id)    ((UDMA_DMACPY_ID(id) <<
 * UDMA_CHANNEL_NB_EVENTS_LOG2) + UDMA_EVENT_OFFSET_RX) */
/* #define SOC_EVENT_UDMA_DMACPY_TX(id)    ((UDMA_DMACPY_ID(id) <<
 * UDMA_CHANNEL_NB_EVENTS_LOG2) + UDMA_EVENT_OFFSET_TX) */
/* I2S */
#define SOC_EVENT_UDMA_I2S_RX(id)                                              \
	((UDMA_I2S_ID(id) << UDMA_CHANNEL_NB_EVENTS_LOG2) +                    \
	 UDMA_EVENT_OFFSET_RX)
#define SOC_EVENT_UDMA_I2S_TX(id)                                              \
	((UDMA_I2S_ID(id) << UDMA_CHANNEL_NB_EVENTS_LOG2) +                    \
	 UDMA_EVENT_OFFSET_TX)
/* CPI */
#define SOC_EVENT_UDMA_CPI_RX(id)                                              \
	((UDMA_CPI_ID(id) << UDMA_CHANNEL_NB_EVENTS_LOG2) +                    \
	 UDMA_EVENT_OFFSET_RX)

/* UDMA EOT & error events. */
//#define SOC_EVENT_UDMA_I2C_ERROR(id)  (26 + id)

/*! @brief PMU events, no pmu*/
/* #define SOC_EVENT_PMU_CLUSTER_POWER     (31) */
/* #define SOC_EVENT_PMU_CLUSTER_CG        (35) */
/* #define SOC_EVENT_PMU_DLC_BRIDGE_PICL   (36) */
/* #define SOC_EVENT_PMU_DLC_BRIDGE_SCU    (37) */
/* #define SOC_EVENT_PWM(id)               (38 + id) */
#define SOC_EVENT_GPIO	(139)
#define SOC_EVENT_HWPE0 (140)
#define SOC_EVENT_HWPE1 (141)
/* #define SOC_EVENT_RTC_APB               (43) */
/* #define SOC_EVENT_RTC                   (44) */

#define SOC_EVENT_SW(id)	  (160 + (id & (NB_SW_EVENTS - 1)))
#define SOC_EVENT_REF32K_CLK_RISE (168)

/**
 * \brief Cluster IRQ
 *
 * Below are listed cluster IRQ.
 */
#define CL_IRQ_SW_EVT(id)               (id & (NB_SW_EVENTS - 1))
#define CL_IRQ_DMA0                     (8)
#define CL_IRQ_DMA1                     (9)
#define CL_IRQ_TIMER0_LO                (10)
#define CL_IRQ_TIMER0_HI                (11)
#define CL_IRQ_ACC_EVT_0                (12) /* HW Acc. */
#define CL_IRQ_ACC_EVT_1                (13) /* HW Acc. */
#define CL_IRQ_ACC_EVT_2                (14) /* HW Acc. */
#define CL_IRQ_ACC_EVT_3                (15) /* HW Acc. */
#define CL_IRQ_BARRIER_EVT              (16)
#define CL_IRQ_HW_MUTEX_EVT             (17)
#define CL_IRQ_DISPATCH_EVT             (18)
/* #define CL_IRQ_CLUSTER_EVT_0            (22) */
/* #define CL_IRQ_CLUSTER_EVT_1            (23) */
/* #define CL_IRQ_SOC_FIFO_EVT             (27) */
#define CL_EVENT_SOC_EVT (30) /* adapted */



#define CLUSTER_TO_FC_NOTIFY_IRQN       FC_IRQ_SW_EVT(2) /*!< IRQ sent by cluster to FC.
                                                          *   IRQ handler is needed.
                                                          *   Asynchronous.
                                                          */
#define FC_SOC_EVENT_NOTIFY_IRQ         FC_IRQ_SW_EVT(3) /*!< IRQ used by RTC. */


#define FC_TO_CLUSTER_NOTIFY_EVENT      CL_IRQ_SW_EVT(1) /*!< Event sent by FC to cluster.
                                                          *   A cluster core is waiting for this
                                                          *   event.
                                                          *   Synchronous.
                                                          */
#define DMA_SW_IRQN                     CL_IRQ_SW_EVT(2) /*!< Event used when emulating 2D DMA
                                                          *   transfers or large 1D ttransfers.
                                                          *   Master core waits for this SW event,
                                                          *   triggered by CL_IRQ_DMA1 handler.
                                                          */
#define PRINTF_LOCK_IRQN                CL_IRQ_SW_EVT(3) /*!< IRQ used to sync FC and cluster cores
                                                          *   to lock/unlock printf.
                                                          */
#define CL_USER_EVENT                   CL_IRQ_SW_EVT(7) /*!< Event used by user to sync cluster with
                                                          *   FC.
                                                          */

#define FC_NOTIFY_CLUSTER_EVENT         FC_TO_CLUSTER_NOTIFY_EVENT

#endif /* __EVENTS_H__ */
