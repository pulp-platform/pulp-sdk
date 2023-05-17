/*
 * Copyright (C) 2019 ETH Zurich, University of Bologna
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

#ifndef __POS_CHIPS_KRAKEN_SOC_H__
#define __POS_CHIPS_KRAKEN_SOC_H__

#define POS_FLL_EHWPE 4
#define POS_FLL_CL 2
#define POS_FLL_PERIPH 1
#define POS_FLL_FC 0

extern PI_FC_TINY int pos_freq_domains[ARCHI_NB_FLL];

static inline void pos_pmu_cluster_power_up()
{}

void pos_soc_init();

static inline int pos_freq_get_fll(int domain)
{
    switch (domain)
    {
        case PI_FREQ_DOMAIN_FC:
            return POS_FLL_FC;

        case PI_FREQ_DOMAIN_PERIPH:
            return POS_FLL_PERIPH;

        case PI_FREQ_DOMAIN_EHWPE:
            return POS_FLL_EHWPE;

        case PI_FREQ_DOMAIN_CL:
            return POS_FLL_CL;

        default:
            return POS_FLL_CL;
    }
}


#endif
