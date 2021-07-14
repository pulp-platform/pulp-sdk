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


void Rnnacc_v1::setup_streamer_matmul_x() {
    
    int base_addr = this->addr_wx;
    // int base_addr = this->addr_wx;
    int word_stride = 0;
    int line_length = 0;
    int tiles_total = 0;

    int bandwidth_words = 2*this->NR_MASTER_PORTS;

    // NO streamer tiles
    this->n_w_o_rest = 0;
    this->n_w_o_rest  = this->n_output % bandwidth_words; 
    this->n_w_o_tiles = int(this->n_output / bandwidth_words); 

    if(this->n_w_o_rest > 0){
        this->n_w_o_tiles += 1;
    } 

    int stride_inp = 2*bandwidth_words;
    int stride_out = 2*this->n_output_external;

    this->n_w_i_rest  = this->n_input % (1); 
    this->n_w_i_tiles = int(this->n_input / (1)); 

    // set counter to 0
    this->w_o_idx = 0;
    this->w_i_idx = 0;

    // set line length
    line_length = this->n_output * this->n_input;
    tiles_total = this->n_w_i_tiles*this->n_w_o_tiles;

    // create streamer
    this->vl_w = RnnaccVectorLoad<uint8_t>(
        this,
        base_addr,         // base_addr
        2*line_length,     // tot_length
        stride_inp,        // d0_stride
        this->n_w_o_tiles, // d0_length
        stride_out,        // d1_stride
        this->n_w_i_tiles, // d1_length
        0,                 // d2_stride
        this->debug_streamer // debug
    );

    this->trace.msg(vp::trace::LEVEL_DEBUG, "STREAMER - vl_w created\n");
}

int Rnnacc_v1::load_wx_cycle() {
    int64_t cycles = 0;

    xt::xarray<uint8_t> tcdm_data = xt::zeros<uint8_t>({this->NR_MASTER_PORTS*4});

    auto width = 0;
    if((this->w_o_idx == this->n_w_o_tiles-1) && (this->n_w_o_rest > 0))
    {
        width = 4*this->n_w_o_rest;
    } else {
        width = 4*this->NR_MASTER_PORTS;
    }
    tcdm_data = this->vl_w.execute(width, cycles, 4);

    this->buf_w = xt::zeros<int8_t>({ 2*this->NR_MASTER_PORTS, 1});
    for (auto i=0; i<this->NR_MASTER_PORTS*2; i+=2) {
        if((i%this->NR_MASTER_PORTS*2)>=width){
            break;
        }
        xt::view(this->buf_w, i, 1) = 
            ((xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+0)) << 0 ) |
             (xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+1)) << 8 ));
        xt::view(this->buf_w, i+1, 1) =
            ((xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+2)) << 0) |
             (xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+3)) << 8));
    }

    return (int) cycles;
}


bool Rnnacc_v1::wx_exit_idx() {
    if((this->w_o_idx == this->n_w_o_tiles-1) & (this->w_i_idx == this->n_w_i_tiles-1)) {
        return true;
    }
    else {
        return false;
    }
}

void Rnnacc_v1::wx_update_idx() {
    if(this->w_o_idx == this->n_w_o_tiles-1) {
        if(this->w_i_idx < this->n_w_i_tiles-1) {
            this->w_i_idx++;
            this->w_o_idx = 0;
        }
    } else if(this->w_o_idx < this->n_w_o_tiles-1) {
      this->w_o_idx++;
    }
}


int Rnnacc_v1::perform_matmul_x() {

    xt::xarray<int32_t> mult0 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> mult1 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> mult2 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> mult3 = xt::zeros<int32_t>({1}); 

    xt::xarray<int32_t> acc0 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> acc1 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> acc2 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> acc3 = xt::zeros<int32_t>({1}); 


    for (auto i=this->w_o_idx*2*this->NR_MASTER_PORTS; i<(this->w_o_idx+1)*2*this->NR_MASTER_PORTS; i+=4) {

        // Multiply X0 with two new weights W0 W1
        mult0 = xt::view(this->buf_x, this->w_i_idx) * xt::view(this->buf_w, ((i/4)*4+0)%(this->NR_MASTER_PORTS*2), 1);
        mult1 = xt::view(this->buf_x, this->w_i_idx) * xt::view(this->buf_w, ((i/4)*4+1)%(this->NR_MASTER_PORTS*2), 1);

        // Multiply X0 with two new weights W2 W3
        if(this->NR_MASTER_PORTS > 1){
            mult2 = xt::view(this->buf_x, this->w_i_idx) * xt::view(this->buf_w, ((i/4)*4+2)%(this->NR_MASTER_PORTS*2), 1);
            mult3 = xt::view(this->buf_x, this->w_i_idx) * xt::view(this->buf_w, ((i/4)*4+3)%(this->NR_MASTER_PORTS*2), 1);
        }

        // debug print
        if(this->matmul_traces) {
            std::cout << "[MULT] index i=" << std::dec << std::setw(16) << i << std::dec << std::endl;            
            if(this->debug_hex_format){
                std::cout << "[MULT] mult0="<< std::hex << std::setw(16) << mult0 << " x=" << xt::view(this->buf_x, this->w_i_idx) << " w=" << xt::view(this->buf_w, ((i/4)*4+0)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult1="<< std::hex << std::setw(16) << mult1 << " x=" << xt::view(this->buf_x, this->w_i_idx) << " w=" << xt::view(this->buf_w, ((i/4)*4+1)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult2="<< std::hex << std::setw(16) << mult2 << " x=" << xt::view(this->buf_x, this->w_i_idx) << " w=" << xt::view(this->buf_w, ((i/4)*4+2)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult3="<< std::hex << std::setw(16) << mult3 << " x=" << xt::view(this->buf_x, this->w_i_idx) << " w=" << xt::view(this->buf_w, ((i/4)*4+3)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;

            } else {
                std::cout << "[MULT] mult0="<< std::dec << std::setw(16) << mult0 << " x=" << xt::view(this->buf_x, this->w_i_idx) << " w=" << xt::view(this->buf_w, ((i/2)*4+0)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;            
                std::cout << "[MULT] mult1="<< std::dec << std::setw(16) << mult1 << " x=" << xt::view(this->buf_x, this->w_i_idx) << " w=" << xt::view(this->buf_w, ((i/2)*4+1)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult2="<< std::dec << std::setw(16) << mult2 << " x=" << xt::view(this->buf_x, this->w_i_idx) << " w=" << xt::view(this->buf_w, ((i/2)*4+2)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult3="<< std::dec << std::setw(16) << mult3 << " x=" << xt::view(this->buf_x, this->w_i_idx) << " w=" << xt::view(this->buf_w, ((i/2)*4+3)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
            }
        }

        if(i+0>=this->n_output) break;
        xt::view(this->buf_accum, i+0, 1) = xt::view(this->buf_accum, i+0, 1) + xt::view(mult0, 1);

        if(i+1>=this->n_output) break;
        xt::view(this->buf_accum, i+1, 1) = xt::view(this->buf_accum, i+1, 1) + xt::view(mult1, 1);

        if(this->NR_MASTER_PORTS > 1){
            if(i+2>=this->n_output) break;
            xt::view(this->buf_accum, i+2, 1) = xt::view(this->buf_accum, i+2, 1) + xt::view(mult2, 1);
        
            if(i+3>=this->n_output) break;
            xt::view(this->buf_accum, i+3, 1) = xt::view(this->buf_accum, i+3, 1) + xt::view(mult3, 1);
        }

    }

    return 0;
}


void Rnnacc_v1::setup_streamer_matmul_h() {
    
    int base_addr = this->addr_wh;
    int word_stride = 0;
    int line_length = 0;
    int tiles_total = 0;

    int bandwidth_words = 2*this->NR_MASTER_PORTS;

    // NO streamer tiles
    this->n_w_o_rest  = this->n_output % bandwidth_words;
    this->n_w_o_tiles = int(this->n_output / bandwidth_words);

    if(this->n_w_o_rest > 0){
        this->n_w_o_tiles += 1;
    } 

    int stride_inp = 2*bandwidth_words;
    int stride_out = 2*this->n_output_external;

    this->n_w_h_rest  = this->n_output % bandwidth_words;
    this->n_w_h_tiles = int(this->n_hidden / (1));

    // set counter to 0
    this->w_o_idx = 0;
    this->w_h_idx = 0;

    // set line length
    line_length = this->n_output * this->n_output;
    tiles_total = this->n_w_h_tiles*this->n_w_o_tiles;

    this->vl_w = RnnaccVectorLoad<uint8_t>(
        this,
        base_addr,         // base_addr
        2*line_length,     // tot_length
        stride_inp,        // d0_stride
        this->n_w_o_tiles, // d0_length
        stride_out,        // d1_stride
        this->n_w_h_tiles, // d1_length
        0,                 // d2_stride
        this->debug_streamer // debug
    );

    this->trace.msg(vp::trace::LEVEL_DEBUG, "STREAMER - vl_w created\n");
}

int Rnnacc_v1::load_wh_cycle() {
    int64_t cycles = 0;

    xt::xarray<uint8_t> tcdm_data = xt::zeros<uint8_t>({this->NR_MASTER_PORTS*4});

    auto width = 0;
    if((this->w_o_idx == this->n_w_o_tiles-1) && (this->n_w_h_rest > 0))
    {
        width = 2*this->n_w_h_rest;
    } else {
        width = 4*this->NR_MASTER_PORTS;
    }
    tcdm_data = this->vl_w.execute(width, cycles, 4);

    this->buf_w = xt::zeros<int8_t>({ 2*this->NR_MASTER_PORTS, 1});

    for (auto i=0; i<this->NR_MASTER_PORTS*2; i+=2) {
        if((i%this->NR_MASTER_PORTS*2)>=width) {
            break;
        }
        xt::view(this->buf_w, i, 1) = 
            ((xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+0)) << 0 ) |
             (xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+1)) << 8 ));
        xt::view(this->buf_w, i+1, 1) =
            ((xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+2)) << 0) |
             (xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+3)) << 8));
    }

    return (int) cycles;
}


bool Rnnacc_v1::wh_exit_idx() {
    if((this->w_o_idx == this->n_w_o_tiles-1) & (this->w_h_idx == this->n_w_h_tiles-1)) {
        return true;
    }
    else {
        return false;
    }
}

void Rnnacc_v1::wh_update_idx() {
    if(this->w_o_idx == this->n_w_o_tiles-1) {
        if(this->w_h_idx < this->n_w_h_tiles-1) {
            this->w_h_idx++;
            this->w_o_idx = 0;
        }
    } else if(this->w_o_idx < this->n_w_o_tiles-1) {
      this->w_o_idx++;
    }
}


int Rnnacc_v1::perform_matmul_h() {

    xt::xarray<int32_t> mult0 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> mult1 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> mult2 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> mult3 = xt::zeros<int32_t>({1}); 

    xt::xarray<int32_t> acc0 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> acc1 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> acc2 = xt::zeros<int32_t>({1}); 
    xt::xarray<int32_t> acc3 = xt::zeros<int32_t>({1}); 

    for (auto i=this->w_o_idx*2*this->NR_MASTER_PORTS; i<(this->w_o_idx+1)*2*this->NR_MASTER_PORTS; i+=4) {

        // printf("index i: %d", i);
        // Multiply X0 with two new weights W0 W1
        mult0 = xt::view(this->buf_h, this->w_h_idx) * xt::view(this->buf_w, ((i/4)*4+0)%(this->NR_MASTER_PORTS*2), 1);
        mult1 = xt::view(this->buf_h, this->w_h_idx) * xt::view(this->buf_w, ((i/4)*4+1)%(this->NR_MASTER_PORTS*2), 1);

        // Multiply X0 with two new weights W2 W3
        if(this->NR_MASTER_PORTS > 1){
            mult2 = xt::view(this->buf_h, this->w_h_idx) * xt::view(this->buf_w, ((i/4)*4+2)%(this->NR_MASTER_PORTS*2), 1);
            mult3 = xt::view(this->buf_h, this->w_h_idx) * xt::view(this->buf_w, ((i/4)*4+3)%(this->NR_MASTER_PORTS*2), 1);
        }

        // debug print
        if(this->matmul_traces) {
            std::cout << "[MULT] index i=" << std::dec << std::setw(16) << i << std::dec << " n_output=" << this->n_output << std::endl;
            if(this->debug_hex_format){
                std::cout << "[MULT] mult0="<< std::hex << std::setw(16) << mult0 << " x=" << xt::view(this->buf_h, this->w_h_idx) << " w=" << xt::view(this->buf_w, ((i/4)*4+0)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult1="<< std::hex << std::setw(16) << mult1 << " x=" << xt::view(this->buf_h, this->w_h_idx) << " w=" << xt::view(this->buf_w, ((i/4)*4+1)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult2="<< std::hex << std::setw(16) << mult2 << " x=" << xt::view(this->buf_h, this->w_h_idx) << " w=" << xt::view(this->buf_w, ((i/4)*4+2)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult3="<< std::hex << std::setw(16) << mult3 << " x=" << xt::view(this->buf_h, this->w_h_idx) << " w=" << xt::view(this->buf_w, ((i/4)*4+3)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;

            } else {
                std::cout << "[MULT] mult0="<< std::dec << std::setw(16) << mult0 << " x=" << xt::view(this->buf_h, this->w_h_idx) << " w=" << xt::view(this->buf_w, ((i/2)*4+0)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;            
                std::cout << "[MULT] mult1="<< std::dec << std::setw(16) << mult1 << " x=" << xt::view(this->buf_h, this->w_h_idx) << " w=" << xt::view(this->buf_w, ((i/2)*4+1)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult2="<< std::dec << std::setw(16) << mult2 << " x=" << xt::view(this->buf_h, this->w_h_idx) << " w=" << xt::view(this->buf_w, ((i/2)*4+2)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult3="<< std::dec << std::setw(16) << mult3 << " x=" << xt::view(this->buf_h, this->w_h_idx) << " w=" << xt::view(this->buf_w, ((i/2)*4+3)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
            }
        }

        if(i+0>=this->n_output) break;
        xt::view(this->buf_accum, i+0, 1) = xt::view(this->buf_accum, i+0, 1) + xt::view(mult0, 1);
        // std::cout << "[MULT] index i=" << std::dec << std::setw(16) << i << std::dec << " n_output=" << this->n_output << std::endl;

        if(i+1>=this->n_output) break;
        xt::view(this->buf_accum, i+1, 1) = xt::view(this->buf_accum, i+1, 1) + xt::view(mult1, 1);
        // std::cout << "[MULT] index i=" << std::dec << std::setw(16) << i << std::dec << " n_output=" << this->n_output << std::endl;

        if(this->NR_MASTER_PORTS > 1){
            if(i+2>=this->n_output) break;
            xt::view(this->buf_accum, i+2, 1) = xt::view(this->buf_accum, i+2, 1) + xt::view(mult2, 1);
            // std::cout << "[MULT] index i=" << std::dec << std::setw(16) << i << std::dec << " n_output=" << this->n_output << std::endl;

            if(i+3>=this->n_output) break;
            xt::view(this->buf_accum, i+3, 1) = xt::view(this->buf_accum, i+3, 1) + xt::view(mult3, 1);
            // std::cout << "[MULT] index i=" << std::dec << std::setw(16) << i << std::dec << " n_output=" << this->n_output << std::endl;
        }
    }

    return 0;
}