#
# Copyright (C) 2019 GreenWaves Technologies
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# Authors: Germain Haugou, GreenWaves Technologies (germain.haugou@greenwaves-technologies.com)
#

#include "archi/pulp.h"

    .section .text_l2, "ax"
    .global pos_init_entry
pos_init_entry:

    # Performance counters are active after reset, deactivate them to let the
    # user control when they should start counting
#ifdef __cv32e40p__
    # No PCMR on CV32E40P; use mcountinhibit.
    li      t0, -1
    csrw    0x320, t0
#else
    csrw    0xCC1, x0
#endif

#if defined(__riscv_flen)
    # mstatus.FS resets to Off, and a core with a separate FP register file
    # (F extension, as opposed to Zfinx) traps every FP instruction until
    # software enables it.
    li      t0, 0x2000           # mstatus.FS = Initial (bits [14:13] = 0b01)
    csrs    mstatus, t0
#endif

#ifdef ARCHI_NO_FC
    csrr    a0, 0xF14
    andi    a1, a0, 0x1f         # core offset within the cluster
    beqz    a1, pos_no_fc_core0  # core 0 -> boot as FC (fall through)
    j       pos_pe_start         # cores 1..N-1 -> slave dispatch loop
pos_no_fc_core0:
#else
#ifdef ARCHI_HAS_CLUSTER

    # Cluster PEs will also starts here to avoid aligning another entry point
    # Just re-route them to the right entry
    csrr    a0, 0xF14
    andi    a1, a0, 0x1f
    srli    a0, a0, 5
    li      a2, ARCHI_FC_CID
#    bne     a0, a2, pos_pe_start
    beq     a0, a2, label0
    j      pos_pe_start
label0:

#endif
#endif


    # Clear the bss segment
    la      t0, _bss_start
    la      t1, _bss_end
1:
    sw      zero,0(t0)
    addi    t0, t0, 4
    bltu    t0, t1, 1b



    # Stack initialization
    la   x2, stack



    /* Do all other initializations from C code */
    la   t0, pos_init_start
    jalr x1, t0



.section .text_l2

    # On all other chips we simply pass 0.
    addi  a0, x0, 0
    addi  a1, x0, 0

    # Jump to main program entry point (argc = a0, argv = a1).
    la    t2, main
    jalr  x1, t2
    mv    s0, a0

    # If program returns from main, call exit routine
    mv   a0, s0
    la   t0, exit
    jalr x1, t0




.section .vectors, "ax"
.option norvc;


#ifdef ARCHI_CORE_HAS_1_10
    j pos_illegal_instr
#else
    j pos_no_irq_handler
#endif

    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler


    .org 0x80
    .global _start
_start:
    la  a0, pos_init_entry
    jalr x0, a0


pos_illegal_instr:
    j pos_illegal_instr

pos_no_irq_handler:
    mret


    .global pos_semihosting_call
pos_semihosting_call:
    ebreak
    jr          ra

