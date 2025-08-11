/*
 * Copyright (C) 2022-2023 ETH Zurich and University of Bologna
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
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Author: Yvan Tortorella  <yvan.tortorella@unibo.it>
 *
 * High-level architecture of RedMulE
 *
 */

#ifndef __ARCHI_REDMULE_H__
#define __ARCHI_REDMULE_H__

/*
 * |=======================================================================|
 * ||                                                                     ||
 * ||Control and generic configuration register layout                    ||
 * |=======================================================================|
 * || # reg |  offset  |  bits   |   bitmask    ||  content               ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||    0  |  0x0000  |  31: 0  |  0xFFFFFFFF  ||  TRIGGER               ||
 * ||    1  |  0x0004  |  31: 0  |  0xFFFFFFFF  ||  ACQUIRE               ||
 * ||    2  |  0x0008  |  31: 0  |  0xFFFFFFFF  ||  EVT_ENABLE            ||
 * ||    3  |  0x000c  |  31: 0  |  0xFFFFFFFF  ||  STATUS                ||
 * ||    4  |  0x0010  |  31: 0  |  0xFFFFFFFF  ||  RUNNING_JOB           ||
 * ||    5  |  0x0014  |  31: 0  |  0xFFFFFFFF  ||  SOFT_CLEAR            ||
 * |=======================================================================|
 * ||                                                                     ||
 * ||Job-dependent registers layout                                       ||
 * |=======================================================================|
 * || # reg |  offset  |  bits   |   bitmask    ||  content               ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||    0  |  0x0040  |  31: 0  |  0xFFFFFFFF  ||  X_ADDR                ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||    1  |  0x0044  |  31: 0  |  0xFFFFFFFF  ||  W_ADDR                ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||    2  |  0x0048  |  31: 0  |  0xFFFFFFFF  ||  Y_ADDR                ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||    3  |  0x004C  |  31: 0  |  0xFFFFFFFF  ||  Z_ADDR                ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||    4  |  0x0050  |         |              ||  X MATRIX ITERATIONS   ||
 * ||       |          |  31:16  |  0xFFFF0000  ||  ROWS ITERATION        ||
 * ||       |          |  15: 0  |  0x0000FFFF  ||  COLUMNS ITERATION     ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||    5  |  0x0054  |         |              ||  W MATRIX ITERATIONS   ||
 * ||       |          |  31:16  |  0xFFFF0000  ||  ROWS ITERATION        ||
 * ||       |          |  15: 0  |  0x0000FFFF  ||  COLUMNS ITERATION     ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||    6  |  0x0058  |         |              ||  LEFTOVERS             ||
 * ||       |          |  31:24  |  0xFF000000  ||  X/Y ROWS LEFTOVERS    ||
 * ||       |          |  23:16  |  0x00FF0000  ||  X COLUMNS LEFTOVERS   ||
 * ||       |          |  15: 8  |  0x0000FF00  ||  W ROWS LEFTOVERS      ||
 * ||       |          |   7: 0  |  0x000000FF  ||  W/Y COLUMNS LEFTOVERS ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||    7  |  0x005C  |         |              ||  LEFT PARAMETERS       ||
 * ||       |          |  31:16  |  0xFFFF0000  ||  TOTAL NUMBER OF STORES||
 * ||       |          |  23: 0  |  0x0000FFFF  ||  -                     ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||    8  |  0x0060  |  31: 0  |  0xFFFFFFFF  ||  X_D1_STRIDE           ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||    9  |  0x0064  |  31: 0  |  0xFFFFFFFF  ||  W_TOT_LENGTH          ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||   10  |  0x0068  |  31: 0  |  0xFFFFFFFF  ||  TOT_X_READ            ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||   11  |  0x006C  |  31: 0  |  0xFFFFFFFF  ||  W_D0_STRIDE           ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||   12  |  0x0070  |  31: 0  |  0xFFFFFFFF  ||  Y/Z_TOT_LENGTH        ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||   13  |  0x0074  |  31: 0  |  0xFFFFFFFF  ||  Y/Z_D0_STRIDE         ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||   14  |  0x0078  |  31: 0  |  0xFFFFFFFF  ||  Y/Z_D2_STRIDE         ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||   15  |  0x007C  |  31: 0  |  0xFFFFFFFF  ||  X_ROWS_OFFSET         ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||   16  |  0x0080  |  31: 0  |  0xFFFFFFFF  ||  X_SLOTS               ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||   17  |  0x0084  |  31: 0  |  0xFFFFFFFF  ||  X_TOT_LENGTH          ||
 * ||-------+----------+---------+--------------++------------------------||
 * ||   18  |  0x0088  |         |              ||  OPERATION SELECTION   ||
 * ||       |          |  31:29  |  0xE0000000  ||  STAGE 1 ROUND MODE    ||
 * ||       |          |  28:26  |  0x1C000000  ||  STAGE 2 ROUND MODE    ||
 * ||       |          |  25:22  |  0x03C00000  ||  STAGE 1 OPERATION     ||
 * ||       |          |  21:18  |  0x003C0000  ||  STAGE 2 OPERATION     ||
 * ||       |          |  17:15  |  0x00038000  ||  INPUT FORMAT          ||
 * ||       |          |  14:12  |  0x00007000  ||  COMPUTING FORMAT      ||
 * ||       |          |   0: 0  |  0x00000001  ||  GEMM SELECTION        ||
 * |=======================================================================|
 *
 */

#define ARCHI_CL_EVT_ACC0 0
#define ARCHI_CL_EVT_ACC1 1
#define __builtin_bitinsert(a,b,c,d) (a | (((b << (32-c)) >> (32-c)) << d))

// RedMulE architecture
#define ADDR_WIDTH   32
//#define DATA_WIDTH   512
#define REDMULE_FMT  16
//#define ARRAY_HEIGHT 4
//#define PIPE_REGS    3
//#define ARRAY_WIDTH  (PIPE_REGS * ARRAY_HEIGHT) /* Upper limit is ARRAY_HEIGHT*PIPE_REGS */

// Base address
#define REDMULE_BASE_ADD 0x10201000

// Commands
#define REDMULE_TRIGGER                0x00
#define REDMULE_ACQUIRE                0x04
#define REDMULE_FINISHED               0x08
#define REDMULE_STATUS                 0x0C
#define REDMULE_RUNNING_JOB            0x10
#define REDMULE_SOFT_CLEAR             0x14

// Registers
#define REDMULE_REG_OFFS               0x40
#define REDMULE_REG_X_PTR              0x00
#define REDMULE_REG_W_PTR              0x04
#define REDMULE_REG_Y_PTR              0x08
#define REDMULE_REG_Z_PTR              0x0C
#define REDMULE_REG_X_ITER_PTR         0x10
#define REDMULE_REG_W_ITER_PTR         0x14
#define REDMULE_REG_LEFTOVERS_PTR      0x18
#define REDMULE_REG_LEFT_PARAMS_PTR    0x1C
#define REDMULE_REG_X_D1_STRIDE_PTR    0x20
#define REDMULE_REG_W_TOT_LEN_PTR      0x24
#define REDMULE_REG_TOT_X_READ_PTR     0x28
#define REDMULE_REG_W_D0_STRIDE_PTR    0x2C
#define REDMULE_REG_YZ_TOT_LEN_PTR     0x30
#define REDMULE_REG_YZ_D0_STRIDE_PTR   0x34
#define REDMULE_REG_YZ_D2_STRIDE_PTR   0x38
#define REDMULE_REG_X_ROWS_OFFS_PTR    0x3C
#define REDMULE_REG_X_BUFFER_SLOTS_PTR 0x40
#define REDMULE_REG_X_TOT_LEN_PTR      0x44
#define REDMULE_REG_OP_SELECTION       0x48

// OPs definition
#define MATMUL 0x0
#define GEMM   0x1
#define ADDMAX 0x2
#define ADDMIN 0x3
#define MULMAX 0x4
#define MULMIN 0x5
#define MAXMIN 0x6
#define MINMAX 0x7

#define RNE       0x0
#define RTZ       0x1
#define OP_FMADD  0x3
#define OP_ADD    0x5
#define OP_MUL    0x6
#define OP_MINMAX 0xA

// FP Formats encoding
#define FP16    0x2
#define FP8     0x3
#define FP16ALT 0x4
#define FP8ALT  0x5
#define FP32    0x6

#endif