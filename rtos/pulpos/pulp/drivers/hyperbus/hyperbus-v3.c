/*
 * Copyright (C) 2020 ETH Zurich and University of Bologna
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
 * Authors: Nazareno Bruschi, Unibo (nazareno.bruschi@unibo.it)
 */

#include "pmsis.h"

static PI_L2 pos_hyper_t pos_hyper[ARCHI_UDMA_NB_HYPER];
static PI_L2 pos_hyper_cs_t pos_hyper_cs[HYPER_NB_CS * ARCHI_UDMA_NB_HYPER];
static PI_L2 int hyper_open_count=0;
static PI_L2 int hyper_channel;
static PI_L2 pos_udma_channel_t hyper_tx_channel;
static PI_L2 pos_udma_channel_t hyper_rx_channel;

#define HYPER_PENDING_READ       0
#define HYPER_PENDING_READ_2D    1
#define HYPER_PENDING_WRITE      2
#define HYPER_PENDING_WRITE_2D   3
#define HYPER_PENDING_READ_BI2D  4
#define HYPER_PENDING_WRITE_BI2D 5

static void __pi_hyper_read_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t arg0, uint32_t arg1, uint32_t arg2);
static void __pi_hyper_read_2d_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);
static void __pi_hyper_write_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t arg0, uint32_t arg1, uint32_t arg2);
static void __pi_hyper_write_2d_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);
static void __pi_hyper_read_bi2d_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5);
static void __pi_hyper_write_bi2d_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5);

static void (*__pi_hyper_exec_handlers[])(pos_hyper_t *hyper, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) =
{
  (void (*)(pos_hyper_t *hyper, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))__pi_hyper_read_exec,
  (void (*)(pos_hyper_t *hyper, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))__pi_hyper_read_2d_exec,
  (void (*)(pos_hyper_t *hyper, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))__pi_hyper_write_exec,
  (void (*)(pos_hyper_t *hyper, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))__pi_hyper_write_2d_exec,
  (void (*)(pos_hyper_t *hyper, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))__pi_hyper_read_bi2d_exec,
  (void (*)(pos_hyper_t *hyper, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))__pi_hyper_write_bi2d_exec,
};

static __attribute__((noinline)) void __pi_hyper_enqueue_pending(pos_hyper_t *hyper, uint32_t arg0,
    uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5, uint32_t arg6, pi_task_t *task, uint32_t id)
{
  task->data[0] = id;
  task->data[1] = arg0;
  task->data[2] = arg1;
  task->data[3] = arg2;
  task->data[4] = arg3;
  task->data[5] = arg4;
  task->data[6] = arg5;
  task->data[7] = arg6;

  hyper->waiting_last->next = task;
  hyper->waiting_last = task;
  task->next = NULL;
}

static inline __attribute__((always_inline)) void __pi_hyper_add_first(pos_hyper_t *hyper, pi_task_t *task)
{
  hyper->waiting_first = task;
  hyper->waiting_last = task;
  task->next = NULL;
}

static void pos_hyper_setup(pos_hyper_t *hyper, int cs)
{
  if (hyper->type == PI_HYPER_TYPE_FLASH)
  {
    plp_hyper_setup(hyper->hyper_id, CONFIG_HYPERFLASH_EN_LATENCY_ADD, cs, CONFIG_HYPERFLASH_T_LATENCY_ACCESS);
    plp_hyper_set_reg(UDMA_HYPER_BASE_ADDR(0) + REG_PAGE_BOUND, 0x04);
  }
  else if (hyper->type == PI_HYPER_TYPE_RAM)
  {
    plp_hyper_setup(hyper->hyper_id, CONFIG_HYPERRAM_EN_LATENCY_ADD, cs, CONFIG_HYPERRAM_T_LATENCY_ACCESS);
    plp_hyper_set_reg(UDMA_HYPER_BASE_ADDR(0) + REG_PAGE_BOUND, 0x03);
  }
}

static void pos_hyper_wait_done(pos_hyper_t *hyper)
{
  while (plp_udma_busy(UDMA_HYPER_TX_ADDR(hyper->hyper_id)) & plp_udma_busy(UDMA_HYPER_RX_ADDR(hyper->hyper_id)))
  {
    pos_irq_wait_for_interrupt();
  }

  for(int i=0; i<HYPER_NB_CHANNELS; i++)
  {
    plp_hyper_wait(hyper->hyper_id, i);
  }
}

void pos_hyper_handle_copy(int event, void *arg)
{
  pos_hyper_t *hyper = (pos_hyper_t *)arg;

  pi_task_t *task = hyper->waiting_first;

  if (task)
  {
    pi_task_t *next = task->next;
    hyper->waiting_first = next;

    if (next)
    {
      __pi_hyper_exec_handlers[next->data[0]](hyper, next->data[1], next->data[2], next->data[3],
        next->data[4], next->data[5], next->data[6], next->data[7]);
    }

    pos_task_push_locked(task);
  }
}

void pi_hyper_conf_init(struct pi_hyper_conf *conf)
{
  conf->id = 0;
}

int32_t pi_hyper_open(struct pi_device *device)
{
  int irq = hal_irq_disable();

  struct pi_hyper_conf *conf = (struct pi_hyper_conf *)device->config;

  unsigned char hyper_id = conf->id;
  int periph_id = ARCHI_UDMA_HYPER_ID(hyper_id);
  hyper_channel = UDMA_EVENT_ID(periph_id);
  int cs = conf->cs;

  pos_hyper_t *hyper = &pos_hyper[hyper_id];
  pos_hyper_cs_t *hyper_cs = &pos_hyper_cs[hyper_id + cs];

  hyper_open_count++;

  if (hyper_open_count == 1)
  {
    hyper->waiting_first = NULL;
    hyper->hyper_id = conf->id;
    hyper->type = conf->type;
    hyper->tran_id = 0;

    hyper->rx_channel = &hyper_rx_channel;
    hyper->tx_channel = &hyper_tx_channel;

    pos_soc_event_register_callback(hyper_channel + ARCHI_UDMA_HYPER_EOT_RX_EVT, pos_hyper_handle_copy, (void *)hyper);
    pos_soc_event_register_callback(hyper_channel + ARCHI_UDMA_HYPER_EOT_TX_EVT, pos_hyper_handle_copy, (void *)hyper);

    plp_udma_cg_set(plp_udma_cg_get() | (1<<periph_id));

    soc_eu_fcEventMask_setEvent(hyper_channel+ ARCHI_UDMA_HYPER_EOT_RX_EVT);
    soc_eu_fcEventMask_setEvent(hyper_channel + ARCHI_UDMA_HYPER_EOT_TX_EVT);
  }

  hyper_cs->cs = cs;
  hyper_cs->hyper = hyper;

  device->data = (void *)hyper_cs;

  hal_irq_restore(irq);

  return 0;
}

void pi_hyper_close(struct pi_device *device)
{
  int irq = hal_irq_disable();

  pos_hyper_cs_t *hyper_cs = (pos_hyper_cs_t *)device->data;
  pos_hyper_t *hyper = hyper_cs->hyper;

  struct pi_hyper_conf *conf = (struct pi_hyper_conf *)device->config;
  unsigned char hyper_id = conf->id;
  int periph_id = ARCHI_UDMA_HYPER_ID(hyper_id);

  hyper_open_count--;

  if (hyper_open_count == 0)
  {
    pos_hyper_wait_done(hyper);
    plp_udma_cg_set(plp_udma_cg_get() & ~(1<<periph_id));
  }

  hal_irq_restore(irq);
}

int pi_hyper_ioctl(struct pi_device *device, uint32_t cmd, void *arg)
{
  switch (cmd)
  {
    case PI_HYPER_IOCTL_SET_TRAN_ID:
    {
      uint32_t tran_id = *(uint32_t *)arg;
      if (tran_id >= HYPER_NB_CHANNELS)
        return -1;
      else
      {
        pos_hyper_cs_t *hyper_cs = (pos_hyper_cs_t *)device->data;
        pos_hyper_t *hyper = hyper_cs->hyper;

        hyper->tran_id = tran_id;
      }
      break;
    }
  }
  return 0;
}

static void __pi_hyper_set_regs(pos_hyper_t *hyper, uint32_t cmd, void *arg)
{
  uint32_t offset = UDMA_HYPER_BASE_ADDR(hyper->hyper_id);

  switch (cmd)
  {
    case PI_HYPER_T_LATENCY_ACC:
      offset += REG_T_LATENCY_ACCESS;
      break;
    case PI_HYPER_EN_LATENCY_ADD:
      offset += REG_EN_LATENCY_ADD;
      break;
    case PI_HYPER_T_CS_MAX:
      offset += REG_T_CS_MAX;
      break;
    case PI_HYPER_MEM_SEL:
      offset += MEM_SEL;
      break;
    default:
      break;
  }

  switch(cmd)
  {
    case PI_HYPER_CFG:
      plp_hyper_set_cfg(hyper->hyper_id, *(unsigned short *)arg, hyper->tran_id);
      break;
    case PI_HYPER_TWD:
      plp_hyper_set_twd_param(hyper->hyper_id, (unsigned int *)arg, hyper->tran_id);
      break;
    case PI_HYPER_PAGEBOUND:
      plp_hyper_set_pagebound(hyper->hyper_id, *(unsigned int *)arg);
      break;
    case PI_HYPER_CTL:
      plp_hyper_set_ctl_param(hyper->hyper_id, (unsigned int *)arg, hyper->tran_id);
      break;
    default:
      plp_hyper_set_reg(offset, *(unsigned int *)arg);
      break;
  }
}

void pi_hyper_set_regs(struct pi_device *device, uint32_t cmd, void *arg)
{
  pos_hyper_cs_t *hyper_cs = (pos_hyper_cs_t *)device->data;
  pos_hyper_t *hyper = hyper_cs->hyper;

  __pi_hyper_set_regs(hyper, cmd, arg);
}

int pi_hyper_ctl_status_regs(struct pi_device *device, uint32_t cmd, void *arg)
{
  pos_hyper_cs_t *hyper_cs = (pos_hyper_cs_t *)device->data;
  pos_hyper_t *hyper = hyper_cs->hyper;

  if (arg != NULL)
  {
    hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

    pi_hyper_write(device, cmd, arg, 4);
    return 0;
  }
  else
  {
    int value;
    pi_hyper_read(device, cmd, &value, 4);
    return value;
  }
  return 0;
}

void pi_hyper_read(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size)
{
  pi_task_t task;
  pi_hyper_read_async(device, hyper_addr, addr, size, pi_task_block(&task));
  pi_task_wait_on(&task);
}

static void __pi_hyper_read_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t hyper_addr, uint32_t addr,
   uint32_t size)
{
  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {0,0,0,0,0,0};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x5, hyper_addr};

  pos_hyper_setup(hyper, cs);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  __pi_hyper_set_regs(hyper, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  __pi_hyper_set_regs(hyper, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  plp_hyper_enqueue(UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_RX(hyper->tran_id),
      (uint32_t)addr, size, UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_read_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, struct pi_task *task)
{
  pos_hyper_cs_t *hyper_cs = (pos_hyper_cs_t *)device->data;
  pos_hyper_t *hyper = hyper_cs->hyper;

  if (hyper->waiting_first != NULL)
  {
      __pi_hyper_enqueue_pending(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size, 0, 0, 0, task, HYPER_PENDING_READ);
      return;
  }

  __pi_hyper_add_first(hyper, task);

  __pi_hyper_read_exec(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size);
}

void pi_hyper_read_2d(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t stride, uint32_t length)
{
  pi_task_t task;
  pi_hyper_read_2d_async(device, hyper_addr, addr, size, stride, length, pi_task_block(&task));
  pi_task_wait_on(&task);
}

static void __pi_hyper_read_2d_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t hyper_addr, uint32_t addr,
   uint32_t size, uint32_t stride, uint32_t length)
{
  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {1,length,stride,0,length,stride};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x5, hyper_addr};

  pos_hyper_setup(hyper, cs);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  __pi_hyper_set_regs(hyper, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  __pi_hyper_set_regs(hyper, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  plp_hyper_enqueue(UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_RX(hyper->tran_id),
    (uint32_t)addr, size, UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_read_2d_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t stride, uint32_t length, struct pi_task *task)
{
  pos_hyper_cs_t *hyper_cs = (pos_hyper_cs_t *)device->data;
  pos_hyper_t *hyper = hyper_cs->hyper;

  if (hyper->waiting_first != NULL)
  {
    __pi_hyper_enqueue_pending(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size, stride, length, 0, task, HYPER_PENDING_READ_2D);
    return;
  }

  __pi_hyper_add_first(hyper, task);

  __pi_hyper_read_2d_exec(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size, stride, length);
}

static void __pi_hyper_read_bi2d_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t hyper_addr, uint32_t addr,
   uint32_t size, uint32_t dir, uint32_t stride, uint32_t length)
{
  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {((0x0^dir) & 0x1),length,stride,((0x1^dir) & 0x1),length,stride};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x5, hyper_addr};

  pos_hyper_setup(hyper, cs);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  __pi_hyper_set_regs(hyper, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  __pi_hyper_set_regs(hyper, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  plp_hyper_enqueue(UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_RX(hyper->tran_id),
      (uint32_t)addr, size, UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_read_bi2d_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t dir, uint32_t stride, uint32_t length, struct pi_task *task)
{
  pos_hyper_cs_t *hyper_cs = (pos_hyper_cs_t *)device->data;
  pos_hyper_t *hyper = hyper_cs->hyper;

  if (hyper->waiting_first != NULL)
  {
    __pi_hyper_enqueue_pending(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size, dir, stride, length, task, HYPER_PENDING_READ_BI2D);
    return;
  }

  __pi_hyper_add_first(hyper, task);

  __pi_hyper_read_bi2d_exec(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size, dir, stride, length);
}

void pi_hyper_write(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size)
{
    pi_task_t task;
    pi_hyper_write_async(device, hyper_addr, addr, size, pi_task_block(&task));
    pi_task_wait_on(&task);
}

static void __pi_hyper_write_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t hyper_addr, uint32_t addr, uint32_t size)
{
  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {0,0,0,0,0,0};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x1, hyper_addr};

  pos_hyper_setup(hyper, cs);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  __pi_hyper_set_regs(hyper, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  __pi_hyper_set_regs(hyper, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  plp_hyper_enqueue(UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_TX(hyper->tran_id),
    (uint32_t)addr, size, UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_write_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, struct pi_task *task)
{
  pos_hyper_cs_t *hyper_cs = (pos_hyper_cs_t *)device->data;
  pos_hyper_t *hyper = hyper_cs->hyper;

  if (hyper->waiting_first != NULL)
  {
    __pi_hyper_enqueue_pending(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size, 0, 0, 0, task, HYPER_PENDING_WRITE);
    return;
  }

  __pi_hyper_add_first(hyper, task);

  __pi_hyper_write_exec(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size);
}

void pi_hyper_write_2d(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t stride, uint32_t length)
{
  pi_task_t task;
  pi_hyper_write_2d_async(device, hyper_addr, addr, size, stride, length, pi_task_block(&task));
  pi_task_wait_on(&task);
}

static void __pi_hyper_write_2d_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t hyper_addr, uint32_t addr,
   uint32_t size, uint32_t stride, uint32_t length)
{
  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {1,length,stride,0,length,stride};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x1, hyper_addr};

  pos_hyper_setup(hyper, cs);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  __pi_hyper_set_regs(hyper, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  __pi_hyper_set_regs(hyper, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  plp_hyper_enqueue(UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_TX(hyper->tran_id),
    (uint32_t)addr, size, UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_write_2d_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t stride, uint32_t length, struct pi_task *task)
{
  pos_hyper_cs_t *hyper_cs = (pos_hyper_cs_t *)device->data;
  pos_hyper_t *hyper = hyper_cs->hyper;

  if (hyper->waiting_first != NULL)
  {
    __pi_hyper_enqueue_pending(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size, stride, length, 0, task, HYPER_PENDING_WRITE_2D);
    return;
  }

  __pi_hyper_add_first(hyper, task);

  __pi_hyper_write_2d_exec(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size, stride, length);
}

static void __pi_hyper_write_bi2d_exec(pos_hyper_t *hyper, uint32_t cs, uint32_t hyper_addr, uint32_t addr,
   uint32_t size, uint32_t dir, uint32_t stride, uint32_t length)
{
  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {((0x0^dir) & 0x1),length,stride,((0x1^dir) & 0x1),length,stride};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x1, hyper_addr};

  pos_hyper_setup(hyper, cs);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  __pi_hyper_set_regs(hyper, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  __pi_hyper_set_regs(hyper, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  plp_hyper_enqueue(UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_TX(hyper->tran_id),
    (uint32_t)addr, size, UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_write_bi2d_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t dir, uint32_t stride, uint32_t length, struct pi_task *task)
{
  pos_hyper_cs_t *hyper_cs = (pos_hyper_cs_t *)device->data;
  pos_hyper_t *hyper = hyper_cs->hyper;

  if (hyper->waiting_first != NULL)
  {
    __pi_hyper_enqueue_pending(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size, dir, stride, length, task, HYPER_PENDING_WRITE_BI2D);
    return;
  }

  __pi_hyper_add_first(hyper, task);

  __pi_hyper_write_bi2d_exec(hyper, hyper_cs->cs, hyper_addr, (uint32_t)addr, size, dir, stride, length);
}

int pi_hyper_id_alloc(struct pi_device *device)
{
  pos_hyper_cs_t *hyper_cs = (pos_hyper_cs_t *)device->data;
  pos_hyper_t *hyper = hyper_cs->hyper;

  return plp_hyper_id_alloc(hyper->hyper_id);
}
