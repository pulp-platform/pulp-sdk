/*
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

#ifndef __ARCHI_RISCV_BUILTINS_H__
#define __ARCHI_RISCV_BUILTINS_H__

#include "archi/riscv/vector_types.h"

#include "archi/riscv/builtins_corev_v2.h"
/* Legacy __builtin_pulp_* names for third-party code; forwards to the __XXX
 * layer above, so it must follow it. */
#include "archi/riscv/builtins_pulp_compat.h"

#include "archi/riscv/builtins_v2.h"
#include "archi/riscv/builtins_v2_emu.h"

#endif
