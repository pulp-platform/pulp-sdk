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
 * Title: siracusa_padctrl.h
 * Author: Manuel Eggimann  <meggimann@iis.ee.ethz.ch>
 * Date: 25.08.2021
 *
 * Description: Control the pad configuration and multiplexing of the Siracusa chip
 */

#ifndef SIRACUSA_PADCTRL_H
#define SIRACUSA_PADCTRL_H

#include <stdint.h>
#include "siracusa_pads_functional_regs.h"

#define SIRACUSA_PADCTRL_PAD_CFG_REG_OFFSET 0x0
#define SIRACUSA_PADCTRL_PAD_MUX_SEL_REG_OFFSET 0x4
#define SIRACUSA_PADCTRL_PAD_REG_SEPARATION 0x8

typedef enum {
  DRV_STR_3mA,
  DRV_STR_6mA,
  DRV_STR_9mA,
  DRV_STR_12mA,
  DRV_STR_15mA,
  DRV_STR_18mA,
  DRV_STR_21mA,
  DRV_STR_24mA,
  DRV_STR_27mA,
  DRV_STR_30mA,
  DRV_STR_33mA,
  DRV_STR_36mA,
  DRV_STR_39mA,
  DRV_STR_42mA,
  DRV_STR_45mA,
  DRV_STR_48mA
} siracusa_padctrl_drv_strength_e;

typedef enum {
  NO_PULL, ///< No pull resistor is enabled
  PULL_UP_EN, ///< Enable internal pull-up resistor
  PULL_DOWN_EN ///< Enable internal pull-down resistor
} siracusa_padctrl_pull_cfg_e;

/**
 * Pad configuration struct
 *
 * Keep in mind, that certain peripherals might take over control of certain pad
 * config signals (e.g. rx_en) if connected to a pad. The settings you provide
 * through this API only take effect unless not overriden by the currently
 * connected peripheral.
 */
typedef struct {
  siracusa_padctrl_drv_strength_e drv_str; ///< Driving strength of the pad
  siracusa_padctrl_pull_cfg_e pull_cfg; ///< Pull-up/down settings of the pad
  uint8_t ret_en; ///< Retention mode enable
  uint8_t tx_en; ///< Tx enable
  uint8_t shm_trigg_en; ///< Schmitt-trigger enable
} siracusa_padctrl_cfg_t;

/**
 * Pad modes
 *
 * This enum defines all the different roles a particular IO pad can assume.
 * Keep in mind, that except for the GPIO role you must not assign the same role
 * to the more than one pad.
 */
typedef enum {
  PAD_MODE_DISABLED = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_REGISTER,
  PAD_MODE_GPIO = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_GPIO_GPIO00,
  PAD_MODE_I2C0_SCL = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_I2C0_SCL,
  PAD_MODE_I2C0_SDA = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_I2C0_SDA,
  PAD_MODE_I3C0_PUC = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_I3C0_PUC,
  PAD_MODE_I3C0_SCL = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_I3C0_SCL,
  PAD_MODE_I3C0_SDA = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_I3C0_SDA,
  PAD_MODE_I3C1_PUC = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_I3C1_PUC,
  PAD_MODE_I3C1_SCL = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_I3C1_SCL,
  PAD_MODE_I3C1_SDA = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_I3C1_SDA,
  PAD_MODE_QSPIM0_CSN0 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIM0_CSN0,
  PAD_MODE_QSPIM0_CSN1 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIM0_CSN1,
  PAD_MODE_QSPIM0_CSN2 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIM0_CSN2,
  PAD_MODE_QSPIM0_CSN3 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIM0_CSN3,
  PAD_MODE_QSPIM0_SCK = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIM0_SCK,
  PAD_MODE_QSPIM0_SDIO0 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIM0_SDIO0,
  PAD_MODE_QSPIM0_SDIO1 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIM0_SDIO1,
  PAD_MODE_QSPIM0_SDIO2 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIM0_SDIO2,
  PAD_MODE_QSPIM0_SDIO3 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIM0_SDIO3,
  PAD_MODE_QSPIS0_CSN = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIS0_CSN,
  PAD_MODE_QSPIS0_SCK = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIS0_SCK,
  PAD_MODE_QSPIS0_SDIO0 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIS0_SDIO0,
  PAD_MODE_QSPIS0_SDIO1 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIS0_SDIO1,
  PAD_MODE_QSPIS0_SDIO2 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIS0_SDIO2,
  PAD_MODE_QSPIS0_SDIO3 = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_QSPIS0_SDIO3,
  PAD_MODE_UART0_RX = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_UART0_RX,
  PAD_MODE_UART0_TX = SIRACUSA_PADS_FUNCTIONAL_CONFIG_PAD_GPIO00_MUX_SEL_PAD_GPIO00_MUX_SEL_VALUE_PORT_UART0_TX
} siracusa_padctrl_mode_e;

typedef enum {
  PAD_GPIO00,
  PAD_GPIO01,
  PAD_GPIO02,
  PAD_GPIO03,
  PAD_GPIO04,
  PAD_GPIO05,
  PAD_GPIO06,
  PAD_GPIO07,
  PAD_GPIO08,
  PAD_GPIO09,
  PAD_GPIO10,
  PAD_GPIO11,
  PAD_GPIO12,
  PAD_GPIO13,
  PAD_GPIO14,
  PAD_GPIO15,
  PAD_GPIO16,
  PAD_GPIO17,
  PAD_GPIO18,
  PAD_GPIO19,
  PAD_GPIO20,
  PAD_GPIO21,
  PAD_GPIO22,
  PAD_GPIO23,
  PAD_GPIO24,
  PAD_GPIO25,
  PAD_GPIO26,
  PAD_GPIO27,
  PAD_GPIO28,
  PAD_GPIO29,
  PAD_GPIO30,
  PAD_GPIO31,
  PAD_GPIO32,
  PAD_GPIO33,
  PAD_GPIO34,
  PAD_GPIO35,
  PAD_GPIO36,
  PAD_GPIO37,
  PAD_GPIO38,
  PAD_GPIO39,
  PAD_GPIO40,
  PAD_GPIO41,
  PAD_GPIO42,
  NUM_PADS
} siracusa_padctrl_pad_e;

/**
 * Change the pad configuration of the given pad
 *
 * @param pad The pad to configure
 * @param cfg [in] The configuration settings for the pad
 */
void padctrl_config_set(siracusa_padctrl_pad_e pad, siracusa_padctrl_cfg_t const *cfg);

/**
 * Reads the current pad configuration from the pad control peripheral
 *
 * @param pad The pad from which to read the current configuration
 * @param cfg [out] Pointer to the pad config structure where the obtained config will be written to
 */
void padctrl_config_get(siracusa_padctrl_pad_e pad, siracusa_padctrl_cfg_t *cfg);

/**
 * Change the pad mode (IO multiplex settings)
 *
 * This function reconfigures the pad multiplexer to route the given IO signal
 * to the pad. You must only route the same IO signal to one pad. E.g. you must
 * not put PAD_GPIO04 in mode PAD_MODE_I3C0_SCL while PAD_GPIO01 is already in
 * the same mode. The only exception to this rule is the PAD_MODE_GPIO. You can
 * put all pads into this mode simultaneously to have access to up to NUM_PADS
 * GPIOs.
 *
 * @param pad The pad for which the pad multiplex setting should be changed
 */
void padctrl_mode_set(siracusa_padctrl_pad_e pad, siracusa_padctrl_mode_e mode);

/**
 * Obtain the current mode of a pad
 *
 * @param pad The pad for which to return the current pad mode
 * @returns The current mode of the pad
 */
siracusa_padctrl_mode_e padctrl_mode_get(siracusa_padctrl_pad_e pad);


#endif /* SIRACUSA_PADCTRL_H */
