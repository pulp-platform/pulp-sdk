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

#include "pmsis.h"
#include "hal/bitfield.h"


/**
 * Global variable declaration of current clock domain frequencies. The runtime
 * currently makes use of thi variable to calculate the correct divider settings
 * for peripherals.
 */
int pos_freq_domains[PI_FREQ_NB_DOMAINS];


pll_t pll_get_handle(pll_clock_domain_e clock_domain) {
  pll_t handle;
  handle.clock_domain = clock_domain;
  switch (clock_domain) {
  case PLL_SOC_DOMAIN:
    handle.base_addr = ARCHI_SOC_PERIPHERALS_ADDR + ARCHI_SOC_PLL_OFFSET;
    return handle;
  case PLL_CLUSTER_DOMAIN:
    handle.base_addr = ARCHI_SOC_PERIPHERALS_ADDR + ARCHI_CLUSTER_PLL_OFFSET;
    return handle;
  case PLL_PER_DOMAIN:
    handle.base_addr = ARCHI_SOC_PERIPHERALS_ADDR + ARCHI_PER_PLL_OFFSET;
    return handle;
  }
}

/**
 * Calculate multiplication and post division factors to generate the target frequency.
 *
 * @param target_freq_hz - The target frequency in unit Hertz
 * @param mult - A pointer to where the calculated freq. multiplication factor should be written to.
 * @param postdiv - A pointer to where the calculated output divider value should be written to.
 * @param vco_div2_enable - Enable VCO divide by two.
 * @return 0 if a usable combination of mut  and postdiv has been found. -1 otherwise.
 */
__attribute__((noinline)) static int pll_calc_freq_div_settings(uint32_t target_freq_hz, uint16_t* mult, uint8_t* postdiv, uint8_t* vco_div2_enable) {
  uint32_t temp_mult;
  //printf("Calculating settings for target freq: %d\n", target_freq_hz);
  for (int temp_div = 32; temp_div > 16; temp_div-=2) {
    temp_mult = target_freq_hz/PLL_REF_CLK_FREQ_HZ * temp_div;
    //printf("Temp mult: %d, temp_div: %d\n", temp_mult, temp_div);
    if (temp_mult > PLL_MIN_VCO_MULT && temp_mult < PLL_MAX_VCO_MULT) {
      *mult = temp_mult;
      *postdiv = temp_div/2-1;
      *vco_div2_enable = 1;
      return 0;
    }
  }
  for (int temp_div = 16; temp_div > 0; temp_div--) {
    temp_mult = target_freq_hz/PLL_REF_CLK_FREQ_HZ * temp_div;
    //printf("Temp mult: %d, temp_div: %d\n", temp_mult, temp_div);
    if (temp_mult > PLL_MIN_VCO_MULT && temp_mult < PLL_MAX_VCO_MULT) {
      *mult = temp_mult;
      *postdiv = temp_div-1;
      *vco_div2_enable = 0;
      return 0;
    }
  }
  //printf("Runtime failed to generate frequency of %d Hz\n", target_freq_hz);
  return -1;
}


/**
 * Calculate multiplication and post division factors to generate the target frequency.
 *
 * @param target_freq_hz - The target frequency in unit Hertz
 * @param mult - A pointer to where the calculated freq. multiplication factor should be written to.
 * @param postdiv - A pointer to where the calculated output divider value should be written to.
 * @param vco_div2_enable - Enable VCO divide by two.
 * @return 0 if a usable combination of mut  and postdiv has been found. -1 otherwise.
 */
__attribute__((noinline)) static int pll_calc_freq_div_settings_lp(uint32_t target_freq_hz, uint16_t* mult, uint8_t* postdiv, uint8_t* vco_div2_enable) {
  uint32_t temp_mult;
  //printf("Calculating settings for target freq: %d\n", target_freq_hz);
  for (int temp_div = 1; temp_div < 16; temp_div++) {
    temp_mult = target_freq_hz/PLL_REF_CLK_FREQ_HZ * temp_div;
    //printf("Temp mult: %d, temp_div: %d\n", temp_mult, temp_div);
    if (temp_mult > PLL_MIN_VCO_MULT && temp_mult < PLL_MAX_VCO_MULT) {
      *mult = temp_mult;
      *postdiv = temp_div-1;
      *vco_div2_enable = 0;
      return 0;
    }
  }
  for (int temp_div = 16; temp_div <= 32; temp_div+=2) {
    temp_mult = target_freq_hz/PLL_REF_CLK_FREQ_HZ * temp_div;
    //printf("Temp mult: %d, temp_div: %d\n", temp_mult, temp_div);
    if (temp_mult > PLL_MIN_VCO_MULT && temp_mult < PLL_MAX_VCO_MULT) {
      *mult = temp_mult;
      *postdiv = temp_div/2-1;
      *vco_div2_enable = 1;
      return 0;
    }
  }
  //printf("Failed to generate frequency of %d Hz\n", target_freq_hz);
  return -1;
}


unsigned int pll_init(pll_t* pll) {
    uint32_t reg = 0;

    // Configure Config1 register
    reg = bitfield_bit32_write(reg, PLL_CONFIG_CFG1_PLLEN_BIT, 1);
    reg = bitfield_bit32_write(reg, PLL_CONFIG_CFG1_FOUTPOSTDIVEN_BIT, 1);
    reg = bitfield_bit32_write(reg, PLL_CONFIG_CFG1_FOUTVCOEN_BIT, 0);
    reg = bitfield_field32_write(reg, PLL_CONFIG_CFG1_LOCKCOUNT_FIELD, PLL_LOCK_COUNT_16CYCLS);
    reg = bitfield_bit32_write(reg, PLL_CONFIG_CFG1_LOCKGATE_BIT, 1);
    pulp_write32(pll->base_addr+PLL_CONFIG_CFG1_REG_OFFSET, reg);


    // Configure Config3 register - Calibration
    reg = 0; // We use auto calibration during initialization
    pulp_write32(pll->base_addr+PLL_CONFIG_CFG3_REG_OFFSET, reg);

    // Configure Config2 register
    reg = 0;
    reg = bitfield_field32_write(reg, PLL_CONFIG_CFG2_FBDIV_FIELD, 6106); // 100 MHz VCO output clock
    reg = bitfield_bit32_write(reg, PLL_CONFIG_CFG2_VCODIVSEL_BIT, 1);
    reg = bitfield_field32_write(reg, PLL_CONFIG_CFG2_POSTDIV_FIELD, 0);
    reg = bitfield_bit32_write(reg, PLL_CONFIG_CFG2_FAILSAFE_EN_BIT, 1);
    reg = bitfield_field32_write(reg, PLL_CONFIG_CFG2_FREQ_CHANGE_MASK_CNT_FIELD, 32);
    pulp_write32(pll->base_addr+PLL_CONFIG_CFG2_REG_OFFSET, reg);
    return 6106 * PLL_REF_CLK_FREQ_HZ/2;
  }

__attribute__((noinline)) unsigned int pll_set_freq(pll_t* pll, int unsigned freq_hz, pll_calibration_t* opt_calibration_input) {
   uint8_t div_sel = 0;
   uint16_t mult_sel = 0;
   uint8_t vco_div2_enable = 0;

   uint8_t failure = pll_calc_freq_div_settings_lp(freq_hz, &mult_sel, &div_sel, &vco_div2_enable);
   if (!failure) {
     uint32_t reg = 0;
     reg = bitfield_bit32_write(reg, PLL_CONFIG_CFG2_VCODIVSEL_BIT, vco_div2_enable);
     reg = bitfield_field32_write(reg, PLL_CONFIG_CFG2_FBDIV_FIELD, mult_sel);
     reg = bitfield_field32_write(reg, PLL_CONFIG_CFG2_POSTDIV_FIELD, div_sel);
     reg = bitfield_bit32_write(reg, PLL_CONFIG_CFG2_FAILSAFE_EN_BIT, 1);
     reg = bitfield_field32_write(reg, PLL_CONFIG_CFG2_FREQ_CHANGE_MASK_CNT_FIELD, 12);
     pulp_write32(pll->base_addr+PLL_CONFIG_CFG2_REG_OFFSET, reg);
     return mult_sel*PLL_REF_CLK_FREQ_HZ/(div_sel+1)/(vco_div2_enable+1);
   } else {
     return 0;
   }
 }

void pll_set_lock_mode(pll_t* pll, uint8_t enable_clock_gate) {
  uint32_t reg = 0;
  reg = pulp_read32(pll->base_addr+PLL_CONFIG_CFG1_REG_OFFSET);
  reg = bitfield_bit32_write(reg, PLL_CONFIG_CFG1_LOCKGATE_BIT, enable_clock_gate);
  pulp_write32(pll->base_addr+PLL_CONFIG_CFG1_REG_OFFSET, reg);
}

void pll_set_power(pll_t* pll, uint8_t enable) {
  uint32_t reg = 0;
  reg = pulp_read32(pll->base_addr+PLL_CONFIG_CFG1_REG_OFFSET);
  reg = bitfield_bit32_write(reg, PLL_CONFIG_CFG1_PLLEN_BIT, enable);
  pulp_write32(pll->base_addr+PLL_CONFIG_CFG1_REG_OFFSET, reg);
}

pll_status_t pll_get_status(pll_t* pll) {
  uint32_t reg = 0;
  pll_status_t status;
  reg = pulp_read32(pll->base_addr+PLL_CONFIG_STATUS_REG_OFFSET);
  status.is_locked = bitfield_bit32_read(reg, PLL_CONFIG_STATUS_LOCK_BIT);
  status.calibration_output_code = bitfield_field32_read(reg, PLL_CONFIG_STATUS_FCALOUT_FIELD);
  reg = pulp_read32(pll->base_addr+PLL_CONFIG_CFG2_REG_OFFSET);
  int mult = bitfield_field32_read(reg, PLL_CONFIG_CFG2_FBDIV_FIELD);
  int postdiv  = bitfield_field32_read(reg, PLL_CONFIG_CFG2_POSTDIV_FIELD);
  uint8_t vcodiv = bitfield_bit32_read(reg, PLL_CONFIG_CFG2_VCODIVSEL_BIT);
  status.pll_freq_hz = mult * PLL_REF_CLK_FREQ_HZ/(postdiv+1)/(vcodiv+1);
  return status;
}

unsigned int pos_fll_set_freq(int fll, unsigned int frequency) {
  pll_t handle;
  switch (fll) {
  case POS_FLL_FC:
    handle = pll_get_handle(PLL_SOC_DOMAIN);
    break;
  case POS_FLL_CL:
    handle = pll_get_handle(PLL_CLUSTER_DOMAIN);
    break;
  case POS_FLL_PERIPH:
    handle = pll_get_handle(PLL_PER_DOMAIN);
    break;
  }
  return pll_set_freq(&handle, frequency, NULL);
}


unsigned int pos_fll_init(int fll) {
  pll_t handle;
  switch (fll) {
  case POS_FLL_FC:
    handle = pll_get_handle(PLL_SOC_DOMAIN);
    //printf("Init soc clock. Base addr: %x\n", handle.base_addr);
    break;
  case POS_FLL_CL:
    handle = pll_get_handle(PLL_CLUSTER_DOMAIN);
    //printf("Init cluster clock. Base addr: %x\n", handle.base_addr);
    break;
  case POS_FLL_PERIPH:
    handle = pll_get_handle(PLL_PER_DOMAIN);
    //printf("Init peripheral clock. Base addr: %x\n", handle.base_addr);
    break;
  }
  return pll_init(&handle);
}
