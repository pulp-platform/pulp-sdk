#ifndef __ARCHI_RNNACC_V1_H__
#define __ARCHI_RNNACC_V1_H__

/*
 * Copyright (C) 2021 ETH Zurich and University of Bologna
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
 * Authors:  Gianna Paulin, ETH Zurich <pauling@iis.ee.ethz.ch>
 */

/************************* RNNACC defines**********************/
/*
 * Control and generic configuration register layout 
 *********************FOR RNNACC********************* 
 * ================================================================================
 *  # reg |  offset  |  bits   |   bitmask    ||  content
 * -------+----------+---------+--------------++-----------------------------------
 *     0  |  0x0000  |  31: 0  |  0xffffffff  ||  TRIGGER
 *     1  |  0x0004  |  31: 0  |  0xffffffff  ||  ACQUIRE
 *     2  |  0x0008  |  31: 0  |  0xffffffff  ||  FINISHED_JOBS
 *     3  |  0x000c  |  31: 0  |  0xffffffff  ||  STATUS
 *     4  |  0x0010  |  31: 0  |  0xffffffff  ||  RUNNING_TASK
 *     5  |  0x0014  |  31: 0  |  0xffffffff  ||  SOFT_CLEAR
 *     6  |  0x0018  |  31: 0  |  0xffffffff  ||  CHECK_STATE
 *     7  |          |         |              ||  Reserved
 * ================================================================================
 *
 * Job-dependent registers layout
 * ================================================================================
 *  # reg |  offset  |  bits   |   bitmask    ||  content
 * -------+----------+---------+--------------++-----------------------------------
 *     0  |  0x0020  |  31: 0  |  0xffffffff  ||  ADDR_B
 *     1  |  0x0024  |  31: 0  |  0xffffffff  ||  ADDR_X
 *     2  |  0x0028  |  31: 0  |  0xffffffff  ||  ADDR_WX
 *     3  |  0x002c  |  31: 0  |  0xffffffff  ||  ADDR_H
 *     4  |  0x0030  |  31: 0  |  0xffffffff  ||  ADDR_WH
 *     5  |  0x0034  |  31: 0  |  0xffffffff  ||  ADDR_DST
 *     6  |  0x0038  |  31: 0  |  0xffffffff  ||  N_INPUT
 *     7  |  0x003c  |  31: 0  |  0xffffffff  ||  N_OUTPUT
 *     8  |  0x0040  |   0: 0  |  0x00000001  ||  JOB_MODE -> BIAS_ENABLE
 *     8  |  0x0040  |   1: 1  |  0x00000002  ||  JOB_MODE -> MATMUL_JOB | TWOMATMUL_JOB
 *     8  |  0x0040  |   2: 2  |  0x00000004  ||  JOB_MODE -> TWOMATMUL_MULTIJOB
 * ================================================================================
 *
 */

#define RNNACC_NB_MASTER_PORTS	4
#define RNNACC_NB_REGS 15

// Control and generic configuration registers
#define RNNACC_TRIGGER          	0x00
#define RNNACC_ACQUIRE          	0x04
#define RNNACC_FINISHED_JOBS    	0x08
#define RNNACC_STATUS           	0x0c
#define RNNACC_RUNNING_TASK     	0x10
#define RNNACC_SOFT_CLEAR       	0x14
#define RNNACC_CHECK_STATE      	0x18

// Job-dependent registers
#define RNNACC_ADDR_B               0x20
#define RNNACC_ADDR_X               0x24
#define RNNACC_ADDR_WX              0x28
#define RNNACC_ADDR_H               0x2c
#define RNNACC_ADDR_WH              0x30
#define RNNACC_ADDR_DST             0x34
#define RNNACC_N_INPUT              0x38
#define RNNACC_N_OUTPUT             0x3c

#define RNNACC_JOB_MODE     0x40

#define RNNACC_MASK_BIAS_ENABLE        0x00000001
#define RNNACC_MASK_TWOMATMUL_JOB      0x00000002
#define RNNACC_MASK_TWOMATMUL_MULTIJOB 0x00000004

// #define RNNACC_J_HIGHT            	0x40
// #define RNNACC_J_SRC_ADDR          0x44
// #define RNNACC_J_DST_ADDR          0x48
// #define RNNACC_J_SRC_STRID         0x4c
// #define RNNACC_J_DST_STRID         0x50

// #define RNNACC_ADC_LOW             0x54
// #define RNNACC_ADC_HIGH            0x58

// #define RNNACC_FETCH_LENGTH        0x5c
// #define RNNACC_STORE_LENGTH        0x60
// #define RNNACC_JOB_LINE_LENGTH     0x64
// #define RNNACC_JOB_FEAT_STRIDE     0x68
// #define RNNACC_JOB_FEAT_LENGTH     0x6c

// #define RNNACC_NUM_JOBS            0x70

// #define RNNACC_ALPHA_IN_LENGTH     0x74
// #define RNNACC_ALPHA_IN_STRIDE     0x78
// #define RNNACC_BETA_IN_LENGTH      0x7c
// #define RNNACC_BETA_IN_STRIDE      0x80

// #define RNNACC_ALPHA_OUT_LENGTH    0x84
// #define RNNACC_ALPHA_OUT_STRIDE    0x88
// #define RNNACC_BETA_OUT_LENGTH     0x8c
// #define RNNACC_BETA_OUT_STRIDE     0x90

// #define RNNACC_JOB_LL_MEMT         0x94
// #define RNNACC_JOB_FEAT_ROLL       0x98
// #define RNNACC_DW_MODE             0x9c

// #define RNNACC_PR_ADDR_X           0xa0
// #define RNNACC_PR_ADDR_Y           0xa4
// #define RNNACC_PR_VAL              0xa8

/* time in ns */
// #define RNNACC_EVAL_TIME       69
// #define RNNACC_WRITE_TIME      1000
// #define RNNACC_READ_TIME       10

// #define STOR_DWIDTH 		4
// #define DAC_PRECISION 		8
// #define ADC_PRECISION 		8

#endif 