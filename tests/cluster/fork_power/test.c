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
#include "stdio.h"

#include "pmsis/cluster/cluster_team/cl_team.h"

#define NB_CORES 8


static void pe_entry(void *arg)
{

  for (int i=0; i<10; i++)
  {
    for (int i=0; i<(8+1)*100; i++)
    {
      // This shoudl increase dynamic power on l1 bank0
      *(volatile int *)0x10000000;
    }
    //pi_cl_team_barrier();
  }


}


static void cluster_entry(void *arg)
{
  // These 2 special accesses defines the windows where power consumption is captured.
  // This is the start command to start capturing.
  *(volatile int *)0x10000000 = 0xabbaabba;
  pi_cl_team_fork(NB_CORES, pe_entry, NULL);
  // This is the stop command to stop capturing and dump results to a file.
  // This will dump the results in power_report.csv in the build folder.
  // Calling several times start/stop will dump several reports in the file.
  *(volatile int *)0x10000000 = 0xdeadcaca;
}


static int test_task_sync()
{
  struct pi_device cluster_dev;
  struct pi_cluster_conf conf;
  struct pi_cluster_task task;
  int errors = 0;

  pi_cluster_conf_init(&conf);
  conf.id = 0;

  pi_open_from_conf(&cluster_dev, &conf);

  pi_cluster_open(&cluster_dev);

  pi_freq_set(PI_FREQ_DOMAIN_CL, 400000000);

    pi_cluster_task(&task, &cluster_entry, (void *)&errors);
    pi_cluster_send_task_to_cl(&cluster_dev, &task);

  pi_cluster_close(&cluster_dev);

  return errors;
}


int test_entry()
{
  int errors = 0;

  printf("Starting test\n");

  errors += test_task_sync();

  if (errors)
    printf("Test failure\n");
  else
    printf("Test success\n");

  return errors;
}


void test_kickoff(void *arg)
{
  int ret = test_entry();
  pmsis_exit(ret);
}


int main()
{
  return pmsis_kickoff((void *)test_kickoff);
}
