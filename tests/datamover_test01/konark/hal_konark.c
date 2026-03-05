// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Authors: Sergio Mazzola <smazzola@iis.ee.ethz.ch>

#include <stdint.h>

#include "konark/hal_konark.h"
#include "konark/hal_cim.h"
#include "konark/hal_datamover.h"

/////////////
// Drivers //
/////////////

void konark_hwpe_subsystem_clk_en_reg_write(konark_hwpe_cg_mode_t cg_mode) {
  // cg_mode = Clock gating mode to set (refer to `konark_hwpe_cg_mode_t`)
  KONARK_HWPE_SUBSYSTEM_REG_WRITE(HWPE_SUBSYSTEM_CLK_EN_REG_OFFSET, cg_mode);
}

konark_hwpe_cg_mode_t konark_hwpe_subsystem_clk_en_reg_read(void) {
  uint32_t cg_mode;
  cg_mode = KONARK_HWPE_SUBSYSTEM_REG_READ(HWPE_SUBSYSTEM_CLK_EN_REG_OFFSET);
  return (konark_hwpe_cg_mode_t)cg_mode;
}

void konark_hwpe_subsystem_evt_clr_reg_write(uint8_t core_id) {
  // core_id = ID of the core to clear the HWPE event for
  KONARK_HWPE_SUBSYSTEM_REG_WRITE(HWPE_SUBSYSTEM_EVT_CLR_REG_OFFSET, core_id);
}

/////////
// HAL //
/////////

/* HWPEs Clock enable/disable */

void konark_hwpe_cim_clk_enable(void) {
  konark_hwpe_cg_mode_t current_cg_mode;
  current_cg_mode = konark_hwpe_subsystem_clk_en_reg_read();
  konark_hwpe_subsystem_clk_en_reg_write((konark_hwpe_cg_mode_t)(current_cg_mode | CLK_EN_CIM));
}

void konark_hwpe_cim_clk_disable(void) {
  konark_hwpe_cg_mode_t current_cg_mode;
  current_cg_mode = konark_hwpe_subsystem_clk_en_reg_read();
  konark_hwpe_subsystem_clk_en_reg_write((konark_hwpe_cg_mode_t)(current_cg_mode & ~CLK_EN_CIM));
}

void konark_hwpe_datamover_clk_enable(void) {
  konark_hwpe_cg_mode_t current_cg_mode;
  current_cg_mode = konark_hwpe_subsystem_clk_en_reg_read();
  konark_hwpe_subsystem_clk_en_reg_write((konark_hwpe_cg_mode_t)(current_cg_mode | CLK_EN_DATAMOVER));
}

void konark_hwpe_datamover_clk_disable(void) {
  konark_hwpe_cg_mode_t current_cg_mode;
  current_cg_mode = konark_hwpe_subsystem_clk_en_reg_read();
  konark_hwpe_subsystem_clk_en_reg_write((konark_hwpe_cg_mode_t)(current_cg_mode & ~CLK_EN_DATAMOVER));
}

void konark_hwpe_enable_all_clk(void) {
  konark_hwpe_subsystem_clk_en_reg_write(CLK_EN_ALL);
}

void konark_hwpe_disable_all_clk(void) {
  konark_hwpe_subsystem_clk_en_reg_write(CLK_EN_NONE);
}

/* Datamover */

void konark_datamover_init(void) {
  // konark_hwpe_datamover_clk_enable();
  datamover_soft_clear();
}
