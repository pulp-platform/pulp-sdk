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

void Rnnacc_v1::clear_all() {
  this->addr_b   = 0;
  this->addr_x   = 0;
  this->addr_wx  = 0;
  this->addr_h   = 0;
  this->addr_wh  = 0;
  this->addr_dst = 0;
  this->n_input  = 0;
  this->n_output = 0;

  this->bias      = 0;
  this->matmul    = 0;
  this->twomatmul = 0;
  this->multi_job = 0;

  // this->multijob_counter = 0;
  // this->matmul_state = false;

  // this->clear_buf_x();
  // this->clear_buf_h();
  // this->clear_buf_accum();
}

void Rnnacc_v1::clear_all_buf() {
  this->clear_buf_x();
  this->clear_buf_h();
  this->clear_buf_accum();}

void Rnnacc_v1::clear_buf_x() {
  xt::view(this->buf_x, xt::all()) = 0;
}

void Rnnacc_v1::clear_buf_h() {
  xt::view(this->buf_h, xt::all()) = 0;
}

void Rnnacc_v1::clear_buf_accum() {
  printf("CLEAR BUF_ACCUM");
  xt::view(this->buf_accum, xt::all()) = 0;
  xt::view(this->buf_w,     xt::all()) = 0;
}
