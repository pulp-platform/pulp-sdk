/*
 * Copyright 2019 GreenWaves Technologies
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

#ifndef __IMPLEMENTATION_SPECIFIC_DEFINES_H__
#define __IMPLEMENTATION_SPECIFIC_DEFINES_H__

#include <stdint.h>
#include "stdlib.h"
#include "string.h"

/* #if defined(__GAP8__) */
/* #include "system_gap8.h" */
/* #elif defined(__GAP9__) */
/* #include "system_gap9.h" */
/* #elif defined(__PULP__) */
/* #include "system_pulp_ri5cy.h" */
/* #else */
/* #error "Target specific macro is not set. Recommended to use __PULP__." */
/* #endif */

#define __INC_TO_STRING(x) #x

#define PMSIS_TASK_EVENT_KERNEL_PRIORITY     2
#define DEFAULT_MALLOC_INC                   __INC_TO_STRING(pmsis/rtos/malloc/l2_malloc.h)

// default malloc for driver structs etc (might not be compatible with udma!)
/* TODO: rereoute to newlib malloc (?)*/
#define pi_default_malloc(x)  malloc(x)
#define pi_default_free(x,y)  free(x)
#define pi_data_malloc(x)     malloc(x)
#define pi_data_free(x,y)     free(x)
/* TODO: hack */
#define pmsis_l2_malloc       pi_l2_malloc
#define pmsis_l2_malloc_align pi_l2_malloc_align
#define pmsis_l2_malloc_free  pi_l2_free
#define pmsis_l2_malloc_init  pi_l2_malloc_init
#define pmsis_l2_malloc_dump  pi_l2_malloc_dump

#define PI_TASK_IMPLEM                          \
    uint8_t destroy;
#define CLUSTER_TASK_IMPLEM                     \
    uint32_t cluster_team_mask;
#endif  /* __IMPLEMENTATION_SPECIFIC_DEFINES_H__ */
