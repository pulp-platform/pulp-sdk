/*
 * Copyright (C) 2026 Fondazione Chips-IT
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

#ifndef __ARCHI_RISCV_PCER_CV32E40P_H__
#define __ARCHI_RISCV_PCER_CV32E40P_H__

/* Event ids -- from hpm_events[] in cv32e40p_cs_registers.sv. */
#define CV32E40P_PERF_CYCLES          0  /* Cycles (event line is constant 1) */
#define CV32E40P_PERF_INSTR           1  /* Instructions retired (minstret) */
#define CV32E40P_PERF_LD_STALL        2  /* Load-use hazards */
#define CV32E40P_PERF_JMP_STALL       3  /* Jump-register hazards */
#define CV32E40P_PERF_IMISS           4  /* Cycles waiting for instruction fetch, excluding jumps/branches */
#define CV32E40P_PERF_LD              5  /* Loads */
#define CV32E40P_PERF_ST              6  /* Stores */
#define CV32E40P_PERF_JUMP            7  /* Unconditional jumps (j, jr, jal, jalr) */
#define CV32E40P_PERF_BRANCH          8  /* Conditional branches */
#define CV32E40P_PERF_TAKEN_BRANCH    9  /* Conditional branches taken */
#define CV32E40P_PERF_RVC            10  /* Compressed instructions */
#define CV32E40P_PERF_ELW            11  /* Extra cycles from ELW; needs COREV_CLUSTER */
#define CV32E40P_PERF_APU_TYPECONF   12  /* APU type conflict; needs FPU */
#define CV32E40P_PERF_APU_CONT       13  /* APU contention */
#define CV32E40P_PERF_APU_DEP        14  /* APU dependency stall */
#define CV32E40P_PERF_APU_WB         15  /* APU writeback */

#define CV32E40P_PERF_NB_EVENTS      16

/*
 * RI5CY events 12..16 (LD_EXT, ST_EXT, LD_EXT_CYC, ST_EXT_CYC, TCDM_CONT) do
 * not exist here -- those ids are the APU events instead. Reading them gives
 * APU counts or zero, not external-memory or contention data. Use this to skip
 * them rather than reporting a wrong number.
 */
#define CV32E40P_PERF_EVENT_VALID(id)  ((id) < CV32E40P_PERF_NB_EVENTS)

/* CSRs */
#define CV32E40P_CSR_MCOUNTINHIBIT  0x320
#define CV32E40P_CSR_MHPMEVENT3     0x323
#define CV32E40P_CSR_MCYCLE         0xB00
#define CV32E40P_CSR_MINSTRET       0xB02
#define CV32E40P_CSR_MHPMCOUNTER3   0xB03

/* Event N is counted by mhpmcounter(3+N). */
#define CV32E40P_PERF_COUNTER_ID(evt)   (3 + (evt))
#define CV32E40P_PERF_INHIBIT_MASK      0xFFFFFFFFu

/*
 * (event id, counter CSR, event-select CSR). Literals, because inline asm needs
 * the CSR number as a token -- the preprocessor cannot compute 0xB03+N.
 */
#define CV32E40P_PERF_FOREACH(F)   \
  F( 0, 0xB03, 0x323)              \
  F( 1, 0xB04, 0x324)              \
  F( 2, 0xB05, 0x325)              \
  F( 3, 0xB06, 0x326)              \
  F( 4, 0xB07, 0x327)              \
  F( 5, 0xB08, 0x328)              \
  F( 6, 0xB09, 0x329)              \
  F( 7, 0xB0A, 0x32A)              \
  F( 8, 0xB0B, 0x32B)              \
  F( 9, 0xB0C, 0x32C)              \
  F(10, 0xB0D, 0x32D)              \
  F(11, 0xB0E, 0x32E)              \
  F(12, 0xB0F, 0x32F)              \
  F(13, 0xB10, 0x330)              \
  F(14, 0xB11, 0x331)              \
  F(15, 0xB12, 0x332)

#define CV32E40P_PERF_NAME(id) \
  ((id) ==  0 ? "Cycles"       : (id) ==  1 ? "Instructions" : \
   (id) ==  2 ? "LD_Stall"     : (id) ==  3 ? "Jmp_Stall"    : \
   (id) ==  4 ? "IMISS"        : (id) ==  5 ? "LD"           : \
   (id) ==  6 ? "ST"           : (id) ==  7 ? "JUMP"         : \
   (id) ==  8 ? "BRANCH"       : (id) ==  9 ? "TAKEN_BRANCH" : \
   (id) == 10 ? "RVC"          : (id) == 11 ? "ELW"          : \
   (id) == 12 ? "APU_TYPECONF" : (id) == 13 ? "APU_CONT"     : \
   (id) == 14 ? "APU_DEP"      : (id) == 15 ? "APU_WB"       : "NA")

#endif
