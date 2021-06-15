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

#ifndef __HAL_RISCV_RISCV_V5_H__
#define __HAL_RISCV_RISCV_V5_H__

#include "archi/pulp.h"

#include "hal/riscv/types.h"
#include "archi/riscv/builtins_v2.h"
#include "archi/riscv/builtins_v2_emu.h"

#ifndef ACHI_HAS_COREV
#define CSR_PCMR_ACTIVE 0x1
#else
#define CSR_MCOUNTINHIBIT_ACTIVE  0x0
#define CSR_MCOUNTINHIBIT_INHIBIT 0xfffffffd
#endif

#define SR_MTVEC  0x305


#if defined(__OPTIMIZE__) && defined(CORE_PULP_BUILTINS) && !defined(__LLVM__)

static inline unsigned int hal_spr_read_then_clr(unsigned int reg, unsigned int val)
{
  return __builtin_pulp_read_then_spr_bit_clr(reg, val);
}

static inline unsigned int hal_spr_read_then_set(unsigned int reg, unsigned int val)
{
  return __builtin_pulp_read_then_spr_bit_set(reg, val);
}

static inline void hal_spr_write(unsigned int reg, unsigned int val)
{
  __builtin_pulp_spr_write(reg, val);
}

static inline unsigned int hal_spr_read(unsigned int reg)
{
  return __builtin_pulp_spr_read(reg);
}

#else

#if defined(__LLVM__)

#else

#define hal_spr_read_then_clr(reg,val) \
  ({ \
    int state; \
    asm volatile ("csrrc %0, %1, %2" :  "=r" (state) : "I" (reg), "I" (val) ); \
    state; \
  })

#define hal_spr_read_then_set(reg,val) \
  ({ \
    int state; \
    asm volatile ("csrrs %0, %1, %2" :  "=r" (state) : "I" (reg), "I" (val) ); \
    state; \
  })

#define hal_spr_read_then_clr_from_reg(reg,val) \
  ({ \
    int state; \
    asm volatile ("csrrc %0, %1, %2" :  "=r" (state) : "I" (reg), "r" (val) ); \
    state; \
  })

#define hal_spr_read_then_set(reg,val) \
  ({ \
    int state; \
    asm volatile ("csrrs %0, %1, %2" :  "=r" (state) : "I" (reg), "I" (val) ); \
    state; \
  })

#define hal_spr_read_then_set_from_reg(reg,val) \
  ({ \
    int state; \
    asm volatile ("csrrs %0, %1, %2" :  "=r" (state) : "I" (reg), "r" (val) ); \
    state; \
  })

#define hal_spr_write(reg,val) \
do { \
  asm volatile ("csrw %0, %1" :  : "I" (reg), "r" (val) ); \
} while(0)

#define hal_spr_read(reg) \
({ \
  int result; \
  asm volatile ("csrr %0, %1" : "=r" (result) : "I" (reg) ); \
  result; \
})

#endif

#endif





#if defined(__LLVM__)

#define csr_read(csr)           \
({                \
  register unsigned int __v;       \
  __asm__ __volatile__ ("csrr %0, " #csr      \
            : "=r" (__v));      \
  __v;              \
})

#define hal_mepc_read() csr_read(0x341)

#else
#define hal_mepc_read() hal_spr_read(RV_CSR_MEPC)
#endif

static inline unsigned int core_id() {
  int hart_id;
#if RISCV_VERSION >= 4 && !defined(RISCV_1_7)
#if PULP_CHIP_FAMILY == CHIP_GAP
  asm("csrr %0, 0x014" : "=r" (hart_id) : );
#else
  asm("csrr %0, 0xF14" : "=r" (hart_id) : );
#endif
#else
  asm("csrr %0, 0xF10" : "=r" (hart_id) : );
#endif
  // in PULP the hart id is {22'b0, cluster_id, core_id}
  return hart_id & 0x01f;
}

static inline unsigned int cluster_id() {  int hart_id;
#if RISCV_VERSION >= 4 && !defined(RISCV_1_7)
#if PULP_CHIP_FAMILY == CHIP_GAP
  asm("csrr %0, 0x014" : "=r" (hart_id) : );
#else
  asm("csrr %0, 0xF14" : "=r" (hart_id) : );
#endif
#else
  asm("csrr %0, 0xF10" : "=r" (hart_id) : );
#endif
  // in PULP the hart id is {22'b0, cluster_id, core_id}
  return (hart_id >> 5) & 0x3f;
}

#ifndef PLP_NO_BUILTIN

static inline unsigned int hal_core_id() {
  return core_id();
  //return __builtin_pulp_CoreId();
}

static inline unsigned int hal_cluster_id() {
  //return cluster_id();
  return __builtin_pulp_ClusterId();
}

// TODO replace by compiler builtin
static inline __attribute__((always_inline)) unsigned int hal_has_fc() {
#ifdef ARCHI_HAS_FC
  return 1;
#else
  return 0;
#endif
}

static inline __attribute__((always_inline)) unsigned int hal_is_fc() {
#ifndef ARCHI_HAS_FC
  return 0;
#else
  if (hal_has_fc()) return hal_cluster_id() == ARCHI_FC_CID;
  else return 0;
#endif
}

#else

static inline __attribute__((always_inline)) unsigned int hal_core_id() {
  int hart_id;
#if RISCV_VERSION >= 4 && !defined(RISCV_1_7)
#if PULP_CHIP_FAMILY == CHIP_GAP
  asm("csrr %0, 0x014" : "=r" (hart_id) : );
#else
  asm("csrr %0, 0xF14" : "=r" (hart_id) : );
#endif
#else
  asm("csrr %0, 0xF10" : "=r" (hart_id) : );
#endif
  // in PULP the hart id is {22'b0, cluster_id, core_id}
  return hart_id & 0x01f;
}

static inline __attribute__((always_inline)) unsigned int hal_cluster_id() {
  int hart_id;
#if RISCV_VERSION >= 4 && !defined(RISCV_1_7)
#if PULP_CHIP_FAMILY == CHIP_GAP
  asm("csrr %0, 0x014" : "=r" (hart_id) : );
#else
  asm("csrr %0, 0xF14" : "=r" (hart_id) : );
#endif
#else
  asm("csrr %0, 0xF10" : "=r" (hart_id) : );
#endif
  // in PULP the hart id is {22'b0, cluster_id, core_id}
  return (hart_id >> 5) & 0x3f;
}

static inline __attribute__((always_inline)) unsigned int hal_has_fc() {
#ifdef ARCHI_HAS_FC
  return 1;
#else
  return 0;
#endif
}

static inline __attribute__((always_inline)) unsigned int hal_is_fc() {
#ifndef ARCHI_HAS_FC
  return 0;
#else
  if (hal_has_fc()) return hal_cluster_id() == ARCHI_FC_CID;
  else return 0;
#endif
}

#endif



#if defined(__LLVM__)

static inline int hal_irq_disable()
{
  return 0;
}

static inline void hal_irq_restore(int state)
{
}

static inline void hal_irq_enable()
{
}

#else

static inline int hal_irq_disable()
{
  int irq = hal_spr_read_then_clr(0x300, 0x1<<3);
  // This memory barrier is needed to prevent the compiler to cross the irq barrier
  __asm__ __volatile__ ("" : : : "memory");
  return irq;
}

static inline void hal_irq_restore(int state)
{
  // This memory barrier is needed to prevent the compiler to cross the irq barrier
  __asm__ __volatile__ ("" : : : "memory");
  hal_spr_write(0x300, state);
}

static inline void hal_irq_enable()
{
  // This memory barrier is needed to prevent the compiler to cross the irq barrier
  __asm__ __volatile__ ("" : : : "memory");
  hal_spr_read_then_set(0x300, 0x1<<3);
}

#endif

/*
 * PERFORMANCE COUNTERS
 *
 * API for accessing performance counters registers.
 * Have a look at file spr-defs.h to speficy registers through defines
 * SPR_PCER_* and SPR_PCMR_*
 */

#ifndef ARCHI_HAS_COREV
#define PCER_NB_EVENTS CSR_PCER_NB_EVENTS
#define PCER_ALL_EVENTS_MASK CSR_PCER_ALL_EVENTS_MASK
#define PCMR_ACTIVE CSR_PCMR_ACTIVE
#define PCMR_SATURATE CSR_PCMR_SATURATE
#else
#define MCOUNTINHIBIT_ACTIVE CSR_MCOUNTINHIBIT_ACTIVE
#define MCOUNTINHIBIT_RESET CSR_MCOUNTINHIBIT_RESET
#endif

#define CSR_CONVERT(x)    #x
#define CSR_WRITE(x, var)  asm volatile ("csrw "CSR_CONVERT(x)", %0" :: "r" (var))
#define CSR_READ(x, var)   asm volatile ("csrr %0, "CSR_CONVERT(x)"" : "=r" (var) :)

/* Configure the active events. eventMask is an OR of events got through SPR_PCER_EVENT_MASK */
static inline void cpu_perf_conf_events(unsigned int eventMask)
{
#ifndef PLP_NO_PERF_COUNTERS
  #ifndef ARCHI_HAS_COREV
  CSR_WRITE(CSR_PCER, eventMask);
  #else

  /* mhpmcounter3 is used for sharing events when only one counter is implemented */
  CSR_WRITE(CSR_MHPMEVENT(0),  eventMask);

  /* Associate events to counters 1:1 using the Event Selector*/
  CSR_WRITE(CSR_MHPMEVENT(1),  (1 << 0)  & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(2),  (1 << 1)  & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(3),  (1 << 2)  & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(4),  (1 << 3)  & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(5),  (1 << 4)  & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(6),  (1 << 5)  & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(7),  (1 << 6)  & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(8),  (1 << 7)  & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(9),  (1 << 8)  & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(10), (1 << 9)  & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(11), (1 << 10) & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(12), (1 << 11) & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(13), (1 << 12) & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(14), (1 << 13) & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(15), (1 << 14) & eventMask);
  CSR_WRITE(CSR_MHPMEVENT(16), (1 << 15) & eventMask);
  #endif
#endif
}

/* Return events configuration */
static inline unsigned int cpu_perf_conf_events_get()
{
#ifndef PLP_NO_PERF_COUNTERS
  unsigned int result;
  CSR_READ(CSR_PCER, result);
  return result;
#else
  return 0;
#endif
}

/* Configure the mode. confMask is an OR of all SPR_PCMR_* macros */
static inline void cpu_perf_conf(unsigned int confMask)
{
#ifndef PLP_NO_PERF_COUNTERS
  #ifndef ARCHI_HAS_COREV
  CSR_WRITE(CSR_PCMR, confMask);
  #else
  CSR_WRITE(CSR_MCOUNTINHIBIT, confMask); // Enable all the counters
  #endif
#endif
}

/* Starts counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_start(unsigned int conf) {
#ifndef PLP_NO_PERF_COUNTERS
  #ifndef ARCHI_HAS_COREV
  cpu_perf_conf(conf | CSR_PCMR_ACTIVE); // TODO
  #else
  cpu_perf_conf(conf);
  #endif
#endif
}

/* Stops counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_stop(unsigned int conf) {
#ifndef PLP_NO_PERF_COUNTERS
  cpu_perf_conf(conf); // TODO
#endif
}

/* Set the specified counter to the specified value */
static inline void cpu_perf_set(unsigned int counterId, unsigned int value) {

}

/* Set all counters to the specified value */
static inline void cpu_perf_setall(unsigned int value) {
#ifndef PLP_NO_PERF_COUNTERS
  #ifndef ARCHI_HAS_COREV
  asm volatile ("csrw 0x79F, %0" :: "r" (value));
  #else
  CSR_WRITE(CSR_MCOUNTINHIBIT, value);
  #endif
#endif
}

#ifndef PLP_NO_PERF_COUNTERS
  #ifdef ARCHI_HAS_COREV
    static inline unsigned int cpu_perf_get_mcycle() {
      unsigned int value=0;
      CSR_READ(CSR_MCYCLE, value);

      return value;
    }
    static inline unsigned int cpu_perf_get_minstret() {
      unsigned int value=0;
      CSR_READ(CSR_MINSTRET, value);

      return value;
    }
  #endif
#endif

/* Return the value of the specified counter */
static inline unsigned int cpu_perf_get(const unsigned int counterId) {
#ifndef PLP_NO_PERF_COUNTERS
  unsigned int value = 0;

  #ifndef ARCHI_HAS_COREV
  switch(counterId) {
   case  0: CSR_READ(CSR_PCCR(0),  value); break;
   case  1: CSR_READ(CSR_PCCR(1),  value); break;
   case  2: CSR_READ(CSR_PCCR(2),  value); break;
   case  3: CSR_READ(CSR_PCCR(3),  value); break;
   case  4: CSR_READ(CSR_PCCR(4),  value); break;
   case  5: CSR_READ(CSR_PCCR(5),  value); break;
   case  6: CSR_READ(CSR_PCCR(6),  value); break;
   case  7: CSR_READ(CSR_PCCR(7),  value); break;
   case  8: CSR_READ(CSR_PCCR(8),  value); break;
   case  9: CSR_READ(CSR_PCCR(9),  value); break;
   case 10: CSR_READ(CSR_PCCR(10), value); break;
   case 11: CSR_READ(CSR_PCCR(11), value); break;
   case 12: CSR_READ(CSR_PCCR(12), value); break;
   case 13: CSR_READ(CSR_PCCR(13), value); break;
   case 14: CSR_READ(CSR_PCCR(14), value); break;
   case 15: CSR_READ(CSR_PCCR(15), value); break;
   case 16: CSR_READ(CSR_PCCR(16), value); break;
   case 17: CSR_READ(CSR_PCCR(17), value); break;
   case 18: CSR_READ(CSR_PCCR(18), value); break;
   case 19: CSR_READ(CSR_PCCR(19), value); break;
   case 20: CSR_READ(CSR_PCCR(20), value); break;
   case 21: CSR_READ(CSR_PCCR(21), value); break;
   case 22: CSR_READ(CSR_PCCR(22), value); break;
   case 23: CSR_READ(CSR_PCCR(23), value); break;
   case 24: CSR_READ(CSR_PCCR(24), value); break;
   case 25: CSR_READ(CSR_PCCR(25), value); break;
   case 26: CSR_READ(CSR_PCCR(26), value); break;
   case 27: CSR_READ(CSR_PCCR(27), value); break;
   case 28: CSR_READ(CSR_PCCR(28), value); break;
   case 29: CSR_READ(CSR_PCCR(29), value); break;
   case 30: CSR_READ(CSR_PCCR(30), value); break;
  }

  #else

  // Read from performance counters
  switch(counterId) {
   case  0: CSR_READ(CSR_MHPMCOUNTER(0),  value); break;  // mhpmcounterh3
   case  1: CSR_READ(CSR_MHPMCOUNTER(1),  value); break;
   case  2: CSR_READ(CSR_MHPMCOUNTER(2),  value); break;
   case  3: CSR_READ(CSR_MHPMCOUNTER(3),  value); break;
   case  4: CSR_READ(CSR_MHPMCOUNTER(4),  value); break;
   case  5: CSR_READ(CSR_MHPMCOUNTER(5),  value); break;
   case  6: CSR_READ(CSR_MHPMCOUNTER(6),  value); break;
   case  7: CSR_READ(CSR_MHPMCOUNTER(7),  value); break;
   case  8: CSR_READ(CSR_MHPMCOUNTER(8),  value); break;
   case  9: CSR_READ(CSR_MHPMCOUNTER(9),  value); break;
   case 10: CSR_READ(CSR_MHPMCOUNTER(10), value); break;
   case 11: CSR_READ(CSR_MHPMCOUNTER(11), value); break;
   case 12: CSR_READ(CSR_MHPMCOUNTER(12), value); break;
   case 13: CSR_READ(CSR_MHPMCOUNTER(13), value); break;
   case 14: CSR_READ(CSR_MHPMCOUNTER(14), value); break;
   case 15: CSR_READ(CSR_MHPMCOUNTER(15), value); break;
   case 16: CSR_READ(CSR_MHPMCOUNTER(16), value); break;
   case 17: CSR_READ(CSR_MHPMCOUNTER(17), value); break;
   case 18: CSR_READ(CSR_MHPMCOUNTER(18), value); break;
   case 19: CSR_READ(CSR_MHPMCOUNTER(19), value); break;
   case 20: CSR_READ(CSR_MHPMCOUNTER(20), value); break;
   case 21: CSR_READ(CSR_MHPMCOUNTER(21), value); break;
   case 22: CSR_READ(CSR_MHPMCOUNTER(22), value); break;
   case 23: CSR_READ(CSR_MHPMCOUNTER(23), value); break;
   case 24: CSR_READ(CSR_MHPMCOUNTER(24), value); break;
   case 25: CSR_READ(CSR_MHPMCOUNTER(25), value); break;
   case 26: CSR_READ(CSR_MHPMCOUNTER(26), value); break;
   case 27: CSR_READ(CSR_MHPMCOUNTER(27), value); break;
   case 28: CSR_READ(CSR_MHPMCOUNTER(28), value); break; // mhpmcounterh31
  }
  #endif
  return value;
#else
  return 0;
#endif
}

static inline const char *cpu_perf_name(int event) {
  #ifndef ARCHI_HAS_COREV
  switch (event) {
    case 0: return "CYCLES";
    case 1: return "INSTR";
    case 2: return "LD_STALL";
    case 3: return "JMP_STALL";
    case 4: return "IMISS";
    case 5: return "LD";
    case 6: return "ST";
    case 7: return "JUMP";
    case 8: return "BRANCH";
    case 9: return "TAKEN_BRANCH";
    case 10: return "RVC";
    case 11: return "LD_EXT";
    case 12: return "ST_EXT";
    case 13: return "LD_EXT_CYC";
    case 14: return "ST_EXT_CYC";
    case 15: return "TCDM_CONT";
  }
  #else
  switch (event) {
    case 0: return "CYCLES";
    case 1: return "INSTR";
    case 2: return "LD_STALL";
    case 3: return "JMP_STALL";
    case 4: return "IMISS";
    case 5: return "LD";
    case 6: return "ST";
    case 7: return "JUMP";
    case 8: return "BRANCH";
    case 9: return "TAKEN_BRANCH";
    case 10: return "RVC";
    case 11: return "PIPE_STALL";
    case 12: return "APU_TYPE";
    case 13: return "APU_CONT";
    case 14: return "APU_DEP";
    case 15: return "APU_WB";
  }
  #endif
  return (char *)0;
}



/*
 * Stack checking
 */

static inline void cpu_stack_check_enable(unsigned int base, unsigned int end)
{
  asm volatile ("csrwi 0x7D0, 0" :: );
  asm volatile ("csrw  0x7D1, %0" :: "r" (base));
  asm volatile ("csrw  0x7D2, %0" :: "r" (end));
  asm volatile ("csrwi 0x7D0, 1" :: );
}

static inline void cpu_stack_check_disable()
{
  asm volatile ("csrwi 0x7D0, 0" :: );
}

#endif
