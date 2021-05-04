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

#include <vp/vp.hpp>
#include <vp/itf/io.hpp>
#include <stdio.h>
#include <string.h>
#include <archi/ima/ima_v1.h>
#include "ima_v1_impl.hpp"

#define TOTAL_REQ 10


ima_v1::ima_v1(js::config *config)
: vp::component(config)
{

}


void ima_v1::reset(bool active)
{
  if (active)
  {
    memset(this->regs, 0, sizeof(unsigned int)*IMA_NB_REGS);

    this->clear_ima();
  }
}

/* Reset states and counters */
void ima_v1::clear_ima()
{
  this->state = IMA_STATE_IDLE;
  this->eval_state = IMA_EVAL_STATE_IDLE;

  this->port_id = 0;
  this->job->port = this->port_id;

  memset(this->buffer_in, 0, sizeof(int8_t)*xbar_y);
  memset(this->buffer_out, 0, sizeof(int8_t)*xbar_x);

  this->regs[IMA_RUNNING_TASK/4] = -1;
  this->remaining_jobs = 0;
  this->pending_read = false;
  this->pending_write = false;

  this->pending_req = 0;
  this->enqueued_req = 0;
  this->remaining_in_req = 0;
  this->remaining_out_req = 0;
  this->stalled = false;

  this->extra_latency_in = 0;

  this->step_count = 0;
  this->feat_count = 0;
  this->roll_count = 0;

  this->alpha_in_count  = 0;
  this->alpha_out_count = 0;
  this->beta_in_count   = 0;
  this->beta_out_count  = 0;

  this->line_fetch_lfover = 0;
  this->line_store_lfover = 0;

  /* Event cycles depend on analog time to perform the specific task and on the cluster frequency */ 
  this->job->analog_latency    = (((this->get_frequency() * this->eval_time) / 1000000000) + 1);
  this->pw_req->latency        = (((this->get_frequency() * this->plot_write_time) / 1000000000) + 1);
  this->pr_req->latency        = (((this->get_frequency() * this->plot_read_time) / 1000000000) + 1);

  this->count_stream_in  = 0;
  this->count_stream_out = 0;
  this->count_compute    = 0;
}

/* Stream-in - Compute - Stream-out */
void ima_v1::job_handler(void *__this, vp::clock_event *event)
{
  ima_v1 *_this = (ima_v1 *)__this;

  ima_job_t *job = _this->job;

  if(_this->state == IMA_STATE_J_EVAL)
    _this->trace.msg("Entered job handler (current_state: %s)\n", get_eval_state_name(_this->eval_state).c_str());
  else
    _this->warning.force_warning("Invalid state (current_state: %s)\n", get_state_name(_this->state).c_str());

  switch (_this->eval_state)
  {
    case IMA_EVAL_STATE_IDLE:
    {
      /* Clear the streamers counter before starting new job */
      _this->step_count = 0;
      _this->feat_count = 0;
      _this->roll_count = 0;

      _this->remaining_in_req = (job->height >> 2) + ((job->height & 0x3) != 0);
      _this->remaining_out_req = (job->width >> 2) + ((job->width & 0x3) != 0);

      /* Two cycles per idle */
      job->latency=2;

      /* Minimum delay between stream-out and new stream-in request is 5.
      It is mitigated by 2 cycles of FIFOs, 2 cycle of idle state and then other 1 cycle from stream-out
      itself, if it is larger than 1. Otherwise it has to be considered in the latency of the next stream-in */
      if((_this->remaining_out_req / (_this->nb_master_ports >> 1)) <= 1)
      {
        _this->extra_latency_in = 1;
      }
      else
      {
        _this->extra_latency_in = 0;  
      }

      _this->eval_state = IMA_EVAL_STATE_STREAM_IN;

      if(_this->remaining_jobs == 0)
      {
        if(_this->stats)
          printf("Job Statistics (cycles): Stream-in = %d, Compute = %d, Stream-out = %d\n", _this->count_stream_in, _this->count_compute, _this->count_stream_out);
        _this->set_state(IMA_STATE_IDLE);
      }
      else
      {
        if(_this->remaining_jobs < job->jobs)
        {
          _this->job_update();
        }
      }

      break;
    }

    case IMA_EVAL_STATE_STREAM_IN:
    {
      _this->stream_reqs(0);

      if(_this->remaining_in_req == 0)
      {
        /* Due to the streamer FIFOs */
        job->latency+=(2 + _this->extra_latency_in);
        /* Count the number of memory fetches completed */
        _this->enqueued_req = 0;
        _this->eval_state = IMA_EVAL_STATE_COMPUTATION;
      }
      _this->count_stream_in+=job->latency;

      break;
    }

    case IMA_EVAL_STATE_COMPUTATION:
    {
      _this->exec_job();
      _this->update_finished_jobs(1);
      job->latency = job->analog_latency;
      _this->count_compute+=job->latency;
      _this->eval_state = IMA_EVAL_STATE_STREAM_OUT;

      break;
    }

    case IMA_EVAL_STATE_STREAM_OUT:
    {
      _this->stream_reqs(1);

      if(_this->remaining_out_req == 0)
      {
        _this->count_stream_out+=job->latency;
        _this->count_stream_in+=2;
        /* Due to the streamer FIFOs */
        job->latency+=2;
        _this->enqueued_req = 0;
        _this->remaining_jobs--;
        _this->update_finished_jobs(-1);
        _this->eval_state = IMA_EVAL_STATE_IDLE;
      }
      else
      {
        _this->count_stream_out+=job->latency;
      }

      break;
    }
  }

  if(_this->state != IMA_STATE_IDLE)
  {
    _this->check_requests();
  }
}

/* Writing and Reading Crossbar */
void ima_v1::plot_handler(void *__this, vp::clock_event *event)
{
  ima_v1 *_this = (ima_v1 *)__this;

  if(_this->state == IMA_STATE_P_RD || _this->state == IMA_STATE_P_WR)
    _this->trace.msg("Entered plot handler\n");
  else
    _this->warning.force_warning("Invalid state (current_state: %s)\n", get_state_name(_this->state).c_str());
  
  switch (_this->state)
  {
    case IMA_STATE_P_WR:
    {
      ima_pw_t *plot = _this->pw_req;
      vp::io_slave *port = (vp::io_slave *)plot->pending_plot_req->arg_pop();

      _this->exec_write_plot();
      plot->pending_plot = 0;
      plot->index_x+=8;

      if(plot->index_x >= plot->start_x + plot->width)
      {
        plot->index_y++;

        if(plot->index_y == plot->start_y + plot->height)
        {
          _this->trace.msg("Finish writing crossbar values (written %d values)", (plot->index_y) * (plot->index_x));
          plot->index_x = -1;
          plot->index_y = -1;
          _this->set_state(IMA_STATE_IDLE); 
        }
        else
        {
          plot->index_x = plot->start_x;
        }   
      }

      port->resp(plot->pending_plot_req);
      _this->pending_write = false;

      break;
    }

    case IMA_STATE_P_RD:
    {
      ima_pr_t *plot = _this->pr_req;
      vp::io_slave *port = (vp::io_slave *)plot->pending_plot_req->arg_pop();

      _this->exec_read_plot();

      _this->set_state(IMA_STATE_IDLE);

      *(uint32_t*)plot->pending_plot_req->data = plot->pending_plot;
      port->resp(plot->pending_plot_req);
      _this->pending_read = false;

      break;
    }
  }
}


void ima_v1::set_state(int new_state)
{
  this->trace.msg("Setting new state (new_state: %s)\n", get_state_name(new_state).c_str());
  this->state = new_state;

  this->regs[IMA_CHECK_STATE/4] = this->state;
  /* Ensure that other cores or different jobs can take the IMA control */
  if(this->state == IMA_STATE_IDLE)
  {
    this->regs[IMA_STATUS/4] = 0;
    /* IMA is free */
    this->set_id(-1);
    this->clear_ima();
  }
  else
  {
    this->regs[IMA_STATUS/4] = 1;
  }
}


void ima_v1::set_id(int id)
{
  this->trace.msg("Setting running task id (id: %d)\n", id);

  this->regs[IMA_RUNNING_TASK/4] = id;
}


/* Every core can request IMA acquiring but only when it is ready will be successfully done */
vp::io_req_status_e ima_v1::acquire_req(vp::io_req *req)
{
  if (!req->get_is_write())
  {
    switch (this->state)
    {
      case IMA_STATE_IDLE:
      {
        /* IMA backs to idle for one cycle when it finishes one job but more then 0 jobs are already enqueued */
        if(this->regs[IMA_STATUS/4] == 0)
        {
          this->regs[IMA_ACQUIRE/4] = IMA_ACQUIRE_READY;
          this->trace.msg("IMA is ready\n");
          this->set_state(IMA_STATE_ACQUIRE);
          /* TODO: find the id of running task */
          this->set_id(0);     
        }
        break;
      }

      default:
      {
        this->regs[IMA_ACQUIRE/4] = IMA_ACQUIRE_LOCKED;
        this->trace.msg("IMA is locked\n");
        break;
      }
    }
    return vp::IO_REQ_OK;
  }

  return vp::IO_REQ_INVALID;
}

/* Enqueue a job request */
vp::io_req_status_e ima_v1::trigger_req(vp::io_req *req, int new_state)
{
  if(req->get_is_write())
  {
    /* TODO: find id of running task and of submitter */
    if(0 != this->regs[IMA_RUNNING_TASK/4])
    {
      /* TODO: find id of running task and of submitter */
      this->warning.force_warning("Invalid request (running_task: %d, submitter_id: %d)\n", this->regs[IMA_RUNNING_TASK/4], 0);
      return vp::IO_REQ_INVALID;
    }
    else
    {
      this->set_state(new_state);
      this->remaining_jobs = this->job->jobs;
      this->enqueue_job();
    }
    return vp::IO_REQ_OK; 
  }

  return vp::IO_REQ_INVALID;
}

/* Enqueue a plote writing request */
vp::io_req_status_e ima_v1::submit_req(vp::io_req *req, int new_state)
{
  if(req->get_is_write())
  {
    /* TODO: find id of running task and of submitter */
    if(0 != this->regs[IMA_RUNNING_TASK/4])
    {
      /* TODO: find id of running task and of submitter */
      this->warning.force_warning("Invalid request (running_task: %d, submitter_id: %d)\n", this->regs[IMA_RUNNING_TASK/4], 0);
      return vp::IO_REQ_INVALID;
    }
    else
    {
      if(!this->pending_write)
      {
        vp::io_slave *port = (vp::io_slave *)req->arg_pop();
        this->pending_write = true;
        this->set_state(new_state);
        this->pw_req->pending_plot = this->regs[IMA_SUBMIT_PLOT/4];
        this->enqueue_write();
        this->pw_req->pending_plot_req = req;
        port->grant(this->pw_req->pending_plot_req);
        return vp::IO_REQ_PENDING; 
      }
      else
      {
        return vp::IO_REQ_DENIED; 
      }
    }
    return vp::IO_REQ_OK; 
  }

  return vp::IO_REQ_INVALID;
}

/* Enqueue a plot reading request */
vp::io_req_status_e ima_v1::read_req(vp::io_req *req, int new_state)
{
  if(!req->get_is_write())
  {
    /* TODO: find id of running task and of submitter */
    if(0 != this->regs[IMA_RUNNING_TASK/4])
    {
      /* TODO: find id of running task and of submitter */
      this->warning.force_warning("Invalid request (running_task: %d, submitter_id: %d)\n", this->regs[IMA_RUNNING_TASK/4], 0);
      return vp::IO_REQ_INVALID;
    }
    else
    {
      if(!this->pending_read)
      {
        vp::io_slave *port = (vp::io_slave *)req->arg_pop();
        this->pending_read = true;
        this->set_state(new_state);
        this->enqueue_read();
        this->pr_req->pending_plot_req = req;
        port->grant(this->pr_req->pending_plot_req);
        return vp::IO_REQ_PENDING; 
      }
      else
      {
        return vp::IO_REQ_DENIED; 
      }
      return vp::IO_REQ_OK; 
    } 
  }

  return vp::IO_REQ_INVALID;
}

/* Build IMA requests */
vp::io_req_status_e ima_v1::ima_req(vp::io_req *req, int new_state)
{
  if(req->get_is_write())
  {
    /* TODO: find id of running task and of submitter */
    if(0 != this->regs[IMA_RUNNING_TASK/4])
    {
      /* TODO: find id of running task and of submitter */
      this->warning.force_warning("Invalid request (running_task: %d, submitter_id: %d)\n", this->regs[IMA_RUNNING_TASK/4], 0);
      return vp::IO_REQ_INVALID;
    }
    else
    {
      this->set_state(new_state);
      switch(this->state)
      {
        case IMA_STATE_P_WREQ:
        {
          this->pw_req->start_x = this->regs[IMA_P_START_X/4];
          this->pw_req->start_y = this->regs[IMA_P_START_Y/4];
          this->pw_req->width   = this->regs[IMA_P_WIDTH/4];
          this->pw_req->height  = this->regs[IMA_P_HIGHT/4];

          this->pw_req->index_x = this->pw_req->start_x;
          this->pw_req->index_y = this->pw_req->start_y;

          this->trace.msg("Creating new plot write req (start_x: %d, start_y: %d, width: %d, height: %d)\n", this->pw_req->start_x, this->pw_req->start_y, this->pw_req->width, this->pw_req->height);
          break;
        }

        case IMA_STATE_P_RREQ:
        {
          this->pr_req->addr_x = this->regs[IMA_PR_ADDR_X/4];
          this->pr_req->addr_y = this->regs[IMA_PR_ADDR_Y/4];

          this->pr_req->index_x = 0;
          this->pr_req->index_y = 0;

          this->trace.msg("Creating new plot read req (addr_x: %d, addr_y: %d)\n", this->pr_req->addr_x, this->pr_req->addr_y);
          break;
        }

        case IMA_STATE_J_REQ:
        {
          this->job->start_x            = this->regs[IMA_J_START_X/4];
          this->job->start_y            = this->regs[IMA_J_START_Y/4];
          this->job->width              = this->regs[IMA_J_WIDTH/4];
          this->job->height             = this->regs[IMA_J_HIGHT/4];
          this->job->src_addr           = this->regs[IMA_J_SRC_ADDR/4];
          this->job->dest_addr          = this->regs[IMA_J_DST_ADDR/4];
          this->job->stride_src         = this->regs[IMA_J_SRC_STRID/4];
          this->job->stride_dest        = this->regs[IMA_J_DST_STRID/4];

          this->job->adc_high           = this->regs[IMA_ADC_HIGH/4];
          this->job->adc_low            = this->regs[IMA_ADC_LOW/4];
          this->job->fetch_length       = this->regs[IMA_FETCH_LENGTH/4];
          this->job->store_length       = this->regs[IMA_STORE_LENGTH/4];

          this->job->jobs               = this->regs[IMA_NUM_JOBS/4];

          this->job->line_length        = this->regs[IMA_JOB_LINE_LENGTH/4];
          this->job->feat_stride        = this->regs[IMA_JOB_FEAT_STRIDE/4];
          this->job->feat_length        = this->regs[IMA_JOB_FEAT_LENGTH/4];

          //this->job->roll_length        = this->regs[IMA_JOB_FEAT_ROLL/4];
          this->job->roll_length        = 1;
          this->job->memt               = this->regs[IMA_JOB_LL_MEMT/4];
          this->job->dw_mode            = this->regs[IMA_DW_MODE/4];

          this->job->alpha_in_length    = this->regs[IMA_ALPHA_IN_LENGTH/4];
          this->job->alpha_in_stride    = this->regs[IMA_ALPHA_IN_STRIDE/4];
          this->job->beta_in_length     = this->regs[IMA_BETA_IN_LENGTH/4];
          this->job->beta_in_stride     = this->regs[IMA_BETA_IN_STRIDE/4];
          this->job->alpha_out_length   = this->regs[IMA_ALPHA_OUT_LENGTH/4];
          this->job->alpha_out_stride   = this->regs[IMA_ALPHA_OUT_STRIDE/4];
          this->job->beta_out_length    = this->regs[IMA_BETA_OUT_LENGTH/4];
          this->job->beta_out_stride    = this->regs[IMA_BETA_OUT_STRIDE/4];

          this->trace.msg("Creating new job req (start_x: %d, start_y: %d, width: %d, height: %d, src: %x, dst: %x)\n", this->job->start_x, this->job->start_y, this->job->width, this->job->height, \
                              this->job->src_addr, this->job->dest_addr);
          break;
        }
      }
    }
    return vp::IO_REQ_OK;    
  }

  return vp::IO_REQ_INVALID;
}


/* Programming phase entry point */
vp::io_req_status_e ima_v1::req(void *__this, vp::io_req *req)
{
  ima_v1 *_this = (ima_v1 *)__this;
  vp::io_req_status_e err = vp::IO_REQ_OK;

  uint64_t offset = req->get_addr();
  uint8_t *data = req->get_data();
  uint64_t size = req->get_size();
  bool is_write = req->get_is_write();

  _this->trace.msg("IMA access (offset: 0x%x, size: 0x%x, is_write: %d)\n", offset, size, is_write);

  int reg_id = offset / 4;

  if (reg_id >= IMA_NB_REGS)
  {
    return vp::IO_REQ_INVALID;
  }

  if (!req->get_is_write())
  {
    *(uint32_t *)(req->get_data()) = _this->regs[reg_id];
  }
  else
  {
    _this->regs[reg_id] = *(uint32_t *)(req->get_data());
  }

  /* Reading/writing a register from peripherals interconnect requires 4 cycles */
  req->inc_latency(1);

  switch (reg_id)
  {
    case IMA_ACQUIRE/4:
    {
      err = _this->acquire_req(req);
      break;
    }

    /* Job */
    case IMA_J_START_X/4:

    case IMA_J_START_Y/4:

    case IMA_J_WIDTH/4:

    case IMA_J_HIGHT/4:

    case IMA_J_SRC_ADDR/4:

    case IMA_J_DST_ADDR/4:

    case IMA_J_SRC_STRID/4:

    case IMA_J_DST_STRID/4:
    {
      /* TODO: find the id of running task */
      err = _this->ima_req(req, IMA_STATE_J_REQ);
      break;
    }

    case IMA_TRIGGER/4:
    {
      err = _this->trigger_req(req, IMA_STATE_J_EVAL);
      break;
    }

    /* Plot write */
    case IMA_P_START_X/4:

    case IMA_P_START_Y/4:

    case IMA_P_WIDTH/4:

    case IMA_P_HIGHT/4:
    {
      err = _this->ima_req(req, IMA_STATE_P_WREQ);
      break;
    }

    case IMA_SUBMIT_PLOT/4:
    {
      _this->trace.msg("Submitting new word to write in crossbar (value: %x)\n", *(uint32_t *)req->get_data());
      err = _this->submit_req(req, IMA_STATE_P_WR);
      break;
    }

    /* Plot read */
    case IMA_PR_ADDR_X/4:

    case IMA_PR_ADDR_Y/4:
    {
      err = _this->ima_req(req, IMA_STATE_P_RREQ);
      break;
    }
 
    case IMA_PR_VAL/4:
    {
      _this->trace.msg("Requesting new word to read in crossbar\n");
      err = _this->read_req(req, IMA_STATE_P_RD);
      break;
    }
  }

  return err;
}

/* Upper and lower bounds are programmed before the trigger command */
int8_t ima_v1::adc_clipping(float value)
{
  if(value >= this->job->adc_high)
  {
    value = this->job->adc_high;
  }
  else if(value <= -this->job->adc_low)
  {
    value = -this->job->adc_low;
  }

  value = (float) (value * (((1 << (ADC_PRECISION - 1)) - 1))) / ((this->job->adc_high + this->job->adc_low));

  if(value >= (((1 << (ADC_PRECISION - 1)) - 1)))
  {
    value = (((1 << (ADC_PRECISION - 1)) - 1));
  }
  else if(value <= -(((1 << (ADC_PRECISION - 1)) - 1)))
  {
    value = -(((1 << (ADC_PRECISION - 1)) - 1)); 
  }
  /* Round float before casting */
  if(value >= 0)
  {
    return ((value - (int) value >= 0.5f) ? ((int) value + 1) : ((int) value));
  }
  else
  {
    return ((value - (int) value <= -0.5f) ? ((int) value - 1) : ((int) value));
  }
}


/* Matrix-Vector Multiplication (MVM) */
void ima_v1::exec_job()
{
  for(int i=0; i<this->job->width; i++)
  {
    float sum = 0;

    for(int j=0; j<this->job->height; j++)
    {
      sum += ((float) this->buffer_in[this->job->start_y + j] / ((1 << (DAC_PRECISION - 1)) - 1)) * ((float) this->crossbar[this->job->start_y + j][this->job->start_x + i]) / ((1 << (STOR_DWIDTH - 1)) - 1);
      //printf("in=%d, scaled_in=%f, cb=%d, scaled_cb=%f, sum=%f, scaled_sum=%f, ind_x=%d, ind_y=%d\n", this->buffer_in[this->job->start_y + j], \
              (float) this->buffer_in[this->job->start_y + j] / ((1 << (DAC_PRECISION - 1)) - 1), \
              this->crossbar[this->job->start_y + j][this->job->start_x + i], \
              (float) this->crossbar[this->job->start_y + j][this->job->start_x + i] / ((1 << (STOR_DWIDTH - 1)) - 1), \
              sum, (float) this->adc_clipping(sum), this->job->start_x + i, this->job->start_y + j);
    }
    this->buffer_out[this->job->start_x + i] = this->adc_clipping(sum);
    this->trace.msg("Written in output buffer at index %d (sum: %f, out: %x)\n", this->job->start_x + i, sum, (uint8_t) this->buffer_out[this->job->start_x + i]);
  }

  this->trace.msg("Computed %dx%d MVM (remaining_jobs: %d)\n", this->job->width, this->job->height, (this->remaining_jobs - 1));
}


void ima_v1::exec_write_plot()
{
  ima_pw_t *plot = pw_req;

  /* Crossbar values are 4-bit signed in range [-7, 7] */
  this->crossbar[plot->index_y][plot->index_x + 0] = (int8_t) (((((plot->pending_plot & 0x0000000F) >> 0)  & 0x8) == 0x8) ? (((plot->pending_plot & 0x0000000F) >> 0)  | 0xF0) : (((plot->pending_plot & 0x0000000F) >> 0)));
  this->crossbar[plot->index_y][plot->index_x + 1] = (int8_t) (((((plot->pending_plot & 0x000000F0) >> 4)  & 0x8) == 0x8) ? (((plot->pending_plot & 0x000000F0) >> 4)  | 0xF0) : (((plot->pending_plot & 0x000000F0) >> 4)));
  this->crossbar[plot->index_y][plot->index_x + 2] = (int8_t) (((((plot->pending_plot & 0x00000F00) >> 8)  & 0x8) == 0x8) ? (((plot->pending_plot & 0x00000F00) >> 8)  | 0xF0) : (((plot->pending_plot & 0x00000F00) >> 8)));
  this->crossbar[plot->index_y][plot->index_x + 3] = (int8_t) (((((plot->pending_plot & 0x0000F000) >> 12) & 0x8) == 0x8) ? (((plot->pending_plot & 0x0000F000) >> 12) | 0xF0) : (((plot->pending_plot & 0x0000F000) >> 12)));
  this->crossbar[plot->index_y][plot->index_x + 4] = (int8_t) (((((plot->pending_plot & 0x000F0000) >> 16) & 0x8) == 0x8) ? (((plot->pending_plot & 0x000F0000) >> 16) | 0xF0) : (((plot->pending_plot & 0x000F0000) >> 16)));
  this->crossbar[plot->index_y][plot->index_x + 5] = (int8_t) (((((plot->pending_plot & 0x00F00000) >> 20) & 0x8) == 0x8) ? (((plot->pending_plot & 0x00F00000) >> 20) | 0xF0) : (((plot->pending_plot & 0x00F00000) >> 20)));
  this->crossbar[plot->index_y][plot->index_x + 6] = (int8_t) (((((plot->pending_plot & 0x0F000000) >> 24) & 0x8) == 0x8) ? (((plot->pending_plot & 0x0F000000) >> 24) | 0xF0) : (((plot->pending_plot & 0x0F000000) >> 24)));
  this->crossbar[plot->index_y][plot->index_x + 7] = (int8_t) (((((plot->pending_plot & 0xF0000000) >> 28) & 0x8) == 0x8) ? (((plot->pending_plot & 0xF0000000) >> 28) | 0xF0) : (((plot->pending_plot & 0xF0000000) >> 28)));

  this->trace.msg("Written in crossbar starting from row: %d and column: %d\n", plot->index_x, plot->index_y);
}


void ima_v1::exec_read_plot()
{
  ima_pr_t *plot = pr_req;

  plot->pending_plot = ((this->crossbar[plot->addr_y][plot->addr_x + 0] & 0x0000000F) << 0)   |     \
                       ((this->crossbar[plot->addr_y][plot->addr_x + 1] & 0x0000000F) << 4)   |     \
                       ((this->crossbar[plot->addr_y][plot->addr_x + 2] & 0x0000000F) << 8)   |     \
                       ((this->crossbar[plot->addr_y][plot->addr_x + 3] & 0x0000000F) << 12)  |     \
                       ((this->crossbar[plot->addr_y][plot->addr_x + 4] & 0x0000000F) << 16)  |     \
                       ((this->crossbar[plot->addr_y][plot->addr_x + 5] & 0x0000000F) << 20)  |     \
                       ((this->crossbar[plot->addr_y][plot->addr_x + 6] & 0x0000000F) << 24)  |     \
                       ((this->crossbar[plot->addr_y][plot->addr_x + 7] & 0x0000000F) << 28);

  this->trace.msg("Read in crossbar starting from row: %d and column: %d (read %x)\n", plot->index_x, plot->index_y, plot->pending_plot);
}


void ima_v1::update_finished_jobs(int increment)
{
  this->regs[IMA_FINISHED_JOBS/4] += increment;
}


void ima_v1::enqueue_job()
{
  if (!this->job_event->is_enqueued())
  {
    event_enqueue(this->job_event, 1);
  }
}


void ima_v1::enqueue_write()
{
  if (!this->plot_event->is_enqueued())
  {
    /* Latency depends to the writing process */
    event_enqueue(this->plot_event, this->pw_req->latency);
  }
}

void ima_v1::enqueue_read()
{
  if (!this->plot_event->is_enqueued())
  {
    /* Latency depends to the reading process */
    event_enqueue(this->plot_event, this->pr_req->latency);
  }
}


void ima_v1::check_requests()
{
  // We can continue to send requests if we are not stalled, we didn't send all
  // requests and there are less than 2 pending requests.
  if (!this->stalled && this->pending_req < 2)
  {
    event_enqueue(this->job_event, this->job->latency);
  }

}


void ima_v1::grant(void *__this, vp::io_req *req)
{
  ima_v1 *_this = (ima_v1 *)__this;

  _this->trace.msg("Got grant (req: %p)\n", req);

  _this->stalled = false;

  _this->check_requests();
}


void ima_v1::response(void *__this, vp::io_req *req)
{
  ima_v1 *_this = (ima_v1 *)__this;

  _this->trace.msg("Got response (req: %p)\n", req);

  _this->pending_req--;

  switch (_this->eval_state)
  {
    case IMA_EVAL_STATE_STREAM_IN:
    {
      _this->remaining_in_req--;
      break;
    }

    case IMA_EVAL_STATE_COMPUTATION:
    {
      break;
    }

    case IMA_EVAL_STATE_STREAM_OUT:
    {
      _this->remaining_out_req--;
      break;
    }
  }

  _this->check_requests();
}


void ima_v1::job_update()
{
  ima_job_t *job = this->job;

  /* Input tensor */
  if(this->beta_in_count < job->beta_in_length)
  {
    if(this->alpha_in_count < job->alpha_in_length)
    {
      this->regs[IMA_J_SRC_ADDR/4] += job->alpha_in_stride;
      this->alpha_in_count++;
    }
    else
    {
      this->regs[IMA_J_SRC_ADDR/4] += job->beta_in_stride;
      this->alpha_in_count = 0;
      this->beta_in_count++;

      if(this->beta_in_count == job->beta_in_length)
        this->beta_in_count = 0;
    }
    
    job->src_addr = this->regs[IMA_J_SRC_ADDR/4];
  }

  /* Output tensor */
  if(this->beta_out_count < job->beta_out_length)
  {
    if(this->alpha_out_count < job->alpha_out_length)
    {
      this->regs[IMA_J_DST_ADDR/4] += job->alpha_out_stride;
      this->alpha_out_count++;
    }
    else
    {
      this->regs[IMA_J_DST_ADDR/4] += job->beta_out_stride;
      this->alpha_out_count = 0;
      this->beta_out_count++;

      if(this->beta_out_count == job->beta_out_length)
        this->beta_out_count = 0;
    }

    job->dest_addr = this->regs[IMA_J_DST_ADDR/4];
  }

  this->trace.msg("Updating job addresses (src: %x, dst: %x)\n", job->src_addr, job->dest_addr);
}

/* Mount request for L1 access */
int ima_v1::stream_access(int port, uint32_t addr, uint8_t *data, int size, bool is_write, int64_t *latency)
{
  vp::io_req *req = &this->reqs[port];

  req->set_addr(addr - 0x10000000);
  req->set_size(size);
  req->set_data(data);
  req->set_is_write(is_write);
  req->set_latency(0);

  int err = this->out[port].req(req);

  *((int64_t *)latency) = req->get_latency();

  return err;
}

/* Update the address and call the request builder */
int ima_v1::stream_update(int port, bool is_write)
{
  ima_job_t *job = this->job;

  int err = vp::IO_REQ_INVALID;

  int addr = 0;

  if(is_write)
  {
    addr = job->dest_addr + (this->feat_count * job->stride_dest) + (this->roll_count * job->feat_stride);

    if(this->roll_count < job->roll_length)
    {
      if(this->feat_count < job->feat_length)
      {
        if(this->step_count < job->line_length)
        {
          addr += this->step_count;

          if(this->step_count + 4 >= job->line_length)
          {
            this->line_store_lfover = (this->step_count + 4) & 0x3;

            err = this->stream_access(port, addr, (uint8_t *)(this->buffer_out + job->start_x + this->enqueued_req), this->line_store_lfover, is_write, (int64_t *) &job->latency);

            this->line_store_lfover = 0;
          }
          else
          {
            err = this->stream_access(port, addr, (uint8_t *)(this->buffer_out + job->start_x + this->enqueued_req), 4, is_write, (int64_t *) &job->latency);
          }

          this->step_count+=4;
        }

        if(this->step_count >= job->line_length)
        {
          this->trace.msg("Line %d is fetched\n", this->feat_count); 

          this->port_id = -1;
          this->step_count = 0;
          this->feat_count++;
        }
      }

      if(this->feat_count == job->feat_length)
      {
        this->trace.msg("Feat is finished\n");
        
        this->step_count = 0;
        this->feat_count = 0;
        this->roll_count++;
      }
    }

    if(this->roll_count == job->roll_length)
    {
      this->trace.msg("Roll is finished\n");

      this->step_count = 0;
      this->feat_count = 0;
      this->roll_count = 0;

      err = vp::IO_REQ_OK;
    }
  } /* is write to L1 */
  else /* is read from L1 */
  {
    addr = job->src_addr + (this->feat_count * job->stride_src) + (this->roll_count * job->feat_stride);

    if(this->roll_count < job->roll_length)
    {
      if(this->feat_count < job->feat_length)
      {
        if(this->step_count < job->line_length)
        {
          addr += this->step_count;

          err = this->stream_access(port, addr, new uint8_t[4], 4, is_write, (int64_t *) &job->latency);

          this->step_count+=4;
        }

        if(this->step_count >= job->line_length)
        {
          this->trace.msg("Line %d is fetched\n", this->feat_count);

          /* After every line, reset the ports counter */
          this->port_id = -1;
          this->step_count = 0;
          this->feat_count++;
          this->line_fetch_lfover = job->line_length & 0x3;
        }
      }
      
      if(this->feat_count == job->feat_length)
      {
        this->trace.msg("Feat is finished\n");

        this->step_count = 0;
        this->feat_count = 0;
        this->roll_count++;
      }
    }
    
    if(this->roll_count == job->roll_length)
    {
      this->trace.msg("Rolls are finished\n");
      this->step_count = 0;
      this->feat_count = 0;
      this->roll_count = 0;

      err = vp::IO_REQ_OK;
    }
  }

  return err;
}

/* Fetch/Store from/to L1 */ 
void ima_v1::stream_reqs(bool is_write)
{
  ima_job_t *job = this->job;

  this->trace.msg("New stream request (is_write: %d, port: %d)\n", is_write, job->port);

  /* Every master port fetch 32 bits from L1 contiguously each other */
  int err = this->stream_update(job->port, is_write);

  if (err)
  {
    if (err == vp::IO_REQ_DENIED)
    {
        // If the access is not granted, block the ima until it is granted
      this->stalled = true;
        // We mark it as pending now as we will receive a call to the grant callback
        // telling that the requests is now pending
      this->pending_req++;
    }
    else if (err == vp::IO_REQ_PENDING)
    {
        // Request was granted but is pending, just account it
      this->pending_req++;
    }
  }
  else
  {
    // Request is finished
    if(is_write)
    {
      this->remaining_out_req--;
      /* Reset the port id at the stream-out ending */
      if(this->remaining_out_req == 0)
      {
        this->port_id = -1;
      }

      this->enqueued_req+=4;
    }
    else
    {
      /* Streamers fetch without considering leftovers. This block compute the amount of data to neglect, writing in DAC buffer just the right ones */
      if(this->line_fetch_lfover)
      {
        int8_t temp[4];
        *(uint32_t *)(temp) = (*(uint32_t *)(this->reqs[job->port].get_data()));

        for(int i=0; i<line_fetch_lfover; i++)
        {
          *(uint8_t *)(this->buffer_in + job->start_y + this->enqueued_req + i) = temp[i];
        }

        this->line_fetch_lfover = 0;
      }
      else
      {
        *(uint32_t *)(this->buffer_in + job->start_y + this->enqueued_req) = *(uint32_t *)(this->reqs[job->port].get_data());
      }
      this->remaining_in_req--;
      this->enqueued_req+=4;
    }
  }
  /* Ports requests are sent in parallel. Add latency if port -1 is fetching/storing from/to L1 */
  if(this->port_id == ((nb_master_ports >> 1) - 1))
  {
    /* Reset ports counter */
    this->port_id = 0;
    job->latency++;
  }
  else
  {
    if(this->port_id == -1)
    {
      job->latency++;
    }

    this->port_id++;
  }

  job->port = this->port_id;
}


int ima_v1::build()
{
  traces.new_trace("trace", &trace, vp::DEBUG);

  nb_master_ports = get_config_int("nb_masters");
  xbar_x = get_config_int("xbar_x");
  xbar_y = get_config_int("xbar_y");

  eval_time = get_config_int("eval_ns");
  plot_write_time = get_config_int("plot_write_ns");
  plot_read_time = get_config_int("plot_read_ns");

  stats = get_config_bool("statistics");

  out = new vp::io_master[nb_master_ports];
  reqs = new vp::io_req[nb_master_ports];

  in.set_req_meth(&ima_v1::req);
  new_slave_port("input", &in);

  for (int i=0; i<nb_master_ports; i++)
  {
    out[i].set_resp_meth(&ima_v1::response);
    out[i].set_grant_meth(&ima_v1::grant);
    new_master_port("out_" + std::to_string(i), &out[i]);
  }

  new_master_port("irq", &irq);

  plot_event = event_new(&ima_v1::plot_handler);
  job_event = event_new(&ima_v1::job_handler);

  regs = new unsigned int[IMA_NB_REGS];
  buffer_in = new int8_t[xbar_y];
  buffer_out = new int8_t[xbar_x];

  crossbar = new int8_t*[xbar_y];
  for(int i=0; i<xbar_y; i++)
  {
    crossbar[i] = new int8_t[xbar_x];
  }

  job = new ima_job_t;
  pw_req = new ima_pw_t;
  pr_req = new ima_pr_t;

  return 0;
}


void ima_v1::start()
{
}


extern "C" void *vp_constructor(js::config *config)
{
  return new ima_v1(config);
}
