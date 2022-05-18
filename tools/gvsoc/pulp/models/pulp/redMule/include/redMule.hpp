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
 * Authors:  Angelo Garofalo <angelo.garofalo@studio.unibo.it>
 * redMule GVSOC model -- Header file
 */

#ifndef __REDMULE_HPP__
#define __REDMULE_HPP__

// includes
#include <vp/vp.hpp>
#include <vp/itf/io.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <assert.h>
#include <string>
#include <bitset>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xindex_view.hpp"
#include "xtensor/xrandom.hpp"
#include "xtensor/xadapt.hpp"
#include "xtensor/xvectorize.hpp"
#include "xtensor/xpad.hpp"

// redMule REGISTER FILE
// Matrix addresses
#define IN_FEAT_ADD 0  // 0x00 -- inputs' address
#define WEIGHT_ADD 1   // 0x04 -- weights' address
#define OUT_FEAT_ADD 2 // 0x08 -- outputs' address
// Number of iterations on input and weight matrixes
// (15 bits for number of rows iterations, 15 bits for number of columns iterations)
#define IN_FEAT_ITERS 3 // 0x0C --> [31:16] -> ROWS ITERATIONS, [15:0] -> COLUMNS ITERATIONS
#define WEIGHT_ITERS 4  // 0x10 --> [31:16] -> ROWS ITERATIONS, [15:0] -> COLUMNS ITERATIONS
// Number of residual rows and columns (8 bits for each)
// [31:24] -> IN_FEAT_ROWS LEFTOVERS
// [23:16] -> IN_FEAT_COLS LEFTOVERS
// [15:8]  -> WEIGHT_ROWS  LEFTOVERS
// [7:0]   -> WEIGHT_COLS  LEFTOVERS
#define LEFTOVERS 5 // 0x14
// We keep a register for the remaining params
// [31:16] -> TOT_NUMBER_OF_STORES
// [15]    -> 1'b0: input rows >= ARRAY_WIDTH; 1'b1: input rows < ARRAY_WIDTH
// [14]    -> 1'b0: input cols/weight_rows >= ARRAY_HEIGHT; 1'b1: input cols/weight_rows < ARRAY_HEIGHT
// [13]    -> 1'b0: weight cols >= TILE ( TILE = (PIPE_REGS + 1)*ARRAY_HEIGHT ); 1'b1: weight cols < TILE ( TILE = (PIPE_REGS + 1)*ARRAY_HEIGHT )
#define LEFT_PARAMS 6    // 0x18
#define IN_D1_STRIDE 7   // 0x1C
#define W_TOT_LEN 8      // 0x20
#define TOT_IN_READ 9    // 0x24
#define W_D0_STRIDE 10   // 0x28
#define OUT_TOT_LEN 11   // 0x2C
#define OUT_D0_STRIDE 12 // 0x30
#define OUT_D2_STRIDE 13 // 0x34
#define IN_ROWS_OFFS 14  // 0x38
#define IN_TOT_LEN 15    // 0x3C

#define RM_NB_REGS 16

// special defines for TRACES (DEBUG ONLY)
#define RM_SPECIAL_TRACE_REG RM_NB_REGS
#define RM_SPECIAL_FORMAT_TRACE_REG RM_NB_REGS + 1
#define DEFAULT_TRACE_LEVEL L0_CONFIG

// Main ctrl FSM states
enum redMuleState
{
  TPU_IDLE,
  TPU_STARTING,
  TPU_COMPUTING,
  TPU_BUFFERING,
  TPU_STORING,
  TPU_FINISHED
};

// Set amount of trace levels
enum redMuleTraceLevel
{
  L0_CONFIG,
  L1_ACTIV_INOUT,
  L2_DEBUG,
  L3_ALL
};

// forward definitions
class redMule;

/***
FROM NE16 -- TO CHECK WHETER IT IS NEEDED FOR REDMULE
#define STREAM_MAX_WIDTH_BYTES 40
***/

// STREAM CLASS
class redMuleStreamAccess
{
public:
  redMuleStreamAccess(
      redMule *redmule,
      int base_addr,
      int d0_length,
      int d0_stride,
      int d1_length,
      int d1_stride,
      int d2_length,
      int d2_stride,
      bool debug = false);
  // STREAM methods -- defined in redMule_stream.cpp
  void reset_iteration();
  int iterate();
  void print_config();
  int get_base_addr();
  int get_d0_length();
  int get_d0_stride();
  int get_d1_length();
  int get_d1_stride();
  int get_d2_length();
  int get_d2_stride();

protected:
  redMule *redmule;
  int base_addr;
  int d0_length;
  int d0_stride;
  int d1_length;
  int d1_stride;
  int d2_length;
  int d2_stride;
  bool debug;
  // internal
  int current_addr;
  int ba;
  int la;
  int wa;
  int bc;
  int wc;
  int lc;
  int oc;
};

// TEMPLATE CLASS FOR STREAMIN FUNCTIONS
template <class T>
class redMuleVectorLoad : public redMuleStreamAccess
{
public:
  redMuleVectorLoad(
      redMule *_this,
      int base_addr,
      int d0_length,
      int d0_stride,
      int d1_length,
      int d1_stride,
      int d2_length,
      int d2_stride,
      bool debug);
  redMuleVectorLoad();
  xt::xarray<T> ex(int width, int64_t &cycles);
  void foo();
};

// TEMPLATE CLASS FOR STREAMOUT FUNCTIONS
template <class T>
class redMuleVectorStore : public redMuleStreamAccess
{
public:
  redMuleVectorStore(
      redMule *_this,
      int base_addr,
      int d0_length,
      int d0_stride,
      int d1_length,
      int d1_stride,
      int d2_length,
      int d2_stride,
      bool debug);
  redMuleVectorStore();
  xt::xarray<T> ex(xt::xarray<T> data, int width, int64_t &cycles, int32_t enable);
};

// REDMULE MAIN CLASS
class redMule : public vp::component
{
public:
  redMule(js::config *config); // json config file
  int build();                 // build the redMule module -- mandatory method
  void reset(bool active);     // module reset

  // were private before, but did not work with stream.hpp
  vp::io_req io_req;
  vp::trace trace;
  vp::io_master out;
  vp::reg_32 fsm_state;
  vp::reg_8 activity;
  redMuleTraceLevel trace_level; // trace level
  int trace_format;              // variable to specify trace formats

private:
  // HW PARAMETERS
  int ID_WIDTH;
  int N_CORES;
  int DW;          // TCDM port dimension (in bits)
  int IO_REGS;     // Number of configurable registers
  int N_CONTEXT;   // Number of sequential jobs for the slave device
  int FpFormat;    // Data format (default is FP16)
  int Height;      // Number of PEs within a row
  int Width;       // Number of parallel rows
  int NumPipeRegs; // Number of pipeline registers within each PE
  int PipeConfig;
  int BITW; // Number of bits for the given format

  // SLAVE PORT ON HWPE SLAVE
  static vp::io_req_status_e
  hwpe_slave(void *__this, vp::io_req *req);

  // DEBUG SETTINGS
  bool fsm_traces;
  /***
  put here boolean variables to enable more traces
  e.g.
  bool streamin_traces;
  bool input_buff_traces;
  and so on...
  ***/

  /***
  methods for traces
  e.g.
  void debug_input_buffer();

  ***/
  void debug_input_buffer();

  // EVENT HANDLERS
  static void fsm_start_handler(void *__this, vp::clock_event *event);
  static void fsm_handler(void *__this, vp::clock_event *event);
  static void fsm_end_handler(void *__this, vp::clock_event *event);

  // MAIN FSM AND LOOP -- redMule_fsm.cpp
  void fsm_loop();
  int  ctrl_fsm();

  // REGISTER FILE MEMBER FUNCTIONS
  int regfile_read(int);
  void regfile_write(int, int);
  void regfile_cxt();
  void printout();
  void commit();
  int acquire();
  bool status();

  // REGISTER FILE AND HWPE CTRL
  int cxt0[RM_NB_REGS];
  int cxt1[RM_NB_REGS];
  int cxt_cfg_ptr;
  int cxt_use_ptr;
  int job_pending;
  int job_state;
  unsigned char job_id;
  int cxt_job_id[2];
  char running_job_id;
  int job_running;

  // REGISTER FILE CONFIG PARAMS
  int input_address;
  int weight_address;
  int output_address;
  int16_t nr_input_rows_iters;
  int16_t nr_input_cols_iters;
  int16_t nr_weight_rows_iters;
  int16_t nr_weight_cols_iters;
  int8_t input_rows_leftover;
  int8_t input_cols_leftover;
  int8_t weight_rows_leftover;
  int8_t weight_cols_leftover;
  int16_t nr_stores_total;
  bool input_rows_gt_width;
  bool input_cols_gt_height; // equivalent to weight_rows_gt_height
  bool weight_cols_gt_tile;
  int input_d1_stride;
  int weight_total_length;
  int nr_input_reads_total;
  int weight_d0_stride;
  int output_total_length;
  int output_d0_stride;
  int output_d2_stride;
  int input_rows_offsets;
  int input_total_length;

  // FUNCTIONS OF CLEAR

  // STREAMING FUNCTIONS

  // COMPUTE FUNCTIONS

  // STREAMOUT FUNCTIONS

  // STREAMIN STATE

  // COMPUTE STATE

  // STREAMOUT STATE

  // IO PORTS
  vp::io_slave in;
  vp::wire_master<bool> irq;

  // FSM STATE EVENTS
  vp::clock_event *fsm_start_event;
  vp::clock_event *fsm_event;
  vp::clock_event *fsm_end_event;
};
#endif /*__REDMULE_HPP__*/