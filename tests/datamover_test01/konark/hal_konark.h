// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Authors: Sergio Mazzola <smazzola@iis.ee.ethz.ch>

#ifndef __HAL_KONARK_H__
#define __HAL_KONARK_H__

#include <stdint.h>

#include "konark_cluster_raw_addrmap.h"

///////////
// Archi //
///////////

#define HWPE_SUBSYSTEM_BASE_ADDR KONARK_CLUSTER_ADDRMAP_CLUSTER_HWPE_SUBSYSTEM_BASE_ADDR

#define HWPE_SUBSYSTEM_CLK_EN_REG_OFFSET  KONARK_CLUSTER_ADDRMAP_CLUSTER_HWPE_SUBSYSTEM_CLK_EN_REG_OFFSET
#define HWPE_SUBSYSTEM_EVT_CLR_REG_OFFSET KONARK_CLUSTER_ADDRMAP_CLUSTER_HWPE_SUBSYSTEM_EVT_CLR_REG_OFFSET

/////////////
// Defines //
/////////////

#define __HAL_KONARK_REG_WRITE(base, offset, value) *(volatile int *)(base + offset) = value
#define __HAL_KONARK_REG_READ(base, offset)         *(volatile int *)(base + offset)

#define KONARK_HWPE_SUBSYSTEM_REG_WRITE(offset, value) __HAL_KONARK_REG_WRITE(HWPE_SUBSYSTEM_BASE_ADDR, offset, value)
#define KONARK_HWPE_SUBSYSTEM_REG_READ(offset)         __HAL_KONARK_REG_READ(HWPE_SUBSYSTEM_BASE_ADDR, offset)

///////////
// Types //
///////////

typedef enum {
  CLK_EN_NONE      = 0x00,
  CLK_EN_CIM       = 0x01,
  CLK_EN_DATAMOVER = 0x02,
  CLK_EN_ALL       = 0x03
} konark_hwpe_cg_mode_t;

////////////////
// Prototypes //
////////////////

// Drivers
void konark_hwpe_subsystem_clk_en_reg_write(konark_hwpe_cg_mode_t cg_mode);
konark_hwpe_cg_mode_t konark_hwpe_subsystem_clk_en_reg_read(void);
void konark_hwpe_subsystem_evt_clr_reg_write(uint8_t core_id);

// HAL
void konark_hwpe_cim_clk_enable(void);
void konark_hwpe_cim_clk_disable(void);
void konark_hwpe_datamover_clk_enable(void);
void konark_hwpe_datamover_clk_disable(void);
void konark_hwpe_enable_all_clk(void);
void konark_hwpe_disable_all_clk(void);
// Datamover
void konark_datamover_init(void);

#endif // __HAL_KONARK_H__
