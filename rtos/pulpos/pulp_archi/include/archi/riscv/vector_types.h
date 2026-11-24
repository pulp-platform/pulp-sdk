/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
 * Copyright (C) 2026 Fondazione Chips-IT
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

/*
 * Split out of builtins_v2.h / builtins_v2_emu.h so a provider can be included
 * before either of them -- it needs v2s/v4s to declare its ops.
 */

#ifndef __ARCHI_RISCV_VECTOR_TYPES_H__
#define __ARCHI_RISCV_VECTOR_TYPES_H__

typedef   signed short v2s __attribute__((vector_size (4)));
typedef unsigned short v2u __attribute__((vector_size (4)));

typedef   signed char  v4s __attribute__((vector_size (4)));
typedef unsigned char  v4u __attribute__((vector_size (4)));

#ifdef __EMUL__
typedef void * rt_pointerT;
#else
typedef unsigned int rt_pointerT;
#endif

#endif
