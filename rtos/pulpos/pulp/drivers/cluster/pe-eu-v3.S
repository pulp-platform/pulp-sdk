/*
 * Copyright (C) 2019 GreenWaves Technologies
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
 * Authors: Germain Haugou, GreenWaves Technologies (germain.haugou@greenwaves-technologies.com)
 */

#include <pos/data/data.h>

    .section .cluster.text , "ax"

    .global pos_pe_start
pos_pe_start:
    csrr    a0, 0xF14
    andi    a1, a0, 0x1f
    srli    a0, a0, 5

    // Activate a few events
    li      t0, (1<<ARCHI_CL_EVT_DISPATCH) | (1<<ARCHI_CL_EVT_BAR) | (1<<ARCHI_CL_EVT_MUTEX)
    li      t1, ARCHI_EU_DEMUX_ADDR
    sw      t0, EU_CORE_MASK(t1)

#ifdef ARCHI_CC_CORE_ID
    li      t2, ARCHI_CC_CORE_ID
#else
    li      t2, 0
#endif
    bne     a1, t2, pos_slave_start

    li      t0, (1<<ARCHI_CL_EVT_DMA1)
    li      t1, ARCHI_EU_DEMUX_ADDR
    sw      t0, EU_CORE_MASK_IRQ_OR(t1)

    // Prepare few values that will be kept in saved registers to optimize the loop
    la      s0, pos_cluster_pool
    li      s3, ARCHI_EU_DEMUX_ADDR
    li      s4, 1<<POS_EVENT_CLUSTER_CALL_EVT
    la      s5, pos_master_event
    la      s7, pos_cluster
    li      t2, POS_CLUSTER_T_SIZEOF
    mul     t2, t2, a0
    add     s7, s7, t2
    addi    s7, s7, POS_CLUSTER_T_CL_TASKS
    li      s9, ARCHI_FC_ITC_ADDR + ITC_STATUS_SET_OFFSET
    li      s8, 1<<POS_EVENT_FC_ENQUEUE

    la      s10, pos_set_slave_stack
    ori     s10, s10, 1

    csrwi   0x300, 0x8

    j       pos_master_loop






pos_master_event:
    lw      s6, PI_CLUSTER_TASK_COMPLETION_CALLBACK(s6)
    beq     s6, x0, pos_master_loop

pos_push_event_to_fc_retry:
    // Now we have to push the termination event to FC side
    // First wait until the slot for posting events is free
    lw      t0, 0(s7)
    bne     t0, x0, pos_push_event_to_fc_wait

    // Push it
    sw      s6, 0(s7)

    // And notify the FC side with a HW event in case it is sleeping
    sw      s8, 0(s9)


pos_master_loop:
    // Check if a task is ready in the pool
    lw      s6, 0(s0)

    // Check if a call is ready, e.g. if nb_pe is not zero
    // otherwise go to sleep
    beq     s6, x0, pos_master_sleep

pos_master_loop_update_next:
    lw      t4, PI_CLUSTER_TASK_NEXT(s6)
    sw      t4, 0(s0)

    // Check again next pointer in case it was updated by the FC.
    // If so, do it it again as this will ensure that either we see the new
    // value or the FC sees our write
    lw      t5, PI_CLUSTER_TASK_NEXT(s6)
    bne     t4, t5, pos_master_loop_update_next

pos_master_loop_exec_task:
    // Reads entry point information
    lw      a0, PI_CLUSTER_TASK_ARG(s6)
    lw      t0, PI_CLUSTER_TASK_ENTRY(s6)
    lw      sp, PI_CLUSTER_TASK_STACKS(s6)
    lw      t1, PI_CLUSTER_TASK_STACK_SIZE(s6)
    lw      t2, PI_CLUSTER_TASK_SLAVE_STACK_SIZE(s6)
    lw      t5, PI_CLUSTER_TASK_CORE_MASK(s6)
    lw      t6, PI_CLUSTER_TASK_NB_CORES(s6)
    mv      ra, s5

    add     sp, sp, t1

#ifdef CONFIG_NO_STD_RELOC
    sw      t6, %tiny(pos_cluster_nb_active_pe)(x0)
#else
    sw      t6, pos_cluster_nb_active_pe, t3
#endif

pos_no_stack_check:
    // Whatever the number of cores, we need to setup the barrier as the master code is compiled to use it
    sw      t5, EU_DISPATCH_DEMUX_OFFSET + EU_DISPATCH_TEAM_CONFIG(s3)
    // When we have a cluster controller, don't configure the slave barrier
    // if we don't have have any slave
#ifdef ARCHI_CC_CORE_ID
    beqz    t5, pos_master_no_slave_barrier
#endif
    sw      t5, EU_BARRIER_DEMUX_OFFSET + EU_HW_BARR_TRIGGER_MASK(s3)
    sw      t5, EU_BARRIER_DEMUX_OFFSET + EU_HW_BARR_TARGET_MASK(s3)
pos_master_no_slave_barrier:
#ifdef ARCHI_CC_CORE_ID
    ori     t6, t5, 1<<ARCHI_CC_CORE_ID
    sw      t6, EU_BARRIER_DEMUX_OFFSET + EU_HW_BARR_TRIGGER_MASK + EU_BARRIER_SIZE(s3)
    sw      t6, EU_BARRIER_DEMUX_OFFSET + EU_HW_BARR_TARGET_MASK + EU_BARRIER_SIZE(s3)
#endif

    // Set stack on slaves
    // For that we push first the function for setting stack, then the stack size and the base
    p.beqimm t5, 0, pos_master_loop_no_slave
    sw      s10, EU_DISPATCH_DEMUX_OFFSET + EU_DISPATCH_FIFO_ACCESS(s3)
    sw      t2, EU_DISPATCH_DEMUX_OFFSET + EU_DISPATCH_FIFO_ACCESS(s3)
    sw      sp, EU_DISPATCH_DEMUX_OFFSET + EU_DISPATCH_FIFO_ACCESS(s3)

pos_master_loop_no_slave:

    // Call the entry point, this will directly come back to the master loop
    jr      t0


pos_master_sleep:
    sw      s4, EU_CORE_MASK_OR(s3)
    p.elw   x0, EU_CORE_EVENT_WAIT_CLEAR(s3)
    sw      s4, EU_CORE_MASK_AND(s3)
    j       pos_master_loop



pos_push_event_to_fc_wait:
    sw      s4, EU_CORE_MASK_OR(s3)
    p.elw   x0, EU_CORE_EVENT_WAIT_CLEAR(s3)
    sw      s4, EU_CORE_MASK_AND(s3)
    j       pos_push_event_to_fc_retry



























pos_slave_start:

    li      s2, ARCHI_EU_DEMUX_ADDR
    csrr    s3, 0xF14
    and     s3, s3, 0x1f


    la      s4, pos_fork_return
    la      s5, pos_wait_for_dispatch
    j       pos_wait_for_dispatch


pos_fork_return:

#ifdef ARCHI_HAS_CC
    // When the cluster has a controller barrier 0 is used for normal team barrier
    // and barrier 1 is used for end of offload
    p.elw   t0, EU_BARRIER_DEMUX_OFFSET + EU_HW_BARR_TRIGGER_WAIT_CLEAR + EU_BARRIER_SIZE(s2)
#else
    p.elw   t0, EU_BARRIER_DEMUX_OFFSET + EU_HW_BARR_TRIGGER_WAIT_CLEAR(s2)
#endif    

pos_wait_for_dispatch:

    // Wait for PC + arg information from dispatcher
    p.elw   t0, EU_DISPATCH_DEMUX_OFFSET + EU_DISPATCH_FIFO_ACCESS(s2)
    p.elw   a0, EU_DISPATCH_DEMUX_OFFSET + EU_DISPATCH_FIFO_ACCESS(s2)

    // Check if this is an entry with a barrier at the end (fork entry)
    andi    t1, t0, 1
    bne     t1, zero, pos_other_entry

pos_fork_entry:

    // Jump to the handler and prepare r9 to jump back just before the main loop
    add     ra, s4, x0
    jr      t0

pos_other_entry:

  // Jump to the handler and prepare r9 to jump back directly to the main loop
    add     ra, s5, x0
    jr      t0



  .global pos_set_slave_stack
pos_set_slave_stack:

    // Multiply the stack size by the core ID and add the stack base to get our stack
    p.elw   t0, EU_DISPATCH_DEMUX_OFFSET + EU_DISPATCH_FIFO_ACCESS(s2)
#if defined(CONFIG_PULP)
    addi     t5, s3, 0
#else
    // If the cluster has a cluster controller, the first slave has core ID 0
    // and thus we need to take the next stack
    addi     t5, s3, 1
#endif
    p.mul   t4, t5, a0
    add     sp, t4, t0

    ret
