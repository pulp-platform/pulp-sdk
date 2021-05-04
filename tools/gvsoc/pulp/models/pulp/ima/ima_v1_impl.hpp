/*
 * Copyright (C) 2020 ETH Zurich and University of Bologna
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
 * Authors: Nazareno Bruschi, Unibo (nazareno.bruschi@unibo.it)
 */

/* IMA main FSM States */
#define IMA_STATE_IDLE            0
#define IMA_STATE_ACQUIRE         1
#define IMA_STATE_P_WREQ          2
#define IMA_STATE_P_WR            3
#define IMA_STATE_J_REQ           4
#define IMA_STATE_J_EVAL          5
#define IMA_STATE_P_RREQ          6
#define IMA_STATE_P_RD            7

/* IMA eval FSM States */
#define IMA_EVAL_STATE_IDLE           0
#define IMA_EVAL_STATE_STREAM_IN      1
#define IMA_EVAL_STATE_COMPUTATION    2
#define IMA_EVAL_STATE_STREAM_OUT     3

/* Returns values of Acquire state */
#define IMA_ACQUIRE_LOCKED       -1
#define IMA_ACQUIRE_READY         0

class ima_job_t
{

public:

  /* Job parameters */
  int start_x;
  int start_y;
  int width;
  int height;
  /* Streamer parameters */
  int src_addr;
  int dest_addr;
  int stride_src;
  int stride_dest;
  int fetch_length;
  int store_length;
  int line_length;
  int feat_stride;
  int feat_length;
  int roll_length;
  int memt;
  int dw_mode;
  /* Multi-jobs parameters */
  int jobs;
  int alpha_in_length;
  int alpha_in_stride;
  int beta_in_length;
  int beta_in_stride;
  int alpha_out_length;
  int alpha_out_stride;
  int beta_out_length;
  int beta_out_stride;
  /* Analog parameters */
  int adc_high;
  int adc_low;
  int analog_latency;

  int port;
  int latency;
};

class ima_plot_t
{

public:

  unsigned int pending_plot;
  vp::io_req *pending_plot_req;

  int index_x;
  int index_y;

  int latency;
};

class ima_pw_t : public ima_plot_t
{

public:

  int start_x;
  int start_y;
  int width;
  int height;
};

class ima_pr_t : public ima_plot_t
{

public:

  int addr_x;
  int addr_y;
};


class ima_v1 : public vp::component
{

public:

  ima_v1(js::config *config);

  int build();
  void start();
  void reset(bool active);

  static vp::io_req_status_e req(void *__this, vp::io_req *req);
  static void job_handler(void *_this, vp::clock_event *event);
  static void plot_handler(void *_this, vp::clock_event *event);
  static void grant(void *_this, vp::io_req *req);
  static void response(void *_this, vp::io_req *req);
  void check_requests();

  void enqueue_write();
  void enqueue_read();
  void enqueue_job();

private:

  int nb_master_ports;
  int xbar_x;
  int xbar_y;
  int eval_time;
  int plot_write_time;
  int plot_read_time;

  bool stats;

  int state;
  int eval_state;

  void set_state(int new_state);
  void set_id(int id);

  void clear_ima();

  void exec_write_plot();
  void exec_read_plot();
  void exec_job();
  int8_t adc_clipping(float value);

  void job_update();
  void stream_reqs(bool is_write);
  int stream_access(int port, uint32_t addr, uint8_t *data, int size, bool is_write, int64_t *latency);
  int stream_update(int port, bool is_write);

  unsigned int *regs;
  int8_t *buffer_in;
  int8_t *buffer_out;
  int8_t **crossbar;

  ima_job_t *job;
  int remaining_jobs;

  ima_pw_t *pw_req;
  bool pending_write;

  ima_pr_t *pr_req;
  bool pending_read;

  vp::io_req_status_e ima_req(vp::io_req *req, int new_state);
  vp::io_req_status_e trigger_req(vp::io_req *req, int new_state);
  vp::io_req_status_e submit_req(vp::io_req *req, int new_state);
  vp::io_req_status_e read_req(vp::io_req *req, int new_state);
  vp::io_req_status_e acquire_req(vp::io_req *req);

  void update_finished_jobs(int increment);

  vp::trace trace;

  vp::io_slave in;
  vp::io_master *out;
  vp::io_req *reqs;
  int port_id;

  vp::wire_master<bool> irq;

  vp::clock_event *job_event;
  vp::clock_event *plot_event;
  vp::clock_event *stream_event;

  int pending_req;
  int enqueued_req;
  int remaining_in_req;
  int remaining_out_req;
  bool stalled;

  int extra_latency_in;

  int step_count;
  int feat_count;
  int roll_count;

  int line_fetch_lfover;
  int line_store_lfover;

  int alpha_in_count;
  int alpha_out_count;
  int beta_in_count;
  int beta_out_count;

  int count_stream_in;
  int count_stream_out;
  int count_compute;

};

static string get_state_name(int state) {
  switch (state) {
    case IMA_STATE_IDLE: return "idle";
    case IMA_STATE_ACQUIRE: return "acquire";
    case IMA_STATE_P_WREQ: return "wreq";
    case IMA_STATE_P_WR: return "write";
    case IMA_STATE_J_REQ: return "jreq";
    case IMA_STATE_J_EVAL: return "eval";
    case IMA_STATE_P_RREQ: return "rreq";
    case IMA_STATE_P_RD: return "read";
  }
  return "unknown";
}

static string get_eval_state_name(int state) {
  switch (state) {
    case IMA_EVAL_STATE_IDLE: return "idle";
    case IMA_EVAL_STATE_STREAM_IN: return "stream-in";
    case IMA_EVAL_STATE_COMPUTATION: return "compute";
    case IMA_EVAL_STATE_STREAM_OUT: return "stream-out";
  }
  return "unknown";
}