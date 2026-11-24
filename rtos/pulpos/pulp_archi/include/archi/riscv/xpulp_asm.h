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

/*
 * xpulp mnemonics used by the kernel .S files. pulp-gcc spells them "p.<op>",
 * the CoreV assembler "cv.<op>" and rejects "p.". Use these aliases so one
 * source covers both toolchains.
 */

#ifndef __ARCHI_RISCV_XPULP_ASM_H__
#define __ARCHI_RISCV_XPULP_ASM_H__

#ifdef __cv32e40p__

#define XPULP_ELW      cv.elw
#define XPULP_LW       cv.lw
#define XPULP_SW       cv.sw
#define XPULP_BSETR    cv.bsetr
#define XPULP_EXTRACTU cv.extractu
#define XPULP_BEQIMM   cv.beqimm

#else

#define XPULP_ELW      p.elw
#define XPULP_LW       p.lw
#define XPULP_SW       p.sw
#define XPULP_BSETR    p.bsetr
#define XPULP_EXTRACTU p.extractu
#define XPULP_BEQIMM   p.beqimm

#endif

#endif
