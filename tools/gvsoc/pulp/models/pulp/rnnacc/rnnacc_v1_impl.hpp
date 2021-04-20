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
 * Authors: Gianna Paulin, ETH Zurich (pauling@iis.ee.ethz.ch)
 */

#ifndef __RNNACC_HPP__
#define __RNNACC_HPP__

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


/* RNNACC main FSM States */
#define RNNACC_ADDR_B               0
#define RNNACC_ADDR_X               1
#define RNNACC_ADDR_WX              2
#define RNNACC_ADDR_H               3
#define RNNACC_ADDR_WH              4
#define RNNACC_ADDR_DST             5
#define RNNACC_N_INPUT              6
#define RNNACC_N_OUTPUT             7

#define RNNACC_JOB_MODE     8

#define RNNACC_NB_REG 9


enum RnnaccState {
    START,
    START_LOAD,
    LOAD_BIAS,
    LOAD_X,
    LOAD_H,
    STREAM_MATMUL_X,
    STREAM_MATMUL_H,
    STREAMOUT,
    END
};

// forward definitions
class Rnnacc_v1;

// #define STREAM_MAX_WIDTH_BYTES 40

class RnnaccStreamAccess {
  public:
    RnnaccStreamAccess(
      Rnnacc_v1 *rnnacc,
      int base_addr,
      int d0_length,
      int d0_stride,
      int d1_length,
      int d1_stride,
      int d2_length,
      int d2_stride,
      bool debug = false
    );
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
    Rnnacc_v1 *rnnacc;
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


template <class T>
class RnnaccVectorLoad : public RnnaccStreamAccess {
  public:
    RnnaccVectorLoad(
      Rnnacc_v1 *_this,
      int base_addr,
      int d0_length,
      int d0_stride,
      int d1_length,
      int d1_stride,
      int d2_length,
      int d2_stride,
      bool debug
    );
    RnnaccVectorLoad();
    xt::xarray<T> execute(int width, int64_t& cycles, int stride);
    void foo();
};

template <class T>
class RnnaccVectorStore : public RnnaccStreamAccess {
  public:
    RnnaccVectorStore(
      Rnnacc_v1 *_this,
      int base_addr,
      int d0_length,
      int d0_stride,
      int d1_length,
      int d1_stride,
      int d2_length,
      int d2_stride,
      bool debug
    );
    RnnaccVectorStore();
    xt::xarray<T> execute(xt::xarray<T> data, int width, int64_t& cycles, int32_t enable);
};


class Rnnacc_v1 : public vp::component
{

public:

    Rnnacc_v1(js::config *config);

    int build();
//   void start();
    void reset(bool active);

//   static vp::io_req_status_e req(void *__this, vp::io_req *req);
//   static void job_handler(void *_this, vp::clock_event *event);
//   static void plot_handler(void *_this, vp::clock_event *event);
//   static void grant(void *_this, vp::io_req *req);
//   static void response(void *_this, vp::io_req *req);
//   void check_requests();

    void enqueue_write();
    void enqueue_read();
    void enqueue_job();

    // HARDWARE parameters
    int NR_MASTER_PORTS;
    int NR_REGS_X;
    int NR_REGS_H;
    int NR_REGS_ACCUM;

    int STREAM_MAX_WIDTH_BYTES;

    vp::io_master *tcdm_ports; // TCDM master interface
    vp::io_req *tcdm_reqs;   // TCDM request object to send via TCDM Master interface

    // TRACE
    vp::trace trace;

private:

    // INTERFACE to Rnnacc
    vp::io_slave in;    // HWPE slave interface
    vp::wire_master<bool> irq; // event interface
    int port_id;


    void update_finished_jobs(int increment);

    int eval_state;

    void set_state(int new_state);
    void set_id(int id);

    // CLEAR
    void clear_all();
    void clear_all_buf();
    void clear_buf_x();
    void clear_buf_h();
    void clear_buf_accum();

    // void stream_reqs(bool is_write);
    // int stream_access(int port, uint32_t addr, uint8_t *data, int size, bool is_write, int64_t *latency);
    // int stream_update(int port, bool is_write);

    unsigned int *regs;
    int8_t *buffer_in;
    int8_t *buffer_out;
    int8_t **crossbar;

//   rnnacc_job_t *job;
      int remaining_jobs;

//   rnnacc_pw_t *pw_req;
      bool pending_write;

//   rnnacc_pr_t *pr_req;
     bool pending_read;

    int pending_req;
    int enqueued_req;
    int remaining_in_req;
    int remaining_out_req;
    bool stalled;

//   int extra_latency_in;

//   int step_count;
//   int feat_count;
//   int roll_count;

//   int line_fetch_lfover;
//   int line_store_lfover;

//   int alpha_in_count;
//   int alpha_out_count;
//   int beta_in_count;
//   int beta_out_count;

    static vp::io_req_status_e hwpe_slave(void *__this, vp::io_req *req);
    static void data_access_test_handler(void *__this, vp::clock_event *event);


    // EVENTS
    vp::clock_event *data_access_test_event;
    vp::clock_event *fsm_start_event;
    vp::clock_event *fsm_event;
    vp::clock_event *fsm_end_event;

    vp::clock_event *job_event; // really needed?


    // EVENT handlers
    static void fsm_start_handler(void *__this, vp::clock_event *event);
    static void fsm_handler(void *__this, vp::clock_event *event);
    static void fsm_end_handler(void *__this, vp::clock_event *event);

    // MAIN FSM and LOOP
    int  fsm();
    void fsm_loop();
    RnnaccState state;

    // REGISTER FILE member functions
    int  regfile_rd(int);
    void regfile_wr(int, int);
    void regfile_cxt();
    void commit();
    int  acquire();
    bool status();
    void printout();

    // REGISTER FILE and HWPE CTRL
    int  cxt0[RNNACC_NB_REG];
    int  cxt1[RNNACC_NB_REG];
    int  cxt_cfg_ptr;
    int  cxt_use_ptr;
    int  job_pending;
    int  job_state;
    unsigned char job_id;

    // HWPE SLAVE REGISTER
    int addr_b;
    int addr_x;
    int addr_wx;
    int addr_h;
    int addr_wh;
    int addr_dst;
    int n_input_external;
    int n_output;
  
    // CONFIG BASED ON HWPE SLAVE REGS
    int n_input;
    int n_hidden_external;

    bool mj_i_tile_en;
    bool mj_h_tile_en;

    int mj_i_tile_nr;
    int mj_h_tile_nr;

    int mj_i_tile_cnt;
    int mj_h_tile_cnt;

    // bool mj_i_tile_cnt;
    // int mj_i_tile_nr;
    // int mj_i_tile_cnt;


    // CONSTANTS depending on HWPE Regs
    bool bias;
    bool matmul;
    bool twomatmul;
    bool multi_job;
    bool load_h;

    // CURRENT STATES
    int multijob_counter;
    bool matmul_state;

    // STREAMER BIAS
    int n_bias_tiles;
    int n_bias_rest;
    int n_bias_idx;

    void setup_streamer_bias();
    int load_bias_cycle();
    bool bias_exit_idx();
    void bias_update_idx();
 
    // STREAMER FEATURES X
    int n_x_tiles;
    int n_x_rest;
    int n_x_idx;

    void setup_streamer_x();
    int load_x_cycle();
    bool x_exit_idx();
    void x_update_idx();

    // STREAMER FEATURES X
    int n_h_tiles;
    int n_h_rest;
    int n_h_idx;

    void setup_streamer_h();
    int load_h_cycle();
    bool h_exit_idx();
    void h_update_idx();

    // STREAMER loads
    RnnaccVectorLoad<uint8_t> vl_bias;
    RnnaccVectorLoad<uint8_t> vl_x;
    RnnaccVectorLoad<uint8_t> vl_h;
    RnnaccVectorLoad<uint8_t> vl_w;

    // STREAMER MATMUL X
    int n_w_o_tiles;
    int n_w_o_rest;
    int w_o_idx;
    int n_w_i_tiles;
    int n_w_i_rest;
    int w_i_idx;

    void setup_streamer_matmul_x();
    int load_wx_cycle();
    bool wx_exit_idx();
    void wx_update_idx();

    int perform_matmul_x();

    // STREAMER MATMUL H
    int n_w_h_tiles;
    int n_w_h_rest;
    int w_h_idx;

    void setup_streamer_matmul_h();
    int load_wh_cycle();
    bool wh_exit_idx();
    void wh_update_idx();

    int perform_matmul_h();

    // STREAMER WRITEOUT
    int n_store_tiles;
    int n_store_rest;
    int store_idx;

    void streamout_setup();
    int streamout_cycle();
    bool streamout_exit_idx();
    void streamout_update_idx();

    RnnaccVectorStore<uint8_t> vs_y;

    // STATEFUL BUFFERS
    xt::xarray<int16_t> buf_x;     // activation registers
    xt::xarray<int16_t> buf_h;     // hidden state registers
    // xt::xarray<int16_t> buf_accum;  // activation registers
    xt::xarray<int32_t> buf_accum;  // activation registers
    xt::xarray<int16_t> buf_w;      // weight fifo?

    // DEBUG settings
    bool fsm_traces;
    bool buf_x_traces;
    bool buf_h_traces;
    bool buf_accum_traces;
    bool buf_w_traces;
    bool matmul_traces;

    bool debug_hex_format;

    void debug_buf_x();
    void debug_buf_h();
    void debug_buf_accum();
    void debug_buf_w();

};

#endif /* __RNNACC_HPP__ */
