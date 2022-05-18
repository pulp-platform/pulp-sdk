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
 * redMule GVSOC model -- Main
 */

#include <redMule.hpp>

using namespace std::placeholders;

redMule::redMule(js::config *config)
    : vp::component(config)
{
    this->ID_WIDTH     = 8;
    this->N_CORES      = 8;
    this->DW           = 256;
    this->IO_REGS      = 15;
    this->N_CONTEXT    = 2;
    this->FpFormat     = 2;
    this->Height       = 4;
    this->Width        = 8;
    this->NumPipeRegs  = 3;
    this->PipeConfig   = 3;
    this->BITW         = 16;
}

// ENTRY POINT -- models an access to the redMule SLAVE interface
vp::io_req_status_e redMule::hwpe_slave(void *__this, vp::io_req *req)
{
    redMule *_this = (redMule *)__this;
    if (_this->trace_level == L1_ACTIV_INOUT || _this->trace_level == L2_DEBUG || _this->trace_level == L3_ALL)
    {
        _this->trace.msg(vp::trace::LEVEL_DEBUG, "Received request (addr: 0x%x, size: 0x%x, is_write: %d, data: %p\n", req->get_addr(), req->get_size(), req->get_is_write(), req->get_data());
    }
    uint8_t *data = req->get_data(); // size depends on data get_size

    // Dispatch the register file access to the correct function
    if (req->get_is_write())
    {
        // why need to manipulate get_addr so much?
        if (((req->get_addr() & 0xfff) - 0x40) >> 2 == RM_SPECIAL_TRACE_REG)
        {
            if (*data == 0)
            {
                _this->trace_level = L0_CONFIG;
                _this->trace.msg("Setting tracing level to L0_CONFIG\n");
            }
            else if (*data == 1)
            {
                _this->trace_level = L1_ACTIV_INOUT;
                _this->trace.msg("Setting tracing level to L1_ACTIV_INOUT\n");
            }
            else if (*data == 2)
            {
                _this->trace_level = L2_DEBUG;
                _this->trace.msg("Setting tracing level to L2_DEBUG\n");
            }
            else
            {
                _this->trace_level = L3_ALL;
                _this->trace.msg("Setting tracing level to L3_ALL\n");
            }
            return vp::IO_REQ_OK;
        }
        else if (((req->get_addr() & 0xfff) - 0x20) >> 2 == RM_SPECIAL_FORMAT_TRACE_REG)
        {
            _this->trace_format = *data;
            _this->trace.msg("Setting tracing format to %s\n", *data ? "Hex" : "Dec");
            return vp::IO_REQ_OK;
        }
        else if ((req->get_addr() & 0x17f) == 0x0)
        {
            _this->commit();
            if (!_this->job_running && !_this->fsm_start_event->is_enqueued() && *(uint32_t *)data == 0)
            {
                _this->event_enqueue(_this->fsm_start_event, 1);
            }
        }
        else
        {
            if (_this->trace_level == L1_ACTIV_INOUT || _this->trace_level == L2_DEBUG || _this->trace_level == L3_ALL)
            {
                _this->trace.msg(vp::trace::LEVEL_DEBUG, "offset: %d data: %08x\n", ((req->get_addr() & 0x17f) - 0x20) >> 2, *(uint32_t *)data);
            }
            _this->regfile_write(((req->get_addr() & 0x17f) - 0x20) >> 2, *(uint32_t *)data);
        }
    }
    else
    {
        if ((req->get_addr() & 0x17f) == 0x4)
        {
            *(uint32_t *)data = _this->acquire();
            if (_this->trace_level == L1_ACTIV_INOUT || _this->trace_level == L2_DEBUG || _this->trace_level == L3_ALL)
            {
                _this->trace.msg("Returning %x\n", *(uint32_t *)data);
            }
        }
        else if ((req->get_addr() & 0x17f) == 0xc)
        {
            *(uint32_t *)data = ((_this->cxt_job_id[0] >= 0 ? 0x1 : 0) | (_this->cxt_job_id[1] >= 0 ? 0x100 : 0));
            if (_this->trace_level == L1_ACTIV_INOUT || _this->trace_level == L2_DEBUG || _this->trace_level == L3_ALL)
            {
                _this->trace.msg("Returning %x\n", *(uint32_t *)data);
            }
        }
        else if ((req->get_addr() & 0x17f) == 0x10)
        {
            // Returns the active running job or the last jobid that was run
            *(uint32_t *)data = _this->running_job_id;
            if (_this->trace_level == L1_ACTIV_INOUT || _this->trace_level == L2_DEBUG || _this->trace_level == L3_ALL)
            {
                _this->trace.msg("Returning %x\n", *(uint32_t *)data);
            }
        }
        else
        {
            *(uint32_t *)data = _this->regfile_read(((req->get_addr() & 0x17f) - 0x20) >> 2);
            if (_this->trace_level == L1_ACTIV_INOUT || _this->trace_level == L2_DEBUG || _this->trace_level == L3_ALL)
            {
                _this->trace.msg("Returning %x\n", *(uint32_t *)data);
            }
        }
    }

    return vp::IO_REQ_OK;
}

int redMule::build()
{
    // new trace API call
    this->traces.new_trace("trace", &this->trace, vp::DEBUG);
    // new register to handle fsm state -- reg_32 -- line 1135 of gvsoc/common/engine/src/vp.cpp for info
    this->new_reg("fsm_state", &this->fsm_state, 32);
    // new register to notify redMule busys tate -- reg_8
    this->new_reg("redMule_busy", &this->activity, 8);
    this->activity.set(0);
    this->fsm_state.set(TPU_IDLE);

    /* INPUT/OUTPUT PORTS */
    // one master port (data interface of the HWPE)
    this->new_master_port("out", &this->out);
    // one master port (irq)
    this->new_master_port("irq", &this->irq);
    // slave port (ctrl interface of the HWPE -- for configuration)
    this->new_slave_port("input", &this->in);

    // set request method -- hwpe_slave function
    this->in.set_req_meth(&redMule::hwpe_slave);

    // events handlers for redMule

    this->fsm_start_event = this->event_new(&redMule::fsm_start_handler);
    this->fsm_event = this->event_new(&redMule::fsm_handler);
    this->fsm_end_event = this->event_new(&redMule::fsm_end_handler);

    // DEBUG -- trace level and format
    this->trace_level = L0_CONFIG;
    this->trace_format = 1;

    return 0;
}

extern "C" vp::component *vp_constructor(js::config *config)
{
    return new redMule(config);
}