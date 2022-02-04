/* 
 * Copyright (C) 2017 ETH Zurich, University of Bologna and GreenWaves Technologies
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 *
 * Authors: Germain Haugou, ETH (germain.haugou@iis.ee.ethz.ch)
 */

#include "pmsis.h"


#define NB_PRIV_DATA 16

PI_L2_DATA_PRIV int my_static_priv_data[NB_PRIV_DATA] = {0};

static int test_entry()
{
    int * my_dynamic_priv_data = pi_priv2_malloc(NB_PRIV_DATA * sizeof(int));

    int * pointer_to_bigger_l2_addr = (int *)0x1c090000;

    *(int *)pointer_to_bigger_l2_addr = 0x12345678;

    for(int i=0; i<NB_PRIV_DATA; i++)
    {
      my_static_priv_data[i]  = i;
      my_dynamic_priv_data[i] = NB_PRIV_DATA - i;
    }

    printf("Shared L2 acces -> %x, %p\n", *(int *)pointer_to_bigger_l2_addr, pointer_to_bigger_l2_addr);

    for(int i=0; i<NB_PRIV_DATA; i++)
    {
      printf("[%d]: PRIV2 static allocation: %x, PRIV2 dynamic allocation: %x\n", i, my_static_priv_data[i], my_dynamic_priv_data[i]);
    }

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
