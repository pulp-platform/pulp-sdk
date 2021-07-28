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
 *          Francesco Conti, University of Bologna & GreenWaves Technologies (f.conti@unibo.it)
 */

// #include <archi/rnnacc/rnnacc_v1.h>
#include "../rnnacc_v1_impl.hpp"

int Rnnacc_v1::regfile_rd(int addr) {
  if(addr < RNNACC_NB_REG) {
    if (this->cxt_cfg_ptr == 0) {
      return this->cxt0[addr];
    }
    else {
      return this->cxt1[addr];
    }
  }
  else if (addr < 2*RNNACC_NB_REG) {
    return this->cxt0[addr - RNNACC_NB_REG];
  }
  else {
    return this->cxt1[addr - 2*RNNACC_NB_REG];
  }
}

void Rnnacc_v1::regfile_wr(int addr, int value) {
  if(addr < RNNACC_NB_REG) {
    if((addr== RNNACC_N_INPUT) || (addr ==RNNACC_N_OUTPUT) || (addr== RNNACC_JOB_MODE)) {
      this->cxt0[addr] = value;
      if (this->cxt_use_ptr_global == 0) {
        this->cxt0[addr] = value;
      }
      else if(this->cxt_use_ptr_global == 1) {
        this->cxt1[addr] = value;
      }
    }
    else {
      if (this->cxt_cfg_ptr == 0) {
        this->cxt0[addr] = value;
      }
      else if(this->cxt_cfg_ptr == 1) {
        this->cxt1[addr] = value;
      }
    }
  }
  else if (addr < 2*RNNACC_NB_REG) {
    this->cxt0[addr - RNNACC_NB_REG] = value;
    if (this->cxt_cfg_ptr == 0) {
    }
  }
  else {
    this->cxt1[addr - 2*RNNACC_NB_REG] = value;
    if (this->cxt_cfg_ptr == 1) {
    }
  }
}

void Rnnacc_v1::regfile_cxt() {

  for(auto addr=0; addr<RNNACC_NB_REG; addr++) {

    auto value = this->cxt_use_ptr == 0 ? this->cxt0[addr] : this->cxt1[addr];
    auto value_global = this->cxt_use_ptr_global == 0 ? this->cxt0[addr] : this->cxt1[addr];

    switch(addr) {

      case RNNACC_ADDR_B:
        this->addr_b = value;
        break;

      case RNNACC_ADDR_X:
        this->addr_x = value;
        break;

      case RNNACC_ADDR_WX:
        this->addr_wx = value;
        break;

      case RNNACC_ADDR_H:
        this->addr_h = value;
        break;

      case RNNACC_ADDR_WH:
        this->addr_wh = value;
        break;

      case RNNACC_ADDR_DST:
        this->addr_dst = value;
        break;

      case RNNACC_N_INPUT:
        this->n_input_external = value_global;
        if(this->NR_REGS_X >= this->n_input_external){
          this->skip_load_x = true;
        } else {
          this->skip_load_x = false;
        }
        break;

      case RNNACC_N_OUTPUT:
        this->n_output_external = value_global;
        this->n_hidden_external = value_global;
        if(this->NR_REGS_H >= this->n_hidden_external){
          this->skip_load_h = true;
        } else {
          this->skip_load_h = false;
        }
        break;

      case RNNACC_JOB_MODE:
        uint32_t mask = 0;
        uint32_t masked_value = 0;
        
        mask         = 1;
        masked_value = (value_global & mask);
        this->bias   = masked_value;

        mask         = 2;
        masked_value = (value & mask) >> 1;
        if( masked_value==1 ){
          this->matmul    = false;
          this->twomatmul = true;
        } else {
          this->matmul    = true;
          this->twomatmul = false;
          this->multi_job = 0;
          this->multijob_counter = 0;
        }
        mask            = 4;
        masked_value    = (value & mask) >> 2;
        this->multi_job = masked_value;

        mask            = 8;
        masked_value    = (value & mask) >> 3;
        if((this->n_output_external <= this->NR_REGS_H) && \
        (this->n_output_external <= this->NR_REGS_X) && \
        (this->n_output_external>=0))
        {
          this->load_x_from_accum = masked_value;
        }
        break;

    }
  }
}

void Rnnacc_v1::printout() {
  // REGISTER FILE and HWPE CTRL
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) ctx[0]=%d\n", this->cxt0);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) ctx[1]=%d\n", this->cxt1);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) \n");
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) cxt_cfg_ptr=%d\n", this->cxt_cfg_ptr);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) cxt_cfg_ptr_global=%d\n", this->cxt_cfg_ptr_global);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) cxt_use_ptr=%d\n", this->cxt_use_ptr);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) cxt_use_ptr_global=%d\n", this->cxt_use_ptr_global);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) job_pending=%d\n", this->job_pending);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) job_state=%d\n", this->job_state);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) job_id=%d\n", this->job_id);
  // REGISTER FILE configuration parameters
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) bias=%d\n", this->bias);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) matmul=%d\n", this->matmul);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) twomatmul=%d\n", this->twomatmul);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) multi_job=%d\n", this->multi_job);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) multijob_counter=%d\n", this->multijob_counter);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) matmul_state=%d\n", this->matmul_state);
  // this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) skip_load_x=%d\n", this->skip_load_x);
  // this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) skip_load_h=%d\n", this->skip_load_h);
  // this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) load_x_from_accum=%d\n", this->load_x_from_accum);
  // CONVENIENCE configuration
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_b=%x\n", this->addr_b);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_x=%x\n", this->addr_x);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_wx=%x\n", this->addr_wx);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_h=%x\n", this->addr_h);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_wh=%x\n", this->addr_wh);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_dst=%x\n", this->addr_dst);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) n_input=%d\n", this->n_input);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) n_hidden=%d\n", this->n_hidden);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) n_output=%d\n", this->n_output);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) n_input_external=%d\n", this->n_input_external);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) n_hidden_external=%d\n", this->n_hidden_external);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) n_output_external=%d\n", this->n_output_external);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_i_tile_en=%d\n", this->mj_i_tile_en);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_i_tile_nr=%d\n", this->mj_i_tile_nr);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_i_tile_cnt=%d\n", this->mj_i_tile_cnt);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_h_tile_en=%d\n", this->mj_h_tile_en);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_h_tile_nr=%d\n", this->mj_h_tile_nr);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_h_tile_cnt=%d\n", this->mj_h_tile_cnt);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_o_tile_en=%d\n", this->mj_o_tile_en);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_o_tile_nr=%d\n", this->mj_o_tile_nr);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_o_tile_cnt=%d\n", this->mj_o_tile_cnt);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) n_h_idx=%d\n", this->n_h_idx);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) w_i_idx=%d\n", this->w_i_idx);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) w_o_idx=%d\n", this->w_o_idx);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) store_idx=%d\n", this->store_idx);

  // this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) w_o_idx=%d\n", this->w_o_idx);
  // this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) w_o_idx=%d\n", this->w_o_idx);
  // this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) w_o_idx=%d\n", this->w_o_idx);
}

void Rnnacc_v1::commit() {
  this->job_pending++;
  this->job_state = 0;
  this->cxt_cfg_ptr = 1-this->cxt_cfg_ptr;
  // this->cxt_use_ptr_global = this->cxt_cfg_ptr_global;
  // if(this->cxt_use_ptr_global != this->cxt_cfg_ptr_global) {
  //   this->cxt_use_ptr_global = 1-this->cxt_use_ptr_global;
  // }
}

int Rnnacc_v1::acquire() {
  if(this->job_state == 0 & this->job_pending < 2) {
    this->job_id++;
    this->job_state = -2;
    return 0;
  }
  else if(this->job_pending == 2) {
    return -1;
  }
  else {
    return this->job_state;
  }
}

bool Rnnacc_v1::status() {
  this->trace.msg(vp::trace::LEVEL_DEBUG, "HWPE SLAVE - status - job_state=%d job_pending=%d\n", this->job_state, this->job_pending);
  if(this->job_state == 0 & this->job_pending == 0)
    return false;
  else
    return true;
}