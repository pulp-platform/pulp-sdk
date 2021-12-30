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

#ifndef __PROPERTIES_H__
#define __PROPERTIES_H__


/* PULP defs */
#define PULP

#define ARCHI_FPGA_FREQUENCY 5000000

/* hardware modules */
#define ARCHI_NUM_TIMER 1
#define ARCHI_NUM_FLL	2

#define ARCHI_REF_CLOCK_LOG2 15
#define ARCHI_REF_CLOCK	     (1 << ARCHI_REF_CLOCK_LOG2)

#define ARCHI_NB_FLL 3

#define __RT_FLL_CL	2
#define __RT_FLL_PERIPH 1
#define __RT_FLL_FC	0

#define __RT_FREQ_DOMAIN_FC	0
#define __RT_FREQ_DOMAIN_CL	2
#define __RT_FREQ_DOMAIN_PERIPH 1
#define RT_FREQ_NB_DOMAIN	3

/* Cores & cluster */
/* FC */
#define ARCHI_FC_CID (32)

/* Cluster */
#define ARCHI_HAS_CLUSTER   (1)
#define ARCHI_CL_CID(id)    (id)
#define ARCHI_CLUSTER_NB_PE (8) /* Processing elements. */

/* Memories */
/* FC memory */
#define ARCHI_HAS_FC_TCDM  (1)
#define ARCHI_HAS_FC_ALIAS (1)
#define FC_TCDM_SIZE	   (0x00004000) /* 16kB. */

/* L2 memory */
#define ARCHI_HAS_L2_ALIAS (0)
#define L2_SHARED_SIZE	   (0x00080000) /* 512kB. */

/* L1 cluster memory */
#define ARCHI_HAS_CL_L1_ALIAS (1)
#define CL_L1_SIZE	      (0x00010000) /* 64kB. */

/* L1 cluster TS */
#define ARCHI_HAS_CL_L1_TS (1)


/* TODO: fix this table */
/* UDMA peripherals */
#define UDMA_HAS_SPIM	(1)
#define UDMA_HAS_HYPER	(0)
#define UDMA_HAS_UART	(1)
#define UDMA_HAS_I2C	(1)
#define UDMA_HAS_DMACPY (0)
#define UDMA_HAS_I2S	(1)
#define UDMA_HAS_CPI	(1)

/* TODO: fix this table */
/* Number of UDMA peripherals */
#define UDMA_NB_SPIM   (2)
#define UDMA_NB_HYPER  (0)
#define UDMA_NB_UART   (1)
#define UDMA_NB_I2C    (2)
#define UDMA_NB_DMACPY (0)
#define UDMA_NB_I2S    (1)
#define UDMA_NB_CPI    (1)

/* TODO: fix this table */
/* #define UDMA_NB_PERIPH              ((UDMA_HAS_SPIM ? UDMA_NB_SPIM) + \ */
/*                                      (UDMA_HAS_HYPER ? UDMA_NB_HYPER) + \ */
/*                                      (UDMA_HAS_UART ? UDMA_NB_UART) + \ */
/*                                      (UDMA_HAS_I2C ? UDMA_NB_I2C) + \ */
/*                                      (UDMA_HAS_DMACPY ? UDMA_NB_DMACPY) + \
 */
/*                                      (UDMA_HAS_I2S ? UDMA_NB_I2S) + \ */
/*                                      (UDMA_HAS_CPI ? UDMA_NB_CPI)) */
#define UDMA_NB_PERIPH (10)
/* Size of each UDMA peripheral */
#define UDMA_PERIPH_SIZE_LOG2 (7)
#define UDMA_PERIPH_SIZE      (1 << UDMA_PERIPH_SIZE_LOG2)

/* UDMA peripherals ID, this maps to PER_ID_* in udma_subsystem.sv */
#define UDMA_SPIM_ID(id) (1 + (id))
/* #define UDMA_HYPER_ID(id)           (3   + (id)) */
#define UDMA_UART_ID(id) (0 + (id))
#define UDMA_I2C_ID(id)	 (2 + (id))
/* #define UDMA_DMACPY_ID(id)          (7   + (id)) */
#define ARCHI_UDMA_FILTER_ID(id) (7 + (id))
#define UDMA_I2S_ID(id)		 (5 + (id))
#define UDMA_CPI_ID(id)		 (6 + (id))
#define UDMA_SDIO_ID(id)	 (4 + (id))


/* Pads & GPIO. */
#define ARCHI_NB_PAD  (48)
#define ARCHI_NB_GPIO (32)



/* Cluster. */
/* Cores & cluster. */
#define ARCHI_HAS_CLUSTER           (1)
#define ARCHI_CL_CID(id)            (id)
#define ARCHI_CLUSTER_NB_CORES      (9)
#define ARCHI_CLUSTER_NB_PE         (8) /* Processing elements. */
#define ARCHI_CLUSTER_PE_MASK       (((1 << ARCHI_CLUSTER_NB_PE) - 1))
#define ARCHI_CLUSTER_MASTER_CORE   (0)
#define ARCHI_CLUSTER_SYNC_BARR_ID  (1)
#define ARCHI_CLUSTER_LEGCY_BARR_ID (0)

#define ARCHI_HAS_CLUSTER_CLK_GATE  (1)
#define ARCHI_CLUSTER_SIZE          (0x00400000)

/* Memories. */
/* L1 cluster memory. */
#define ARCHI_HAS_CL_L1_ALIAS       (1)
#define CL_L1_SIZE                  (0x00010000) /* 64kB. */

/* L1 cluster memory TS. */
#define ARCHI_HAS_CL_L1_TS          (1)
#define ARCHI_CL_L1_TS_OFFSET       (1 << 20)


/* Cluster peripherals. */
#define CL_NB_HW_MUTEX              (8)
#define CL_NB_HW_BARRIER            (8)


#define CL_EU_HW_BARRIER_RESERVED       (2)
#define CL_EU_HW_BARRIER_RESERVED_MASK  ((1 << CL_EU_HW_BARRIER_RESERVED) - 1)

#define CL_ALLOC_INIT_BARRIER           (((1 << CL_NB_HW_BARRIER) - 1) & \
                                         ~CL_EU_HW_BARRIER_RESERVED_MASK)


#define CL_CTRL_OFFSET              (0x00000000)
#define CL_TIMER_OFFSET             (0x00000400)
#define CL_GLOB_EU_CORE_OFFSET      (0x00000800)
#define CL_GLOB_EU_BARRIER_OFFSET   (0x00000C00)
#define CL_GLOB_EU_SW_EVENT_OFFSET  (0x00000E00)
#define CL_GLOB_EU_SOC_EVENT_OFFSET (0x00000F00)
#define CL_GLOB_EU_DISPATCH_OFFSET  (0x00000F80)
#define CL_HWCE_OFFSET              (0x00001000)
#define CL_ICACHE_CTRL_OFFSET       (0x00001400)
#define CL_DMA_OFFSET               (0x00001800)
#define CL_DECOMP_OFFSET            (0x00002000)
#define CL_DEMUX_PERIPH_OFFSET      (0x00004000)


/*! Event_Unit Demux offset */
#define CLUSTER_DEMUX_EU_CORE_OFFSET              ( 0x00000000 )
#define CLUSTER_DEMUX_EU_LOOP_OFFSET              ( 0x00000060 )
#define CLUSTER_DEMUX_EU_DISPATCH_OFFSET          ( 0x00000080 )
#define CLUSTER_DEMUX_EU_HW_MUTEX_OFFSET          ( 0x000000C0 )
#define CLUSTER_DEMUX_EU_SW_EVENT_OFFSET          ( 0x00000100 )
#define CLUSTER_DEMUX_EU_HW_BARRIER_OFFSET        ( 0x00000200 )


/*! Event_Unit Core Demux */
#define CL_DEMUX_EU_CORE_EVENT_MASK               ( 0x00 )
#define CL_DEMUX_EU_CORE_EVENT_MASK_AND           ( 0x04 )
#define CL_DEMUX_EU_CORE_EVENT_MASK_OR            ( 0x08 )
#define CL_DEMUX_EU_CORE_IRQ_MASK                 ( 0x0C )
#define CL_DEMUX_EU_CORE_IRQ_MASK_AND             ( 0x10 )
#define CL_DEMUX_EU_CORE_IRQ_MASK_OR              ( 0x14 )
#define CL_DEMUX_EU_CORE_STATUS                   ( 0x18 )
#define CL_DEMUX_EU_CORE_BUFFER                   ( 0x1C )
#define CL_DEMUX_EU_CORE_BUFFER_MASKED            ( 0x20 )
#define CL_DEMUX_EU_CORE_BUFFER_IRQ_MASKED        ( 0x24 )
#define CL_DEMUX_EU_CORE_BUFFER_CLEAR             ( 0x28 )
#define CL_DEMUX_EU_CORE_SW_EVT_MASK              ( 0x2C )
#define CL_DEMUX_EU_CORE_SW_EVT_MASK_AND          ( 0x30 )
#define CL_DEMUX_EU_CORE_SW_EVT_MASK_OR           ( 0x34 )
#define CL_DEMUX_EU_CORE_EVENT_WAIT               ( 0x38 )
#define CL_DEMUX_EU_CORE_EVENT_WAIT_CLEAR         ( 0x3C )
#define CL_DEMUX_EU_CORE_SEC_IRQ_MASK             ( 0x40 )
#define CL_DEMUX_EU_CORE_SEC_IRQ_MASK_AND         ( 0x44 )
#define CL_DEMUX_EU_CORE_SEC_IRQ_MASK_OR          ( 0x48 )

/*! Event_Unit Loop Demux */
#define CL_DEMUX_EU_LOOP_STATE                    ( 0x00 )
#define CL_DEMUX_EU_LOOP_START                    ( 0x04 )
#define CL_DEMUX_EU_LOOP_END                      ( 0x08 )
#define CL_DEMUX_EU_LOOP_INCR                     ( 0x0C )
#define CL_DEMUX_EU_LOOP_CHUNK                    ( 0x10 )
#define CL_DEMUX_EU_LOOP_EPOCH                    ( 0x14 )
#define CL_DEMUX_EU_LOOP_SINGLE                   ( 0x18 )

/*! Event_Unit Dispatch Demux */
#define CL_DEMUX_EU_DISPATCH_FIFO_ACCESS          ( 0x00 )
#define CL_DEMUX_EU_DISPATCH_TEAM_CONFIG          ( 0x04 )

/*! Event_Unit Mutex Demux */
#define CL_DEMUX_EU_HW_MUTEX_MUTEX                ( 0x00 )

/*! Event_Unit SW_Events Demux */
#define CL_DEMUX_EU_SW_EVT_TRIGGER                ( 0x00 )
#define CL_DEMUX_EU_SW_EVT_TRIGGER_WAIT           ( 0x40 )
#define CL_DEMUX_EU_SW_EVT_TRIGGER_WAIT_CLEAR     ( 0x80 )

/*! Event_Unit HW Barrier Demux */
#define CL_DEMUX_EU_HW_BARRIER_TRIGGER_MASK       ( 0x00 )
#define CL_DEMUX_EU_HW_BARRIER_STATUS             ( 0x04 )
#define CL_DEMUX_EU_HW_BARRIER_STATUS_SUMMARY     ( 0x08 )
#define CL_DEMUX_EU_HW_BARRIER_TARGET_MASK        ( 0x0C )
#define CL_DEMUX_EU_HW_BARRIER_TRIGGER            ( 0x10 )
#define CL_DEMUX_EU_HW_BARRIER_TRIGGER_SELF       ( 0x14 )
#define CL_DEMUX_EU_HW_BARRIER_TRIGGER_WAIT       ( 0x18 )
#define CL_DEMUX_EU_HW_BARRIER_TRIGGER_WAIT_CLEAR ( 0x1C )

#define CL_DEMUX_EU_HW_BARRIER_SIZE               ( 0x20 )


/* Cluster demux peripherals.*/
#define CL_DEMUX_EU_CORE_OFFSET         (CL_DEMUX_PERIPH_OFFSET + CLUSTER_DEMUX_EU_CORE_OFFSET)
#define CL_DEMUX_EU_LOOP_OFFSET         (CL_DEMUX_PERIPH_OFFSET + CLUSTER_DEMUX_EU_LOOP_OFFSET)
#define CL_DEMUX_EU_DISPATCH_OFFSET     (CL_DEMUX_PERIPH_OFFSET + CLUSTER_DEMUX_EU_DISPATCH_OFFSET)
#define CL_DEMUX_EU_HW_MUTEX_OFFSET     (CL_DEMUX_PERIPH_OFFSET + CLUSTER_DEMUX_EU_HW_MUTEX_OFFSET)
#define CL_DEMUX_EU_SW_EVENT_OFFSET     (CL_DEMUX_PERIPH_OFFSET + CLUSTER_DEMUX_EU_SW_EVENT_OFFSET)
#define CL_DEMUX_EU_HW_BARRIER_OFFSET   (CL_DEMUX_PERIPH_OFFSET + CLUSTER_DEMUX_EU_HW_BARRIER_OFFSET)
#define CL_DEMUX_DMA_OFFSET             (CL_DEMUX_PERIPH_OFFSET + 0x00000400)


#define CL_DMA_ID(id)                   (id)


#define PRINTF_BUFFER_SIZE              ( 128 )
#endif /* __PROPERTIES_H__ */
