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

/* 
 * Authors: Germain Haugou, GreenWaves Technologies (germain.haugou@greenwaves-technologies.com)
 */

#include "pmsis.h"
#include <soc.h>


void _start();


pos_cluster_t pos_cluster[ARCHI_NB_CLUSTER];

/*
 * Cluster tiny data
 * They are in tiny area for fast access from cluster side. As they local
 * they will be instantiated once for each cluster and are thus
 * naturally supporting multi cluster.
 */

PI_CL_L1_TINY pos_cluster_call_pool_t pos_cluster_pool;
PI_CL_L1_TINY int pos_cluster_nb_active_pe;
PI_CL_L1 char pos_cluster_exec_mode;

PI_CL_L1 uint32_t pos_cluster_fc_task_lock;


void pos_master_task_with_stack(void *arg);


void pi_cluster_conf_init(struct pi_cluster_conf *conf)
{
    conf->id = 0;
    conf->flags = PI_CLUSTER_FLAGS_FORK_BASED;
}


static void pos_init_cluster_data(int cid, struct pi_cluster_conf *conf)
{

    // In case the chip does not support L1 preloading, the initial L1 data are in L2, we need to copy them to L1
    int *l1_preload_start = (int *)pos_cluster_tiny_addr(cid, (void *)&_l1_preload_start);
    int *l1_preload_start_inL2 = (int *)&_l1_preload_start_inL2;
    int l1_preload_size = (int)&_l1_preload_size;

    CL_TRACE(POS_LOG_INFO, "L1 preloading data copy from L2 to L1 (L2 start: 0x%x, L2 end: 0x%x, L1 start: 0x%x)\n", (int)l1_preload_start_inL2, (int)l1_preload_start_inL2 + (int)l1_preload_size, (int)l1_preload_start);
    for (; l1_preload_size > 0; l1_preload_size-=4, l1_preload_start++, l1_preload_start_inL2++) {
        *l1_preload_start = *l1_preload_start_inL2;
    }

    int nb_cluster = pos_nb_cluster();

    pos_cluster[cid].cid = cid;
    pos_cluster[cid].task_trig_addr = eu_evt_trig_cluster_addr(cid, POS_EVENT_CLUSTER_CALL_EVT);
    pos_cluster[cid].stacks = NULL;
    pos_cluster[cid].trig_addr = eu_evt_trig_cluster_addr(cid, POS_EVENT_CLUSTER_CALL_EVT);
    pos_cluster[cid].pool = (pos_cluster_call_pool_t *)pos_cluster_tiny_addr(cid, &pos_cluster_pool);
    pos_cluster[cid].cl_tasks = NULL;
    pos_cluster[cid].cluster_exec_mode = conf->flags;
    pos_cluster[cid].stack_set = 0;

    pos_cluster_exec_mode = conf->flags;
}


static int pos_cluster_init()
{
  pos_irq_set_handler(POS_EVENT_FC_ENQUEUE, pos_task_remote_enqueue);
  
  pos_irq_mask_set(1<<POS_EVENT_FC_ENQUEUE);

  return 0;
}


int pi_cluster_open(struct pi_device *cluster_dev)
{
    struct pi_cluster_conf *conf = (struct pi_cluster_conf *)cluster_dev->config;
    int cid = conf->id;
    pos_cluster_t *cluster = &pos_cluster[cid];

    CL_TRACE(POS_LOG_INFO, "Opening cluster (id: %d)\n", cid);

    cluster_dev->data = (void *)cluster;
    cluster->cid = cid;

#if __PLATFORM__ != ARCHI_PLATFORM_FPGA
    pos_pmu_cluster_power_up();
#endif

    pos_cluster_init();

    pos_cluster_call_pool_t *pool = (pos_cluster_call_pool_t *)pos_cluster_tiny_addr(cid, &pos_cluster_pool);

    cluster->last_call_fc = NULL;
    pool->first_call_fc_for_cl = NULL;
    pool->first_pe_task = NULL;
    pool->first_call_from_pe = NULL;

    pos_cluster_fc_task_lock = 0;

#if __PLATFORM__ != ARCHI_PLATFORM_FPGA && !defined(SKIP_PLL_INIT)
    /* { */
    /*     // Setup FLL */
    /*     int init_freq = pos_fll_init(POS_FLL_CL); */

    /*     // Check if we have to restore the cluster frequency */
    /*     // otherwise just set it to the one returned by the fll */
    /*     int freq = pi_freq_get(PI_FREQ_DOMAIN_CL); */

    /*     if (freq) */
    /*     { */
    /*         pi_freq_set(PI_FREQ_DOMAIN_CL, freq); */
    /*     } */
    /*     else */
    /*     { */
    /*         pos_freq_set_value(PI_FREQ_DOMAIN_CL, init_freq); */
    /*     } */
    /* } */
#endif

    /* Activate cluster top level clock gating */
    cluster_ctrl_unit_clock_gate_set(ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(cid) + ARCHI_CLUSTER_CTRL_OFFSET, 1);

    // Initialize cluster global variables
    pos_init_cluster_data(cid, conf);

    // Initialize cluster L1 memory allocator
    pos_alloc_init_l1(cid);

    // Activate icache
    cluster_icache_ctrl_enable_set(ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(cid) + ARCHI_ICACHE_CTRL_OFFSET, 0xFFFFFFFF);

    // Fetch all cores, they will directly jump to the PE loop waiting from orders through the dispatcher
    for (int i=0; i<pi_cl_cluster_nb_pe_cores(); i++) 
    {
      GAP_WRITE(ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(cid) + ARCHI_CLUSTER_CTRL_OFFSET, CLUSTER_CTRL_UNIT_BOOT_ADDR0_OFFSET + i*4, (int)_start);
    }

    uint32_t core_mask = (1<<pi_cl_cluster_nb_pe_cores()) - 1;

#ifdef ARCHI_CC_CORE_ID
    core_mask |= 1 << ARCHI_CC_CORE_ID;
    GAP_WRITE(ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(cid) + ARCHI_CLUSTER_CTRL_OFFSET, CLUSTER_CTRL_UNIT_BOOT_ADDR0_OFFSET + ARCHI_CC_CORE_ID*4, (int)_start);
#endif

    cluster_ctrl_unit_fetch_en_set(ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(cid) + ARCHI_CLUSTER_CTRL_OFFSET, core_mask);

#ifdef CONFIG_PE_TASK
    if (cluster->cluster_exec_mode == PI_CLUSTER_FLAGS_TASK_BASED)
    {
        struct pi_cluster_task cluster_task;

        pi_cluster_send_task_to_cl(
            cluster_dev,
            pi_cluster_task(&cluster_task, pos_master_task_with_stack, &cluster_task)
        );
    }
#endif

    CL_TRACE(POS_LOG_INFO, "Cluster opened (id: %d)\n", cid);

    return 0;
}


int pi_cluster_close(struct pi_device *cluster_dev)
{
    return 0;
}


int __attribute__((noinline)) pos_cluster_task_set_stack(struct pi_device *device, struct pi_cluster_task *task)
{
    pos_cluster_t *data = (pos_cluster_t *)device->data;

    data->stack_set = 1;

    if (task->stack_size == 0)
    {
        task->stack_size = 0x800;
        task->slave_stack_size = 0x400;
    }

    if (task->slave_stack_size == 0)
        task->slave_stack_size = task->stack_size;

    int stacks_size = task->stack_size + task->slave_stack_size * (task->nb_cores - 1);

    if (data->stacks == NULL || stacks_size != data->stacks_size)
    {
        if (data->stacks)
            pi_cl_l1_free(device, data->stacks, data->stacks_size);

        data->stacks_size = stacks_size;
        data->stacks = pi_cl_l1_malloc(device, stacks_size);

        if (data->stacks == NULL)
            return -1;
    }

    task->stacks = data->stacks;

    return 0;
}

int pi_cluster_send_task_to_cl_async(struct pi_device *device, struct pi_cluster_task *task, pi_task_t *async_task)
{
    pos_cluster_t *data = (pos_cluster_t *)device->data;

    pos_cluster_call_pool_t *cl_data = data->pool;

    if (likely(data->cluster_exec_mode == PI_CLUSTER_FLAGS_TASK_BASED))
    {
      if (unlikely(task->stacks == NULL && !data->stack_set))
      {
          if (pos_cluster_task_set_stack(device, task))
              return -1;
      }
    }
    else
    {
        if (unlikely(task->stacks == NULL))
        {
            int res = pos_cluster_task_set_stack(device, task);
            if (res)
                return -1;
        }

  #ifdef ARCHI_HAS_CC
        task->core_mask = (1<<(task->nb_cores-1)) - 1;
  #else
        task->core_mask = (1<<task->nb_cores) - 1;
  #endif
    }

    task->next = NULL;
    task->completion_callback = async_task;

    hal_compiler_barrier();

    int irq = hal_irq_disable();

    if (data->last_call_fc)
    {
        data->last_call_fc->next = task;
    }

    data->last_call_fc = task;

    hal_compiler_barrier();
    
    if (cl_data->first_call_fc_for_cl == NULL)
        cl_data->first_call_fc_for_cl = task;

    hal_compiler_barrier();
    eu_evt_trig(data->task_trig_addr, 0);

    hal_irq_restore(irq);

    return 0;
}

int pi_cluster_send_tasklet_to_cl_async(struct pi_device *device, struct pi_cluster_task *task, pi_task_t *async_task)
{
    pos_cluster_t *data = (pos_cluster_t *)device->data;

    pos_cluster_call_pool_t *cl_data = data->pool;

    task->next = NULL;
    task->completion_callback = async_task;

    hal_compiler_barrier();

    int irq = hal_irq_disable();

    if (data->last_call_fc)
    {
        data->last_call_fc->next = task;
    }

    data->last_call_fc = task;

    hal_compiler_barrier();
    
    if (cl_data->first_call_fc_for_cl == NULL)
        cl_data->first_call_fc_for_cl = task;

    hal_compiler_barrier();
    eu_evt_trig(data->task_trig_addr, 0);

    hal_irq_restore(irq);

    return 0;
}


int pi_cluster_send_task_to_cl(struct pi_device *device, struct pi_cluster_task *task)
{
    pi_task_t fc_task;

    pi_task_block(&fc_task);

    if (pi_cluster_send_task_to_cl_async(device, task, &fc_task))
    {
        return -1;
    }

    pi_task_wait_on(&fc_task);

    return 0;
}


void pos_cluster_push_fc_event(pi_task_t *event)
{
    eu_mutex_lock_from_id(0);

    pos_cluster_t *data = &pos_cluster[hal_cluster_id()];

    // First wait until the slot to post events is free
    while(*(volatile pi_task_t **)&(data->cl_tasks) != NULL)
    {
        eu_mutex_unlock_from_id(0);
        eu_evt_maskWaitAndClr(1<<POS_EVENT_CLUSTER_CALL_EVT);
        eu_mutex_lock_from_id(0);
    }

    // Push the event and notify the FC with a HW evet in case it
    // is sleeping
    data->cl_tasks = event;
    #ifdef ITC_VERSION
    hal_itc_status_set(1<<POS_EVENT_FC_ENQUEUE);
    #else
    eu_evt_trig(eu_evt_trig_fc_addr(POS_EVENT_FC_ENQUEUE), 0);
    #endif

    eu_mutex_unlock_from_id(0);
}


extern void pos_cluster_task_slave_set_stack();



#ifdef CONFIG_PE_TASK

// This function is called in task-based programming model to set the stack of 
// all workers when the master receives a task from FC with stack information
void pos_cluster_task_set_stacks(struct pi_cluster_task *cluster_task, void *stacks, int stack_size, uint32_t nb_cores)
{
    pi_cluster_pe_task_t task;

    // Send stack information to all slave cores so that they can set their stack
    pi_cluster_pe_task_init(&task, pos_cluster_task_slave_set_stack);
    pi_cluster_pe_task_arg(&task, 0, (uint32_t)stacks);
    pi_cluster_pe_task_arg(&task, 1, (uint32_t)stack_size);
    pi_cluster_pe_task_nb_cores(&task, nb_cores - 1);

    pi_cluster_pe_task_push(&task);

    // Wait notification back, so that we can free the task structure on the stack
    pi_cluster_pe_task_wait(&task);

    pi_cl_cluster_task_terminate(cluster_task);
}

#endif