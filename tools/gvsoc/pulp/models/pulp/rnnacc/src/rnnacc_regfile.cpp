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
    // this->regfile_wr_cxt(addr, value);
    if (this->cxt_cfg_ptr == 0) {
      this->cxt0[addr] = value;
    }
    else if(this->cxt_cfg_ptr == 1) {
      this->cxt1[addr] = value;
    }
  }
  else if (addr < 2*RNNACC_NB_REG) {
    this->cxt0[addr - RNNACC_NB_REG] = value;
    if (this->cxt_cfg_ptr == 0) {
    //   this->regfile_wr_cxt(addr - RNNACC_NB_REG, value);
    }
  }
  else {
    this->cxt1[addr - 2*RNNACC_NB_REG] = value;
    if (this->cxt_cfg_ptr == 1) {
    //   this->regfile_wr_cxt(addr - 2*RNNACC_NB_REG, value);
    }
  }
}

void Rnnacc_v1::regfile_cxt() {

  for(auto addr=0; addr<RNNACC_NB_REG; addr++) {

    auto value = this->cxt_use_ptr == 0 ? this->cxt0[addr] : this->cxt1[addr];

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
        this->n_input_external = value;
        break;

      case RNNACC_N_OUTPUT:
        this->n_output = value;
        break;

      case RNNACC_JOB_MODE:
        uint32_t mask = 0;
        uint32_t masked_value = 0;
        
        mask         = 1;
        masked_value = (value & mask);
        this->bias   = masked_value;

        mask         = 2;
        masked_value = (value & mask) >> 1;
        if( masked_value==1 ){
          this->matmul    = false;
          this->twomatmul = true;
        } else {
          this->matmul    = true;
          this->twomatmul = false;
        }
        mask            = 4;
        masked_value    = (value & mask) >> 2;
        this->multi_job = masked_value;
        break;

    }
  }
}

void Rnnacc_v1::printout() {
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) TP_IN=%d\n", this->TP_IN);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) TP_IN_LINEAR=%d\n", this->TP_IN_LINEAR);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) TP_OUT=%d\n", this->TP_OUT);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) QA_IN=%d\n", this->QA_IN);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) QA_OUT=%d\n", this->QA_OUT);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) NR_COLUMN=%d\n", this->NR_COLUMN);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) COLUMN_SIZE=%d\n", this->COLUMN_SIZE);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) BLOCK_SIZE=%d\n", this->BLOCK_SIZE);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) F_BUFFER_SIZE=%d\n", this->F_BUFFER_SIZE);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) FILTER_SIZE=%d\n", this->FILTER_SIZE);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) SHIFT_CYCLES=%d\n", this->SHIFT_CYCLES);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) OVERHEAD_LD_1X1=%d\n", this->OVERHEAD_LD_1X1);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) OVERHEAD_LD_3X3=%d\n", this->OVERHEAD_LD_3X3);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) OVERHEAD_MV=%d\n", this->OVERHEAD_MV);
//   this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) QUANT_PER_CYCLE=%d\n", this->QUANT_PER_CYCLE);
  // REGISTER FILE and HWPE CTRL
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) cxt_cfg_ptr=%d\n", this->cxt_cfg_ptr);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) cxt_use_ptr=%d\n", this->cxt_use_ptr);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) job_pending=%d\n", this->job_pending);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) job_state=%d\n", this->job_state);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (ctrl) job_id=%d\n", this->job_id);
  // REGISTER FILE configuration parameters
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) bias=%d\n", this->bias);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) matmul=%d\n", this->matmul);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) twomatmul=%d\n", this->twomatmul);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) multi_job=%d\n", this->multi_job);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (cfg) multijob_counter=%d\n", this->multijob_counter);
  // CONVENIENCE configuration
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_b=%x\n", this->addr_b);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_x=%x\n", this->addr_x);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_wx=%x\n", this->addr_wx);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_h=%x\n", this->addr_h);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_wh=%x\n", this->addr_wh);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) addr_dst=%x\n", this->addr_dst);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) n_input=%d\n", this->n_input);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) n_output=%d\n", this->n_output);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) n_input_external=%d\n", this->n_input_external);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) n_hidden_external=%d\n", this->n_hidden_external);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_i_tile_en=%d\n", this->mj_i_tile_en);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_i_tile_nr=%d\n", this->mj_i_tile_nr);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_i_tile_cnt=%d\n", this->mj_i_tile_cnt);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_h_tile_en=%d\n", this->mj_h_tile_en);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_h_tile_nr=%d\n", this->mj_h_tile_nr);
  this->trace.msg(vp::trace::LEVEL_DEBUG, "    (model) mj_h_tile_cnt=%d\n", this->mj_h_tile_cnt);
}

void Rnnacc_v1::commit() {
  this->job_pending++;
  this->job_state = 0;
  this->cxt_cfg_ptr = 1-this->cxt_cfg_ptr;
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