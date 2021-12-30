/*
 * Copyright 2020 GreenWaves Technologies
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
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __PI_FLL_H__
#define __PI_FLL_H__

#include <stdint.h>
#include "properties.h"
#include "memory_map.h"
#include "freq.h"

#define FLL_STATUS_OFFSET     0x000
#define FLL_CONF1_OFFSET      0x004
#define FLL_CONF2_OFFSET      0x008
#define FLL_INTEGRATOR_OFFSET 0x00C


/** FLL_CTRL - Registers Layout Typedef */
typedef struct {
	volatile uint32_t FLL_STATUS;	  /**< FLL_CTRL Status register, offset:
					     0x00 */
	volatile uint32_t FLL_CONF1;	  /**< FLL_CTRL Configuration1 register,
					     offset: 0x04 */
	volatile uint32_t FLL_CONF2;	  /**< FLL_CTRL Configuration2 register,
					     offset: 0x08 */
	volatile uint32_t FLL_INTEGRATOR; /**< FLL_CTRL INTEGRATOR register,
					     offset: 0x0C */
} fll_ctrl_t;


typedef enum _fll_type {
	FLL_SOC = 0,
	FLL_PERI = 1,
	FLL_CLUSTER = 2
} fll_type_t;

/*! @name FLL_STATUS - FLL_CTRL status register */
#define FLL_CTRL_STATUS_MULTI_FACTOR_MASK  (0xFFFFU)
#define FLL_CTRL_STATUS_MULTI_FACTOR_SHIFT (0U)

/*! @name SOC_CONF1 - FLL_CTRL configuration 1 register */
#define FLL_CTRL_CONF1_MULTI_FACTOR_MASK  (0xFFFFU)
#define FLL_CTRL_CONF1_MULTI_FACTOR_SHIFT (0U)

#define FLL_CTRL_CONF1_DCO_INPUT_MASK  (0x3FF0000U)
#define FLL_CTRL_CONF1_DCO_INPUT_SHIFT (16U)

#define FLL_CTRL_CONF1_CLK_OUT_DIV_MASK	 (0x3C000000U)
#define FLL_CTRL_CONF1_CLK_OUT_DIV_SHIFT (26U)

#define FLL_CTRL_CONF1_OUTPUT_LOCK_EN_MASK  (0x40000000U)
#define FLL_CTRL_CONF1_OUTPUT_LOCK_EN_SHIFT (30U)

#define FLL_CTRL_CONF1_MODE_MASK  (0x80000000U)
#define FLL_CTRL_CONF1_MODE_SHIFT (31U)

/*! @name SOC_CONF2 - FLL_CTRL configuration 2 register */
#define FLL_CTRL_CONF2_LOOPGAIN_MASK  (0xFU)
#define FLL_CTRL_CONF2_LOOPGAIN_SHIFT (0U)

#define FLL_CTRL_CONF2_DEASSERT_CYCLES_MASK  (0x3F0U)
#define FLL_CTRL_CONF2_DEASSERT_CYCLES_SHIFT (4U)

#define FLL_CTRL_CONF2_ASSERT_CYCLES_MASK  (0xFC00U)
#define FLL_CTRL_CONF2_ASSERT_CYCLES_SHIFT (10U)

#define FLL_CTRL_CONF2_LOCK_TOLERANCE_MASK  (0xFFF0000U)
#define FLL_CTRL_CONF2_LOCK_TOLERANCE_SHIFT (16U)

#define FLL_CTRL_CONF2_CONF_CLK_SEL_MASK  (0x20000000U)
#define FLL_CTRL_CONF2_CONF_CLK_SEL_SHIFT (29U)

#define FLL_CTRL_CONF2_OPEN_LOOP_MASK  (0x40000000U)
#define FLL_CTRL_CONF2_OPEN_LOOP_SHIFT (30U)

#define FLL_CTRL_CONF2_DITHERING_MASK  (0x80000000U)
#define FLL_CTRL_CONF2_DITHERING_SHIFT (31U)

/*! @name SOC_INTEGRATOR - FLL_CTRL configuration 2 register */
#define FLL_CTRL_INTEGRATOR_FRACT_PART_MASK  (0xFFC0U)
#define FLL_CTRL_INTEGRATOR_FRACT_PART_SHIFT (6U)

#define FLL_CTRL_INTEGRATOR_INT_PART_MASK  (0x3FF0000U)
#define FLL_CTRL_INTEGRATOR_INT_PART_SHIFT (16U)

/*! @name FLL_CONVERGE - FLL_CTRL configuration 2 register */
#define FLL_CTRL_SOC_FLL_CONV_MASK  (0x1U)
#define FLL_CTRL_SOC_FLL_CONV_SHIFT (0U)

#define FLL_CTRL_CLUSTER_FLL_CONV_MASK	(0x2U)
#define FLL_CTRL_CLUSTER_FLL_CONV_SHIFT (1U)


/* The number of FLL */
#define FLL_NUM ARCHI_NB_FLL
/* The FLL reference frequency*/
#define FLL_REF_CLK ARCHI_REF_CLOCK


/* FLL_CTRL - Peripheral instance base addresses */
/** Peripheral FLL_CTRL base address */
#define FLL_CTRL_BASE (SOC_PERIPHERALS_ADDR)
/** Peripheral FLL_CTRL base pointer */
#define FLL_CTRL ((fll_ctrl_t *)FLL_CTRL_BASE)
/** Array initializer of FLL_CTRL base addresses */
#define FLL_CTRL_BASE_ADDRS                                                    \
	{                                                                      \
		FLL_CTRL_BASE                                                  \
	}
/** Array initializer of FLL_CTRL base pointers */
#define FLL_CTRL_BASE_PTRS                                                     \
	{                                                                      \
		FLL_CTRL                                                       \
	}


#define DCDC_OPER_POINTS (4)

#define DCDC_DEFAULT_NV	 (1200)
#define DCDC_DEFAULT_MV	 (1200)
#define DCDC_DEFAULT_LV	 (1000)
#define DCDC_DEFAULT_RET (800)
#define DCDC_RANGE	 (5)
#define DCDC_RANGE_MASK	 (0x1F)
#define DCDC_BASE_VALUE	 (550)
#define DCDC_STEP	 (50)

#define MAX_DCDC_VARIATION ((int32_t)(0.1 * 32767))

#define FLL_LV_MAX_FREQUENCY	  150000000
#define FLL_NV_MAX_FREQUENCY	  250000000
#define FLL_SOC_MIN_FREQUENCY	  150000000
#define FLL_SOC_MAX_FREQUENCY	  250000000
#define FLL_CLUSTER_MIN_FREQUENCY 87000000
#define FLL_CLUSTER_MAX_FREQUENCY 175000000

#define FLL_SOC_FV_SLOPE                                                       \
	((FLL_SOC_MAX_FREQUENCY - FLL_SOC_MIN_FREQUENCY) /                     \
	 (DCDC_DEFAULT_NV - DCDC_DEFAULT_LV))
#define FLL_CLUSTER_FV_SLOPE                                                   \
	((FLL_CLUSTER_MAX_FREQUENCY - FLL_CLUSTER_MIN_FREQUENCY) /             \
	 (DCDC_DEFAULT_NV - DCDC_DEFAULT_LV))


/*!
 * @brief Initialize one FLL.
 *
 * @param which_fll       SoC's or Cluster's fll.
 * @param ret_state       Retention state.
 *
 * @note .
 */
void pi_fll_init(fll_type_t which_fll, uint32_t ret_state);

/*!
 * @brief Deinitalize one FLL.
 *
 * @param which_fll       SoC's or Cluster's fll.
 *
 * @note .
 */
void pi_fll_deinit(fll_type_t which_fll);

/*!
 * @brief Clean all FLL configuration.
 *
 * @note .
 */
void pi_fll_clear();


/*!
 * @brief Set specific FLL to wanted frequency.
 *
 * @param which_fll       SoC's or Cluster's fll.
 * @param frequency       The frequency value to set.
 * @param check           Check frequency.
 *
 * @note .
 * @return check result of frequency.
 */
int pi_fll_set_frequency(fll_type_t which_fll, uint32_t frequency, int check);

/*!
 * @brief Get specific FLL's frequency.
 *
 * @param which_fll       SoC's or Cluster's fll.
 *
 * @note .
 * @return frequency value.
 */
int pi_fll_get_frequency(fll_type_t which_fll, uint8_t real);

/*!
 * @brief Calculate FC SOC domain's max frequency with certain voltage
 *
 * @param  voltage  Given voltage
 *
 * @return max frquency.
 */
static inline int pi_fll_soc_max_freq_at_V(int voltage)
{
	return (FLL_SOC_MIN_FREQUENCY +
		(voltage - DCDC_DEFAULT_LV) * FLL_SOC_FV_SLOPE);
}

/*!
 * @brief Calculate cluster domain's max frequency with certain voltage
 *
 * @param  voltage  Given voltage
 *
 * @return max frquency.
 */
static inline int pi_fll_cluster_max_freq_at_V(int voltage)
{
	return (FLL_CLUSTER_MIN_FREQUENCY +
		(voltage - DCDC_DEFAULT_LV) * FLL_CLUSTER_FV_SLOPE);
}



#endif /* __FLL_H__ */
