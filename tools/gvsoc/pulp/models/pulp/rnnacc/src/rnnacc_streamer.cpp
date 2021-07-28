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

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xrandom.hpp"
#include "xtensor/xadapt.hpp"
#include "xtensor/xpad.hpp"
#include "../rnnacc_v1_impl.hpp"

RnnaccStreamAccess::RnnaccStreamAccess(
  Rnnacc_v1 *rnnacc,
  int base_addr,
  int d0_length,
  int d0_stride,
  int d1_length,
  int d1_stride,
  int d2_length,
  int d2_stride,
  bool debug
) : rnnacc       ( rnnacc    ),
    base_addr    ( base_addr ),
    d0_length    ( d0_length ),
    d0_stride    ( d0_stride ),
    d1_length    ( d1_length ),
    d1_stride    ( d1_stride ),
    d2_length    ( d2_length ),
    d2_stride    ( d2_stride ),
    debug        ( debug     ),
    current_addr ( 0         )
{
  this->reset_iteration();
  if(this->debug) {
    this->print_config();
  }
}

int RnnaccStreamAccess::get_base_addr() {
  return this->base_addr;
}
int RnnaccStreamAccess::get_d0_length() {
  return this->d0_length;
}
int RnnaccStreamAccess::get_d0_stride() {
  return this->d0_stride;
}
int RnnaccStreamAccess::get_d1_length() {
  return this->d1_length;
}
int RnnaccStreamAccess::get_d1_stride() {
  return this->d1_stride;
}
int RnnaccStreamAccess::get_d2_length() {
  return this->d2_length;
}
int RnnaccStreamAccess::get_d2_stride() {
  return this->d2_stride;
}

void RnnaccStreamAccess::print_config() {
  std::cout << "[STREAMER] base_addr="  << std::hex << this->base_addr << std::dec << std::endl;
  std::cout << "[STREAMER] tot_length=" << this->d0_length << std::endl;
  std::cout << "[STREAMER] d0_stride="  << this->d0_stride << std::endl;
  std::cout << "[STREAMER] d0_length="  << this->d1_length << std::endl;
  std::cout << "[STREAMER] d1_stride="  << this->d1_stride << std::endl;
  std::cout << "[STREAMER] d1_length="  << this->d2_length << std::endl;
  std::cout << "[STREAMER] d2_stride="  << this->d2_stride << std::endl;
}

void RnnaccStreamAccess::reset_iteration() {
  this->wa = 0;
  this->la = 0;
  this->ba = 0;
  this->wc = 1;
  this->lc = 1;
  this->bc = 1;
  this->oc = 0;
}

int RnnaccStreamAccess::iterate() {
  if (this->d1_length < 0) {
    this->current_addr = this->base_addr + this->wa;
    if(this->debug) {
      std::cout << "[STREAMER] current_addr0=" << std::hex << this->current_addr << std::dec  << std::endl;
    }
  }
  else if(this->d2_length < 0) {
    this->current_addr = this->base_addr + this->la + this->wa;
    if(this->debug) {
      std::cout << "[STREAMER] current_addr1=" << std::hex << this->current_addr << std::dec  << std::endl;
    }
  }
  else {
    this->current_addr = this->base_addr + this->ba + this->la + this->wa;
    if(this->debug) {
      std::cout << "[STREAMER] current_addr2="<< std::hex << this->current_addr << std::dec << std::endl;
    }
  }
  this->oc++;
  if(this->debug) {
    std::cout << "[STREAMER] wa=" << this->wa << " la=" << this->la << " ba=" << this->ba << " oc=" << this->oc << std::endl;
    std::cout << "[STREAMER] wc=" << this->wc << " lc=" << this->lc << " bc=" << this->bc << " oc=" << this->oc << std::endl;
  }
  if((this->wc < this->d1_length) || (this->d1_length < 0)) {
    this->wa += this->d0_stride;
    this->wc += 1;
    // std::cout << "[STREAMER] 0" << std::endl;
  }
  else if ((this->lc < this->d2_length) || (this->d2_length < 0)) {
    this->wa = 0;
    this->la += this->d1_stride;
    this->wc = 1;
    this->lc += 1;
    // std::cout << "[STREAMER] 1" << std::endl;
  }
  else {
    this->wa = 0;
    this->la = 0;
    this->ba += this->d2_stride;
    this->wc = 1;
    this->lc = 1;
    this->bc += 1;
    // std::cout << "[STREAMER] 2" << std::endl;
  }

  // std::cout << "[STREAMER] iterate done Bla" << std::endl;
  return this->current_addr;
}

template <class T>
RnnaccVectorLoad<T>::RnnaccVectorLoad(
  Rnnacc_v1 *rnnacc,
  int base_addr,
  int d0_length,
  int d0_stride,
  int d1_length,
  int d1_stride,
  int d2_length,
  int d2_stride,
  bool debug
) : RnnaccStreamAccess(rnnacc, base_addr, d0_length, d0_stride, d1_length, d1_stride, d2_length, d2_stride, debug) {
}

template <class T>
RnnaccVectorLoad<T>::RnnaccVectorLoad() : RnnaccStreamAccess((Rnnacc_v1 *) NULL, 0, 0, 0, 0, 0, 0, 0, false) {
}

template <class T>
xt::xarray<T> RnnaccVectorLoad<T>::execute(int width, int64_t& cycles, int stride) {

  this->rnnacc->trace.msg(vp::trace::LEVEL_DEBUG, "[VectorLoad] width %d %d\n", width, width*sizeof(T));

  auto addr = this->iterate();
  uint8_t load_data[this->rnnacc->STREAM_MAX_WIDTH_BYTES];
  auto width_padded = width + 4;
  auto addr_padded = addr & ~0x3;

  int64_t max_latency = 0;
  // iterate over all output ports
  for(auto i=0; i<this->rnnacc->NR_MASTER_PORTS; i++) {
    
    this->rnnacc->tcdm_reqs[i].init();
    this->rnnacc->tcdm_reqs[i].set_addr(addr_padded+i*stride & 0x0fffffff);
    this->rnnacc->tcdm_reqs[i].set_size(4);
    this->rnnacc->tcdm_reqs[i].set_data(load_data+i*4);
    this->rnnacc->tcdm_reqs[i].set_is_write(false);

    int err = this->rnnacc->tcdm_ports[i].req(&this->rnnacc->tcdm_reqs[i]);
    
    if (err == vp::IO_REQ_OK) {
      int64_t latency = this->rnnacc->tcdm_reqs[i].get_latency();
      if (latency > max_latency) {
        max_latency = latency;
      }
    }
    else {
      this->rnnacc->trace.fatal("[VectorLoad] Unsupported asynchronous reply\n");
    }
    this->rnnacc->trace.msg(vp::trace::LEVEL_DEBUG, "[VectorLoad] tcdm_reqs[%d] LD @%08x ==> %08x\n", i, addr_padded+i*4 & 0x0fffffff, *(uint32_t *)(load_data+i*4));
  }

  std::ostringstream stringStream;
  this->rnnacc->trace.msg(vp::trace::LEVEL_DEBUG, "[VectorLoad] Issuing read request (addr=0x%08x, size=%dB, latency=%d)\n", addr & 0x0fffffff, width*sizeof(T), cycles+1);
  
  xt::xarray<T> x = xt::zeros<T>({width});
  for(auto i=0; i<width; i++) {
    xt::view(x, i) = *(T *)(load_data + (addr & 0x3) + i*sizeof(T));
  }
  xt::print_options::set_line_width(1000);
  stringStream << "[VectorLoad] Read data: " << std::hex << x << std::dec << "\n";
  string s = stringStream.str();
  this->rnnacc->trace.msg(vp::trace::LEVEL_DEBUG, s.c_str());
  cycles += max_latency + 1;

  return x;
}

template <class T>
RnnaccVectorStore<T>::RnnaccVectorStore(
  Rnnacc_v1 *rnnacc,
  int base_addr,
  int d0_length,
  int d0_stride,
  int d1_length,
  int d1_stride,
  int d2_length,
  int d2_stride,
  bool debug
) : RnnaccStreamAccess(rnnacc, base_addr, d0_length, d0_stride, d1_length, d1_stride, d2_length, d2_stride, debug) {
}

template <class T>
RnnaccVectorStore<T>::RnnaccVectorStore() : RnnaccStreamAccess((Rnnacc_v1 *) NULL, 0, 0, 0, 0, 0, 0, 0, false) {
}

template <class T>
xt::xarray<T> RnnaccVectorStore<T>::execute(xt::xarray<T> data, int width, int64_t& cycles, int32_t enable) {
  this->rnnacc->trace.msg(vp::trace::LEVEL_DEBUG, "[VectorStore] width %d %d\n", width, width*sizeof(T));
  auto addr = this->iterate();
  uint8_t store_data[this->rnnacc->STREAM_MAX_WIDTH_BYTES];
  for(auto i=0; i<this->rnnacc->STREAM_MAX_WIDTH_BYTES; i++) {
    store_data[i] = 0;
  }
  for(auto i=0; i<width; i++) {
    *(T *)(store_data + i*sizeof(T)) = data(i);
    this->rnnacc->trace.msg(vp::trace::LEVEL_DEBUG, " %d <== %08x\n", i, data(i));
  }
  auto width_bytes = width*sizeof(T);
  int64_t max_latency = 0;
  if(enable) {
    for(auto i=0; i<this->rnnacc->NR_MASTER_PORTS; i++) {
      if(width_bytes<=i*4){
        break;
      }

      this->rnnacc->tcdm_reqs[i].init();
      this->rnnacc->tcdm_reqs[i].set_addr(addr+i*4 & 0x0fffffff);
      this->rnnacc->tcdm_reqs[i].set_size(4);
      this->rnnacc->tcdm_reqs[i].set_data(store_data+i*4);
      this->rnnacc->tcdm_reqs[i].set_is_write(true);
      int err = this->rnnacc->tcdm_ports[i].req(&this->rnnacc->tcdm_reqs[i]);
      if (err == vp::IO_REQ_OK) {
        int64_t latency = this->rnnacc->tcdm_reqs[i].get_latency();
        if (latency > max_latency) {
          max_latency = latency;
        }
      }
      else {
        this->rnnacc->trace.fatal("Unsupported asynchronous reply\n");
      }
      this->rnnacc->trace.msg(vp::trace::LEVEL_DEBUG, " ST @%08x <== %08x\n", addr+i*4 & 0x0fffffff, *(uint32_t *)(store_data+i*4));
    }
  }
  std::ostringstream stringStream;
  this->rnnacc->trace.msg(vp::trace::LEVEL_DEBUG, "Issuing write request (addr=0x%08x, size=%dB, latency=%d)\n", addr & 0x0fffffff, width*sizeof(T), cycles+1);
  xt::print_options::set_line_width(1000);
  if(enable) {
    stringStream << "Write data: " << std::hex << data << std::dec << "\n";
    string s = stringStream.str();
    this->rnnacc->trace.msg(vp::trace::LEVEL_DEBUG, s.c_str());
  }
  else {
    stringStream << "Write disabled" << "\n";
    string s = stringStream.str();
    this->rnnacc->trace.msg(vp::trace::LEVEL_DEBUG, s.c_str());
  }
  cycles += max_latency + 1;
  return data;
}

// template instantiations
template class RnnaccVectorLoad<uint8_t>;
template class RnnaccVectorStore<uint8_t>;
