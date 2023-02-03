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
#include "pll_regs.h"

#define PLL_REF_CLK_FREQ_HZ 32768
#define PLL_MIN_VCO_MULT 3052
#define PLL_MAX_VCO_MULT 15500
//#define PLL_MAX_VCO_MULT 18000

/**
 * Enum of available clock domains
 */
typedef enum {
  PLL_SOC_DOMAIN = 0,     /**<PLL that generate the clock for the entire SoC domain
                      including the fabric controller and SoC peripherals */
  PLL_CLUSTER_DOMAIN = 1, /**< PLL that generates the clock for the PULP cluster
                      including NE16, MRAM and other modules situated in the
                      cluster */
  PLL_PER_DOMAIN = 2 /**< PLL that generates the clock signal used to clock the IO
                 side of peripherals e.g. SPI clock is derived from this PLL. */
} pll_clock_domain_e;

typedef enum {
  PLL_LOCK_COUNT_8CYCLS = 0, /**< 8 Cycles */
  PLL_LOCK_COUNT_16CYCLS = 1, /**< 16 Cycles */
  PLL_LOCK_COUNT_32CYCLS = 2, /**< 32 Cycles */
  PLL_LOCK_COUNT_64CYCLS = 3, /**< 64 Cycles */
} pll_lock_count_e;

/**
 * Handle to a PLL. You can obtain such a handle using the
 */
typedef struct pll {
  uint32_t base_addr;
  pll_clock_domain_e clock_domain;
} pll_t;

typedef uint8_t pll_calibration_t;

/**
 * Output satatus of a PLL
 */
typedef struct pll_status{
  uint8_t is_locked; /**< Indicates whether the PLL is currently locked or not.*/
  pll_calibration_t calibration_output_code; /**< The calibration output code. Can be used to speedup locking if supplied to pll_set_freq(). */
  uint32_t pll_freq_hz; /**< The PLL frequency in unit Hz */
} pll_status_t;


/**
 * Return a handle to the PLL that controls the provided clock_domain.
 *
 * @param clock_domain - The clock domain for which a handle shall be returned.
 *
 * @return The handle to the provided clock_domain.
 */
pll_t pll_get_handle(pll_clock_domain_e clock_domain);

/**
 * Initialize the selected PLL
 *
 * The function will initialize the given PLL with reasonable default values and
 * returns the programmed frequency. This functions causes the
 *
 * @param pll - Handle to the PLL to initialize
 * @return The programmed default frequency in unit Hz.
 */
unsigned int pll_init(pll_t* pll);

/**
 * Change the PLL frequency
 *
 * Change the PLLs multiplication factor to generate the desired frequency. If
 * opt_calibration_input is null, the frequency is changed using auto
 * calibration. If a pointer to a calibration code is supplied, the frequency
 * change is performed bypassing internal calibration using the supplied calibration code.
 *
 * @param pll - Handle to the PLL to configure
 * @param freq_hz - The target frequency
 * @param opt_calibration_input - Optional pointer to calibration input value (not implemented yet)
 * @return The actually programmed frequency (should be close to the freq_hz) or 0 if the desired frequency cannot be generated.
 */
unsigned int pll_set_freq(pll_t* pll, int unsigned freq_hz, pll_calibration_t* opt_calibration_input);

/**
 * Enable/Disable the PLLs lock-clock gate
 *
 * The PLLs can optionally clock gate the output clock until the PLL is locked.
 * This function enables or disables the feature.
 * @param pll - Handle to the PLL to configure
 */
void pll_set_lock_mode(pll_t* pll, uint8_t enable_clock_gate);

/**
 * Power on/off the PLL
 *
 * This function enables or disables an entire PLL. Make sure not to disable the
 * SoC PLL. Otherwise you will no longer be able to run code on the fabric controller.
 */
void pll_set_power(pll_t* pll,  uint8_t enable);

/**
 * Return the current status of the PLL.
 *
 * @param pll - Handle to the PLL
 * @return The current status of the give PLL.
 */
pll_status_t pll_get_status(pll_t* pll);

/**
 * PULP runtime wrapper function.
 *
 * This function is used in the pulp runtime during startup intialitation. It
 * just maps to the pll_set_freq function for Siracusa.
 */
unsigned int pos_fll_set_freq(int fll, unsigned int frequency);

/**
 * PULP runtime wrapper function.
 *
 * This function is used in the pulp runtime during startup intialitation. It
 * just maps to the pll_init function for Siracusa.
 */
unsigned int pos_fll_init(int fll);
