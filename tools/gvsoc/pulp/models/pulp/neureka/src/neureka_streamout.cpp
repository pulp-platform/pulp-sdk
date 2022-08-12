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

void Neureka::streamout_setup() {

  auto tp = this->depthwise ? this->TP_IN_S : this->TP_OUT;

  auto outfeat_hom_iter = this->H_SIZE * this->outfeat_d2_stride;
  auto outfeat_wom_iter = this->W_SIZE * this->outfeat_d1_stride;
 
  auto base_addr_y = this->outfeat_ptr + this->i_major*outfeat_hom_iter + this->j_major*outfeat_wom_iter + this->k_out_major*tp*this->quantization_bits/8;

  auto streamout_k_out_lim = !this->depthwise ? this->mv_k_out_lim : (this->k_out_major == this->subtile_nb_ko-1 && this->subtile_rem_ko != this->TP_IN_S && this->subtile_rem_ko != 0) ? this->subtile_rem_ko : this->TP_IN;
;
  auto h_size_out_X_w_size_out_with_strb = (this->quantization_bits == 8) || (streamout_k_out_lim <= 8)  ? this->NR_COLUMN  :
                                                                             (streamout_k_out_lim <= 16) ? 2*this->NR_COLUMN :
                                                                             (streamout_k_out_lim <= 24) ? 3*this->NR_COLUMN : 4*this->NR_COLUMN;

  
  this->col_enable = xt::zeros<int32_t>({this->H_SIZE,this->W_SIZE});
  for(auto i=0; i<this->h_size_out; i++) {
    for(auto j=0; j<this->w_size_out; j++) {
      xt::view(this->col_enable, i, j) = 1;
    }
  }
  if(this->strided2x2) {
    xt::view(this->col_enable, 0, 1) = 0;
    xt::view(this->col_enable, 1, xt::all()) = 0;
    xt::view(this->col_enable, 2, 1) = 0;
  }

  this->vst_y = NeurekaVectorStore<uint8_t>(
    this,
    base_addr_y, // base_addr
    h_size_out_X_w_size_out_with_strb*(this->quantization_bits == 32 ? (streamout_k_out_lim/8 > 0 ? streamout_k_out_lim/8 + (streamout_k_out_lim%8 == 0 ? 0 : 1) : 1) : 1), // word_length
    this->outfeat_d0_stride, // word_stride
    this->quantization_bits == 32 ? (streamout_k_out_lim/8 > 0 ? streamout_k_out_lim/8 + (streamout_k_out_lim%8 == 0 ? 0 : 1) : 1) : 1, //w_size_out, // line_length
    this->outfeat_d1_stride, // line_stride
    this->H_SIZE, // block_length
    this->outfeat_d2_stride, // block_stride
    false
  );

  this->streamout_k_out_lim = this->quantization_bits == 32 ? this->depthwise ? (this->TP_IN/8):tp/8 : 1;
  if(this->k_out_major == this->subtile_nb_ko-1 && this->subtile_rem_ko != tp && this->subtile_rem_ko != 0) { // last k_in tile, only if it requires padding
    this->streamout_k_out_lim = this->quantization_bits == 32 ? this->subtile_rem_ko/8 + (this->subtile_rem_ko%8 == 0 ? 0 : 1) : 1;
  }
  this->streamout_k_out_iter = 0;
  this->streamout_i_out_iter = 0;
  this->streamout_j_out_iter = 0;

  // relu is here because of easier modeling
  if(this->quantization_bits == 8 & (this->output_quant)) {
    if(this->use_relu) {
      if(!this->signed_activation){
        xt::view(this->accum, xt::all()) = xt::clip(this->accum, 0, 255);
      }
      else {
        xt::view(this->accum, xt::all()) = xt::clip(this->accum, 0, 127);
      }
    }
    else {
      xt::view(this->accum, xt::all()) = xt::clip(this->accum, -128, 127);
    }
  }
  else if(this->use_relu & (this->output_quant)) {
    xt::view(this->accum, xt::all()) = xt::clip(this->accum, 0, 0xffffffff);
  }
  if(this->accum_traces) {
    this->debug_accum();
  }

  if(this->trace_level == L3_ALL) {
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   k_out=%d\n", this->k_out);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   w_out=%d\n", this->w_out);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   h_size_out=%d\n", this->h_size_out);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   w_size_out=%d\n", this->w_size_out);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   i_major=%d\n", this->i_major);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   j_major=%d\n", this->j_major);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   k_out_major=%d\n", this->k_out_major);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   tp=%d\n", tp);
  }
}

int Neureka::streamout_cycle() { 
  int64_t cycles = 0;
  auto tp = this->depthwise ? this->TP_IN_S : this->TP_OUT;
  auto ko = ((this->subtile_nb_ko-1)*tp + this->subtile_rem_ko);
  auto ko_rem = (this->subtile_nb_ko==1) ? this->subtile_rem_ko : ((ko%tp)==0)? tp : ko%tp ;
  auto ko_rem_index = (this->subtile_nb_ko==1) ? ko_rem : (this->k_out_major==this->subtile_nb_ko-1)?ko_rem : tp;
  
  xt::xarray<uint8_t> xx = xt::zeros<uint8_t>({32});
  if(this->quantization_bits == 32) {
    this->trace.msg(vp::trace::LEVEL_DEBUG, "  Before streamout_k_out_lim=%d\n", this->streamout_k_out_lim);
    auto k_out_last = (this->streamout_k_out_iter == this->streamout_k_out_lim-1) & (this->depthwise) ? ko_rem_index : (this->streamout_k_out_iter+1)*8;
    if(this->k_out_major == this->subtile_nb_ko-1 && this->subtile_rem_ko != tp && this->subtile_rem_ko != 0) { // last k_in tile, only if it requires padding
      this->trace.msg(vp::trace::LEVEL_DEBUG, "  Inside the if before subtile_rem_ko=%d\n", this->subtile_rem_ko);
      this->trace.msg(vp::trace::LEVEL_DEBUG, "  Inside the if before subtile_nb_ko=%d\n", this->subtile_nb_ko);
      this->trace.msg(vp::trace::LEVEL_DEBUG, "  Inside the if before k_out_last=%d\n", k_out_last);
      k_out_last = (k_out_last < (this->subtile_rem_ko)) ? k_out_last : (this->subtile_rem_ko);
      // k_out_last = this->subtile_rem_ko;
      this->trace.msg(vp::trace::LEVEL_DEBUG, "  Inside the if after k_out_last=%d\n", k_out_last);
    }
    for (auto i=this->streamout_k_out_iter*8; i<k_out_last; i++) {
      for(auto j=0; j<4; j++) {
        xt::view(xx, (i-this->streamout_k_out_iter*8)*4+j) = (xt::view(this->accum, i, this->streamout_i_out_iter*this->H_SIZE+this->streamout_j_out_iter) >> (j*8)) & 0xff;
      }
    }
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   k_out_last=%d\n", k_out_last);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   streamout_k_out_iter=%d\n", this->streamout_k_out_iter);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   streamout_i_out_iter=%d\n", this->streamout_i_out_iter);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   streamout_j_out_iter=%d\n", this->streamout_j_out_iter);
    this->vst_y.ex(xx, (k_out_last-this->streamout_k_out_iter*8)*4, cycles, this->col_enable (this->streamout_i_out_iter, this->streamout_j_out_iter));
  }
  else if(this->quantization_bits == 8) {
    auto k_out_last = tp;
    if(this->k_out_major == this->subtile_nb_ko-1 && this->subtile_rem_ko != tp && this->subtile_rem_ko != 0) { // last k_in tile, only if it requires padding
      k_out_last = this->subtile_rem_ko;
    }
    for (auto i=0; i<k_out_last; i++) {
      xt::view(xx, i) = xt::view(this->accum, i, this->streamout_i_out_iter*this->H_SIZE+this->streamout_j_out_iter);
    }
    this->vst_y.ex(xx, k_out_last, cycles, this->col_enable (this->streamout_i_out_iter, this->streamout_j_out_iter));
  }
  return (int) cycles;
}

bool Neureka::streamout_exit_idx() {
  auto h_size_out = this->h_size_out;
  auto w_size_out = this->w_size_out;
  if(this->streamout_i_out_iter == h_size_out-1 && this->streamout_j_out_iter == w_size_out-1 && this->streamout_k_out_iter == this->streamout_k_out_lim-1) {
    return true;
  }
  else {
    return false;
  }
}

void Neureka::streamout_update_idx() {
  if(this->streamout_k_out_iter < this->streamout_k_out_lim-1) {
    this->streamout_k_out_iter++;
  } 
  else if(this->streamout_j_out_iter < this->W_SIZE-1) {
    this->streamout_k_out_iter = 0;
    this->streamout_j_out_iter++;
  }
  else {
    this->streamout_k_out_iter = 0;
    this->streamout_j_out_iter = 0;
    this->streamout_i_out_iter++;
  }
}

bool Neureka::streamout_to_end_idx() {
  if((this->k_out_major == this->subtile_nb_ko-1) && (this->i_major == this->subtile_nb_ho-1) && (this->j_major == this->subtile_nb_wo-1)) {
    return true;
  }
  else {
    return false;
  }
}
