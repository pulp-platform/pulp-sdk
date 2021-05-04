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

#define BUFF_SIZE 2048
#define PROGRAM_SIZE_RTL   BUFF_SIZE
#define PROGRAM_SIZE_OTHER ((1<<18)*4)

#define NB_ITER 2


static inline void get_info(unsigned int *program_size)
{
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


static PI_L2 unsigned char rx_buffer[BUFF_SIZE];
static PI_L2 unsigned char tx_buffer[BUFF_SIZE];

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

  for (int j=0; j<NB_ITER; j++)
  {
    // The beginning of the flash may contain runtime data such as the boot binary.
    // Round-up the flash start with the sector size to not erase it.
    // Also add small offset to better test erase (sector size aligned) and program (512 byte aligned).
    uint32_t flash_addr = ((flash_info.flash_start + flash_info.sector_size - 1) & ~(flash_info.sector_size - 1)) + 128;

    int size;
    get_info(&size);

    pi_flash_erase(&flash, flash_addr, size);

    while(size > 0)
    {
      for (int i=0; i<BUFF_SIZE; i++)
      {
        tx_buffer[i] = i*(j+1);
        rx_buffer[i] = 0;
      }

      pi_flash_bwrite(&flash, flash_addr, tx_buffer, BUFF_SIZE);
      pi_flash_read(&flash, flash_addr, rx_buffer, BUFF_SIZE);

      for (int i=0; i<BUFF_SIZE; i++)
      {
          if (rx_buffer[i] != (unsigned char)(i*(j+1)))
          {
            printf("Error at index %d, expected 0x%2.2x, got 0x%2.2x\n", i, (unsigned char)i, rx_buffer[i]);
            printf("TEST FAILURE\n");
            return -2;
          }
      }
      size -= BUFF_SIZE;
      flash_addr += BUFF_SIZE;
    }
  }

  pi_flash_close(&flash);

  printf("TEST SUCCESS\n");

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
