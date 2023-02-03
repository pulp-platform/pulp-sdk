/*
 * Copyright (C) 2020 ETH Zurich
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
 * Authors: Germain Haugou, ETH Zurich (germain.haugou@iis.ee.ethz.ch)
 */

#include "pmsis.h"

void pos_soc_init()
{
#if __PLATFORM__ != ARCHI_PLATFORM_FPGA && !defined(SKIP_PLL_INIT)
  pos_freq_domains[PI_FREQ_DOMAIN_FC] = pos_fll_init(POS_FLL_FC);

  pos_freq_domains[PI_FREQ_DOMAIN_PERIPH] = pos_fll_init(POS_FLL_PERIPH);

  pos_freq_domains[PI_FREQ_DOMAIN_CL] = pos_fll_init(POS_FLL_CL);

#else
  pos_freq_domains[PI_FREQ_DOMAIN_FC] = ARCHI_FPGA_FC_FREQUENCY;

  pos_freq_domains[PI_FREQ_DOMAIN_PERIPH] = ARCHI_FPGA_FREQUENCY;

  pos_freq_domains[PI_FREQ_DOMAIN_CL] = ARCHI_FPGA_CL_FREQUENCY;

#endif
}

