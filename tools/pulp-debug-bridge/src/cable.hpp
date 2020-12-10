/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
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
 * Authors: Germain Haugou, ETH (germain.haugou@iis.ee.ethz.ch)
 */

#ifndef __CABLES_CABLE_HPP__
#define __CABLES_CABLE_HPP__

#include "json.hpp"
#include "cables/log.h"


class Cable_jtag_itf
{
public:

  virtual bool bit_inout(char* inbit, char outbit, bool last) { printf ("i am bit_inout virtual fct in cable class\n"); return false; }

  virtual bool stream_inout(char* instream, char* outstream, unsigned int n_bits, bool last) { printf ("i am stream_inout virtual fct in cable class\n"); return false; }

  virtual int flush() { return -1; }
  virtual bool jtag_reset(bool active) { printf("JTAG\n"); return false; }

  virtual void device_select(unsigned int i) {}

  bool jtag_soft_reset();
  bool jtag_write_tms(int val);
  bool jtag_shift_ir();
  bool jtag_shift_dr();
  bool jtag_idle();
  bool jtag_shift(int width, char *bits);
  bool jtag_shift_ir(unsigned int ir, int ir_len=-1);
  bool jtag_set_reg(unsigned int reg, int width, unsigned int value, int ir_len=-1);
  bool jtag_get_reg(unsigned int reg, int width, unsigned int *out_value, unsigned int value, int ir_len=-1);
};



class Cable_io_itf
{
public:
  virtual bool access(bool write, unsigned int addr, int size, char* buffer, int device=-1) { return false; }
  virtual bool reg_access(bool write, unsigned int addr, char* buffer, int device=-1) { return false; }
};



class Cable_ctrl_itf
{
public:
  virtual bool chip_reset(bool active, int duration) { return false; }
  virtual bool chip_config(uint32_t config) { return false; }
};



class Cable : public Cable_io_itf, public Cable_jtag_itf, public Cable_ctrl_itf
{
public:
  Cable(js::config *config) : config(config) {}

  virtual bool connect(js::config *config) { return true; }

  virtual void lock() { }

  virtual void unlock() { }

  js::config *get_config() { return this->config; }

protected:  
  js::config *config;

};

#endif
