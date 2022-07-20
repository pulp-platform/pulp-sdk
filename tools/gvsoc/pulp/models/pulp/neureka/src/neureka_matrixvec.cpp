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

// as the internal max precision of NE16 is 32 bits, this is emulated by casting x to 32 bits here
#include <neureka.hpp>
xt::xarray<uint8_t> __Weight_transform_1x1(xt::xarray<uint8_t> W)
{
  xt::xarray<uint8_t> wout_1x1 = xt::zeros<uint8_t>({32});
  int index_q8, index_r4, index_q4r2, index_l, index_h;
  for(int i=0; i<32; i++)
  {
    index_q8 = i/8;
    index_r4 = i%4;
    index_q4r2 = (i/4)%2;
    index_l = 8*index_r4+index_q8;
    index_h = 8*index_r4+index_q8+4;
    if(index_q4r2==0){
      wout_1x1[i] = (W[index_l] & 0x0F) + ((W[index_h] & 0x0F)<<4);
    }
    else{
      wout_1x1[i] = ((W[index_l] & 0xF0)>>4) + 16*((W[index_h] & 0xF0)>>4);
    }
  }
  return wout_1x1;
}

xt::xarray<uint8_t>  __Weight_transform_28(xt::xarray<uint8_t> W)
{
  xt::xarray<uint8_t> wout_3x3 = xt::zeros<uint8_t>({36});
  int index0=0;
  int index1=0;
  for(int i=0; i<32; i++)
  {
    index0 = i % 7;
    index1 = i / 7;
    if(index0==3)
    {
      wout_3x3[index1*8+index0] = (W[i] & 0x0F);
    }
    else if(index0>3)
    {
      wout_3x3[index1*8+index0] = ((W[i-1] & 0xF0) >> 4) + ((W[i] & 0x0F)*16);
      if(index0==6)
        wout_3x3[index1*8+index0+1] = ((W[i] & 0xF0) >> 4);
    }
    else{
      wout_3x3[index1*8+index0] = W[i];
    }
  }
  return wout_3x3;
}


xt::xarray<int64_t> __NormQuant(
  xt::xarray<int64_t> x,
  xt::xarray<int32_t> kappa_bn,
  xt::xarray<int32_t> lambda_bn,
  int32_t shift_reqnt,
  int clip_min,
  int clip_max,
  bool use_rounding,
  bool use_clip,
  bool use_cast
) {
  if(use_clip) {
    if(use_cast)
      return xt::clip((xt::cast<int32_t>(x) * kappa_bn + lambda_bn + (use_rounding ? 1<<(shift_reqnt-1) : 0)) >> shift_reqnt, clip_min, clip_max);
    else
      return xt::clip((x * kappa_bn + lambda_bn + (use_rounding ? 1<<(shift_reqnt-1) : 0)) >> shift_reqnt, clip_min, clip_max);
  }
  else
    return (xt::cast<int32_t>(x) * kappa_bn + lambda_bn + (use_rounding ? 1<<(shift_reqnt-1) : 0)) >> shift_reqnt;
}

xt::xarray<uint8_t> __WeightUnpack(
  xt::xarray<uint8_t> w,
  int                 size,
  int                 TP_IN
) {

  w = w.reshape({size,(TP_IN/8),1});

  xt::xarray<uint8_t> wu = xt::zeros<uint8_t>({size,(TP_IN/8),8});

  xt::view(wu, xt::all(), xt::all()) = xt::view(w, xt::all(), xt::all());

  wu = (wu >> xt::linspace(0, 7, 8).reshape({1, 1, 8})) & 0x1;

  auto shape = xt::adapt(wu.shape());

  return wu.reshape({size, (TP_IN/8)*8});
}

xt::xarray<int64_t> __BinConvBlock(
  xt::xarray<uint8_t> w,
  xt::xarray<int8_t> x,
  xt::xarray<uint8_t> ux,
  bool signed_activation,
  int scale=0,
  int TP_IN=32
) {
  if(signed_activation)
    return xt::sum(w * x, 0) * scale;
  else 
    return xt::sum(w * ux, 0) * scale;
}

void Neureka::__BinConvArray(
  xt::xarray<uint8_t>& weight,
  int                  scale,
  int                  idx,
  xt::xarray<int32_t>  row_enable,
  xt::xarray<int32_t>  mac_enable,
  bool                 weight_shift,
  bool                 weight_invert,
  bool                 use_row_as_scale
) {
  for(auto c=0; c<this->NR_COLUMN; c++) { // spatial loop - over columns
    xt::view(this->psum_column, c) = 0;
    for(auto r=0; r<this->COLUMN_SIZE; r++) { // spatial loop - over blocks in a column
      if(row_enable(r) == 0) // row disabling to implement filter masks
        continue;
      auto scale_loc = use_row_as_scale ? 1 << r : scale;
      auto activ = xt::view(this->x_array, c, r, xt::all()); // 16x channels of 8-bit
      if(this->binconv_traces) {
        std::ostringstream stringStream;
        stringStream << "binconv: weight=" << xt::view(weight, r)*mac_enable << "activ=" << activ << " scale=" << scale_loc << " ==> " << xt::view(weight, r)*mac_enable * activ << " ==> " << std::hex << xt::sum(xt::view(weight, r)*mac_enable*activ, 0)*scale << std::dec << "\n";
        std::string copyOfStr = stringStream.str();
        this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr.c_str());
      }
      
      xt::view(this->psum_block, c, r) = __BinConvBlock(xt::view(weight, r) * mac_enable, activ, activ, this->signed_activation,  scale_loc, this->TP_IN);
      if(weight_shift && weight_invert) {
        xt::view(this->psum_block, c, r) = -xt::view(this->psum_block, c, r);
      }
      xt::view(this->psum_column, c) += xt::view(this->psum_block, c, r);
    }

    if(weight_shift) {
      xt::view(this->accum, xt::all(), c) += xt::view(this->psum_column, c);
    } 
    else {
      xt::view(this->accum, idx, c) += xt::view(this->psum_column, c);
    }
  }
}

void Neureka::__weightoffs(
  int dw_iter,
  xt::xarray<int32_t> row_enable,
  xt::xarray<int32_t> mac_enable
) {
  auto start_s = 1;
  for(auto s=start_s; s<this->SHIFT_CYCLES; s++) { // temporal loop - fake weight for Wmin offsetting // FIXME: how to properly do this in 1x1 mode?

    // fake-load and unpack weight bits
    auto read_size = this->FILTER_SIZE*this->FILTER_SIZE;
    xt::xarray<uint8_t> weight_ld = xt::zeros<uint8_t>({read_size, this->TP_IN/8});
    if(this->fs == 3)
      xt::view(weight_ld, xt::all()) = 0xff;
    else
      xt::view(weight_ld, 0, xt::all()) = 0xff;
    auto weight = __WeightUnpack(weight_ld, read_size, this->TP_IN); //this->mode16 & this->mode_linear);
    
    auto scale = this->Wmin;
    
    this->__BinConvArray(weight, scale, this->depthwise ? dw_iter : 0, row_enable, mac_enable, !this->depthwise, false, false); 
  }
}

void Neureka::depthwise_setup() {
  this->k_out_lim_dw = (this->k_in_major == this->subtile_nb_ki-1 && this->subtile_rem_ki != this->TP_IN_S && this->subtile_rem_ki != 0) ? this->subtile_rem_ki : this->TP_IN_S;
  this->dw_lim = this->depthwise ? this->k_out_lim_dw : 1;
  this->dw_iter = 0;
  this->mac_enable = xt::zeros<int32_t>({this->TP_IN});
}

void Neureka::depthwise_update_idx() {
  this->dw_iter++;
}

void Neureka::weightoffs() {
  if(this->depthwise) {
    xt::view(this->mac_enable, xt::all()) = 0;
    xt::view(this->mac_enable, this->dw_iter) = 1;
  }
  else {
    xt::view(this->mac_enable, xt::range(0,this->TP_IN_S)) = 1;
    xt::view(this->mac_enable, xt::range(this->TP_IN_S, this->TP_IN)) = (this->fs==3) ? 0 : 1;
  }
  this->__weightoffs(this->dw_iter, this->row_enable, this->mac_enable);
}

void Neureka::matrixvec_setup() {
  auto k_in_major = this->depthwise ? this->k_out_major : this->k_in_major_iter;

  // set up streamer to address weights (32b word-based)
  // depthwise mode: layout is (subtile_nb_ki*qw, 9, TP_IN/8)
  this->base_addr_W_dw = this->weights_ptr + (k_in_major*this->qw) * 8 * (this->TP_IN/8);
  this->vld_W_dw = NeurekaVectorLoad<uint8_t>(
    this,
    this->base_addr_W_dw, // base_addr
    this->subtile_nb_ki*this->qw, // word_length
    this->weights_d0_stride, // word_stride
    this->qw, // line_length
    this->weights_d1_stride, // line_stride
    -1, // block_length
    this->weights_d2_stride, // block_stride
    false
  );

  
  // 3x3 mode: layout is (k_out, subtile_nb_ki*qw, 9, TP_IN/8)
  this->base_addr_W_3x3 = this->weights_ptr + (k_out_major*this->TP_OUT*this->subtile_nb_ki*this->qw + k_in_major*this->qw) * 8 * (this->TP_IN/8);
  this->vld_W_3x3 = NeurekaVectorLoad<uint8_t>(
    this,
    this->base_addr_W_3x3, // base_addr
    this->TP_OUT*this->subtile_nb_ki*this->qw, // word_length
    this->weights_d0_stride, // word_stride
    this->qw, // line_length
    this->weights_d1_stride, // line_stride
    -1, // block_length
    this->weights_d2_stride, // block_stride
    false
  );

  // 1x1 mode: layout is (k_out, subtile_nb_ki, qw, TP_IN/8)
  this->base_addr_W_1x1 = this->weights_ptr + (k_out_major*this->TP_OUT*this->subtile_nb_ki*8 + k_in_major*8) * (this->TP_IN/8);
  this->vld_W_1x1 = NeurekaVectorLoad<uint8_t>(
    this,
    this->base_addr_W_1x1, // base_addr
    this->TP_OUT*this->subtile_nb_ki, // word_length
    this->weights_d0_stride, // word_stride
    1, // line_length
    this->weights_d1_stride, // line_stride
    -1, // block_length
    this->weights_d2_stride, // block_stride
    false
  );

  this->mv_qw_lim = (this->fs == 3) ? this->qw : 1;
  this->mv_k_out_lim = this->depthwise ? 1 :
                       (this->k_out_major == this->subtile_nb_ko-1 && this->subtile_rem_ko != this->TP_OUT && this->subtile_rem_ko != 0) ? this->subtile_rem_ko : this->TP_OUT;
  this->mv_k_out_iter = 0;
  this->mv_qw_iter = 0; // was simply qw
}

int Neureka::matrixvec_cycle() {
  auto& vld_W = this->depthwise ? this->vld_W_dw       : (this->fs == 3) ? this->vld_W_3x3       : this->vld_W_1x1;

  auto read_size = 8;//fixed for differnt layout in pointwise mode

  auto k_out = this->depthwise ? this->dw_iter : this->mv_k_out_iter;
  // load and unpack weight bits
  int64_t cycles = 0;
  bool fetch_weight;
  xt::xarray<uint8_t> weight_ld;
  
  if((this->depthwise && (this->dw_iter==0)) || (!this->depthwise)){
    weight_ld=vld_W.ex(read_size*(this->TP_IN/8), this->weight_demux, cycles);
    if(this->depthwise)
    {
      if(this->mv_qw_iter==0){
        this->reset_dw_weight_buffer();
      }
      xt::view(this->dw_weight_buffer, this->mv_qw_iter,xt::all()) = xt::view(weight_ld,xt::all());
    }
  }
  else 
  {
    weight_ld = xt::view(this->dw_weight_buffer, this->mv_qw_iter, xt::all());
  }

  std::ostringstream stringStream;
  stringStream << "Weight Read =" << xt::view(weight_ld, xt::all())<<"\n";
  std::string copyOfStr = stringStream.str();
  this->trace.msg(vp::trace::LEVEL_DEBUG, copyOfStr.c_str());

  auto shape = xt::adapt(weight_ld.shape());

  xt::xarray<uint8_t> weight_ld_transform = (this->fs == 3) ? __Weight_transform_28(weight_ld) : __Weight_transform_1x1(weight_ld);

  auto weight = __WeightUnpack(weight_ld_transform, (this->fs==3) ? 9 : read_size, this->TP_IN);
  auto scale = 1 << this->mv_qw_iter;

  xt::xarray<int32_t> space = xt::logspace(0, 7, 8, 2);
  space = xt::reshape_view(space, {8, 1});

  this->__BinConvArray(weight, scale, k_out, this->row_enable, this->mac_enable, false, false, this->fs==1);

  return (int) cycles;
}

void Neureka::reset_dw_weight_buffer() {
  this->dw_weight_buffer = xt::zeros<uint8_t>({8, 32});
}

bool Neureka::matrixvec_exit_idx() {
  if(this->mv_k_out_iter == this->mv_k_out_lim-1 && this->mv_qw_iter == this->mv_qw_lim-1) {
    return true;
  }
  else {
    return false;
  }
}

void Neureka::matrixvec_update_idx() {
  if(this->mv_qw_iter == this->mv_qw_lim-1) {
    this->mv_qw_iter = 0;
    this->mv_k_out_iter++;
  }
  else {
    this->mv_qw_iter++;
  }
}

bool Neureka::matrixvec_to_matrixvec_idx() {
  if((this->dw_iter == this->dw_lim-1)) {
    return true;
  }
  else {
    return false;
  }
}

bool Neureka::matrixvec_to_load_idx() {
  auto k_in_major_lim = this->depthwise ? 1 : this->subtile_nb_ki;
  if(this->k_in_major_iter == k_in_major_lim-1) {
    return true;
  }
  else {
    return false;
  }
}
