// Copyright 2023 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Andrea Belano <andrea.belano@studio.unibo.it>
//

#ifndef __ARCHI_SOFTEX__
#define __ARCHI_SOFTEX__

#define DATA_WIDTH      128

//#define SOFTEX_BASE_ADD    0x00100000

// Commands
#define SOFTEX_TRIGGER     0x00
#define SOFTEX_ACQUIRE     0x04
#define SOFTEX_FINISHED    0x08
#define SOFTEX_STATUS      0x0C
#define SOFTEX_RUNNING_JOB 0x10
#define SOFTEX_SOFT_CLEAR  0x14

#define SOFTEX_REG_OFFS    0x20

#define SOFTEX_IN_ADDR         SOFTEX_REG_OFFS + 0x00
#define SOFTEX_OUT_ADDR        SOFTEX_REG_OFFS + 0x04
#define SOFTEX_TOT_LEN         SOFTEX_REG_OFFS + 0x08
#define SOFTEX_COMMANDS        SOFTEX_REG_OFFS + 0x0C
#define SOFTEX_CACHE_BASE_ADDR SOFTEX_REG_OFFS + 0x10
#define SOFTEX_CAST_CTRL       SOFTEX_REG_OFFS + 0x14


#define SOFTEX_CMD_ACC_ONLY        0x00000001
#define SOFTEX_CMD_DIV_ONLY        0x00000002
#define SOFTEX_CMD_ACQUIRE_SLOT    0x00000004
#define SOFTEX_CMD_LAST            0x00000008
#define SOFTEX_CMD_SET_CACHE_ADDR  0x00000010
#define SOFTEX_CMD_NO_OP           0x00000020
#define SOFTEX_CMD_INT_INPUT       0x00000040
#define SOFTEX_CMD_INT_OUTPUT      0x00000080

#endif