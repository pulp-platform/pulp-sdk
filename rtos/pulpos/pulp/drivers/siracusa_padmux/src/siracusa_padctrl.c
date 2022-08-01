/*
 * Copyright (C) 2021 ETH Zurich, University of Bologna
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
 *
 * Title: siracusa_padctrl.c
 * Author: Manuel Eggimann  <meggimann@iis.ee.ethz.ch>
 * Date: 25.08.2021
 *
 * Description: Implementation for Siracusa's pad control IP driver
 */

#include "siracusa_padctrl.h"
#include "hal/pulp.h"

void padctrl_config_set(siracusa_padctrl_pad_e pad, siracusa_padctrl_cfg_t const *cfg) {
  const bitfield_field32_t drv_str_field = {.mask = 0xf, .index = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_DRV0_BIT};
  const uint32_t cfg_addr = ARCHI_PADCTRL_ADDR + pad*SIRACUSA_PADCTRL_PAD_REG_SEPARATION + SIRACUSA_PADCTRL_PAD_CFG_REG_OFFSET;
  uint32_t reg = pulp_read32(cfg_addr);
  reg = bitfield_bit32_write(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_TX_EN_BIT, cfg->tx_en != 0);
  reg = bitfield_bit32_write(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_PULL_EN_BIT, cfg->pull_cfg != NO_PULL);
  reg = bitfield_bit32_write(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_PULL_SEL_BIT, cfg->pull_cfg == PULL_UP_EN);
  reg = bitfield_bit32_write(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_RET_EN_BIT, cfg->ret_en != 0);
  reg = bitfield_bit32_write(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_ST_EN_BIT, cfg->shm_trigg_en != 0);
  reg = bitfield_field32_write(reg, drv_str_field, cfg->drv_str);
  pulp_write32(cfg_addr, reg);
}

void padctrl_config_get(siracusa_padctrl_pad_e pad, siracusa_padctrl_cfg_t *cfg) {
  const uint32_t cfg_addr = ARCHI_PADCTRL_ADDR + pad*SIRACUSA_PADCTRL_PAD_REG_SEPARATION + SIRACUSA_PADCTRL_PAD_CFG_REG_OFFSET;
  const bitfield_field32_t drv_str_field = {.mask = 0xf, .index = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_DRV0_BIT};
  const uint32_t reg = pulp_read32(cfg_addr);
  uint8_t pull_en;
  uint8_t pull_sel;
  cfg->drv_str = bitfield_field32_read(reg, drv_str_field);
  cfg->tx_en = bitfield_bit32_read(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_TX_EN_BIT);
  cfg->shm_trigg_en = bitfield_bit32_read(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_ST_EN_BIT);
  cfg->ret_en = bitfield_bit32_read(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_RET_EN_BIT);
  pull_en = bitfield_bit32_read(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_PULL_EN_BIT);
  pull_sel = bitfield_bit32_read(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_CFG_PULL_SEL_BIT);
  cfg->pull_cfg = pull_en? (pull_sel? PULL_UP_EN : PULL_DOWN_EN) : NO_PULL;
}

void padctrl_mode_set(siracusa_padctrl_pad_e pad, siracusa_padctrl_mode_e mode) {
  const uint32_t cfg_addr = ARCHI_PADCTRL_ADDR + pad*SIRACUSA_PADCTRL_PAD_REG_SEPARATION + SIRACUSA_PADCTRL_PAD_MUX_SEL_REG_OFFSET;
  uint32_t reg = pulp_read32(cfg_addr);
  reg = bitfield_field32_write(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_FIELD, mode);
  pulp_write32(cfg_addr, reg);
}

siracusa_padctrl_mode_e padctrl_mode_get(siracusa_padctrl_pad_e pad) {
  const uint32_t cfg_addr = ARCHI_PADCTRL_ADDR + pad*SIRACUSA_PADCTRL_PAD_REG_SEPARATION + SIRACUSA_PADCTRL_PAD_MUX_SEL_REG_OFFSET;
  const uint32_t reg = pulp_read32(cfg_addr);
  return bitfield_field32_read(reg, SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_FIELD);
}
