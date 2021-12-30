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
 */

/* Author: Robert Balas */

#ifndef __TARGET_H__
#define __TARGET_H__

#include "properties.h"
#include "riscv.h"

static inline void hal_compiler_barrier()
{
	asm volatile("" : : : "memory");
}

static inline void hal_write32(volatile void *addr, uint32_t value)
{
	asm volatile("" : : : "memory");
	*((volatile uint32_t *)addr) = value;
	asm volatile("" : : : "memory");
}

static inline void hal_write8(volatile void *addr, uint8_t value)
{
	asm volatile("" : : : "memory");
	*((volatile uint8_t *)addr) = value;
	asm volatile("" : : : "memory");
}

static inline void hal_or32(volatile void *addr, uint32_t value)
{
	asm volatile("" : : : "memory");
	*((volatile uint32_t *)addr) |= value;
	asm volatile("" : : : "memory");
}

static inline void hal_and32(volatile void *addr, uint32_t value)
{
	asm volatile("" : : : "memory");
	*((volatile uint32_t *)addr) &= value;
	asm volatile("" : : : "memory");
}

static inline uint32_t hal_read32(volatile void *addr)
{
	asm volatile("" : : : "memory");
	uint32_t ret = *((volatile uint32_t *)addr);
	asm volatile("" : : : "memory");
	return ret;
}

static inline uint8_t hal_read8(volatile void *addr)
{
	asm volatile("" : : : "memory");
	uint8_t ret = *((volatile uint8_t *)addr);
	asm volatile("" : : : "memory");
	return ret;
}


static inline uint32_t __native_core_id()
{
	/* encoding of mhartid: {21'b0, cluster_id_i[5:0], 1'b0, core_id_i[3:0]}
	 */
	uint32_t mhartid = csr_read(MHARTID_ADDR);
	return mhartid & 0x01f;
}

static inline uint32_t __native_cluster_id()
{
	/* encoding of mhartid {21'b0, cluster_id_i[5:0], 1'b0, core_id_i[3:0]}
	 */
	uint32_t mhartid = csr_read(MHARTID_ADDR);
	return (mhartid >> 5) & 0x3f;
}

static inline uint32_t __native_is_fc()
{
	return 1;
}


static inline uint32_t pi_core_id()
{
	return __native_core_id();
}

static inline uint32_t pi_cluster_id()
{
	return __native_cluster_id();
}

static inline uint32_t pi_is_fc()
{
	return __native_is_fc();
}

static inline uint32_t pi_nb_cluster_cores()
{
	return ARCHI_CLUSTER_NB_PE;
}

static inline int pi_cl_cluster_nb_cores()
{
	return ARCHI_CLUSTER_NB_PE;
}

static inline uint32_t pi_cl_cluster_nb_pe_cores(void)
{
	return ARCHI_CLUSTER_NB_PE;
}

#endif
