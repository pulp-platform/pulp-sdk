/*
 * Copyright (C) 2020-2022  GreenWaves Technologies, ETH Zurich, University of Bologna
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
 * Authors: Francesco Conti, University of Bologna & GreenWaves Technologies (f.conti@unibo.it)
 *          Arpan Suravi Prasad, ETH Zurich (prasadar@iis.ee.ethz.ch)
 */

#include <neureka.hpp>

void Neureka::debug_x_buffer() {
    std::ostringstream stringStream;
    stringStream << "x_buffer[8,8,32] = \n" << (this->trace_format?std::hex:std::dec) << std::setw(2) << xt::view(this->x_buffer,xt::range(0,2),xt::range(0,2),xt::range(0,1)) << std::dec << "\n";
    // stringStream << "x_buffer[8,8,32] = \n" << (this->trace_format?std::hex:std::dec) << std::setw(2) << xt::view(this->x_buffer,xt::all(),xt::all(),xt::all()) << std::dec << "\n";
    std::string copyOfStr = stringStream.str();
    this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr.c_str());
}

void Neureka::debug_x_array() {
    std::ostringstream stringStream;
    stringStream << "x_array[36,9,32] = \n" << xt::print_options::threshold(10000) << (this->trace_format?std::hex:std::dec) << std::setw(2) << xt::view(this->x_array,xt::range(0,2),xt::all(),xt::range(0,1)) << std::dec << "\n";
    std::string copyOfStr = stringStream.str();
    this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr.c_str());

    stringStream << "x_array[36,9,32] = \n" << xt::print_options::threshold(10000) << (this->trace_format?std::hex:std::dec) << std::setw(2) << xt::view(this->x_array,xt::range(6,8),xt::all(),xt::range(0,1)) << std::dec << "\n";
    std::string copyOfStr2 = stringStream.str();
    this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr2.c_str());
}

void Neureka::debug_accum(){
  std::ostringstream stringStream;
  stringStream << "accum[32,36] = \n" << (this->trace_format?std::hex:std::dec) << std::setw(8) << xt::view(this->accum,xt::range(0,1),xt::range(0,2)) << std::dec << "\n";
  std::string copyOfStr = stringStream.str();
  this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr.c_str());
  stringStream << "accum[32,36] = \n" << (this->trace_format?std::hex:std::dec) << std::setw(8) << xt::view(this->accum,xt::range(0,1),xt::range(6,8)) << std::dec << "\n";
  std::string copyOfStr2 = stringStream.str();
  this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr2.c_str());
}

void Neureka::debug_psum_block(){
  std::ostringstream stringStream;
  stringStream << "psum_block[36,9] = \n" << (this->trace_format?std::hex:std::dec) << std::setw(8) << xt::cast<int32_t>(this->psum_block) << std::dec << "\n";
  std::string copyOfStr = stringStream.str();
  this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr.c_str());
}

