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
 * redMule GVSOC model -- fsm handlers
 */

#include <redMule.hpp>

// START HANDLER
void redMule::fsm_start_handler(void *__this, vp::clock_event *event) {
  redMule *_this = (redMule *)__this;
  _this->fsm_state.set(TPU_STARTING);
  _this->running_job_id = _this->cxt_job_id[_this->cxt_use_ptr];
  if(_this->trace_level == L3_ALL) {
    _this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM START EVENT\n");
  }

  // clear state and propagate context
  /**
   TO DEFINE THIS FUNCTION
    _this->clear_all();
  **/
  _this->regfile_cxt();
  _this->job_running = 1;

  /** 
   * ADD THINGS HERE (CHECK NE16)
   **/

  _this->fsm_loop();
}

// FSM HANDLER
void redMule::fsm_handler(void *__this, vp::clock_event *event) {
  redMule *_this = (redMule *)__this;
  _this->fsm_loop();
}

// END STATE FSM
void redMule::fsm_end_handler(void *__this, vp::clock_event *event) {
  redMule *_this = (redMule *)__this;
  int job_id = _this->cxt_job_id[_this->cxt_use_ptr];
  _this->job_running = 0;
  _this->cxt_job_id[_this->cxt_use_ptr] = -1;
  _this->cxt_use_ptr = 1-_this->cxt_use_ptr;
  _this->job_pending--;
  _this->irq.sync(true);
  _this->trace.msg(vp::trace::LEVEL_INFO, "Ending job (id=%d).\n", job_id);
  if (!_this->fsm_start_event->is_enqueued() && _this->job_pending > 0) {
      _this->event_enqueue(_this->fsm_start_event, 1);
      _this->trace.msg(vp::trace::LEVEL_INFO, "Starting a new job from the queue.\n");
  }
  _this->activity.set(0);
  _this->fsm_state.set(TPU_IDLE);
}

// LOOP FSM WHEN REDMULE IS RUNNING
void redMule::fsm_loop() {
  auto latency = 0;
  do {
    latency = this->ctrl_fsm();
  } while(latency == 0 && fsm_state.get() != TPU_FINISHED);
  if(fsm_state.get() == TPU_FINISHED && !this->fsm_end_event->is_enqueued()) {
    this->event_enqueue(this->fsm_end_event, latency);
  }
  else if (!this->fsm_event->is_enqueued()) {
    this->event_enqueue(this->fsm_event, latency);
  }
}

// MAIN FSM MODEL
int redMule::ctrl_fsm()
{
    auto fsm_state_next = this->fsm_state.get();
    auto latency = 0;

    switch(this->fsm_state.get())
    {
        case TPU_IDLE:
            break;
        case TPU_STARTING:
            break;
        case TPU_COMPUTING:
            break;
        case TPU_BUFFERING:
            break;
        case TPU_STORING:
            break;
        case TPU_FINISHED:
            if (this->fsm_traces)
            {
                this->trace.msg(vp::trace::LEVEL_DEBUG, "FSM REACHED TPU_FINISHED (END) \n");
            }
            latency = 1;
            break;
    }

    return latency;
}