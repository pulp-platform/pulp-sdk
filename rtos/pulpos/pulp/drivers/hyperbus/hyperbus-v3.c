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

static PI_L2 pos_hyper_t pos_hyper[HYPER_NB_CS * ARCHI_UDMA_NB_HYPER];
static PI_L2 int hyper_open_count=0;
static PI_L2 int hyper_channel;
static PI_L2 pos_udma_channel_t hyper_tx_channel;
static PI_L2 pos_udma_channel_t hyper_rx_channel;

static void pos_hyper_setup(pos_hyper_t *hyper)
{
  if (hyper->type == PI_HYPER_TYPE_FLASH)
  {
    plp_hyper_setup(hyper->hyper_id, CONFIG_HYPERFLASH_EN_LATENCY_ADD, hyper->cs, CONFIG_HYPERFLASH_T_LATENCY_ACCESS);
    plp_hyper_set_reg(UDMA_HYPER_BASE_ADDR(0) + REG_PAGE_BOUND, 0x04);
  }
  else if (hyper->type == PI_HYPER_TYPE_RAM)
  {
    plp_hyper_setup(hyper->hyper_id, CONFIG_HYPERRAM_EN_LATENCY_ADD, hyper->cs, CONFIG_HYPERRAM_T_LATENCY_ACCESS);
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

void pos_hyper_enqueue(pos_udma_channel_t *channel, pi_task_t *task, uint32_t offset, uint32_t buffer, uint32_t size, uint32_t cfg)
{
    // A UDMA channel has 2 slots, enqueue the copy to the UDMA if one of them is available, otherwise
    // put the transfer on hold.
    if (channel->pendings[0] == NULL)
    {
        channel->pendings[0] = task;
        plp_hyper_enqueue(offset, buffer, size, UDMA_CHANNEL_CFG_EN | cfg);
    }
    else if (channel->pendings[1] == NULL)
    {
        channel->pendings[1] = task;
        plp_hyper_enqueue(offset, buffer, size, UDMA_CHANNEL_CFG_EN | cfg);
    }
    else
    {
        task->data[0] = buffer;
        task->data[1] = size;
        task->data[2] = cfg;

        task->data[3] = offset;

        if (channel->waitings_first == NULL)
            channel->waitings_first = task;
        else
            channel->waitings_last->next = task;

        channel->waitings_last = task;
        task->next = NULL;
    }
}

void pos_hyper_handle_copy(int event, void *arg)
{
    pos_udma_channel_t *channel = arg;

    pi_task_t *pending_1 = channel->pendings[1];
    pi_task_t *pending_0 = channel->pendings[0];
    pi_task_t *pending_first = channel->waitings_first;
    channel->pendings[0] = pending_1;

    if (pending_first)
    {
        channel->waitings_first = pending_first->next;
        channel->pendings[1] = pending_first;

        plp_hyper_enqueue(pending_first->data[3], pending_first->data[0], pending_first->data[1], UDMA_CHANNEL_CFG_EN | pending_first->data[2]);
    }
    else
    {
        channel->pendings[1] = NULL;
    }

    pos_task_push_locked(pending_0);
}

void pos_hyper_create_channel(pos_udma_channel_t *channel, int channel_id, int soc_event)
{
    pos_soc_event_register_callback(soc_event, pos_hyper_handle_copy, (void *)channel);
    channel->pendings[0] = NULL;
    channel->pendings[1] = NULL;
    channel->waitings_first = NULL;
    channel->base = hal_udma_channel_base(channel_id);
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

  pos_hyper_t *hyper = &pos_hyper[hyper_id + cs];

  hyper->hyper_id = conf->id;
  hyper->type = conf->type;
  hyper->cs = cs;
  hyper->tran_id = 0;

  hyper->rx_channel = &hyper_rx_channel;
  hyper->tx_channel = &hyper_tx_channel;

  device->data = (void *)hyper;

  if (hyper_open_count == 0)
  {
    pos_hyper_create_channel(hyper->rx_channel, UDMA_CHANNEL_ID(periph_id), hyper_channel + ARCHI_UDMA_HYPER_EOT_RX_EVT);
    pos_hyper_create_channel(hyper->tx_channel, UDMA_CHANNEL_ID(periph_id)+1, hyper_channel + ARCHI_UDMA_HYPER_EOT_TX_EVT);
  }

  hyper_open_count++;

  plp_udma_cg_set(plp_udma_cg_get() | (1<<periph_id));

  soc_eu_fcEventMask_setEvent(hyper_channel+ ARCHI_UDMA_HYPER_EOT_RX_EVT);
  soc_eu_fcEventMask_setEvent(hyper_channel + ARCHI_UDMA_HYPER_EOT_TX_EVT);

  hal_irq_restore(irq);

  return 0;
}

void pi_hyper_close(struct pi_device *device)
{
  int irq = hal_irq_disable();

  pos_hyper_t *hyper = (pos_hyper_t *)device->data;

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
        pos_hyper_t *hyper = (pos_hyper_t *)device->data;
        hyper->tran_id = tran_id;
      }
      break;
    }
  }
  return 0;
}

void pi_hyper_set_regs(struct pi_device *device, uint32_t cmd, void *arg)
{
  pos_hyper_t *hyper = (pos_hyper_t *)device->data;
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

int pi_hyper_ctl_status_regs(struct pi_device *device, uint32_t cmd, void *arg)
{
  pos_hyper_t *hyper = (pos_hyper_t *)device->data;

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

void pi_hyper_read_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, struct pi_task *task)
{
  pos_hyper_t *hyper = (pos_hyper_t *)device->data;

  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {0,0,0,0,0,0};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x5, hyper_addr};

  pos_hyper_setup(hyper);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  pi_hyper_set_regs(device, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  pi_hyper_set_regs(device, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  pos_hyper_enqueue(hyper->rx_channel, task, UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_RX(hyper->tran_id), (uint32_t)addr, size, UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_read_2d(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t stride, uint32_t length)
{
  pi_task_t task;
  pi_hyper_read_2d_async(device, hyper_addr, addr, size, stride, length, pi_task_block(&task));
  pi_task_wait_on(&task);  
}

void pi_hyper_read_2d_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t stride, uint32_t length, struct pi_task *task)
{
  pos_hyper_t *hyper = (pos_hyper_t *)device->data;

  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {1,length,stride,0,length,stride};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x5, hyper_addr};

  pos_hyper_setup(hyper);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  pi_hyper_set_regs(device, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  pi_hyper_set_regs(device, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  pos_hyper_enqueue(hyper->rx_channel, task, UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_RX(hyper->tran_id), (uint32_t)addr, size, UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_read_bi2d_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t dir, uint32_t stride, uint32_t length, struct pi_task *task)
{
  pos_hyper_t *hyper = (pos_hyper_t *)device->data;

  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {((0x0^dir) & 0x1),length,stride,((0x1^dir) & 0x1),length,stride};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x5, hyper_addr};

  pos_hyper_setup(hyper);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  pi_hyper_set_regs(device, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  pi_hyper_set_regs(device, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  pos_hyper_enqueue(hyper->rx_channel, task, UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_RX(hyper->tran_id), (uint32_t)addr, size, UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_write(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size)
{
  pos_hyper_t *hyper = (pos_hyper_t *)device->data;

  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {0,0,0,0,0,0};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x0, hyper_addr};

  pos_hyper_setup(hyper);

  while(plp_hyper_nb_tran(hyper->hyper_id, hyper->tran_id)>HYPER_FIFO_DEPTH-1){}

  pi_hyper_set_regs(device, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  pi_hyper_set_regs(device, PI_HYPER_CTL, (unsigned int *)ctl_cmd);
  pi_hyper_set_regs(device, PI_HYPER_CFG, (unsigned short *)addr);

  plp_hyper_enqueue(UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_TX(hyper->tran_id), 0x0, 0x0, UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_write_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, struct pi_task *task)
{
  pos_hyper_t *hyper = (pos_hyper_t *)device->data;

  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {0,0,0,0,0,0};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x1, hyper_addr};

  pos_hyper_setup(hyper);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  pi_hyper_set_regs(device, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  pi_hyper_set_regs(device, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  pos_hyper_enqueue(hyper->tx_channel, task, UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_TX(hyper->tran_id), (uint32_t)addr, size, UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_write_2d(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t stride, uint32_t length)
{
  pi_task_t task;
  pi_hyper_write_2d_async(device, hyper_addr, addr, size, stride, length, pi_task_block(&task));
  pi_task_wait_on(&task); 
}

void pi_hyper_write_2d_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t stride, uint32_t length, struct pi_task *task)
{
  pos_hyper_t *hyper = (pos_hyper_t *)device->data;

  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {1,length,stride,0,length,stride};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x1, hyper_addr};

  pos_hyper_setup(hyper);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  pi_hyper_set_regs(device, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  pi_hyper_set_regs(device, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  pos_hyper_enqueue(hyper->tx_channel, task, UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_TX(hyper->tran_id), (uint32_t)addr, size, UDMA_CHANNEL_CFG_SIZE_8);
}

void pi_hyper_write_bi2d_async(struct pi_device *device, uint32_t hyper_addr, void *addr, uint32_t size, uint32_t dir, uint32_t stride, uint32_t length, struct pi_task *task)
{
  pos_hyper_t *hyper = (pos_hyper_t *)device->data;

  unsigned int twd_cmd[HYPER_NB_TWD_REGS] = {((0x0^dir) & 0x1),length,stride,((0x1^dir) & 0x1),length,stride};
  unsigned int ctl_cmd[HYPER_NB_CTL_REGS] = {0x1, hyper_addr};

  pos_hyper_setup(hyper);

  hyper->tran_id = plp_hyper_id_alloc(hyper->hyper_id);

  pi_hyper_set_regs(device, PI_HYPER_TWD, (unsigned int *)twd_cmd);
  pi_hyper_set_regs(device, PI_HYPER_CTL, (unsigned int *)ctl_cmd);

  pos_hyper_enqueue(hyper->tx_channel, task, UDMA_HYPER_BASE_ADDR(hyper->hyper_id) + UDMA_HYPER_CHANNEL_TX(hyper->tran_id), (uint32_t)addr, size, UDMA_CHANNEL_CFG_SIZE_8);
}

int pi_hyper_id_alloc(struct pi_device *device)
{
  pos_hyper_t *hyper = (pos_hyper_t *)device->data;  
  return plp_hyper_id_alloc(hyper->hyper_id);
}
