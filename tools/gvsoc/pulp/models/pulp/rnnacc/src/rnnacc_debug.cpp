/*
 * Copyright (C) 2020  GreenWaves Technologies, SAS
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
 *          Francesco Conti, University of Bologna & GreenWaves Technologies (f.conti@unibo.it) */

#include "../rnnacc_v1_impl.hpp"

void Rnnacc_v1::debug_buf_accum(){
    std::ostringstream stringStream;
    if(this->debug_hex_format){
        stringStream << "buf_accum[" << this->NR_REGS_ACCUM << ",1] = \n" << std::hex << std::setw(32) << xt::cast<int32_t>(this->buf_accum) << std::dec << "\n";
    }
    else {
        stringStream << "buf_accum[" << this->NR_REGS_ACCUM << ",1] = \n" << std::dec << std::setw(32) << xt::cast<int32_t>(this->buf_accum) << std::dec << "\n";
    }
    std::string copyOfStr = stringStream.str();
    this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr.c_str());
}

void Rnnacc_v1::debug_buf_x(){
    std::ostringstream stringStream;
    if(this->debug_hex_format){
        stringStream << "buf_x[" << this->NR_REGS_X << ",1] = \n" << std::hex << std::setw(16) << xt::cast<int32_t>(this->buf_x) << std::dec << "\n";
    }
    else {
        stringStream << "buf_x[" << this->NR_REGS_X << ",1] = \n" << std::dec << std::setw(16) << xt::cast<int32_t>(this->buf_x) << std::dec << "\n";
    }
    std::string copyOfStr = stringStream.str();
    this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr.c_str());
}

void Rnnacc_v1::debug_buf_h(){
    std::ostringstream stringStream;
    if(this->debug_hex_format){
        stringStream << "buf_h[" << this->NR_REGS_H << ",1] = \n" << std::hex << std::setw(16) << xt::cast<int32_t>(this->buf_h) << std::dec << "\n";
    }
    else {
        stringStream << "buf_h[" << this->NR_REGS_H << ",1] = \n" << std::dec << std::setw(16) << xt::cast<int32_t>(this->buf_h) << std::dec << "\n";
    }
    std::string copyOfStr = stringStream.str();
    this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr.c_str());
}

void Rnnacc_v1::debug_buf_w(){
    std::ostringstream stringStream;
    if(this->debug_hex_format){
        stringStream << "buf_w[" << 2*this->NR_MASTER_PORTS << ",1] = \n" << std::hex << std::setw(16) << xt::cast<int32_t>(this->buf_w) << std::dec << "\n";
    }
    else {
        stringStream << "buf_w[" << 2*this->NR_MASTER_PORTS << ",1] = \n" << std::dec << std::setw(16) << xt::cast<int32_t>(this->buf_w) << std::dec << "\n";
    }
    std::string copyOfStr = stringStream.str();
    this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr.c_str());
}
