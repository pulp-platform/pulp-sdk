/*
 * Copyright (C) 2020 ETH Zurich and University of Bologna
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
/* Driver to control the soc control clock divider used in some PULP based
 * chips/IPs. This driver is mutually exclusive with the fll driver. There are
 * three clock dividers one for the fc, cluster and peripheral subsystems each
 * in control-pulp that replace the FLLs. The clock dividers for the fc and
 * cluster are driven by a variable system clock and while the peripheral clock
 * divider is driven a fixed reference clock. The divider value is limited to
 * eight bits.
 */
/* Author: Robert Balas (balasr@iis.ee.ethz.ch)
 */

#include <stdint.h>

#include "clkdiv.h"
#include "memory_map.h"
#include "freq.h"
#include "io.h"

static uint32_t freq_from_div(uint32_t freq, uint32_t div)
{
	if (div == 0 || div == 1)
		return freq;

	if (div % 2 == 0)
		return freq / div;
	else
		return freq / (2 * (div - 1));
}

uint32_t pi_freq_get(pi_freq_domain_e domain)
{
	uint32_t div = 0;

	/* TODO: build time switch between asic and fpga build */
	switch (domain) {
	case PI_FREQ_DOMAIN_FC:
		div = readw((uintptr_t)(CLKDIV_ADDR + CLKDIV_FC_OFFSET));
		return freq_from_div(ASIC_SYSTEM_CLOCK, div);
	case PI_FREQ_DOMAIN_CL:
		div = readw((uintptr_t)(CLKDIV_ADDR + CLKDIV_CL_OFFSET));
		return freq_from_div(ASIC_SYSTEM_CLOCK, div);
	case PI_FREQ_DOMAIN_PERIPH:
		div = readw((uintptr_t)(CLKDIV_ADDR + CLKDIV_PERIPH_OFFSET));
		return freq_from_div(ASIC_PERIPH_CLOCK, div);
	}
	/* unreachable */
	return 0;
}

static uint32_t div_from_freq(uint32_t base_freq, uint32_t target_freq)
{
	return base_freq / target_freq;
}

int32_t pi_freq_set(pi_freq_domain_e domain, uint32_t freq)
{
	/* impossible */
	if (freq > ASIC_SYSTEM_CLOCK || freq == 0)
		return 1;

	uint32_t div = 0; 

	switch (domain) {
	case PI_FREQ_DOMAIN_FC:
		div = div_from_freq(ASIC_SYSTEM_CLOCK, freq);
		if (div >= 0xff)
			return -1;
		writew(div, (uintptr_t)(CLKDIV_ADDR + CLKDIV_FC_OFFSET));
		break;
	case PI_FREQ_DOMAIN_CL:
		div = div_from_freq(ASIC_SYSTEM_CLOCK, freq);
		if (div >= 0xff)
			return -1;
		writew(div, (uintptr_t)(CLKDIV_ADDR + CLKDIV_CL_OFFSET));
		break;
	case PI_FREQ_DOMAIN_PERIPH:
		div = div_from_freq(ASIC_PERIPH_CLOCK, freq);
		if (div >= 0xff)
			return -1;
		writew(div, (uintptr_t)(CLKDIV_ADDR + CLKDIV_PERIPH_OFFSET));
		break;
	}

	return 0;
}
