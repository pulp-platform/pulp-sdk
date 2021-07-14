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

#include "../rnnacc_v1_impl.hpp"


void Rnnacc_v1::setup_streamer_bias() {
    
    int base_addr = this->addr_b;
    int line_length = 0;

    // define tile numbers
    this->n_bias_rest  = this->n_output % (2*this->NR_MASTER_PORTS); 
    this->n_bias_tiles = int(this->n_output / (2*this->NR_MASTER_PORTS)); 

    if(this->n_bias_rest > 0){
        this->n_bias_tiles += 1;
    }

    // set counter to 0
    this->n_bias_idx = 0;

    // set line length
    line_length = this->n_output;

    // create streamer
    this->vl_bias = RnnaccVectorLoad<uint8_t>(
        this,
        base_addr,     // base_addr
        2*line_length, // tot_length
        4*this->NR_MASTER_PORTS, // d0_stride
        n_bias_tiles,  // d0_length
        0,             // d1_stride
        0,             // d1_length
        0,             // d2_stride
        this->debug_streamer // debug
    );

    this->trace.msg(vp::trace::LEVEL_DEBUG, "STREAMER - vl_bias created\n");

}


int Rnnacc_v1::load_bias_cycle() {
    int64_t cycles = 0;

    xt::xarray<uint8_t> tcdm_data = xt::zeros<uint8_t>({this->NR_MASTER_PORTS*4});

    auto width = 0;
    if((this->n_bias_idx == this->n_bias_tiles-1) && (this->n_bias_rest > 0))
    {
        width = 2*this->n_bias_rest;
    } else {
        width = 4*this->NR_MASTER_PORTS;
    }

    tcdm_data = this->vl_bias.execute(width, cycles, 4);

    for (auto i=this->n_bias_idx*this->NR_MASTER_PORTS*2; i<(this->n_bias_idx+1)*this->NR_MASTER_PORTS*2; i+=2) {
        if(i>=this->n_output) {
            break;
        }
        xt::view(this->buf_accum, i, 1) = xt::cast<int32_t>(
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_bias_idx*this->NR_MASTER_PORTS)*4+0)) << 0 ) |
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_bias_idx*this->NR_MASTER_PORTS)*4+1)) << 8 ));
        xt::view(this->buf_accum, i+1, 1) = xt::cast<int32_t>(
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_bias_idx*this->NR_MASTER_PORTS)*4+2)) << 0) |
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_bias_idx*this->NR_MASTER_PORTS)*4+3)) << 8));
    }

    return (int) cycles;
}

bool Rnnacc_v1::bias_exit_idx() {
    if(this->n_bias_idx == this->n_bias_tiles-1) {
        this->trace.msg(vp::trace::LEVEL_DEBUG, "STREAMER - shift bias << 12\n");
        xt::view(this->buf_accum, xt::all()) = this->buf_accum << 12;
        return true;
    }
    else {
        return false;
    }
}

void Rnnacc_v1::bias_update_idx() {
    if(this->n_bias_idx < this->n_bias_tiles-1) {
      this->n_bias_idx++;
    }
}


void Rnnacc_v1::setup_streamer_x() {
    
    int base_addr = this->addr_x;
    int line_length = 0;
    int tcdm_width = (2*this->NR_MASTER_PORTS);

    // define tile numbers
    this->n_x_rest  = this->n_input % tcdm_width; 
    this->n_x_tiles = int(this->n_input / tcdm_width); 

    if(this->n_x_rest > 0){
        this->n_x_tiles += 1;
    }

    // set counter to 0
    this->n_x_idx = 0;

    // set line length
    line_length = this->n_input;

    // create streamer
    this->vl_x = RnnaccVectorLoad<uint8_t>(
        this,
        base_addr,     // base_addr
        2*line_length, // tot_length
        4*this->NR_MASTER_PORTS, // d0_stride
        n_x_tiles,     // d0_length
        0,             // d1_stride
        0,             // d1_length
        0,             // d2_stride
        this->debug_streamer // debug
    );

    this->trace.msg(vp::trace::LEVEL_DEBUG, "STREAMER - vl_x created\n");
}


int Rnnacc_v1::load_x_cycle() {
    int64_t cycles = 0;

    xt::xarray<uint8_t> tcdm_data = xt::zeros<uint8_t>({this->NR_MASTER_PORTS*4});

    auto width = 0;
    if((this->n_x_idx == this->n_x_tiles-1) && (this->n_x_rest > 0))
    {
        width = 2*this->n_x_rest;
    } else {
        width = 4*this->NR_MASTER_PORTS;
    }
    this->trace.msg(vp::trace::LEVEL_DEBUG, "[load_x_cycles] width %d\n", width);
    tcdm_data = this->vl_x.execute(width, cycles, 4);

    for (auto i=this->n_x_idx*this->NR_MASTER_PORTS*2; i<(this->n_x_idx+1)*this->NR_MASTER_PORTS*2; i+=2) {
        if(i>=this->n_input){
            break;
        }
        xt::view(this->buf_x, i, 1) = 
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_x_idx*this->NR_MASTER_PORTS)*4+0)) << 0 ) |
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_x_idx*this->NR_MASTER_PORTS)*4+1)) << 8 );
        xt::view(this->buf_x, i+1, 1) = 
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_x_idx*this->NR_MASTER_PORTS)*4+2)) << 0) |
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_x_idx*this->NR_MASTER_PORTS)*4+3)) << 8);
    }

    return (int) cycles;
}

bool Rnnacc_v1::x_exit_idx() {
    if(this->n_x_idx == this->n_x_tiles-1) {
        return true;
    }
    else {
        return false;
    }
}

void Rnnacc_v1::x_update_idx() {
    if(this->n_x_idx < this->n_x_tiles-1) {
      this->n_x_idx++;
    }
}


void Rnnacc_v1::setup_streamer_h() {
    
    int base_addr = this->addr_h;
    int line_length = 0;

    // define tile numbers
    this->n_h_rest  = this->n_hidden % (2*this->NR_MASTER_PORTS);
    this->n_h_tiles = int(this->n_hidden / (2*this->NR_MASTER_PORTS));

    if(this->n_h_rest > 0){
        this->n_h_tiles += 1;
    }

    // set counter to 0
    this->n_h_idx = 0;

    // set line length
    line_length = this->n_hidden;

    // create streamer
    this->vl_h = RnnaccVectorLoad<uint8_t>(
        this,
        base_addr,     // base_addr
        2*line_length, // tot_length
        4*this->NR_MASTER_PORTS, // d0_stride
        n_h_tiles,     // d0_length
        0,             // d1_stride
        0,             // d1_length
        0,             // d2_stride
        this->debug_streamer // debug

    );

    this->trace.msg(vp::trace::LEVEL_DEBUG, "STREAMER - vl_h created\n");
}


int Rnnacc_v1::load_h_cycle() {
    int64_t cycles = 0;

    xt::xarray<uint8_t> tcdm_data = xt::zeros<uint8_t>({this->NR_MASTER_PORTS*4});

    auto width = 0;
    if((this->n_h_idx == this->n_h_tiles-1) && (this->n_h_rest > 0))
    {
        width = 2*this->n_h_rest;
    } else {
        width = 4*this->NR_MASTER_PORTS;
    }
    this->trace.msg(vp::trace::LEVEL_DEBUG, "[load_h_cycles] width %d\n", width);
    tcdm_data = this->vl_h.execute(width, cycles, 4);

    for (auto i=this->n_h_idx*this->NR_MASTER_PORTS*2; i<(this->n_h_idx+1)*this->NR_MASTER_PORTS*2; i+=2) {
        if(i>=this->n_hidden){
            break;
        }
        xt::view(this->buf_h, i, 1) = 
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_h_idx*this->NR_MASTER_PORTS)*4+0)) << 0 ) |
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_h_idx*this->NR_MASTER_PORTS)*4+1)) << 8 );
        xt::view(this->buf_h, i+1, 1) = 
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_h_idx*this->NR_MASTER_PORTS)*4+2)) << 0) |
            (xt::cast<int16_t>(xt::view(tcdm_data, (i/2-this->n_h_idx*this->NR_MASTER_PORTS)*4+3)) << 8);
    }

    return (int) cycles;
}

bool Rnnacc_v1::h_exit_idx() {
    if(this->n_h_idx == this->n_h_tiles-1) {
        return true;
    }
    else {
        return false;
    }
}

void Rnnacc_v1::h_update_idx() {
    if(this->n_h_idx < this->n_h_tiles-1) {
      this->n_h_idx++;
    }
}
