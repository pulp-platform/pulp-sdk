/*
* Copyright (C) 2026 ETH Zurich, University of Bologna, and Fondazione Chips-IT
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
* Authors:  Alessandro Nadalini <alessandro.nadalini3@unibo.it>
*/

#ifndef __ARCHI_PCM_HWPE__
#define __ARCHI_PCM_HWPE__

#define PCM_HWPE_TRIGGER                0x00
#define PCM_HWPE_ACQUIRE                0x04
#define PCM_HWPE_FIN_JOBS               0x08
#define PCM_HWPE_STATUS                 0x0C
#define PCM_HWPE_RUN_TASK               0x10
#define PCM_HWPE_SOFT_CLEAR             0x14
#define PCM_HWPE_CHK_STATE              0x18
#define PCM_HWPE_CFG_DIM                0x1C
#define PCM_HWPE_CFG_2S_U_COMP          0x20
#define PCM_HWPE_CFG_2S_U_2W_COMP       0x24
#define PCM_HWPE_CFG_2S_COMP            0x28
#define PCM_HWPE_CFG_2S_2W_COMP         0x2C
#define PCM_HWPE_CFG_SINGLE_STEP        0x30
#define PCM_HWPE_CFG_FAST_SINGLE_STEP   0x34
#define PCM_HWPE_CFG_INPUT_PRECISION    0x38
#define PCM_HWPE_CFG_BL_SETTINGS        0x3C
#define PCM_HWPE_JOB_START_X            0x40
#define PCM_HWPE_JOB_START_Y            0x44
#define PCM_HWPE_JOB_WIDTH              0x48
#define PCM_HWPE_JOB_HIGHT              0x4C
#define PCM_HWPE_JOB_SRC_ADDR           0x50
#define PCM_HWPE_JOB_DST_ADDR           0x54
#define PCM_HWPE_JOB_SRC_STRIDE         0x58
#define PCM_HWPE_JOB_DST_STRIDE         0x5C
#define PCM_HWPE_TOTAL_LENGTH           0x60
#define PCM_HWPE_D0_LENGTH              0x64
#define PCM_HWPE_D0_STRIDE              0x68
#define PCM_HWPE_D1_LENGTH              0x6C
#define PCM_HWPE_D1_STRIDE              0x70
#define PCM_HWPE_D2_STRIDE              0x74
#define PCM_HWPE_OUT_TOTAL_LENGTH       0x78
#define PCM_HWPE_OUT_D0_LENGTH          0x7C
#define PCM_HWPE_OUT_D0_STRIDE          0x80
#define PCM_HWPE_OUT_D1_LENGTH          0x84
#define PCM_HWPE_OUT_D1_STRIDE          0x88
#define PCM_HWPE_OUT_D2_STRIDE          0x8C
#define PCM_HWPE_NUM_JOBS               0x90
#define PCM_HWPE_ALPHA_IN_LENGTH        0x94
#define PCM_HWPE_ALPHA_IN_STRIDE        0x98
#define PCM_HWPE_BETA_IN_LENGTH         0x9C
#define PCM_HWPE_BETA_IN_STRIDE         0xA0
#define PCM_HWPE_ALPHA_OUT_LENGTH       0xA4
#define PCM_HWPE_ALPHA_OUT_STRIDE       0xA8
#define PCM_HWPE_BETA_OUT_LENGTH        0xAC
#define PCM_HWPE_BETA_OUT_STRIDE        0xB0
#define PCM_HWPE_JOB_LINE_LENGTH        0xB4
#define PCM_HWPE_ACT_SECT_3             0xB8
#define PCM_HWPE_ACT_SECT_2             0xBC
#define PCM_HWPE_ACT_SECT_1             0xC0
#define PCM_HWPE_ACT_SECT_0             0xC4
#define PCM_HWPE_ACT_LAYER              0xC8
#define PCM_HWPE_PRECISION              0xCC
#define PCM_HWPE_FIXED_TIME_COMP        0xD0
#define PCM_HWPE_HIGH_PREC_MASK         0xD4
#define PCM_HWPE_LOW_PREC_MASK          0xD8
#define PCM_HWPE_EN_CASTING             0xDC

#endif