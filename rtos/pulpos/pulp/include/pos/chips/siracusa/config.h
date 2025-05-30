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

#ifndef __POS__CHIPS__SIRACUSA__CONFIG_H__
#define __POS__CHIPS__SIRACUSA__CONFIG_H__

#include "archi/pulp_defs.h"

#define PULP_CHIP CHIP_PULP
#define PULP_CHIP_FAMILY CHIP_PULP
#define CONFIG_PULP 1
#define PULP_CHIP_STR siracusa
#define PULP_CHIP_FAMILY_STR siracusa
#define ARCHI_CORE_HAS_PULPV2 1

#define ARCHI_CORE_HAS_1_10 1

#endif
