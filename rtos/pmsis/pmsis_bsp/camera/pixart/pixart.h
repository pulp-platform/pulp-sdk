/*
* Copyright (C) 2019 GreenWaves Technologies
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*
* Authors: Germain Haugou, GreenWaves Technologies (germain.haugou@greenwaves-technologies.com)
*/

// File CIS_PIXART.h

#ifndef __CIS_PIXART_H
#define __CIS_PIXART_H   // avoid recursive inclusions

#define PIXART_MCLK_FREQ                  6000000
#define PIXART_SPI_BAUDRATE               10000000

#define PIXART_TRIGGER_MODE               0
#define PIXART_CONTINUE_MODE              1

#define PIXART_CHIP_ID_REG_L              0x00
#define PIXART_CHIP_ID_REG_H              0x01
#define	PIXART_CHIP_ID                    0x6100

#define PIXART_UPDATE_REG                 0x00

#define PIXART_GLOBAL_RESET_REG           0x0C
#define PIXART_SOFT_RESET                 0x1D

#define PIXART_SWITCH_MODE_CTL1_REG       0x23
#define PIXART_SWITCH_MODE_CTL2_REG       0x24
#define PIXART_BANK_SELECT_REG            0x7F

#endif // ndef __CIS_PIXART_H


