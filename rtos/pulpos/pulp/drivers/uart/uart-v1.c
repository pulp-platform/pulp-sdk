/*
 * Copyright (C) 2020 ETH Zurich
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
 * Authors: Germain Haugou, ETH Zurich (germain.haugou@iis.ee.ethz.ch)
 */

#include "pmsis.h"

#define POS_UART_BAUDRATE 115200



static PI_L2 pos_uart_t pos_uart[ARCHI_UDMA_NB_UART];



void pi_uart_conf_init(struct pi_uart_conf *conf)
{
  conf->baudrate_bps = POS_UART_BAUDRATE;
  conf->uart_id = 0;
  conf->stop_bit_count = 1;
  conf->parity_mode = 0;
  conf->enable_rx = 1;
  conf->enable_tx = 1;
}



static void pos_uart_wait_tx_done(pos_uart_t *uart)
{
  // Wait for all pending transfers to finish
  while (plp_udma_busy(UDMA_UART_TX_ADDR(uart->channel - ARCHI_UDMA_UART_ID(0))))
  {
    pos_irq_wait_for_interrupt();
  }

  // And flush the uart to make sure no bit is transfered anymore
  while(plp_uart_tx_busy(uart->channel - ARCHI_UDMA_UART_ID(0)));

#if 1

// There is a bug in the uart, between 2 bytes, the uart says it is not busy
// and so if we are not lucky, we can continue while the uart is actually 
// still busy. Instead, wait for a few clock refs

#ifdef ITC_VERSION
  for (int i=0; i<50; i++)
  {
    pos_irq_clr(1<<ARCHI_FC_EVT_CLK_REF);
    pos_irq_mask_set(1<<ARCHI_FC_EVT_CLK_REF);
    pos_irq_wait_for_interrupt();
    pos_irq_mask_clr(1<<ARCHI_FC_EVT_CLK_REF);
  }
#else
  volatile int i;
  for (i=0; i<2000; i++);
#endif

#endif
}



static void pos_uart_setup(pos_uart_t *uart)
{
  int div =  (pi_freq_get(PI_FREQ_DOMAIN_PERIPH) + uart->baudrate/2) / uart->baudrate;

  // The counter in the UDMA will count from 0 to div included
  // and then will restart from 0, so we must give div - 1 as
  // divider
  plp_uart_setup(uart->channel - ARCHI_UDMA_UART_ID(0), 0, div-1);
}



static int pos_uart_setfreq_before(void *arg)
{
  // Before changing the periph frequency, we need to handle the pending buffers
  // as a frequency change in the middle of a transfer would corrupt a few data
  // Here is the procedure to handle that correctly:
  //   - We wait until all pending transfers are finished. The only way
  //     is to block the core execution. It is important that no one reenqueue another transfer
  //     meanwhile as we cannot stop the uart while there is at least one transfer in the udma.
  //     This is ensured by the fact we don't enqueue udma transfers from interrupt handlers and
  //     we block core execution.
  //   - We stop the uart, this way other transfers can be enqueued into the udma, the uart will not
  //     try to send them.
  //   - We leave the function and the fll driver will then apply the new frequency.
  //   - When we enter the other callback after the frequency is changed, we activate back the uart
  //     with the new frequency. Pending udma transfers will then start being handled.

  for (int i=0; i<ARCHI_UDMA_NB_UART; i++)
  {
    pos_uart_t *uart = &pos_uart[i];
    if (uart->open_count) {
      // First wait for pending transfers to finish
      pos_uart_wait_tx_done(uart);

      // Then deactivate the uart
      plp_uart_disable(uart->channel - ARCHI_UDMA_UART_ID(0));
    }
  }

  return 0;
}



static int pos_uart_setfreq_after(void *arg)
{
  // Now apply the new frequency to all channels
  // This will also reactive the uart
  for (int i=0; i<ARCHI_UDMA_NB_UART; i++)
  {
    pos_uart_t *uart = &pos_uart[i];
    if (uart->open_count)
    {
      pos_uart_setup(uart);
    }
  }
  return 0;
}


int pi_uart_open(struct pi_device *device)
{
    int irq = hal_irq_disable();

    struct pi_uart_conf *conf = (struct pi_uart_conf *)device->config;

    int uart_id = conf->uart_id;
    int periph_id = ARCHI_UDMA_UART_ID(uart_id);
    int channel = UDMA_EVENT_ID(periph_id);
    int baudrate = conf->baudrate_bps;

    UART_TRACE(POS_LOG_INFO, "[UART] Opening uart device (id: %d, baudrate: %d)\n", uart_id, baudrate);

    pos_uart_t *uart = &pos_uart[uart_id];

    device->data = (void *)uart;

    if (uart->open_count == 0)
    {
        pos_udma_create_channel(&uart->rx_channel, UDMA_CHANNEL_ID(periph_id), channel);
        pos_udma_create_channel(&uart->tx_channel, UDMA_CHANNEL_ID(periph_id)+1, channel+1);
    }

    uart->open_count++;
    uart->baudrate = baudrate;
    uart->channel = channel;

    // First activate uart device
    plp_udma_cg_set(plp_udma_cg_get() | (1<<periph_id));

    soc_eu_fcEventMask_setEvent(channel);
    soc_eu_fcEventMask_setEvent(channel+1);

    soc_eu_prEventMask_setEvent(channel);

    // Then set it up
    pos_uart_setup(uart);

    UART_TRACE(POS_LOG_INFO, "[UART] Successfully opened uart device (handle: %p)\n", uart);

    hal_irq_restore(irq);

  return 0;
}




void pi_uart_close(struct pi_device *device)
{
  int irq = hal_irq_disable();

  pos_uart_t *uart = (pos_uart_t *)device->data;


  UART_TRACE(POS_LOG_INFO, "[UART] Closing uart device (handle: %p)\n", uart);

  uart->open_count--;

  // First wait for pending transfers to finish before stoppping uart in case
  // some printf are still pending
  pos_uart_wait_tx_done(uart);

  // Set enable bits for uart channel back to 0 
  // This is needed to be able to propagate new configs when re-opening
  plp_uart_disable(uart->channel - ARCHI_UDMA_UART_ID(0));      

  // Then stop the uart
  plp_udma_cg_set(plp_udma_cg_get() & ~(1<<uart->channel));

  hal_irq_restore(irq);
}


#if 0
#if defined(ARCHI_HAS_CLUSTER)

static void pos_uart_cluster_req_done(void *_req)
{
  pi_cl_uart_req_t *req = _req;
  req->done = 1;
  pos_cluster_notif_req_done(req->cid);
}

static void pos_uart_cluster_req(void *_req)
{
  pi_cl_uart_req_t *req = _req;
  pi_task_t *event = &req->event;
  pi_task_callback(event, pos_uart_cluster_req_done, (void* )req);
  pi_uart_write_async(req->device, req->buffer, req->size, event);
}

int pi_cl_uart_write(pi_device_t *device, void *buffer, uint32_t size, pi_cl_uart_req_t *req)
{
  req->device = device;
  req->buffer = buffer;
  req->size = size;
  req->cid = pos_cluster_id();
  req->done = 0;
  pos_task_init_from_cluster(&req->event);
  pi_task_callback(&req->event, pos_uart_cluster_req, (void* )req);
  pos_cluster_push_fc_event(&req->event);

  return 0;
}

#endif
#endif


int pi_uart_write_async(struct pi_device *device, void *buffer, uint32_t size, pi_task_t *task)
{
  pos_uart_t *uart = (pos_uart_t *)device->data;
  pos_udma_enqueue(&uart->tx_channel, task, (uint32_t)buffer, size, UDMA_CHANNEL_CFG_SIZE_8);
  return 0;
}



int pi_uart_read_async(struct pi_device *device, void *buffer, uint32_t size, pi_task_t *task)
{
  pos_uart_t *uart = (pos_uart_t *)device->data;
  pos_udma_enqueue(&uart->rx_channel, task, (uint32_t)buffer, size, UDMA_CHANNEL_CFG_SIZE_8);
  return 0;
}



int pi_uart_write(struct pi_device *device, void *buffer, uint32_t size)
{
  pi_task_t task;
  if (pi_uart_write_async(device, buffer, size, pi_task_block(&task)))
    return -1;
  pi_task_wait_on(&task);
  return 0;
}



int pi_uart_read(struct pi_device *device, void *buffer, uint32_t size)
{
  pi_task_t task;
  if (pi_uart_read_async(device, buffer, size, pi_task_block(&task)))
    return -1;
  pi_task_wait_on(&task);
  return 0;
}

int pi_uart_write_byte(pi_device_t *device, uint8_t *byte)
{
  int ret = pi_uart_write(device, byte, 1);
  return ret;
}

int pi_uart_write_byte_async(pi_device_t *device, uint8_t *byte, pi_task_t *callback)
{
  return pi_uart_write_async(device, byte, 1, callback);
}


void __attribute__((constructor)) pos_uart_init()
{
    // In case the peripheral clock can dynamically change, we need to be notified
    // when this happens so that we flush pending transfers before updating the frequency
    int err = 0;

    //err |= pos_cbsys_add(RT_CBSYS_PERIPH_SETFREQ_BEFORE, pos_uart_setfreq_before, NULL);

    //err |= pos_cbsys_add(RT_CBSYS_PERIPH_SETFREQ_AFTER, pos_uart_setfreq_after, NULL);


    for (int i=0; i<ARCHI_UDMA_NB_UART; i++)
    {
        pos_uart[i].open_count = 0;
    }

    //if (err) pos_fatal("Unable to initialize uart driver\n");
}


#ifdef __ZEPHYR__

#include <zephyr.h>
#include <device.h>
#include <init.h>

static int uart_init(struct device *device)
{
  ARG_UNUSED(device);

  pos_uart_init();

  return 0;
}

struct uart_config {
};

struct uart_data {
};

static const struct uart_config uart_cfg = {
};

static struct uart_data uart_data = {
};

DEVICE_INIT(uart, "uart", &uart_init,
    &uart_data, &uart_cfg,
    PRE_KERNEL_2, CONFIG_KERNEL_INIT_PRIORITY_DEVICE);

#endif