/*
 * Copyright (C) 2020  GreenWaves Technologies, SAS
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
 *          Francesco Conti, University of Bologna & GreenWaves Technologies (f.conti@unibo.it) */

#include "../rnnacc_v1_impl.hpp"

void Rnnacc_v1::fsm_start_handler(void *__this, vp::clock_event *event) {
    Rnnacc_v1 *_this = (Rnnacc_v1 *)__this;
    _this->state = START;
    _this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM HANDLER - FSM START EVENT\n");

    // clear state and propagate context
    _this->clear_all();
    _this->regfile_cxt();

    // n_output must be <= NR_REGS_ACCUM
    assert(_this->n_output <= _this->NR_REGS_ACCUM);

    // n_output must be <= NR_REGS_ACCUM
    assert(_this->n_input <= _this->NR_REGS_X);

    // convenience parameters based on regfile
    // input
    if(_this->n_input_external > _this->NR_REGS_X) {
        _this->mj_i_tile_en = true;
        _this->mj_i_tile_nr = _this->n_input_external / _this->NR_REGS_X;
        if((_this->n_input_external % _this->NR_REGS_X) != 0){
            _this->mj_i_tile_nr += 1;
        }
    } 
    else {
        _this->mj_i_tile_en = 0;
        _this->mj_i_tile_nr = 1;
    }

    if(((_this->n_input_external % _this->NR_REGS_X) != 0) & (_this->mj_i_tile_cnt == _this->mj_i_tile_nr-1)){
        _this->n_input = (_this->n_input_external % _this->NR_REGS_X);
    }
    else{
        _this->n_input = _this->NR_REGS_X;
    }

    // hidden
    if(_this->n_hidden_external > _this->NR_REGS_ACCUM) {
        _this->mj_h_tile_en = true;
        _this->mj_h_tile_nr = _this->n_hidden_external / _this->NR_REGS_ACCUM;
        if((_this->n_hidden_external % _this->NR_REGS_ACCUM) != 0){
            _this->mj_h_tile_nr += 1;
        }
    } 
    else {
        _this->mj_h_tile_en = 0;
        _this->mj_h_tile_nr = 1;
    }

    if(((_this->n_hidden_external % _this->NR_REGS_ACCUM) != 0) & (_this->mj_h_tile_cnt == _this->mj_h_tile_nr-1)){
        _this->n_output = (_this->n_hidden_external % _this->NR_REGS_ACCUM);
    }
    else{
        _this->n_output = _this->NR_REGS_ACCUM;
    }

    // output
    if(_this->n_output_external > _this->NR_REGS_ACCUM) {
        _this->mj_o_tile_en = true;
        _this->mj_o_tile_nr = _this->n_output_external / _this->NR_REGS_ACCUM;
        if((_this->n_output_external % _this->NR_REGS_ACCUM) != 0){
            _this->mj_o_tile_nr += 1;
        }
    } 
    else {
        _this->mj_o_tile_en = 0;
        _this->mj_o_tile_nr = 1;
    }

    if(((_this->n_output_external % _this->NR_REGS_ACCUM) != 0) & (_this->mj_o_tile_cnt == _this->mj_o_tile_nr-1)){
        _this->n_output = (_this->n_output_external % _this->NR_REGS_ACCUM);
    }
    else{
        _this->n_output = _this->NR_REGS_ACCUM;
    }

    // _this->n_output_external =_this->n_output;

    // _this->mj_o_tile_en = false;
    // _this->mj_o_tile_nr = 1;
    // _this->mj_o_tile_cnt = 0;

    _this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM HANDLER - Starting a job with the following configuration:\n");
    _this->printout();
    _this->fsm_loop();
}

void Rnnacc_v1::fsm_handler(void *__this, vp::clock_event *event) {
  Rnnacc_v1 *_this = (Rnnacc_v1 *)__this;
  _this->fsm_loop();
}

void Rnnacc_v1::fsm_end_handler(void *__this, vp::clock_event *event) {
  Rnnacc_v1 *_this = (Rnnacc_v1 *)__this;
  _this->cxt_use_ptr = 1-_this->cxt_use_ptr;
  _this->job_pending--;
  _this->irq.sync(true);
  if (!_this->fsm_start_event->is_enqueued() && _this->job_pending > 0) {
      _this->event_enqueue(_this->fsm_start_event, 1);
  }
}

void Rnnacc_v1::fsm_loop() {
  auto latency = 0;
  do {
    latency = this->fsm();
  } while(latency == 0 && state != END);
  if(state == END && !this->fsm_end_event->is_enqueued()) {
    this->event_enqueue(this->fsm_end_event, latency);
  }
  else if (!this->fsm_event->is_enqueued()) {
    this->event_enqueue(this->fsm_event, latency);
  }
}

int Rnnacc_v1::fsm() {
    auto state_next = this->state;
    auto latency = 0;

    this->debug_hex_format = false;
    // this->debug_hex_format = true;

    // this->fsm_traces = false;
    this->fsm_traces = true;

    // this->buf_x_traces = false;
    this->buf_x_traces = true;

    // this->buf_accum_traces = false;
    this->buf_accum_traces = true;

    // this->buf_w_traces = false;
    this->buf_w_traces = true;

    // this->buf_h_traces = false;
    this->buf_h_traces = true;

    // this->matmul_traces = false;
    this->matmul_traces = true;


  switch(this->state) {
    
    case START:
        state_next = START_LOAD;
        break;
    
    case START_LOAD:
        if(this->fsm_traces)
        {
            this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM - State START_LOAD\n");
        }

        if( (this->matmul_state==0) || (this->mj_i_tile_en && (this->mj_i_tile_nr>1)) ) {
            // if(this->mj_o_tile_en || this->matmul_state) {
            //     if(this->matmul_state==1) {
            //         this->setup_streamer_h();
            //         state_next = LOAD_H;
            //     }
            //     else {
            //         this->setup_streamer_x();
            //         state_next = LOAD_X;
            //     }
            // }
            // else { //if(this->mj_i_tile_en) {
                this->setup_streamer_x();
                state_next = LOAD_X;
            // }
        } else if((this->mj_h_tile_cnt==0 && this->matmul_state==1) || (this->mj_o_tile_en && (this->mj_o_tile_nr>1))) {
            // if(this->mj_o_tile_en || this->matmul_state) {
                // if(this->matmul_state==1) {
                    this->setup_streamer_h();
                    state_next = LOAD_H;
            //     }
            //     else {
            //         this->setup_streamer_x();
            //         state_next = LOAD_X;
            //     }
            // }
            // else { //if(this->mj_i_tile_en) {
            //     this->setup_streamer_x();
            //     state_next = LOAD_X;
            // }
        } else {
            if(this->mj_o_tile_en || this->matmul_state) {
                if(this->matmul_state==1) {
                    this->setup_streamer_matmul_h();
                    state_next = STREAM_MATMUL_H;
                }
                else {
                    this->setup_streamer_matmul_x();
                    state_next = STREAM_MATMUL_X;
                }
            }
            else { //if(this->mj_i_tile_en) {
                this->setup_streamer_matmul_x();
                state_next = STREAM_MATMUL_X;
            }
        }
        

        break;

    case LOAD_X:
        if(this->fsm_traces) {
            this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM - State LOAD_X\n");
            this->trace.msg(vp::trace::LEVEL_DEBUG, "    - n_x_tiles = %d\n", this->n_x_tiles);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "    - n_x_rest  = %d\n", this->n_x_rest);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "    - n_x_idx   = %d\n", this->n_x_idx);
        }
        // load data: streamer execution
        latency = this->load_x_cycle();
        // debug activation buffer
        if(this->buf_x_traces) {
            this->debug_buf_x();
        }
        // last load cycle or not
        if(this->x_exit_idx()) {

            // if matmul job
            if(this->matmul) {
                // if first matmul job
                if(this->mj_i_tile_cnt==0) {
                    // clear buf_accum
                    this->clear_buf_accum();
                    if(this->buf_accum_traces) this->debug_buf_accum();
                    if(this->bias) {
                        this->setup_streamer_bias();
                        state_next = LOAD_BIAS;
                    } else {
                        this->setup_streamer_matmul_x();
                        state_next = STREAM_MATMUL_X;
                    }
                } else {
                    if(this->buf_accum_traces) this->debug_buf_accum();
                    this->setup_streamer_matmul_x();
                    state_next = STREAM_MATMUL_X;
                }
            }
            else if(this->twomatmul) {
                if(this->matmul_state==0) {
                    if(mj_i_tile_cnt==0) {

                        // clear buf_accum
                        this->clear_buf_accum();
                        if(this->buf_accum_traces) this->debug_buf_accum();
                        if(this->bias) {
                            this->setup_streamer_bias();
                            state_next = LOAD_BIAS;
                        } else {
                        // this->setup_streamer_h();
                        // state_next = LOAD_H;
                            this->setup_streamer_matmul_x();
                            state_next = STREAM_MATMUL_X;
                        }
                    } else {
                        this->setup_streamer_matmul_x();
                        state_next = STREAM_MATMUL_X;
                    }
                } else {
                    this->setup_streamer_h();
                    state_next = LOAD_H;
                }
            }

            // if(this->mj_i_tile_en) {
            //     if(this->mj_i_tile_cnt==0) {
            //         if(this->bias) {
            //             this->clear_buf_accum();
            //             this->setup_streamer_bias();
            //             state_next = LOAD_BIAS;
            //         } else {
            //             this->clear_buf_accum();
            //             // debug accumulation buffer
            //             if(this->buf_accum_traces) {
            //                 this->debug_buf_accum();
            //             }
            //             // new start
            //             if(this->matmul){
            //                 this->setup_streamer_matmul_x();
            //                 state_next = STREAM_MATMUL_X;
            //             }
            //             else if(this->twomatmul){
            //                 this->setup_streamer_h();
            //                 state_next = LOAD_H;
            //             }
            //             // new stop
            //             // old start
            //             // this->setup_streamer_matmul_x();
            //             // state_next = STREAM_MATMUL_X;
            //             // old stop
            //         }
            //     }
            //     else{
            //         if(this->buf_accum_traces) {
            //             this->debug_buf_accum();
            //         }
            //         // new start
            //         if(this->matmul){
            //             this->setup_streamer_matmul_x();
            //             state_next = STREAM_MATMUL_X;
            //         }
            //         else if(this->twomatmul){
            //             this->setup_streamer_h();
            //             state_next = LOAD_H;
            //         }
            //         // new stop
            //         // old start
            //         // this->setup_streamer_matmul_x();
            //         // state_next = STREAM_MATMUL_X;
            //         // old stop
            //     }
            // }
            // else {
            //     // new start
            //     if(this->matmul) {
            //         // old start
            //         if(this->bias) {
            //             this->clear_buf_accum();
            //             this->setup_streamer_bias();
            //             state_next = LOAD_BIAS;
            //         } else {
            //             this->clear_buf_accum();
            //             // debug accumulation buffer
            //             if(this->buf_accum_traces) {
            //                 this->debug_buf_accum();
            //             }

            //             this->setup_streamer_matmul_x();
            //             state_next = STREAM_MATMUL_X;
            //         }
            //         // old stop
            //     }
            //     else if(this->twomatmul){
            //         this->setup_streamer_h();
            //         state_next = LOAD_H;
            //     }
            //     // new stop
            // }
        }
        else {
            this->x_update_idx();
        }
        break;

    case LOAD_H:
        if(this->fsm_traces) {
            this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM - State LOAD_H\n");
            this->trace.msg(vp::trace::LEVEL_DEBUG, "    - n_h_tiles = %d\n", this->n_h_tiles);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "    - n_h_rest  = %d\n", this->n_h_rest);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "    - n_h_idx   = %d\n", this->n_h_idx);
        }
        // load data: streamer execution
        latency = this->load_h_cycle();
        
        // debug activation buffer
        if(this->buf_h_traces) {
            this->debug_buf_h();
        }
        // last load cycle or not
        if(this->h_exit_idx()) {
            
            if(this->matmul_state==0) {
                this->matmul_state = 1;
                // this->setup_streamer_matmul_x();
                // state_next = STREAM_MATMUL_X;
                this->clear_buf_accum();
                if(this->buf_accum_traces) this->debug_buf_accum();
                this->setup_streamer_bias();
                state_next = LOAD_BIAS;
            } else {
                this->matmul_state = 1;
                this->setup_streamer_matmul_h();
                state_next = STREAM_MATMUL_H;
            }

            // // if(this->mj_i_tile_en) {
            // //     if(this->mj_i_tile_cnt==0) {
            //         if(this->bias) {
            //             this->clear_buf_accum();
            //             this->setup_streamer_bias();
            //             state_next = LOAD_BIAS;
            //         } else {
            //             this->clear_buf_accum();
            //             if(this->buf_accum_traces) {
            //                 this->debug_buf_accum();
            //             }
            //             this->setup_streamer_matmul_x();
            //             state_next = STREAM_MATMUL_X;
            //         }
            // //     }
            // // }
        }
        else {
            this->h_update_idx();
        }
        break;      


    case LOAD_BIAS:
        if(this->fsm_traces) {
            this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM - State LOAD_BIAS\n");
            this->trace.msg(vp::trace::LEVEL_DEBUG, "    - n_bias_tiles = %d\n", this->n_bias_tiles);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "    - n_bias_rest  = %d\n", this->n_bias_rest);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "    - n_bias_idx   = %d\n", this->n_bias_idx);
        }
        // load data: streamer execution
        latency = this->load_bias_cycle();
        // debug accumulation buffer
        if(this->buf_accum_traces) this->debug_buf_accum();
        // last load cycle or not
        if(this->bias_exit_idx()) {
            if(this->matmul_state==0) {
                this->setup_streamer_matmul_x();
                state_next = STREAM_MATMUL_X;
            } else {
                this->setup_streamer_matmul_h();
                state_next = STREAM_MATMUL_H;
            }
            
        }
        else {
            this->bias_update_idx();
        }
        break;

    case STREAM_MATMUL_X:
        if(this->fsm_traces) {
            this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM - State STREAM_MATMUL_X\n");
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  n_w_o_tiles=%d\n", this->n_w_o_tiles);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  n_w_o_rest=%d\n", this->n_w_o_rest);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  w_o_idx=%d\n", this->w_o_idx);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  n_w_i_tiles=%d\n", this->n_w_i_tiles);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  n_w_i_rest=%d\n", this->n_w_i_rest);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  w_i_idx=%d\n", this->w_i_idx);
            // this->trace.msg(vp::trace::LEVEL_DEBUG, "  w_i_idx=%d\n", this->w_i_idx);matmul_state
        }
        latency = this->load_wx_cycle();
        latency += this->perform_matmul_x();
        if(this->buf_w_traces) {
            this->debug_buf_w();
        }
        if(this->buf_accum_traces) {
            this->debug_buf_accum();
        }
        if(this->wx_exit_idx()) {

            if(this->matmul) {
                if((this->mj_i_tile_en == 0) || (this->mj_i_tile_cnt == mj_i_tile_nr-1)) {
                    this->mj_i_tile_cnt = 0;
                    this->streamout_setup();
                    state_next = STREAMOUT;
                } else {
                    this->mj_i_tile_cnt++;
                    state_next = END;
                }
            }
            else if(this->twomatmul) {
                if((this->mj_i_tile_en == 0) || (this->mj_i_tile_cnt == mj_i_tile_nr-1)) {
                    // this->mj_i_tile_cnt = 0;
                    this->matmul_state = 1;
                    printf("MATMUL_STATE=1");
                    // this->setup_streamer_matmul_h();
                    // state_next = STREAM_MATMUL_H;
                    state_next = END;
                } else {
                    this->mj_i_tile_cnt++;
                    state_next = END;
                }
            }


            // if(this->matmul) {
            //     if((this->mj_i_tile_en == 0) || (this->mj_i_tile_cnt == mj_i_tile_nr-1)){
            //         this->mj_i_tile_cnt = 0;
            //         this->streamout_setup();
            //         state_next = STREAMOUT;
            //     }
            //     else {
            //         this->mj_i_tile_cnt++;
            //         state_next = END;
            //     }
            // }
            // else if(this->twomatmul) {
            //     this->setup_streamer_matmul_h();
            //     state_next = STREAM_MATMUL_H;
            // }
        }
        else {
            this->wx_update_idx();
        }
        break;

    case STREAM_MATMUL_H:
        this->matmul_state = 1;
        if(this->fsm_traces) {
            this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM - State STREAM_MATMUL_H\n");
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  n_w_o_tiles=%d\n", this->n_w_o_tiles);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  n_w_o_rest=%d\n", this->n_w_o_rest);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  w_o_idx=%d\n", this->w_o_idx);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  n_w_h_tiles=%d\n", this->n_w_h_tiles);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  n_w_h_rest=%d\n", this->n_w_h_rest);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  w_h_idx=%d\n", this->w_h_idx);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  matmul_state=%d\n", this->matmul_state);
        }
        latency = this->load_wh_cycle();
        latency += this->perform_matmul_h();
        if(this->buf_w_traces) {
            this->debug_buf_w();
        }
        if(this->buf_accum_traces) {
            this->debug_buf_accum();
        }
        if(this->wh_exit_idx()) {

            if(this->twomatmul) {
                if((this->mj_o_tile_en == 0) || (this->mj_o_tile_cnt == mj_o_tile_nr-1)){
                    // this->mj_o_tile_cnt = 0;
                    this->streamout_setup();
                    state_next = STREAMOUT;
                }
                else {
                    this->mj_o_tile_cnt++;
                    state_next = END;
                }
            }
            // else if(this->twomatmul) {
            //     // if((this->mj_i_tile_en == 0) || (this->mj_i_tile_cnt == mj_i_tile_nr-1)){
            //     //     this->mj_i_tile_cnt = 0;
            //         this->streamout_setup();
            //         state_next = STREAMOUT;
            //     // }
            //     // else{
            //         // this->mj_i_tile_cnt++;
            //         // state_next = END;
            //     // }
            // }

            // if(this->matmul) {
            //     if((this->mj_o_tile_en == 0) || (this->mj_o_tile_cnt == mj_o_tile_nr-1)){
            //         this->mj_o_tile_cnt = 0;
            //         this->streamout_setup();
            //         state_next = STREAMOUT;
            //     }
            //     else {
            //         this->mj_o_tile_cnt++;
            //         state_next = END;
            //     }
            // }
            // else if(this->twomatmul) {
            //     // if((this->mj_i_tile_en == 0) || (this->mj_i_tile_cnt == mj_i_tile_nr-1)){
            //     //     this->mj_i_tile_cnt = 0;
            //         this->streamout_setup();
            //         state_next = STREAMOUT;
            //     // }
            //     // else{
            //         // this->mj_i_tile_cnt++;
            //         // state_next = END;
            //     // }
            // }
        }
        else {
            this->wh_update_idx();
        }
        break;
    
    case STREAMOUT:
        if(this->fsm_traces) {
            this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM - State  STREAMOUT\n");
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  n_store_tiles=%d\n", this->n_store_tiles);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  n_store_rest=%d\n", this->n_store_rest);
            this->trace.msg(vp::trace::LEVEL_DEBUG, "  store_idx=%d\n", this->store_idx);
        }
        latency = this->streamout_cycle();
        if(this->streamout_exit_idx()) {
            if((this->mj_h_tile_en == 1) || (this->mj_h_tile_cnt < mj_h_tile_nr-1)){
                this->mj_h_tile_cnt += 1;
                this->matmul_state = 0;
                this->mj_i_tile_cnt = 0;
                this->mj_o_tile_cnt = 0;
                printf("RESET MATMUL_STATE, H_TILE INCREMENT ");
            }
            this->clear_buf_accum();
            state_next = END;
        }
        else {
            this->streamout_update_idx();
        }
        break;

    case END:
      if(this->fsm_traces) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM - State END\n");
      }
      latency = 1;
      break;

  }

  this->state = state_next;
  return latency;
}
