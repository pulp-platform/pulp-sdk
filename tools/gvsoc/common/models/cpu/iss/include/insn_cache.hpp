/*
 * Copyright (C) 2020 GreenWaves Technologies, SAS, ETH Zurich and
 *                    University of Bologna
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
 * Authors: Germain Haugou, GreenWaves Technologies (germain.haugou@greenwaves-technologies.com)
 */

#ifndef __CPU_ISS_ISS_INSN_CACHE_HPP
#define __CPU_ISS_ISS_INSN_CACHE_HPP

int insn_cache_init(iss_t *iss);
void iss_cache_flush(iss_t *iss);
iss_insn_t *insn_cache_get(iss_t *iss, iss_addr_t pc);
iss_insn_t *insn_cache_get_decoded(iss_t *iss, iss_addr_t pc);

#endif