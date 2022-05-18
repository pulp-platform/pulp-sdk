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
 * redMule GVSOC model -- Register File
 */

#include <redMule.hpp>

// REGFILE read function
int redMule::regfile_read(int addr)
{
    if (addr == RM_SPECIAL_TRACE_REG)
    {
        if (this->trace_level == L0_CONFIG)
        {
            return 0;
        }
        else if (this->trace_level == L1_ACTIV_INOUT)
        {
            return 1;
        }
        else if (this->trace_level == L2_DEBUG)
        {
            return 2;
        }
        else
        {
            return 3;
        }
    }
    else if (addr < RM_NB_REGS)
    {
        if (this->cxt_cfg_ptr == 0)
        {
            return this->cxt0[addr];
        }
        else
        {
            return this->cxt1[addr];
        }
    }
    else if (addr < 2 * RM_NB_REGS)
    {
        return this->cxt0[addr - RM_NB_REGS];
    }
    else
    {
        return this->cxt1[addr - 2 * RM_NB_REGS];
    }

}

// REGFILE write function
void redMule::regfile_write(int addr, int value)
{
    if (addr == RM_SPECIAL_TRACE_REG)
    {
        if (value == 0)
        {
            this->trace_level == L0_CONFIG;
        }
        else if (value == 1)
        {
            this->trace_level == L1_ACTIV_INOUT;
        }
        else if (value == 2)
        {
            this->trace_level == L2_DEBUG;
        }
        else if (value == 3)
        {
            this->trace_level == L3_ALL;
        }
    }
    else if (addr == RM_SPECIAL_FORMAT_TRACE_REG)
    {
        this->trace_format = value;
    }
    // aliased 
    else if (addr < RM_NB_REGS)
    {
        if (this->cxt_cfg_ptr == 0)
        {
            this->cxt0[addr] = value;
        }
        else if (this->cxt_cfg_ptr == 1)
        {
            this->cxt1[addr] = value;
        }
    }
    // non-aliased
    else if (addr < 2*RM_NB_REGS)
    {
        this->cxt0[addr - RM_NB_REGS] = value;
    }
    else
    {
        this->cxt1[addr - 2* RM_NB_REGS] = value;
    }
}

// REGFILE read for internal variable assignments
void redMule::regfile_cxt()
{
    for (auto addr = 0; addr < RM_NB_REGS; addr++)
    {
        auto value = this->cxt_use_ptr == 0 ? this->cxt0[addr] : this->cxt1[addr];

        switch (addr)
        {
        case IN_FEAT_ADD:
            this->input_address = value;
            break;
        case WEIGHT_ADD:
            this->weight_address = value;
            break;
        case OUT_FEAT_ADD:
            this->output_address = value;
            break;
        case IN_FEAT_ITERS:
            this->nr_input_rows_iters = (value >> 16) & 0xffff;
            this->nr_input_cols_iters = value & 0xffff;
            break;
        case WEIGHT_ITERS:
            this->nr_weight_rows_iters = (value >> 16) & 0xffff;
            this->nr_weight_cols_iters = value & 0xffff;
            break;
        case LEFTOVERS:
            this->input_rows_leftover = (value >> 24) & 0xff;
            this->input_cols_leftover = (value >> 16) & 0xff;
            this->weight_rows_leftover = (value >> 8) & 0xff;
            this->weight_cols_leftover = (value)&0xff;
            break;
        case LEFT_PARAMS:
            this->nr_stores_total = (value >> 16) & 0xffff;
            this->input_rows_gt_width = ((value >> 15) & 0x1) ? true : false;
            this->input_cols_gt_height = ((value >> 14) & 0x1) ? true : false;
            this->weight_cols_gt_tile = ((value >> 13) & 0x1) ? true : false;
            break;
        case IN_D1_STRIDE:
            this->input_d1_stride = value;
            break;
        case W_TOT_LEN:
            this->weight_total_length = value;
            break;
        case TOT_IN_READ:
            this->nr_input_reads_total = value;
            break;
        case W_D0_STRIDE:
            this->output_d0_stride = value;
            break;
        case OUT_TOT_LEN:
            this->output_total_length = value;
            break;
        case OUT_D0_STRIDE:
            this->weight_address = value;
            break;
        case OUT_D2_STRIDE:
            this->output_d2_stride = value;
            break;
        case IN_ROWS_OFFS:
            this->input_rows_offsets = value;
            break;
        case IN_TOT_LEN:
            this->input_total_length = value;
            break;
        }
    }
}

// DEBUG REGFILE CONTENT
void redMule::printout()
{
    // HW PARAMETERS
    this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) [HW PARAM] ID_WIDTH : %d \n", this->ID_WIDTH);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) [HW PARAM] N_CORES : %d \n", this->ID_WIDTH);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) [HW PARAM] DW : %d \n", this->ID_WIDTH);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) [HW PARAM] IO_REGS : %d \n", this->ID_WIDTH);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) [HW PARAM] N_CONTEXT : %d \n", this->ID_WIDTH);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) [HW PARAM] FpFormat : TO DO TO DO TO DO TO DO \n");
    this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) [HW PARAM] Height : %d \n", this->ID_WIDTH);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) [HW PARAM] Width : %d \n", this->ID_WIDTH);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) [HW PARAM] NumPipeRegs : %d \n", this->ID_WIDTH);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) [HW PARAM] PipeConfig : TO DO TO DO TO DO TO DO \n");
    this->trace.msg(vp::trace::LEVEL_DEBUG, "(archi) [HW PARAM] BITW : %d \n", this->ID_WIDTH);

    // REGISTER FILE CONFIG PARAMS
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  input_address: %X \n", this->input_address);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  weight_address: %X \n", this->weight_address);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  output_address: %X \n", this->output_address);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  nr_input_rows_iters: %d \n", this->nr_input_rows_iters);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  nr_input_cols_iters: %d \n", this->nr_input_cols_iters);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  nr_weight_rows_iters: %d \n", this->nr_weight_rows_iters);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  nr_weight_cols_iters: %d \n", this->nr_weight_cols_iters);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  input_rows_leftover: %d \n", this->input_rows_leftover);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  input_cols_leftover: %d \n", this->input_cols_leftover);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  weight_rows_leftover: %d \n", this->weight_rows_leftover);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  weight_cols_leftover: %d \n", this->weight_cols_leftover);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  nr_stores_total: %d \n", this->nr_stores_total);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  input_rows_gt_width: %s \n", this->input_rows_gt_width ? "true" : "false");
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  input_cols_gt_height: %s \n", this->input_cols_gt_height ? "true" : "false");
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  weight_cols_gt_tile: %s \n", this->weight_cols_gt_tile ? "true" : "false");
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  weight_total_length: %d \n", this->weight_total_length);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  nr_input_reads_total: %d \n", this->nr_input_reads_total);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  weight_d0_stride: %d \n", this->weight_d0_stride);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  output_total_length: %d \n", this->output_total_length);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  output_d0_stride: %d \n", this->output_d0_stride);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  output_d2_stride: %d \n", this->output_d2_stride);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  input_rows_offsets: %d \n", this->input_rows_offsets);
    this->trace.msg(vp::trace::LEVEL_INFO, "(cfg) [REGFILE]  input_total_length: %d \n", this->input_total_length);

    //
}

// COMMIT job config
void redMule::commit() {
  this->trace.msg(vp::trace::LEVEL_DEBUG, "JOB COMMITTED: job_state=%d job_pending=%d job_running=%d\n", this->job_state, this->job_pending, this->job_running);
  this->job_pending++;
  this->job_state = 0;
  this->cxt_cfg_ptr = 1-this->cxt_cfg_ptr;
}

// ACQUIRE -- lock redMule
int redMule::acquire() {
  this->trace.msg(vp::trace::LEVEL_DEBUG, "JOB ACQUIRED: job_state=%d job_pending=%d job_running=%d\n", this->job_state, this->job_pending, this->job_running);
  if(this->job_state == 0 & this->job_pending < 2) {
    int job_id = (int) this->job_id++;
    this->cxt_job_id[this->cxt_cfg_ptr] = job_id;
    this->job_state = -2;
    return job_id;
  }
  else if(this->job_pending == 2) {
    return -1;
  }
  else {
    return this->job_state;
  }
}

// GET redMule job status
bool redMule::status() {
  this->trace.msg(vp::trace::LEVEL_DEBUG, "job_state=%d job_pending=%d\n", this->job_state, this->job_pending);
  if(this->job_state == 0 & this->job_pending == 0)
    return false;
  else
    return true;
}
