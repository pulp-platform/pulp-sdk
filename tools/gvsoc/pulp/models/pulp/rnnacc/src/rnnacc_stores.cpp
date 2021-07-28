/*
 * Copyright (C) 2021 ETH Zurich, University of Bologna, GreenWaves Technologies
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

#include "../rnnacc_v1_impl.hpp"
#include <math.h> // pow

void Rnnacc_v1::streamout_setup() {

    int base_addr = this->addr_dst;
    int line_length = 0;

    // define tile numbers
    this->n_store_rest  = this->n_output % (2*this->NR_MASTER_PORTS); 
    this->n_store_tiles = int(this->n_output / (2*this->NR_MASTER_PORTS)); 

    if(this->n_store_rest > 0){
        this->n_store_tiles += 1;
    }

    // set counter to 0
    this->store_idx = 0;

    // set line length
    line_length = this->n_output;

    // create streamer
    this->vs_y = RnnaccVectorStore<uint8_t>(
        this,
        base_addr,     // base_addr
        2*line_length, // tot_length
        4*this->NR_MASTER_PORTS, // d0_stride
        n_store_tiles, // d0_length
        0,             // d1_stride
        0,             // d1_length
        0,             // d2_stride
        this->debug_streamer // debug
    );

    this->trace.msg(vp::trace::LEVEL_DEBUG, "STREAMER - vs_y created\n");


    if(this->buf_accum_traces) {
        this->debug_buf_accum();
    }
    this->trace.msg(vp::trace::LEVEL_DEBUG, "STREAMER - shift result >> 12\n");
    xt::view(this->buf_accum, xt::all()) = this->buf_accum >> 12;
    if(this->buf_accum_traces) {
        this->debug_buf_accum();
    }

    if(this->buf_accum_traces) {
        this->debug_buf_accum();
    }

}

int Rnnacc_v1::streamout_cycle() { 

    int64_t cycles = 0;
    xt::xarray<uint8_t> tcdm_data = xt::zeros<uint8_t>({this->NR_MASTER_PORTS*4});

    auto width = 0;
    if((this->store_idx == this->n_store_tiles-1) && (this->n_store_rest > 0))
    {
        width = 2*this->n_store_rest;
    } else {
        width = 4*this->NR_MASTER_PORTS;
    }

    this->trace.msg(vp::trace::LEVEL_DEBUG, "STREAMER - width %d\n", width);

    for (auto i=this->store_idx*this->NR_MASTER_PORTS*2; i<(this->store_idx+1)*this->NR_MASTER_PORTS*2; i+=2) {
        if(i>=this->n_output){
            break;
        }
        xt::view(tcdm_data, (i/2-this->store_idx*this->NR_MASTER_PORTS)*4+0) = (xt::cast<int16_t>(xt::view(this->buf_accum, i, 1)) >> 0) & 0xff;
        xt::view(tcdm_data, (i/2-this->store_idx*this->NR_MASTER_PORTS)*4+1) = (xt::cast<int16_t>(xt::view(this->buf_accum, i, 1)) >> 8) & 0xff;
        xt::view(tcdm_data, (i/2-this->store_idx*this->NR_MASTER_PORTS)*4+2) = (xt::cast<int16_t>(xt::view(this->buf_accum, i+1, 1)) >> 0) & 0xff;
        xt::view(tcdm_data, (i/2-this->store_idx*this->NR_MASTER_PORTS)*4+3) = (xt::cast<int16_t>(xt::view(this->buf_accum, i+1, 1)) >> 8) & 0xff;
    }

    this->vs_y.execute(tcdm_data, width, cycles, 1);

    return (int) cycles;
}

bool Rnnacc_v1::streamout_exit_idx() {
  if(this->store_idx == this->n_store_tiles-1) {
        return true;
    }
    else {
        return false;
    }
}

void Rnnacc_v1::streamout_update_idx() {
    if(this->store_idx < this->n_store_tiles-1) {
        this->store_idx++;
    }
}
