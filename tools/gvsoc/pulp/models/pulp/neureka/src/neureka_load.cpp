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

void Neureka::load_setup() {
  // set up streamer to address input activations (byte-based)

  this->h_size_in  = (this->i_major < this->subtile_nb_ho-1) || (this->subtile_rem_hi==0) ? (this->fs == 1 ? this->H_SIZE : this->H_SIZE+this->FILTER_SIZE-1) : this->subtile_rem_hi;
  this->w_size_in  = (this->j_major < this->subtile_nb_wo-1) || (this->subtile_rem_wi==0) ? (this->fs == 1 ? this->W_SIZE : this->W_SIZE+this->FILTER_SIZE-1) : this->subtile_rem_wi;
  this->h_size_out = (this->i_major < this->subtile_nb_ho-1) || (this->subtile_rem_ho==0) ? this->H_SIZE : this->subtile_rem_ho;
  this->w_size_out = (this->j_major < this->subtile_nb_wo-1) || (this->subtile_rem_wo==0) ? this->W_SIZE : this->subtile_rem_wo;

  this->h_size_in_X_w_size_in = this->h_size_in * this->w_size_in;
  this->h_size_out_X_w_size_out = this->h_size_out * this->w_size_out;

  this->h_size_in_hw = (this->i_major<this->subtile_nb_ho-1 || this->subtile_rem_hi==0) ? (this->fs == 3 ? this->F_BUFFER_SIZE : this->H_SIZE) : this->subtile_rem_hi;
  this->w_size_in_hw = (this->j_major<this->subtile_nb_wo-1 || this->subtile_rem_wi==0) ? (this->fs == 3 ? this->F_BUFFER_SIZE : this->W_SIZE) : this->subtile_rem_wi;
 
  auto k_in_major = this->depthwise ? this->k_out_major : this->k_in_major_iter;

  if(this->trace_level == L3_ALL) {
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   h_size_in=%d\n", this->h_size_in);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   w_size_in=%d\n", this->w_size_in);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   h_size_out=%d\n", this->h_size_out);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   w_size_out=%d\n", this->w_size_out);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   h_size_in_X_w_size_in=%d\n", this->h_size_in_X_w_size_in);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   h_size_out_X_w_size_out=%d\n", this->h_size_out_X_w_size_out);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   k_in_major=%d\n", k_in_major);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   h_size_in_hw=%d\n", this->h_size_in_hw);
    this->trace.msg(vp::trace::LEVEL_DEBUG, "   w_size_in_hw=%d\n", this->w_size_in_hw);
  }

  auto infeat_hom_iter = this->H_SIZE * this->infeat_d1_stride;
  auto infeat_wom_iter = this->W_SIZE * this->infeat_d0_stride;

  auto base_addr_x = (this->fs==1) ? this->infeat_ptr + this->i_major*infeat_hom_iter + this->j_major*infeat_wom_iter + k_in_major*this->TP_IN : this->infeat_ptr + this->i_major*infeat_hom_iter + this->j_major*infeat_wom_iter + k_in_major*this->TP_IN_S;

  this->vld_x = NeurekaVectorLoad<uint8_t>(
    this,
    base_addr_x, // base_addr
    this->h_size_in_X_w_size_in, // word_length
    this->infeat_d0_stride, // word_stride
    this->w_size_in, // line_length
    this->infeat_d1_stride, // line_stride
    this->h_size_in_X_w_size_in, // block_length
    this->infeat_d2_stride, // block_stride
    false
  );

  this->load_i_fbuf_lim = this->h_size_in;
  this->load_j_fbuf_lim = this->w_size_in;

  this->load_k_in_lim = this->TP_IN;
  this->load_padding = {0, 0}; // std::vector<uint32_t>

  if(k_in_major == this->subtile_nb_ki-1 && this->subtile_rem_ki != this->TP_IN) { // last k_in tile, only if it requires padding
    this->load_k_in_lim = this->subtile_rem_ki;
    this->load_padding = {0, static_cast<uint32_t>(this->TP_IN-this->subtile_rem_ki)};
  }

  this->load_i_fbuf = 0;
  this->load_j_fbuf = 0;

  xt::view(this->x_buffer, xt::all()) = 0;
}

int Neureka::load_cycle() { // not linear
  int64_t cycles = 0;
  xt::view(this->x_buffer, this->load_i_fbuf, this->load_j_fbuf, xt::all()) = xt::pad(this->vld_x.ex(this->load_k_in_lim, false, cycles), this->load_padding);
  return (int) cycles;
}

void Neureka::load_do_padding() { // not linear

  // add explicit padding
  auto right_lim  = (this->F_BUFFER_SIZE-this->padding_right  > this->w_size_in_hw) ? this->w_size_in_hw : this->F_BUFFER_SIZE-this->padding_right;
  auto bottom_lim = (this->F_BUFFER_SIZE-this->padding_bottom > this->h_size_in_hw) ? this->h_size_in_hw : this->F_BUFFER_SIZE-this->padding_bottom;

  // implicit padding (on the right/bottom) and explicit padding (all dimensions) define
  // sixteen regions:
  // +-------+-------+-------+-------+
  // | TL    | T     | TR    | TRR   |
  // +-------+-------+-------+-------+
  // | L     | body  | R     | RR    |
  // +-------+-------+-------+-------+
  // | BL    | B     | BR    | BRR   |
  // +-------+-------+-------+-------+
  // | BBL   | BB    | BBR   | BBRR  |
  // +-------+-------+-------+-------+

  // FIXME: non0-padding values do not work in mode16 in the model, see similar bug in NE16 RTL
  
  // top-left
  if(this->padding_left  > 0 && this->j_major==0 || this->padding_top > 0 && this->i_major==0) {
    xt::view(this->x_buffer, xt::range(0, this->padding_top), xt::range(0, this->padding_left), xt::range(0, this->load_k_in_lim)) = this->padding_value;
  }

  // top
  if(this->padding_top  > 0 && this->i_major==0) {
    xt::view(this->x_buffer, xt::range(0, this->padding_top), xt::range(this->padding_left, right_lim), xt::range(0, this->load_k_in_lim)) = this->padding_value;
  }

  // top-right
  if((this->padding_right > 0 && this->j_major==this->subtile_nb_wo-1 || this->padding_top > 0 && this->i_major==0) && (right_lim <= this->w_size_in_hw)) {
    xt::view(this->x_buffer, xt::range(0, this->padding_top), xt::range(right_lim, w_size_in_hw), xt::range(0, this->load_k_in_lim)) = this->padding_value;
  }

  // right
  if((this->padding_right > 0 && this->j_major==this->subtile_nb_wo-1) && (right_lim <= this->w_size_in_hw)) {
    xt::view(this->x_buffer, xt::range(this->padding_top, bottom_lim), xt::range(right_lim, this->w_size_in_hw), xt::range(0, this->load_k_in_lim)) = this->padding_value;
  }

  // bottom-right
  if((this->padding_right > 0 && this->j_major==this->subtile_nb_wo-1 || this->padding_bottom > 0 && this->i_major==this->subtile_nb_ho-1) && (right_lim <= this->w_size_in_hw) && (bottom_lim <= this->h_size_in_hw)) {
    xt::view(this->x_buffer, xt::range(bottom_lim, h_size_in_hw), xt::range(right_lim, w_size_in_hw), xt::range(0, this->load_k_in_lim)) = this->padding_value;
  }

  // bottom
  if((this->padding_bottom > 0 && this->i_major==this->subtile_nb_ho-1) && (bottom_lim <= this->h_size_in_hw)) {
    xt::view(this->x_buffer, xt::range(bottom_lim, this->h_size_in_hw), xt::range(this->padding_left, right_lim), xt::range(0, this->load_k_in_lim)) = this->padding_value;
  }

  // bottom-left
  if((this->padding_left > 0 && this->j_major==0 || this->padding_bottom > 0 && this->i_major==this->subtile_nb_ho-1) && (bottom_lim <= this->h_size_in_hw)) {
    xt::view(this->x_buffer, xt::range(bottom_lim, this->h_size_in_hw), xt::range(0, this->padding_left), xt::range(0, this->load_k_in_lim)) = this->padding_value;
  }

  // left
  if(this->padding_left > 0 && this->j_major==0) {
    xt::view(this->x_buffer, xt::range(this->padding_top, bottom_lim), xt::range(0, this->padding_left), xt::range(0, this->load_k_in_lim)) = this->padding_value;
  }
}

void Neureka::load_do_extract() {
  // extract x_array from x_buffer
  if(this->fs == 3) {
    for(auto i_col=0; i_col<this->NR_COLUMN; i_col++) { // spatial loop - implemented as a set of muxes
      auto i = i_col / this->H_SIZE;
      auto j = i_col % this->W_SIZE;
      if(i<this->h_size_out && j<this->w_size_out){
        xt::xarray<uint8_t> x_buffer_view = xt::eval(xt::view(this->x_buffer, xt::range(i, i+this->FILTER_SIZE), xt::range(j, j+this->FILTER_SIZE), xt::all()));
        auto shape = xt::adapt(x_buffer_view.shape());
        xt::view(this->x_array, i_col, xt::all()) = x_buffer_view.reshape({shape[0] * shape[1], shape[2]});
      }
      else {
        xt::xarray<uint8_t> x_buffer_view = xt::eval(xt::zeros<uint8_t>({this->FILTER_SIZE, this->FILTER_SIZE,this->TP_IN}));
        auto shape = xt::adapt(x_buffer_view.shape());
        xt::view(this->x_array, i_col, xt::all()) = x_buffer_view.reshape({shape[0] * shape[1], shape[2]});
      }
    }
  }
  else { // in 1x1 mode, fill only the first qw rows
    xt::view(this->x_array, xt::all()) = 0;
    for(auto i_row=0; i_row<this->qw; i_row++) { // spatial loop - implemented as a set of muxes
      xt::xarray<uint8_t> x_buffer_view = xt::eval(xt::view(this->x_buffer, xt::range(0, this->H_SIZE), xt::range(0, this->W_SIZE), xt::all()));
      auto shape = xt::adapt(x_buffer_view.shape());
      xt::view(this->x_array, xt::all(), i_row) = x_buffer_view.reshape({shape[0] * shape[1], shape[2]});
    }
  }
}

void Neureka::load_filter_masking() {
  // filter masking
  xt::xarray<int32_t> W_mask = xt::ones<int32_t>({this->fs, this->fs});
  if(this->fs == 3) {
    for(int i=0; i<3; i++){
      for(int j=0; j<3; j++){
        int index = 3*i + j;
        xt::view(W_mask, xt::range(i, i+1), xt::range(j,j+1)) = ((((this->filter_mask_top)>>index) & 0x0001)==0x01) ? 0 : 1;
      }
    }
  }
  this->row_enable = xt::flatten(W_mask);
}

bool Neureka::load_exit_idx() {
  if(this->load_i_fbuf == this->load_i_fbuf_lim-1 && this->load_j_fbuf == this->load_j_fbuf_lim-1) {
    return true;
  }
  else {
    return false;
  }
}

void Neureka::load_update_idx() {
  if(this->load_j_fbuf == this->load_j_fbuf_lim-1) {
    this->load_j_fbuf = 0;
    this->load_i_fbuf++;
  }
  else {
    this->load_j_fbuf++;
  }
}
