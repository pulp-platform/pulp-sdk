/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
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

#ifndef _ARCHI_RISCV_PCER_V1_H
#define _ARCHI_RISCV_PCER_V1_H

/*
 * Bit definitions for Performance counters mode registers
 *
 */
#define CSR_MHPMEVENT_CYCLES   0  /* Count the number of cycles the core was running */
#define CSR_MHPMEVENT_INSTR    1  /* Count the number of instructions executed */
#define CSR_MHPMEVENT_LD_STALL   2  /* Number of load use hazards */
#define CSR_MHPMEVENT_JMP_STALL    3  /* Number of jump register hazards */
#define CSR_MHPMEVENT_IMISS    4  /* Cycles waiting for instruction fetches. i.e. the number of instructions wasted due to non-ideal caches */
#define CSR_MHPMEVENT_LD   5  /* Number of memory loads executed. Misaligned accesses are counted twice */
#define CSR_MHPMEVENT_ST   6  /* Number of memory stores executed. Misaligned accesses are counted twice */
#define CSR_MHPMEVENT_JUMP   7  /* Number of jump instructions seen, i.e. j, jr, jal, jalr */
#define CSR_MHPMEVENT_BRANCH   8  /* Number of branch instructions seen, i.e. bf, bnf */
#define CSR_MHPMEVENT_TAKEN_BRANCH 9  /* Number of taken branch instructions seen, i.e. bf, bnf */
#define CSR_MHPMEVENT_RVC    10  /* Number of compressed instructions */
#define CSR_MHPMEVENT_PIPE_STALL    11  /* Cycles from stalled pipeline */
#define CSR_MHPMEVENT_APU_TYPE   12  /* Number of conflicts on APU/FP */
#define CSR_MHPMEVENT_APU_CONT   13  /* Number of contentions on APU/FP */
#define CSR_MHPMEVENT_APU_DEP 14  /* Number of dependency on APU/FP */
#define CSR_MHPMEVENT_APU_WB 15  /* Number of write backs on APU/FP */


#define CSR_MHPMEVENT_NB_EVENTS          16
#define CSR_MHPMEVENT_NB_INTERNAL_EVENTS 12
#define CSR_NUM_MHPMCOUNTERS             31

// Gives from the event ID, the HW mask that can be stored (with an OR with other events mask) to the PCER
#define CSR_PCER_EVENT_MASK(eventId)  (1<<(eventId))
#define CSR_PCER_ALL_EVENTS_MASK  0xffffffff

#define CSR_MCOUNTINHIBIT_ACTIVE  0x0 /* Activate counting in all performance counters*/
#define CSR_MCOUNTINHIBIT_RESET 0xfffffffd /* Inhibit/reset counting in all performance counters*/


#define CSR_PCER_NAME(id) (id == 0 ? "Cycles" : id == 1 ? "Instructions" : id == 2 ? "LD_Stall" : id == 3 ? "Jmp_Stall" : id == 4 ? "IMISS" : id == 5 ? "LD" : id == 6 ? "ST" : id == 7 ? "JUMP" : id == 8 ? "BRANCH" : id == 9 ? "TAKEN_BRANCH" : id == 10 ? "RVC" : id == 11 ? "ELW" : id == 12 ? "LD_EXT" : id == 13 ? "ST_EXT" : id == 14 ? "LD_EXT_CYC" : id == 15 ? "ST_EXT_CYC" : id == 16 ? "TCDM_CONT" : "NA")

#endif
