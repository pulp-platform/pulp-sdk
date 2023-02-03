/*
 * Copyright (C) 2020-2022  GreenWaves Technologies, ETH Zurich, University of Bologna
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
 * Authors: Francesco Conti, University of Bologna & GreenWaves Technologies (f.conti@unibo.it)
 *          Arpan Suravi Prasad, ETH Zurich (prasadar@iis.ee.ethz.ch)
 */

#include <neureka.hpp>

void Neureka::fsm_start_handler(void *__this, vp::clock_event *event) {
  Neureka *_this = (Neureka *)__this;
  _this->state.set(START);
  _this->running_job_id = _this->cxt_job_id[_this->cxt_use_ptr];
  if(_this->trace_level == L3_ALL) {
    _this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM START EVENT\n");
  }

  // clear state and propagate context
  _this->clear_all();
  _this->regfile_cxt();
  _this->job_running = 1;

  // convenience parameters used internally in the model, but not set by register file
  _this->h_out     = (_this->subtile_nb_ho-(_this->subtile_rem_ho ? 1 : 0)) * _this->H_SIZE + _this->subtile_rem_ho;
  _this->w_out     = (_this->subtile_nb_wo-(_this->subtile_rem_wo ? 1 : 0)) * _this->W_SIZE + _this->subtile_rem_wo;
  _this->h_out_int = (_this->h_out/_this->H_SIZE)*_this->H_SIZE + ((_this->h_out%_this->H_SIZE) ? _this->H_SIZE : 0);
  _this->w_out_int = (_this->w_out/_this->W_SIZE)*_this->W_SIZE + ((_this->w_out%_this->W_SIZE) ? _this->W_SIZE : 0);
  _this->h_in_int  = (_this->h_out_int - 1) + _this->fs;
  _this->w_in_int  = (_this->w_out_int - 1) + _this->fs;
  _this->h_in      = (_this->h_out - 1) + _this->fs;
  _this->w_in      = (_this->w_out - 1) + _this->fs;
  _this->k_out     = _this->depthwise ? (_this->subtile_nb_ko-(_this->subtile_rem_ko ? 1 : 0)) * _this->TP_IN  + _this->subtile_rem_ko
                                      : (_this->subtile_nb_ko-(_this->subtile_rem_ko ? 1 : 0)) * _this->TP_OUT + _this->subtile_rem_ko;

  _this->k_in = (_this->subtile_nb_ki-(_this->subtile_rem_ki ? 1 : 0)) * _this->TP_IN  + _this->subtile_rem_ki;

  // streamin mode is not compatible with quantization_bits != 32 at the moment. sorry!
  assert(!(_this->streamin && _this->quantization_bits!=32));

  // padding is not compatible with FS=1. sorry!
  // assert((_this->padding_top==0 && _this->padding_right==0 && _this->padding_bottom==0 && _this->padding_left==0) || _this->fs==3);

  // filter masking is not compatible with FS=1. sorry!
  assert((_this->filter_mask_top==0)|| _this->fs==3);

  // depthwise is not compatible with FS=1. sorry!
  assert((!_this->depthwise) || (_this->fs==3));

  // in depthwise mode k_out == k_in!
  assert((!_this->depthwise) || (_this->subtile_rem_ko==_this->subtile_rem_ki && _this->subtile_nb_ko==_this->subtile_nb_ki));

  // depthwise and 16-bit mode are incompatible. sorry!
  assert((!_this->mode16) || (!_this->depthwise));

  // in 16-bit mode, k_in must be mult(2). sorry!
  assert((!_this->mode16) || (_this->k_in % 2 == 0));
  if(_this->mode16 && _this->mode_linear) {
    _this->k_in = _this->k_in * 2; 
  }

  // in linear mode, Ho=Wo=1 and mode is set to 1x1 (?)
  assert((!_this->mode_linear) || (_this->fs==1));
  if(_this->mode_linear) {
    _this->h_out = 1;
    _this->w_out = 1;
  }

  _this->fsm_loop();
}

void Neureka::fsm_handler(void *__this, vp::clock_event *event) {
  Neureka *_this = (Neureka *)__this;
  if(_this->trace_level == L3_ALL) {
    _this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM HANDLER EVENT\n");
  }
  _this->fsm_loop();
}

void Neureka::fsm_end_handler(void *__this, vp::clock_event *event) {
  Neureka *_this = (Neureka *)__this;
  int job_id = _this->cxt_job_id[_this->cxt_use_ptr];
  _this->job_running = 0;
  _this->cxt_job_id[_this->cxt_use_ptr] = -1;
  _this->cxt_use_ptr = 1-_this->cxt_use_ptr;
  _this->job_pending--;
  _this->irq.sync(true);
  _this->start_cycles = _this->fsm_start_event->get_cycle();
  std::cout<<"FSM START EVENT CYCLES="<<_this->start_cycles<<std::endl;
  std::cout<<"TOTAL CYCLES="<<(_this->end_cycles - _this->start_cycles)<<std::endl;
  _this->trace.msg(vp::trace::LEVEL_INFO, "Ending job (id=%d).\n", job_id);
  if (!_this->fsm_start_event->is_enqueued() && _this->job_pending > 0) {
      _this->event_enqueue(_this->fsm_start_event, 1);
      _this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM Start Event enqueued with cycles=%d\n", _this->fsm_start_event->get_cycle());
      _this->trace.msg(vp::trace::LEVEL_INFO, "Starting a new job from the queue.\n");
  }
  _this->activity.set(0);
  _this->state.set(IDLE);
}

void Neureka::fsm_loop() {
  auto latency = 0;
  do {
    latency = this->fsm();
  } while(latency == 0 && state.get() != END);
  if(state.get() == END && !this->fsm_end_event->is_enqueued()) {
    this->event_enqueue(this->fsm_end_event, latency);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM End Event enqueued with cycles=%d\n", this->fsm_end_event->get_cycle());
    this->end_cycles = this->fsm_end_event->get_cycle();
    std::cout<<"FSM END EVENT CYCLES="<<this->end_cycles<<std::endl;
  }
  else if (!this->fsm_event->is_enqueued()) {
    if(this->trace_level == L3_ALL) {
      std::ostringstream stringStream;
      stringStream << "New Event Enqueued with latency = " <<latency<< "\n";
      std::string copyOfStr = stringStream.str();
      this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr.c_str());
    }
    // this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM Event before enque with cycles=%d\n", this->fsm_event->get_cycle());
    this->event_enqueue(this->fsm_event, latency);
    // std::cout<<"FSM Event enqueued "<<this->fsm_event->get_cycle()<<std::endl;
    this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM Event enqueued with cycles=%d\n", this->fsm_event->get_cycle());
  }
}

int Neureka::fsm() {
  auto state_next = this->state.get();
  auto latency = 0;
  auto k_in_major_lim = 0;

  this->x_buffer_traces = false;
  this->x_buffer_traces_postload = false;
  this->accum_traces_poststreamin = false;
  this->accum_traces = false;
  this->accum_traces_postmatrixvec =false;
  this->accum_traces_normquant = false;
  this->accum_traces_streamout = false;
  this->psum_block_traces = false;
  this->binconv_traces = false;
  this->fsm_traces = false;
  if(this->trace_level == L1_ACTIV_INOUT) {
    this->x_buffer_traces_postload = true;
    this->accum_traces_streamout = true;
  }
  if(this->trace_level == L2_DEBUG) {
    this->x_buffer_traces_postload = true;
    this->fsm_traces = true;
    this->accum_traces_poststreamin = true;
    this->accum_traces_streamout = true;
    this->accum_traces_postmatrixvec = true;
    this->accum_traces_normquant = true;
  }
  if(this->trace_level == L3_ALL) {
    this->x_buffer_traces = true;
    this->x_buffer_traces_postload = true;
    this->fsm_traces = true;
    this->accum_traces = true;
    this->accum_traces_poststreamin = true;
    this->accum_traces_streamout = true;
    this->accum_traces_postmatrixvec = true;
    this->accum_traces_normquant = true;
  }

  switch(this->state.get()) {
    
    case START:
      this->activity.set(1);
      this->trace.msg(vp::trace::LEVEL_INFO, "Starting a job (id=%d) with the following configuration:\n", this->cxt_job_id[this->cxt_use_ptr]);
      this->printout();
      this->reset_dw_weight_buffer();

      state_next = START_STREAMIN;
      break;
    
    case START_STREAMIN:
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "State START_STREAMIN\n");
      }
      this->constant_setup();
      if(this->streamin) {
        this->streamin_setup();
        state_next = STREAMIN;
      }
      else {
        state_next = STREAMIN_LOAD;
      }
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  k_in_major=%d\n", this->k_in_major_iter);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  k_out_major=%d\n", this->k_out_major);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  i_major=%d\n", this->i_major);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  j_major=%d\n", this->j_major);
      }
      break;

    case STREAMIN:
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "State STREAMIN\n");
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  streamin_k_out_iter=%d\n", this->streamin_k_out_iter);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  streamin_j_out_iter=%d\n", this->streamin_j_out_iter);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  streamin_i_out_iter=%d\n", this->streamin_i_out_iter);
      }
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  Before streamin cycle=%d\n", latency);
      latency = this->streamin_cycle();
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After streamin cycle=%d\n", latency);
      if(this->accum_traces) {
        this->debug_accum();
      }
      if(this->streamin_exit_idx()) {
        latency += 10 ; //overhead for streamin
        if(this->accum_traces_poststreamin) {
          this->trace.msg(vp::trace::LEVEL_DEBUG, "State STREAMIN Finished\n");
          this->debug_accum();
        }
        state_next = STREAMIN_LOAD;
      }
      else {
        this->streamin_update_idx();
      }
      break;

    case STREAMIN_LOAD:
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "State STREAMIN_LOAD\n");
      }
      this->load_setup();
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  load_fbuf_lim=%d\n", this->load_fbuf_lim);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  load_i_fbuf_lim=%d\n", this->load_i_fbuf_lim);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  load_j_fbuf_lim=%d\n", this->load_j_fbuf_lim);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  load_k_in_lim=%d\n", this->load_k_in_lim);
      }
      state_next = LOAD;
      this->load_latency = 0;

      // emulate 6 cycles of latency due to FIFOs + ctrl
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  Before streamin load cycle=%d\n", latency);
      latency += 6;
      if(this->activation_prefetch)
        this->load_latency = 3;
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After streamin load cycle=%d\n", latency);
      break;
      
    case LOAD:
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "State LOAD\n");
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  load_fbuf_lim=%d\n", this->load_fbuf_lim);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  load_i_fbuf_lim=%d\n", this->load_i_fbuf_lim);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  load_j_fbuf_lim=%d\n", this->load_j_fbuf_lim);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  load_k_in_lim=%d\n", this->load_k_in_lim);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  load_i_fbuf=%d\n", this->load_i_fbuf);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  load_j_fbuf=%d\n", this->load_j_fbuf);
      }
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  Before load cycle=%d\n", latency);
      latency = this->load_cycle();
      
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After load cycle=%d\n", latency);
      if(this->x_buffer_traces) {
        this->debug_x_buffer();
      }
      if(this->load_exit_idx()) {
        if(this->fs == 1) {
          latency += this->H_SIZE*this->W_SIZE - (this->load_i_fbuf_lim)*(this->load_j_fbuf_lim) + 6;
          // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After load cycle 1x1=%d\n", latency);
        }
        this->load_do_padding();
        this->load_do_extract();
        this->load_filter_masking();
        this->depthwise_setup();
        state_next = LOAD_MATRIXVEC;
      }
      else {
        this->load_update_idx();
      }
      if(this->activation_prefetch)
        this->load_latency += latency;
      break;

    case LOAD_MATRIXVEC:
      if(this->fsm_traces) {
        // this->trace.msg(vp::trace::LEVEL_DEBUG, "State LOAD_MATRIXVEC\n");
      }
      if(this->x_buffer_traces_postload) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  k_in_major=%d\n", this->k_in_major_iter);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  k_out_major=%d\n", this->k_out_major);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  i_major=%d\n", this->i_major);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  j_major=%d\n", this->j_major);
        this->debug_x_buffer();
        this->debug_x_array();
      }
      this->weightoffs();
      if(this->accum_traces) {
        this->debug_accum();
      }
      this->matrixvec_setup();
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  mv_k_out_lim=%d\n", mv_k_out_lim);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  mv_qw_lim=%d\n", mv_qw_lim); // was simply qw
      }
      state_next = MATRIXVEC;
      if((this->matrixvec_latency > this->load_latency) && this->activation_prefetch)
        latency += this->matrixvec_latency - this->load_latency;
      this->matrixvec_latency = 0;

      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  Before load_matrixvec cycle =%d\n", latency);
      // emulate 6 cycles of latency due to FIFOs + ctrl (10 for 1x1 layers)
      if(this->depthwise && this->dw_iter == 0) {

        latency += 34; // Depthwise weight offset cycles
        if(this->activation_prefetch)
          this->matrixvec_latency = 34;
      }
      else if(!this->depthwise) {
        if(this->activation_prefetch){
          this->matrixvec_latency = this->fs == 1 ? 7 : 6;
          latency += this->fs == 1 ? 7 : 6;
        }
        else{
          latency += this->fs == 1 ? 10 : 6;

        }
      }
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After load_matrixvec cycle =%d\n", latency);

      break;
    
    case MATRIXVEC:
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "State MATRIXVEC\n");
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  mv_k_out_iter=%d\n", mv_k_out_iter);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  mv_k_out_lim=%d\n", mv_k_out_lim);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  mv_qw_iter=%d\n", mv_qw_iter); // was simply qw
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  mv_qw_lim=%d\n", mv_qw_lim); // was simply qw
      }
      k_in_major_lim = this->depthwise ? 1 : this->subtile_nb_ki;
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  Before matrixvec cycle =%d\n", latency);
      latency =  this->matrixvec_cycle();//computes for 1 cycle in dw mode, 1 mac is enabled
      if(this->weight_demux)
        latency = 1; // assume 1 cycle latency for weight from WMEM
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After matrixvec cycle =%d\n", latency);

      this->trace.msg(vp::trace::LEVEL_DEBUG, "  j_major =%d, i_major=%d, subtile_nb_wo=%d, subtile_nb_ho=%d, k_in_major_iter=%d, k_in_major_lim=%d\n", this->j_major, this->i_major, this->subtile_nb_wo, this->subtile_nb_ho, this->k_in_major_iter, k_in_major_lim);
      if(this->activation_prefetch && !((this->j_major == this->subtile_nb_wo-1) && (this->i_major == this->subtile_nb_ho-1) && (this->k_in_major_iter == k_in_major_lim-1))){
        this->matrixvec_latency += latency;
        this->trace.msg(vp::trace::LEVEL_DEBUG,"REACHED HERE WITH MATRIXVEC LATENCY=%d, LATENCY=%d\n", this->matrixvec_latency, latency);
        latency = 0;  
      }
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After matrixvec cycle =%d\n", latency);
      if(this->psum_block_traces) {
        this->debug_psum_block();
      }
      if(this->accum_traces) {
        this->debug_accum();
      }
      if(this->matrixvec_exit_idx()) {// true after iteration for qw is completed in depthwise mode

        // emulate 6 cycles of latency due to FIFOs + ctrl
        if(!this->depthwise) {
          latency += this->fs == 3 ? 8 : 0; // MATRIXVEC_OVERHEAD and UPDATEIDX
          if(this->activation_prefetch && !((this->j_major == this->subtile_nb_wo-1) && (this->i_major == this->subtile_nb_ho-1) && (this->k_in_major_iter == k_in_major_lim-1))){
            this->matrixvec_latency += this->fs == 3 ? 8 : 0;
            latency = 0;
          }  
          // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After matrixvec depthwise cycle =%d\n", latency);
        }

        if(this->fsm_traces) {
          this->trace.msg(vp::trace::LEVEL_DEBUG, "Exiting MATRIXVEC\n");
        }
        if(this->accum_traces_postmatrixvec) {
          this->debug_accum();
        }
        if(!matrixvec_to_matrixvec_idx()) {
          depthwise_update_idx();
          state_next = LOAD_MATRIXVEC;
        }
        else if(!this->matrixvec_to_load_idx()) {
          this->k_in_major_update_idx();
          state_next = STREAMIN_LOAD;
        }
        else if(this->output_quant && this->norm_option_shift) {
          this->normquant_shift_setup();
          state_next = NORMQUANT_SHIFT;
        }
        else if(this->output_quant) {

          this->normquant_mult_setup();
          state_next = NORMQUANT_MULT;
        }
        else {
          this->streamout_setup();
          state_next = STREAMOUT;
        }
      }
      else {
        this->matrixvec_update_idx();// for depthwise only updates w.r.t. qw
      }
      break;
      
    case NORMQUANT_SHIFT:
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "State NORMQUANT_SHIFT\n");
      }
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  Before normquant_shift cycle =%d\n", latency);
      latency = this->normquant_shift_cycle();
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After normquant_shift cycle =%d\n", latency);
      this->normquant_mult_setup();
      state_next = NORMQUANT_MULT;
      break;
    
    case NORMQUANT_MULT:
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "State NORMQUANT_MULT\n");
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  nq_iter=%d\n", this->nq_iter);
      }
       // this->trace.msg(vp::trace::LEVEL_DEBUG, "  Before normquant_mult cycle =%d\n", latency);
      latency = this->normquant_mult_cycle();
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After normquant_mult cycle =%d\n", latency);
      if(this->accum_traces_normquant) {
        this->debug_accum();
      }
      if(this->normquant_mult_exit_idx()) {
         latency += 9;
        this->normquant_bias_setup();
        state_next = NORMQUANT_BIAS;
      }
      else {
        this->normquant_mult_update_idx();
      }
      break;
    
    case NORMQUANT_BIAS:
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "State NORMQUANT_BIAS\n");
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  nqb_iter=%d\n", this->nqb_iter);
      }
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  Before normquant_bias cycle =%d\n", latency);
      latency = this->normquant_bias_cycle();
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After normquant_bias cycle =%d\n", latency);
      if(this->accum_traces_normquant) {
        this->debug_accum();
      }
      if(this->normquant_bias_exit_idx()) {
        latency += 8;
        this->streamout_setup();
        state_next = STREAMOUT;
      }
      else {
        this->normquant_bias_update_idx();
      }
      break;
    
    case STREAMOUT:
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "State STREAMOUT\n");
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  streamout_k_out_iter=%d\n", this->streamout_k_out_iter);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  streamout_i_out_iter=%d\n", this->streamout_i_out_iter);
        this->trace.msg(vp::trace::LEVEL_DEBUG, "  streamout_j_out_iter=%d\n", this->streamout_j_out_iter);
      }
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  Before streamout cycle =%d\n", latency);
      latency = this->streamout_cycle();
      // this->trace.msg(vp::trace::LEVEL_DEBUG, "  After streamout cycle =%d\n", latency);
      if(this->streamout_exit_idx()) {

        if(this->fs == 1) {
          latency += 3;
          this->trace.msg(vp::trace::LEVEL_DEBUG, "  After streamout cycle adjust =%d\n", latency);
        }
        if(this->accum_traces_streamout) {
          this->trace.msg(vp::trace::LEVEL_DEBUG, "  k_in_major=%d\n", this->k_in_major_iter);
          this->trace.msg(vp::trace::LEVEL_DEBUG, "  k_out_major=%d\n", this->k_out_major);
          this->trace.msg(vp::trace::LEVEL_DEBUG, "  i_major=%d\n", this->i_major);
          this->trace.msg(vp::trace::LEVEL_DEBUG, "  j_major=%d\n", this->j_major);
          this->debug_accum();
        }
        if(this->streamout_to_end_idx()) {
          state_next = END;
        }
        else {
          this->high_update_idx();
          this->clear_accum();
          this->clear_x_buffer();
          state_next = START_STREAMIN;
        }
      }
      else {
        this->streamout_update_idx();
      }
      break;

    case END:
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "State END\n");
      }
      latency = 1;
      break;

  }

  this->trace.msg(vp::trace::LEVEL_DEBUG, "LOAD LATENCY= %d\n", this->load_latency);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "MATRIXVEC LATENCY= %d\n", this->matrixvec_latency);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "LATENCY= %d\n", latency);

  this->state.set(state_next);
  return latency;
}
