/*
 * Copyright (C) 2021 ETH Zurich and University of Bologna
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
 *
 * PULP FPGA implementations sometimes have non-configurable, constant clocks
 * for soc/cluster/periphs.
 *
 * Author: Robert Balas (balasr@iis.ee.ethz.ch)
 */

#include <stdint.h>

#include "properties.h"
#include "freq.h"

uint32_t pi_freq_get(pi_freq_domain_e domain)
{
	/* TODO: build time switch between asic and fpga build */
	switch (domain) {
	case PI_FREQ_DOMAIN_FC:
		return FPGA_SYSTEM_CLOCK;
	case PI_FREQ_DOMAIN_CL:
		return FPGA_SYSTEM_CLOCK;
	case PI_FREQ_DOMAIN_PERIPH:
		return FPGA_PERIPH_CLOCK;
	}
	/* unreachable */
	return 0;
}

int32_t pi_freq_set(pi_freq_domain_e domain, uint32_t freq)
{
    /* not possible to configure clocks */
    return 1;
}
