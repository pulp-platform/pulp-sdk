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

#include <vp/vp.hpp>
#include <vp/itf/io.hpp>
#include <stdio.h>
#include <string.h>
#include "rnnacc_v1_impl.hpp"

#define TOTAL_REQ 10


Rnnacc_v1::Rnnacc_v1(js::config *config)
: vp::component(config)
{

}

void Rnnacc_v1::reset(bool active)
{
    this->buf_x      = xt::zeros<int8_t>({ this->NR_REGS_X, 1});     // activation registers
    this->buf_h      = xt::zeros<int8_t>({ this->NR_REGS_H, 1});     // hidden state registers
    this->buf_accum  = xt::zeros<int32_t>({ this->NR_REGS_ACCUM, 1}); // accumulation registers
    this->buf_w      = xt::zeros<int16_t>({ 2*this->NR_MASTER_PORTS, 1}); // weight fifo
  
    // if (active)
    // {
    //   this->clear_all_buf();
    // }
}


// The `hwpe_slave` member function models an access to the RNNACC SLAVE interface
vp::io_req_status_e Rnnacc_v1::hwpe_slave(void *__this, vp::io_req *req)
{
    Rnnacc_v1 *_this = (Rnnacc_v1 *)__this;

    _this->trace.msg(vp::trace::LEVEL_DEBUG, "HWPE SLAVE - Received request (addr: 0x%x, size: 0x%x, is_write: %d, data: %p\n", req->get_addr(), req->get_size(), req->get_is_write(), req->get_data());
    _this->trace.msg(vp::trace::LEVEL_DEBUG, "HWPE SLAVE - Received request (addr: 0x%x, size: 0x%x, is_write: %d, data: %p\n", req->get_addr(), req->get_size(), req->get_is_write(), req->get_data());
    uint8_t *data = req->get_data(); // size depends on data get_size

    // Dispatch the register file access to the correct function
    if(req->get_is_write()) {
        if((req->get_addr() & 0x17f) == 0x0) { // RNNACC_TRIGGER
            _this->trace.msg(vp::trace::LEVEL_DEBUG, "HWPE SLAVE - commit(addr: 0x%x, size: 0x%x, is_write: %d, data: %p\n", req->get_addr(), req->get_size(), req->get_is_write(), req->get_data());
            _this->commit();
            if (!_this->fsm_start_event->is_enqueued() && *(uint32_t *) data == 0) {
                _this->event_enqueue(_this->fsm_start_event, 1);
                _this->trace.msg(vp::trace::LEVEL_DEBUG, "HWPE SLAVE - enqueded(addr: 0x%x, size: 0x%x, is_write: %d, data: %p\n", req->get_addr(), req->get_size(), req->get_is_write(), req->get_data());
            }
        }
        else { // Write Job-Dependent Registers
            _this->trace.msg(vp::trace::LEVEL_DEBUG, "HWPE SLAVE - offset: %d data: %08x\n", ((req->get_addr() & 0x17f) - 0x20) >> 2, *(uint32_t *) data);
            _this->regfile_wr(((req->get_addr() & 0x17f) - 0x20)>> 2, *(uint32_t *) data);
        }
    }
    else {
        if((req->get_addr() & 0x17f) == 0x4) { // RNNACC_ACQUIRE
            *(uint32_t *) data = _this->acquire();
            _this->trace.msg("HWPE SLAVE - Returning %x\n", *(uint32_t *) data);
        }
        else if((req->get_addr() & 0x17f) == 0xc) { // RNNACC_STATUS
            *(uint32_t *) data = _this->status() ? 1 : 0;
            _this->trace.msg("HWPE SLAVE - Returning %x\n", *(uint32_t *) data);
        }
        else { // Read Job-Dependent Registers
            *(uint32_t *) data = _this->regfile_rd(((req->get_addr() & 0x17f) - 0x20) >> 2);
            _this->trace.msg("HWPE SLAVE - Returning %x\n", *(uint32_t *) data);
        }
    }

    return vp::IO_REQ_OK;
}


int Rnnacc_v1::build()
{
    // create new traces
    this->traces.new_trace("trace", &this->trace, vp::DEBUG);

    // debug message
    this->trace.msg(vp::trace::LEVEL_DEBUG, "BUILD - start build()\n");

    // get config
    this->NR_MASTER_PORTS = get_config_int("nb_masters");
    this->NR_REGS_X = get_config_int("nb_regs_x");
    this->NR_REGS_H = get_config_int("nb_regs_h");
    this->NR_REGS_ACCUM = get_config_int("nb_regs_accum");

    // set streamer param based on config
    this->STREAM_MAX_WIDTH_BYTES = this->NR_MASTER_PORTS*4;

    // create memory master ports
    tcdm_ports = new vp::io_master[this->NR_MASTER_PORTS];

    // connect memory master ports
    for (int i=0; i<this->NR_MASTER_PORTS; i++)
    {
      new_master_port("tcdm_port_" + std::to_string(i), &tcdm_ports[i]);
    }
  
    // create requests for each memory master port
    tcdm_reqs = new vp::io_req[this->NR_MASTER_PORTS]; // memory master port input

    // event port
    new_master_port("irq", &irq);

    // create new apb slave port
    this->new_slave_port("input", &this->in);

    // run hwpe_slave() when request is sent via apb slave port
    this->in.set_req_meth(&Rnnacc_v1::hwpe_slave);


    // define event handlers
    this->fsm_start_event = this->event_new(&Rnnacc_v1::fsm_start_handler);
    this->fsm_event = this->event_new(&Rnnacc_v1::fsm_handler);
    this->fsm_end_event = this->event_new(&Rnnacc_v1::fsm_end_handler);

    // debug message
    this->trace.msg(vp::trace::LEVEL_DEBUG, "BUILD - finished build()\n");

    return 0;
}


extern "C" void *vp_constructor(js::config *config)
{
  return new Rnnacc_v1(config);
}
