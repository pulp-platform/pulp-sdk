/*
 * Copyright (C) 2019 GreenWaves Technologies
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 *
 * Authors: Germain Haugou, GreenWaves Technologies (germain.haugou@greenwaves-technologies.com)
 */

/*
* With additional modifications:
* Copyright (C) 2021 University of Bologna, ETH Zurich
*
* * Licensed under the Apache License, Version 2.0 (the "License");
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
*
* Authors: Nazareno Bruschi, UniBo (nazareno.bruschi@unibo.it)
*/

#include "pmsis.h"
#include <bsp/bsp.h>

#define HYPER_FLASH 0
#define SPI_FLASH   1
#define MRAM        2

#define BUFF_SIZE 32
#define PROGRAM_SIZE_RTL   BUFF_SIZE
#define PROGRAM_SIZE_OTHER ((1<<18)*4)

#define NB_ITER 2

#define UDMA_HYPERBUS_OFFSET (0x1A100000 + ARCHI_UDMA_OFFSET + 128*9)
#define HYPERBUS_DEVICE_NUM 8
#define CONFIG_REG_OFFSET 0x80

//#define PROGRAM

static inline int udma_hyper_busy(unsigned int tran_id){
  return pulp_read32(UDMA_HYPERBUS_OFFSET + tran_id*CONFIG_REG_OFFSET + 0x24) & 0x00000001;
}

static inline void get_info(unsigned int *program_size)
{
  *program_size = BUFF_SIZE;
#if defined(ARCHI_PLATFORM_RTL)
  if (rt_platform() == ARCHI_PLATFORM_RTL)
  {
    *program_size = PROGRAM_SIZE_RTL;
  }
  else
  {
    *program_size = PROGRAM_SIZE_OTHER;
  }
#else
  *program_size = PROGRAM_SIZE_OTHER;
#endif  /* __PULP_OS__ */
}


static PI_L2 unsigned int rx_buffer[BUFF_SIZE];
static PI_L2 unsigned int tx_buffer[BUFF_SIZE];

static int test_entry()
{
  struct pi_device flash;

  struct pi_hyperflash_conf flash_conf;
  struct pi_flash_info flash_info;

  printf("Entering main controller\n");

  pi_hyperflash_conf_init(&flash_conf);

  pi_open_from_conf(&flash, &flash_conf);

  if (pi_flash_open(&flash))
    return -1;

  pi_flash_ioctl(&flash, PI_FLASH_IOCTL_INFO, (void *)&flash_info);

  uint32_t flash_addr = 0x40000;

  for (int j=0; j<NB_ITER; j++)
  {

    for (int i=0; i<BUFF_SIZE; i++) {
        tx_buffer[i] = i  + (0xffff<<16)*j;
        rx_buffer[i] = 0;
    }

    #ifdef PROGRAM
    pi_flash_bwrite(&flash, flash_addr, tx_buffer, BUFF_SIZE*2);

    for(volatile int p=0; p<5000; p++);

    #endif

    pi_flash_read(&flash, flash_addr, rx_buffer, BUFF_SIZE*4);

    for (int i=0; i<BUFF_SIZE; i++)
    {
      printf("It %d , addr %x, expected 0x%08x, got 0x%08x\n", j, flash_addr + (i*4), tx_buffer[i], rx_buffer[i]);
    }
    flash_addr = flash_addr + (BUFF_SIZE * 4);
  }

  pi_flash_close(&flash);


  return 0;
}

static void test_kickoff(void *arg)
{
  int ret = test_entry();
  pmsis_exit(ret);
}

int main()
{
  return pmsis_kickoff((void *)test_kickoff);
}
