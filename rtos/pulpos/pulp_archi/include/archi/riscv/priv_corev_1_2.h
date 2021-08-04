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

/* COREV has different addresses. Keep compatibility */
#ifndef _ARCHI_RISCV_PRIV_1_11_H
#define _ARCHI_RISCV_PRIV_1_11_H

#define RV_CSR_MSTATUS           0x300
#define RV_CSR_MEPC              0x341
#define RV_CSR_MCAUSE            0x342
#define RV_CSR_MTVAL             0x343

#define RV_CSR_MISA              0x301
#define RV_CSR_MIMPID            0xF13
#define RV_CSR_MHARTID           0xF14

#define CSR_MCYCLE               0xB00
#define CSR_MINSTRET             0xB02
#define CSR_MHPMCOUNTER(N)       (0xB03 + (N))
#define CSR_MCOUNTINHIBIT        0x320
#define CSR_MHPMEVENT(M)         (0x323 + (M))

/* Not specified for COREV */
#define CSR_STACK_CONF           0x7D0
#define CSR_STACK_START          0x7D1
#define CSR_STACK_END            0x7D2

/* Not specified for COREV */
#define CSR_MESTATUS_INTEN_BIT 0
#define CSR_MESTATUS_PRV_BIT 1
#define CSR_MESTATUS_PRV_MACH 3

#define CSR_HWLOOP0_START        0x800
#define CSR_HWLOOP0_END          0x801
#define CSR_HWLOOP0_COUNTER      0x802
#define CSR_HWLOOP1_START        0x804
#define CSR_HWLOOP1_END          0x805
#define CSR_HWLOOP1_COUNTER      0x806

#endif
