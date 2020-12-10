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
 * Authors: Andreas Traber
 */


#include "gdb-server.hpp"

#include <stdio.h>
#include <stdlib.h>

#define INSN_IS_COMPRESSED(instr) ((instr & 0x3) != 0x3)
#define INSN_BP_COMPRESSED   0x9002
#define INSN_BP              0x00100073

Breakpoints::Breakpoints(Gdb_server *top)
: top(top) {
}

bool
Breakpoints::insert(unsigned int addr) {
  bool retval;
  uint32_t data_bp;
  struct bp_insn bp;

  bp.addr = addr;
  retval = top->cable->access(false, addr, 4, (char*)&bp.insn_orig);
  bp.is_compressed = INSN_IS_COMPRESSED(bp.insn_orig);

  breakpoints.push_back(bp);

  if (bp.is_compressed) {
    data_bp = INSN_BP_COMPRESSED;
    retval = retval && top->cable->access(true, addr, 2, (char*)&data_bp);
  } else {
    data_bp = INSN_BP;
    retval = retval && top->cable->access(true, addr, 4, (char*)&data_bp);
  }

  this->top->target->flush();

  return retval;
}

bool
Breakpoints::remove(unsigned int addr) {

  bool retval;
  bool is_compressed;
  uint32_t data;
  for (std::list<struct bp_insn>::iterator it = breakpoints.begin(); it != breakpoints.end(); it++) {
    if (it->addr == addr) {
      data = it->insn_orig;
      is_compressed = it->is_compressed;

      breakpoints.erase(it);

      if (is_compressed)
        retval = top->cable->access(true, addr, 2, (char*)&data);
      else
        retval = top->cable->access(true, addr, 4, (char*)&data);

      this->top->target->flush();

      return retval;
    }
  }

  return false;
}

bool
Breakpoints::clear() {

  bool retval = this->disable_all();

  breakpoints.clear();

  return retval;
}


bool
Breakpoints::at_addr(unsigned int addr) {
  for (std::list<struct bp_insn>::iterator it = breakpoints.begin(); it != breakpoints.end(); it++) {
    if (it->addr == addr) {
      // we found our bp
      return true;
    }
  }

  return false;
}

bool
Breakpoints::enable(unsigned int addr) {
  bool retval;
  uint32_t data;

  for (std::list<struct bp_insn>::iterator it = breakpoints.begin(); it != breakpoints.end(); it++) {
    if (it->addr == addr) {
      if (it->is_compressed) {
        data = INSN_BP_COMPRESSED;
        retval = top->cable->access(1, addr, 2, (char*)&data);
      } else {
        data = INSN_BP;
        retval = top->cable->access(1, addr, 4, (char*)&data);
      }

      return true;
      //return retval && m_cache->flush();
    }
  }

  fprintf(stderr, "bp_enable: Did not find any bp at addr %08X\n", addr);

  return false;
}

bool
Breakpoints::disable(unsigned int addr) {
  bool retval;

  for (std::list<struct bp_insn>::iterator it = breakpoints.begin(); it != breakpoints.end(); it++) {
    if (it->addr == addr) {
      if (it->is_compressed)
        retval = top->cable->access(1, addr, 2, (char*)&it->insn_orig);
      else
        retval = top->cable->access(1, addr, 4, (char*)&it->insn_orig);

      return true;
      //return retval && m_cache->flush();
    }
  }

  fprintf(stderr, "bp_enable: Did not find any bp at addr %08X\n", addr);

  return false;
}

bool
Breakpoints::enable_all() {
  bool retval = true;

  for (std::list<struct bp_insn>::iterator it = breakpoints.begin(); it != breakpoints.end(); it++) {
    retval = retval && this->enable(it->addr);
  }

  return retval;
}

bool
Breakpoints::disable_all() {
  bool retval = true;

  for (std::list<struct bp_insn>::iterator it = breakpoints.begin(); it != breakpoints.end(); it++) {
    retval = retval && this->disable(it->addr);
  }

  return retval;
}
