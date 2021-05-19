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

    // define tile numbers
    this->n_w_o_rest  = this->n_output % (this->NR_MASTER_PORTS); 
    this->n_w_o_tiles = int(this->n_output / (this->NR_MASTER_PORTS)); 

    this->n_w_i_rest  = this->n_input % (2); 
    this->n_w_i_tiles = int(this->n_input / (2)); 

    if(this->n_w_o_rest > 0){
        this->n_w_o_tiles += 1;
    }
    if(this->n_w_i_rest > 0){
        this->n_w_i_tiles += 1;
    }

    // set counter to 0
    this->w_o_idx = 0;
    this->w_i_idx = 0;

    // set line length
    line_length = this->n_output;
    tiles_total = this->n_w_i_tiles*this->n_w_o_tiles;

    // create streamer
    this->vl_w = RnnaccVectorLoad<uint8_t>(
        this,

        base_addr,         // base_addr
        2*line_length,     // tot_length
        2*this->n_input_external*this->NR_MASTER_PORTS, // d0_stride
        this->n_w_o_tiles, // d0_length
        4,                 // d1_stride
        this->n_w_i_tiles, // d1_length
        0,                 // d2_stride
        true               // debug
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
    tcdm_data = this->vl_w.execute(width, cycles, this->n_input_external*2);

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


    for (auto i=this->w_o_idx*this->NR_MASTER_PORTS; i<(this->w_o_idx+1)*this->NR_MASTER_PORTS; i+=2) {

        if(i>=this->n_output){
            break;
            // this->trace.msg(vp::trace::LEVEL_DEBUG, "BREAK\n");
            // this->trace.msg(vp::trace::LEVEL_DEBUG, "i+1: %d tcdm: %d\n", i+1,(i/2-this->n_x_idx*this->NR_MASTER_PORTS)*4+3);
        }

        // debug print
        // if(this->matmul_traces) {

        //     this->trace.msg(vp::trace::LEVEL_DEBUG, "i:   %d buf_w: %d\n", i+0, ((i/2)*4+0)%(this->NR_MASTER_PORTS*2));
        //     this->trace.msg(vp::trace::LEVEL_DEBUG, "i:   %d buf_w: %d\n", i+0, ((i/2)*4+1)%(this->NR_MASTER_PORTS*2));
        //     this->trace.msg(vp::trace::LEVEL_DEBUG, "i+1: %d buf_w: %d\n", i+1, ((i/2)*4+2)%(this->NR_MASTER_PORTS*2));
        //     this->trace.msg(vp::trace::LEVEL_DEBUG, "i+1: %d buf_w: %d\n", i+1, ((i/2)*4+3)%(this->NR_MASTER_PORTS*2));
        // }

        // Multiply X0 and X1 with two new weights
        mult0 = xt::view(this->buf_x, 2*this->w_i_idx+0) * xt::view(this->buf_w, ((i/2)*4+0)%(this->NR_MASTER_PORTS*2), 1);
        mult1 = xt::view(this->buf_x, 2*this->w_i_idx+1) * xt::view(this->buf_w, ((i/2)*4+1)%(this->NR_MASTER_PORTS*2), 1);

        // debug print
        if(this->matmul_traces) {
            if(this->debug_hex_format){
                std::cout << "[MULT] mult0="<< std::hex << std::setw(16) << mult0 << " x=" << xt::view(this->buf_x, 2*this->w_i_idx+0) << " w=" << xt::view(this->buf_w, ((i/2)*4+0)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult1="<< std::hex << std::setw(16) << mult1 << " x=" << xt::view(this->buf_x, 2*this->w_i_idx+1) << " w=" << xt::view(this->buf_w, ((i/2)*4+1)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
            } else {
                std::cout << "[MULT] mult0="<< std::dec << std::setw(16) << mult0 << " x=" << xt::view(this->buf_x, 2*this->w_i_idx+0) << " w=" << xt::view(this->buf_w, ((i/2)*4+0)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;            
                std::cout << "[MULT] mult1="<< std::dec << std::setw(16) << mult1 << " x=" << xt::view(this->buf_x, 2*this->w_i_idx+1) << " w=" << xt::view(this->buf_w, ((i/2)*4+1)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
            }
        }

        // Multiply X0 and X1 with two new weights
        mult2 = xt::view(this->buf_x, 2*this->w_i_idx+0) * xt::view(this->buf_w, ((i/2)*4+2)%(this->NR_MASTER_PORTS*2), 1);
        mult3 = xt::view(this->buf_x, 2*this->w_i_idx+1) * xt::view(this->buf_w, ((i/2)*4+3)%(this->NR_MASTER_PORTS*2), 1);

        // debug print
        if(this->matmul_traces) {
            if(this->debug_hex_format){
                std::cout << "[MULT] mult2="<< std::hex << std::setw(16) << mult2 << " x=" << xt::view(this->buf_x, 2*this->w_i_idx+0) << " w=" << xt::view(this->buf_w, ((i/2)*4+2)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult3="<< std::hex << std::setw(16) << mult3 << " x=" << xt::view(this->buf_x, 2*this->w_i_idx+1) << " w=" << xt::view(this->buf_w, ((i/2)*4+3)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
            } else {
                std::cout << "[MULT] mult2="<< std::dec << std::setw(16) << mult2 << " x=" << xt::view(this->buf_x, 2*this->w_i_idx+0) << " w=" << xt::view(this->buf_w, ((i/2)*4+2)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult3="<< std::dec << std::setw(16) << mult3 << " x=" << xt::view(this->buf_x, 2*this->w_i_idx+1) << " w=" << xt::view(this->buf_w, ((i/2)*4+3)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
            }
        }

        // Accumulate first buf_accum reg with the multipications
        xt::view(acc0, 1) = xt::view(mult0, 1) + xt::view(mult1, 1);
        xt::view(acc1, 1) = xt::view(this->buf_accum, i+0, 1) + xt::view(acc0, 1);

        // debug print
        if(this->matmul_traces) {
            if(this->debug_hex_format){
                std::cout << "[MULT] acc0="<< std::hex << std::setw(16) << acc0 << std::dec << std::endl;
                std::cout << "[MULT] acc1="<< std::hex << std::setw(16) << acc1 << std::dec << std::endl;
            } else {
                std::cout << "[MULT] acc0="<< std::dec << std::setw(16) << acc0 << std::dec << std::endl;
                std::cout << "[MULT] acc1="<< std::dec << std::setw(16) << acc1 << std::dec << std::endl;
            }
        }

        // Accumulate second buf_accum reg with the multipications
        xt::view(acc2, 1) = xt::view(mult2, 1) + xt::view(mult3, 1);
        xt::view(acc3, 1) = xt::view(this->buf_accum, i+1, 1) + xt::view(acc2, 1);

        // debug print
        if(this->matmul_traces) {
            if(this->debug_hex_format){
                std::cout << "[MULT] acc2="<< std::hex << std::setw(16) << acc2<< std::dec << std::endl;
                std::cout << "[MULT] acc3="<< std::hex << std::setw(16) << acc3<< std::dec << std::endl;
            } else {
                std::cout << "[MULT] acc2="<< std::dec << std::setw(16) << acc2<< std::dec << std::endl;
                std::cout << "[MULT] acc3="<< std::dec << std::setw(16) << acc3<< std::dec << std::endl;
            }
        }

        // write back to buf_accum regs
        xt::view(this->buf_accum, i+0, 1) = xt::cast<int32_t>(xt::view(acc1, 1));
        xt::view(this->buf_accum, i+1, 1) = xt::cast<int32_t>(xt::view(acc3, 1));

    }

    return 0;
}


void Rnnacc_v1::setup_streamer_matmul_h() {
    
    int base_addr = this->addr_wh;
    // int base_addr = this->addr_wx;
    int word_stride = 0;
    int line_length = 0;

    // define tile numbers
    this->n_w_o_rest  = this->n_output % (this->NR_MASTER_PORTS); 
    this->n_w_o_tiles = int(this->n_output / (this->NR_MASTER_PORTS)); 

    this->n_w_h_rest  = this->n_output % (2); 
    this->n_w_h_tiles = int(this->n_output /(2)); 

    if(this->n_w_o_rest > 0){
        this->n_w_o_tiles += 1;
    }
    if(this->n_w_h_rest > 0){
        this->n_w_h_tiles += 1;
    }

    // set counter to 0
    this->w_o_idx = 0;
    this->w_h_idx = 0;

    // set line length
    line_length = this->n_output;
    int tiles_total = this->n_w_h_tiles*this->n_w_o_tiles;

    // int line_length2 = 4*this->NR_MASTER_PORTS;
    // int line_length2 = 2*this->NR_MASTER_PORTS*this->n_input;

    // create streamer
    this->vl_w = RnnaccVectorLoad<uint8_t>(
        this,

        base_addr,         // base_addr
        2*line_length,     // tot_length -> 2byte * n_output
        2*this->n_output_external*this->NR_MASTER_PORTS, // d0_stride
        this->n_w_o_tiles, // d0_length
        4,                 // d1_stride
        this->n_w_h_tiles, // d1_length
        0,                 // d2_stride
        true               // debug

        // WORKING WITHOUT TILING
        // base_addr,         // base_addr
        // 2*line_length,     // tot_length -> 2byte * n_output
        // 2*this->n_input*this->NR_MASTER_PORTS, //4*this->NR_MASTER_PORTS, //2*line_length,   // d0_stride
        // this->n_w_o_tiles, // d0_length
        // 4,                 // d1_stride
        // this->n_w_h_tiles, // d1_length
        // 0,                 // d2_stride
        // true               // debug
    );

    this->trace.msg(vp::trace::LEVEL_DEBUG, "STREAMER - vl_w created\n");

}

int Rnnacc_v1::load_wh_cycle() {
    int64_t cycles = 0;

    xt::xarray<uint8_t> tcdm_data = xt::zeros<uint8_t>({this->NR_MASTER_PORTS*4});

    auto width = 0;
    if((this->w_o_idx == this->n_w_o_tiles-1) && (this->n_w_o_rest > 0))
    {
        width = 4*this->n_w_o_rest;
    } else {
        width = 4*this->NR_MASTER_PORTS;
    }
    // tcdm_data = this->vl_w.execute(width, cycles, 4);
    tcdm_data = this->vl_w.execute(width, cycles, this->n_output_external*2);

    // std::ostringstream stringStream;
    // xt::print_options::set_line_width(1000);
    // stringStream << "Read data: " << std::hex << tcdm_data << std::dec << "\n";
    // string s = stringStream.str();
    // this->trace.msg(vp::trace::LEVEL_DEBUG, s.c_str());

    this->buf_w = xt::zeros<int8_t>({ 2*this->NR_MASTER_PORTS, 1});

    for (auto i=0; i<this->NR_MASTER_PORTS*2; i+=2) {
        // if(i>=2*this->n_output){
        if((i%this->NR_MASTER_PORTS*2)>=width){
            // xt::view(this->buf_w, i, 1) = 0;
            // xt::view(this->buf_w, i+1, 1) = 0;
            break;
            // this->trace.msg(vp::trace::LEVEL_DEBUG, "BREAK\n");
            // this->trace.msg(vp::trace::LEVEL_DEBUG, "i+1: %d tcdm: %d\n", i+1,(i/2-this->n_x_idx*this->NR_MASTER_PORTS)*4+3);
        }
        xt::view(this->buf_w, i, 1) = 
            ((xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+0)) << 0 ) |
             (xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+1)) << 8 ));
        xt::view(this->buf_w, i+1, 1) =
            ((xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+2)) << 0) |
             (xt::cast<int32_t>(xt::view(tcdm_data, (i/2)*4+3)) << 8));
        // xt::view(this->buf_w, i, 1) = 
        //     ((xt::cast<int32_t>(xt::view(tcdm_data, (i/2-this->w_o_idx*this->NR_MASTER_PORTS)*4+0)) << 0 ) |
        //      (xt::cast<int32_t>(xt::view(tcdm_data, (i/2-this->w_o_idx*this->NR_MASTER_PORTS)*4+1)) << 8 ));
        // xt::view(this->buf_w, i+1, 1) =
        //     ((xt::cast<int32_t>(xt::view(tcdm_data, (i/2-this->w_o_idx*this->NR_MASTER_PORTS)*4+2)) << 0) |
        //      (xt::cast<int32_t>(xt::view(tcdm_data, (i/2-this->w_o_idx*this->NR_MASTER_PORTS)*4+3)) << 8));
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


    for (auto i=this->w_o_idx*this->NR_MASTER_PORTS; i<(this->w_o_idx+1)*this->NR_MASTER_PORTS; i+=2) {

        if(i>=this->n_output){
            break;
            // this->trace.msg(vp::trace::LEVEL_DEBUG, "BREAK\n");
            // this->trace.msg(vp::trace::LEVEL_DEBUG, "i+1: %d tcdm: %d\n", i+1,(i/2-this->n_x_idx*this->NR_MASTER_PORTS)*4+3);
        }

        // debug print
        // if(this->matmul_traces) {

        //     this->trace.msg(vp::trace::LEVEL_DEBUG, "i:   %d buf_w: %d\n", i+0, ((i/2)*4+0)%(this->NR_MASTER_PORTS*2));
        //     this->trace.msg(vp::trace::LEVEL_DEBUG, "i:   %d buf_w: %d\n", i+0, ((i/2)*4+1)%(this->NR_MASTER_PORTS*2));
        //     this->trace.msg(vp::trace::LEVEL_DEBUG, "i+1: %d buf_w: %d\n", i+1, ((i/2)*4+2)%(this->NR_MASTER_PORTS*2));
        //     this->trace.msg(vp::trace::LEVEL_DEBUG, "i+1: %d buf_w: %d\n", i+1, ((i/2)*4+3)%(this->NR_MASTER_PORTS*2));
        // }

        // Multiply H0 and H1 with two new weights WH0, WH1
        mult0 = xt::view(this->buf_h, 2*this->w_h_idx+0) * xt::view(this->buf_w, ((i/2)*4+0)%(this->NR_MASTER_PORTS*2), 1);
        mult1 = xt::view(this->buf_h, 2*this->w_h_idx+1) * xt::view(this->buf_w, ((i/2)*4+1)%(this->NR_MASTER_PORTS*2), 1);

        // Multiply H0 and H1 with two new weights WH2, WH3
        mult2 = xt::view(this->buf_h, 2*this->w_h_idx+0) * xt::view(this->buf_w, ((i/2)*4+2)%(this->NR_MASTER_PORTS*2), 1);
        mult3 = xt::view(this->buf_h, 2*this->w_h_idx+1) * xt::view(this->buf_w, ((i/2)*4+3)%(this->NR_MASTER_PORTS*2), 1);

        // debug print
        if(this->matmul_traces) {
            if(this->debug_hex_format){
                std::cout << "[MULT] mult0="<< std::hex << std::setw(16) << mult0 << " x=" << xt::view(this->buf_h, 2*this->w_h_idx+0) << " w=" << xt::view(this->buf_w, ((i/2)*4+0)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult1="<< std::hex << std::setw(16) << mult1 << " x=" << xt::view(this->buf_h, 2*this->w_h_idx+1) << " w=" << xt::view(this->buf_w, ((i/2)*4+1)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
            } else {
                std::cout << "[MULT] mult0="<< std::dec << std::setw(16) << mult0 << " x=" << xt::view(this->buf_h, 2*this->w_h_idx+0) << " w=" << xt::view(this->buf_w, ((i/2)*4+0)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;            
                std::cout << "[MULT] mult1="<< std::dec << std::setw(16) << mult1 << " x=" << xt::view(this->buf_h, 2*this->w_h_idx+1) << " w=" << xt::view(this->buf_w, ((i/2)*4+1)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
            }
        }

        // debug print
        if(this->matmul_traces) {
            if(this->debug_hex_format){
                std::cout << "[MULT] mult2="<< std::hex << std::setw(16) << mult2 << " x=" << xt::view(this->buf_h, 2*this->w_h_idx+0) << " w=" << xt::view(this->buf_w, ((i/2)*4+2)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult3="<< std::hex << std::setw(16) << mult3 << " x=" << xt::view(this->buf_h, 2*this->w_h_idx+1) << " w=" << xt::view(this->buf_w, ((i/2)*4+3)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
            } else {
                std::cout << "[MULT] mult2="<< std::dec << std::setw(16) << mult2 << " x=" << xt::view(this->buf_h, 2*this->w_h_idx+0) << " w=" << xt::view(this->buf_w, ((i/2)*4+2)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
                std::cout << "[MULT] mult3="<< std::dec << std::setw(16) << mult3 << " x=" << xt::view(this->buf_h, 2*this->w_h_idx+1) << " w=" << xt::view(this->buf_w, ((i/2)*4+3)%(this->NR_MASTER_PORTS*2), 1) << std::dec << std::endl;
            }
        }

        // Accumulate first buf_accum reg with the multipications
        xt::view(acc0, 1) = xt::view(mult0, 1) + xt::view(mult1, 1);
        xt::view(acc1, 1) = xt::view(this->buf_accum, i+0, 1) + xt::view(acc0, 1);

        // debug print
        if(this->matmul_traces) {
            if(this->debug_hex_format){
                std::cout << "[MULT] acc0="<< std::hex << std::setw(16) << acc0 << std::dec << std::endl;
                std::cout << "[MULT] acc1="<< std::hex << std::setw(16) << acc1 << std::dec << std::endl;
            } else {
                std::cout << "[MULT] acc0="<< std::dec << std::setw(16) << acc0 << std::dec << std::endl;
                std::cout << "[MULT] acc1="<< std::dec << std::setw(16) << acc1 << std::dec << std::endl;
            }
        }

        // Accumulate second buf_accum reg with the multipications
        xt::view(acc2, 1) = xt::view(mult2, 1) + xt::view(mult3, 1);
        xt::view(acc3, 1) = xt::view(this->buf_accum, i+1, 1) + xt::view(acc2, 1);

        // debug print
        if(this->matmul_traces) {
            if(this->debug_hex_format){
                std::cout << "[MULT] acc2="<< std::hex << std::setw(16) << acc2<< std::dec << std::endl;
                std::cout << "[MULT] acc3="<< std::hex << std::setw(16) << acc3<< std::dec << std::endl;
            } else {
                std::cout << "[MULT] acc2="<< std::dec << std::setw(16) << acc2<< std::dec << std::endl;
                std::cout << "[MULT] acc3="<< std::dec << std::setw(16) << acc3<< std::dec << std::endl;
            }
        }

        // write back to buf_accum regs
        xt::view(this->buf_accum, i+0, 1) = xt::cast<int32_t>(xt::view(acc1, 1));
        xt::view(this->buf_accum, i+1, 1) = xt::cast<int32_t>(xt::view(acc3, 1));

    }

    return 0;
}