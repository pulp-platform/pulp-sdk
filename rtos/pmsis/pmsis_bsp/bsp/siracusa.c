/*
 * Copyright (C) 2019 GreenWaves Technologies
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

#include "bsp/bsp.h"
#include "bsp/siracusa.h"
#include "bsp/camera/himax.h"
#include "bsp/flash/hyperflash.h"
#include "bsp/ram/hyperram.h"
#include "bsp/ram/spiram.h"
#include "bsp/eeprom/24xx1025.h"
#include "bsp/eeprom/virtual_eeprom.h"

static int __bsp_init_pads_done = 0;

static void __bsp_init_pads()
{
  if (!__bsp_init_pads_done)
  {
    __bsp_init_pads_done = 1;
  }
}


void bsp_hyperram_conf_init(struct pi_hyperram_conf *conf)
{
  conf->ram_start = CONFIG_HYPERRAM_START;
  conf->ram_size = CONFIG_HYPERRAM_SIZE;
  conf->skip_pads_config = 0;
  conf->hyper_itf = CONFIG_HYPERRAM_HYPER_ITF;
  conf->hyper_cs = CONFIG_HYPERRAM_HYPER_CS;
}


int bsp_hyperram_open(struct pi_hyperram_conf *conf)
{
  __bsp_init_pads();
  return 0;
}



void bsp_hyperflash_conf_init(struct pi_hyperflash_conf *conf)
{
  conf->hyper_itf = CONFIG_HYPERFLASH_HYPER_ITF;
  conf->hyper_cs = CONFIG_HYPERFLASH_HYPER_CS;
}

int bsp_hyperflash_open(struct pi_hyperflash_conf *conf)
{
  __bsp_init_pads();
  return 0;
}



void bsp_init()
{
}


void pi_bsp_init_profile(int profile)
{
}



void pi_bsp_init()
{
  pi_bsp_init_profile(PI_BSP_PROFILE_DEFAULT);
}
