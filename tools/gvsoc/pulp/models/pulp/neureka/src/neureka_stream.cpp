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

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xrandom.hpp"
#include "xtensor/xadapt.hpp"
#include "xtensor/xpad.hpp"
#include <neureka.hpp>

// the NE16 can only access L1 memory in the range 0xY000_0000 -- 0xY001_FFFC, where Y=1 or 0
// in the model, Y is ignored
#define NE16_STREAM_L1_MASK 0x0003FFFF

NeurekaStreamAccess::NeurekaStreamAccess(
  Neureka *neureka,
  int base_addr,
  int d0_length,
  int d0_stride,
  int d1_length,
  int d1_stride,
  int d2_length,
  int d2_stride,
  bool debug
) : neureka ( neureka ),
    base_addr     ( base_addr    ),
    d0_length   ( d0_length  ),
    d0_stride   ( d0_stride  ),
    d1_length   ( d1_length  ),
    d1_stride   ( d1_stride  ),
    d2_length  ( d2_length ),
    d2_stride  ( d2_stride ),
    debug         ( debug        ),
    current_addr  ( 0            )
{
  this->reset_iteration();
  if(this->debug) {
    this->print_config();
  }
}

int NeurekaStreamAccess::get_base_addr() {
  return this->base_addr;
}
int NeurekaStreamAccess::get_d0_length() {
  return this->d0_length;
}
int NeurekaStreamAccess::get_d0_stride() {
  return this->d0_stride;
}
int NeurekaStreamAccess::get_d1_length() {
  return this->d1_length;
}
int NeurekaStreamAccess::get_d1_stride() {
  return this->d1_stride;
}
int NeurekaStreamAccess::get_d2_length() {
  return this->d2_length;
}
int NeurekaStreamAccess::get_d2_stride() {
  return this->d2_stride;
}

void NeurekaStreamAccess::print_config() {
  std::cout << "[STREAMER] base_addr="  << std::hex << this->base_addr << std::dec << std::endl;
  std::cout << "[STREAMER] tot_length=" << this->d0_length << std::endl;
  std::cout << "[STREAMER] d0_stride="  << this->d0_stride << std::endl;
  std::cout << "[STREAMER] d0_length="  << this->d1_length << std::endl;
  std::cout << "[STREAMER] d1_stride="  << this->d1_stride << std::endl;
  std::cout << "[STREAMER] d1_length="  << this->d2_length << std::endl;
  std::cout << "[STREAMER] d2_stride="  << this->d2_stride << std::endl;
}

void NeurekaStreamAccess::reset_iteration() {
  this->wa = 0;
  this->la = 0;
  this->ba = 0;
  this->wc = 1;
  this->lc = 1;
  this->bc = 1;
  this->oc = 0;
}

int NeurekaStreamAccess::iterate() {
  if (this->d1_length < 0) {
    this->current_addr = this->base_addr + this->wa;
  }
  else if(this->d2_length < 0) {
    this->current_addr = this->base_addr + this->la + this->wa;
  }
  else {
    this->current_addr = this->base_addr + this->ba + this->la + this->wa;
  }
  this->oc++;
  if(this->debug) {
    std::cout << "[STREAMER] wa=" << this->wa << " la=" << this->la << " ba=" << this->ba << " oc=" << this->oc << std::endl;
    std::cout << "[STREAMER] wc=" << this->wc << " lc=" << this->lc << " bc=" << this->bc << " oc=" << this->oc << std::endl;
  }
  if((this->wc < this->d1_length) || (this->d1_length < 0)) {
    this->wa += this->d0_stride;
    this->wc += 1;
  }
  else if ((this->lc < this->d2_length) || (this->d2_length < 0)) {
    this->wa = 0;
    this->la += this->d1_stride;
    this->wc = 1;
    this->lc += 1;
  }
  else {
    this->wa = 0;
    this->la = 0;
    this->ba += this->d2_stride;
    this->wc = 1;
    this->lc = 1;
    this->bc += 1;
  }
  return this->current_addr;
}

template <class T>
NeurekaVectorLoad<T>::NeurekaVectorLoad(
  Neureka *neureka,
  int base_addr,
  int d0_length,
  int d0_stride,
  int d1_length,
  int d1_stride,
  int d2_length,
  int d2_stride,
  bool debug
) : NeurekaStreamAccess(neureka, base_addr, d0_length, d0_stride, d1_length, d1_stride, d2_length, d2_stride, debug) {
}

template <class T>
NeurekaVectorLoad<T>::NeurekaVectorLoad() : NeurekaStreamAccess((Neureka *) NULL, 0, 0, 0, 0, 0, 0, 0, false) {
}

template <class T>
xt::xarray<T> NeurekaVectorLoad<T>::ex(int width, bool w_demux, int64_t& cycles) {
  auto addr = this->iterate();
  uint8_t load_data[STREAM_MAX_WIDTH_BYTES];
  auto width_padded = width + 4;
  auto addr_padded = addr & ~0x3;
  auto width_words = width_padded*sizeof(T)/4;
  auto width_rem   = width_padded*sizeof(T)%4;
  int64_t max_latency = 0;
  
  for(auto i=0; i<width_words; i++) {
    this->neureka->io_req.init();
    if(w_demux==true)
      this->neureka->io_req.set_addr(addr_padded+i*4);
    else 
      this->neureka->io_req.set_addr(addr_padded+i*4 & NE16_STREAM_L1_MASK);
    
    this->neureka->io_req.set_size(4);
    this->neureka->io_req.set_data(load_data+i*4);
    this->neureka->io_req.set_is_write(false);
    int err = (w_demux==true) ? this->neureka->wmem_out.req(&this->neureka->io_req) : this->neureka->out.req(&this->neureka->io_req);
    
    if (err == vp::IO_REQ_OK) {
      int64_t latency = this->neureka->io_req.get_latency();
      if (latency > max_latency) {
        max_latency = latency;
      }
    }
    else {
      this->neureka->trace.fatal("Unsupported asynchronous reply\n");
    }
  }
  if(width_rem) {
    this->neureka->io_req.init();
    this->neureka->io_req.set_addr(addr_padded+width_words*4 & NE16_STREAM_L1_MASK);
    this->neureka->io_req.set_size(width_rem);
    this->neureka->io_req.set_data(load_data+width_words*4);
    this->neureka->io_req.set_is_write(false);
    int err = (w_demux==true) ? this->neureka->wmem_out.req(&this->neureka->io_req) : this->neureka->out.req(&this->neureka->io_req);
    if (err == vp::IO_REQ_OK) {
      // int64_t latency = this->neureka->io_req.get_latency();
      // if (latency > max_latency) {
      //   max_latency = latency;
      // }
    }
    else {
      this->neureka->trace.fatal("Unsupported asynchronous reply\n");
    }
  }
  std::ostringstream stringStream;

  // if (this->neureka->trace_level == L3_ALL) {
    this->neureka->trace.msg(vp::trace::LEVEL_DEBUG, "Issuing read request (addr=0x%08x, size=%dB, latency=%d)\n", addr & NE16_STREAM_L1_MASK, width*sizeof(T), cycles+1);
  // }
  xt::xarray<T> x = xt::zeros<T>({width});
  for(auto i=0; i<width; i++) {
    xt::view(x, i) = *(T *)(load_data + (addr & 0x3) + i*sizeof(T));
  }
  xt::print_options::set_line_width(1000);
  stringStream << "Read data: " << (this->neureka->trace_format?std::hex:std::dec) << x << std::dec << "\n";
  string s = stringStream.str();
  if (this->neureka->trace_level == L3_ALL) {
    this->neureka->trace.msg(vp::trace::LEVEL_DEBUG, s.c_str());
  }
  cycles += max_latency + 1;
  return x;
}

template <class T>
NeurekaVectorStore<T>::NeurekaVectorStore(
  Neureka *neureka,
  int base_addr,
  int d0_length,
  int d0_stride,
  int d1_length,
  int d1_stride,
  int d2_length,
  int d2_stride,
  bool debug
) : NeurekaStreamAccess(neureka, base_addr, d0_length, d0_stride, d1_length, d1_stride, d2_length, d2_stride, debug) {
}

template <class T>
NeurekaVectorStore<T>::NeurekaVectorStore() : NeurekaStreamAccess((Neureka *) NULL, 0, 0, 0, 0, 0, 0, 0, false) {
}

template <class T>
xt::xarray<T> NeurekaVectorStore<T>::ex(xt::xarray<T> data, int width, int64_t& cycles, int32_t enable) {
  auto addr = this->iterate();
  uint8_t store_data[STREAM_MAX_WIDTH_BYTES];
  for(auto i=0; i<STREAM_MAX_WIDTH_BYTES; i++) {
    store_data[i] = 0;
  }
  for(auto i=0; i<width; i++) {
    *(T *)(store_data + i*sizeof(T)) = data(i);
  }

  auto width_bytes = width*sizeof(T);
  /* To handle misaligned address*/
  auto addr_start = addr;
  auto addr_end   = addr + width_bytes;

  auto addr_start_aligned = 4*((addr_start+(addr_start%4 ? 4:0))/4);
  auto addr_end_aligned   = 4*(addr_end/4);

  auto misaligned_start_byte =  (width_bytes < 4 ) ? width_bytes : addr_start_aligned - addr_start;
  auto misaligned_end_byte = (width_bytes < 4 ) ?  0 : addr_end - addr_end_aligned;

  auto width_words = (addr_end_aligned> addr_start_aligned) ? (addr_end_aligned - addr_start_aligned)/4 : 0;

  // std::cout<<std::hex<<"addr_start_aligned="<<addr_start_aligned<<", addr_end_aligned="<<addr_end_aligned<<std::endl;
  // std::cout<<std::hex<<"addr_start="<<addr_start<<", addr_end="<<addr_end<<std::endl;

  // std::cout<<std::hex<<"misaligned_start_byte="<<misaligned_start_byte<<", misaligned_end_byte="<<misaligned_end_byte<<std::endl;

  int64_t max_latency = 0;
  if(enable) {
    // std::cout<<std::dec<<"Width="<<width<<", width_bytes="<<width_bytes<<", width_words="<<width_words<<", addr="<<addr<<", sizeof(T)="<<sizeof(T)<<std::endl;
    for(auto i=0; i<width_words+misaligned_start_byte+misaligned_end_byte; i++) {
      this->neureka->io_req.init();

      // if((i<misaligned_start_byte) ||((i>=width_words+misaligned_start_byte)))
      if((i<misaligned_start_byte))
      {
        // std::cout<<i<<" Head of misaligned "<<((addr_start+i) & NE16_STREAM_L1_MASK)<<std::endl;
        this->neureka->io_req.set_addr((addr_start+i) & NE16_STREAM_L1_MASK);
        this->neureka->io_req.set_size(1);
        this->neureka->io_req.set_data(store_data+i);
      }
      else if(i>=misaligned_start_byte+width_words){
        // std::cout<<i<<" Tail of misaligned "<<((addr_start+misaligned_start_byte + 4*width_words+(i- misaligned_start_byte - width_words)) & NE16_STREAM_L1_MASK)<<std::endl;
        this->neureka->io_req.set_addr((addr_start+misaligned_start_byte + 4*width_words+(i- misaligned_start_byte - width_words)) & NE16_STREAM_L1_MASK);
        this->neureka->io_req.set_size(1);
        this->neureka->io_req.set_data(store_data+misaligned_start_byte + 4*width_words+(i- misaligned_start_byte - width_words));
      }
      else
      {
        // std::cout<<i<<" Aligned "<<((addr_start_aligned+4*(i - misaligned_start_byte)) & NE16_STREAM_L1_MASK)<<std::endl;
        this->neureka->io_req.set_addr((addr_start_aligned+4*( i- misaligned_start_byte)) & NE16_STREAM_L1_MASK);
        this->neureka->io_req.set_size(4);
        this->neureka->io_req.set_data(store_data+misaligned_start_byte+4*(i-misaligned_start_byte));
      } 
      this->neureka->io_req.set_is_write(true);
      int err = this->neureka->out.req(&this->neureka->io_req);
      if (err == vp::IO_REQ_OK) {
        if((i>=misaligned_start_byte) && (i<width_words+misaligned_start_byte)) {  // apparently, for non-aligned bytes we get garbage latency
          int64_t latency = this->neureka->io_req.get_latency();
          if (latency > max_latency) {
            max_latency = latency;
          }
        }
      }
      else {
        this->neureka->trace.fatal("Unsupported asynchronous reply\n");
      }
    }
  }
  std::ostringstream stringStream;
  if (this->neureka->trace_level == L3_ALL) {
    this->neureka->trace.msg(vp::trace::LEVEL_DEBUG, "Issuing write request (addr=0x%08x, size=%dB, latency=%d)\n", addr & NE16_STREAM_L1_MASK, width*sizeof(T), cycles+max_latency+1);
  }
  xt::print_options::set_line_width(1000);
  if(enable) {
    stringStream << "Write data: " << (this->neureka->trace_format?std::hex:std::dec) << data << std::dec << "\n";
    string s = stringStream.str();
    if (this->neureka->trace_level == L3_ALL) {
      this->neureka->trace.msg(vp::trace::LEVEL_DEBUG, s.c_str());
    }
  }
  else {
    stringStream << "Write disabled" << "\n";
    string s = stringStream.str();
    if (this->neureka->trace_level == L3_ALL) {
      this->neureka->trace.msg(vp::trace::LEVEL_DEBUG, s.c_str());
    }
  }
  cycles += max_latency + 1;
  
  return data;
}

// template instantiations
template class NeurekaVectorLoad<uint8_t>;
template class NeurekaVectorStore<uint8_t>;
