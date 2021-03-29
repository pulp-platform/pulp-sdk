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
            Germain Haugou, ETH (germain.haugou@iis.ee.ethz.ch)
 */


#include "../udma_impl.hpp"
#include "archi/udma/hyper/udma_hyper_v3.h"
#include "archi/utils.h"
#include "vp/itf/hyper.hpp"


Hyper_periph_v3::Hyper_periph_v3(udma *top, int id, int itf_id) : Udma_periph(top, id)
{
  std::string itf_name = "hyper" + std::to_string(itf_id);

  top->traces.new_trace(itf_name, &trace, vp::DEBUG);

  channel0 = new Hyper_v3_rx_channel(top, this, UDMA_EVENT_ID(id), itf_name + "_rx");
  channel1 = new Hyper_v3_tx_channel(top, this, UDMA_EVENT_ID(id) + 1, itf_name + "_tx");

  this->hyper_itf.set_sync_cycle_meth(&Hyper_periph_v3::rx_sync);
  top->new_master_port(this, itf_name, &this->hyper_itf);

  this->regs = new unsigned int*[HYPER_NB_CHANNELS]{0};
  for(int i=0; i<HYPER_NB_CHANNELS; i++)
  {
    this->regs[i] = new unsigned int[HYPER_NB_CONFIG_REGS]{0};
  }
  this->common_regs = new unsigned int[HYPER_NB_COMMON_REGS];

  this->pending_word_event = top->event_new(this, Hyper_periph_v3::handle_pending_word);

  this->pending_bytes = 0;
  this->next_bit_cycle = -1;
  this->state = HYPER_STATE_IDLE;

  this->rx_channel = static_cast<Hyper_v3_rx_channel *>(this->channel0);
  this->tx_channel = static_cast<Hyper_v3_tx_channel *>(this->channel1);

  int fifo_depth = HYPER_FIFO_DEPTH;

  for(int i=0; i<HYPER_NB_CHANNELS; i++)
  {
    free_fifo[i] = new Udma_queue<Hyper_transfer>(fifo_depth);
    pending_fifo[i] = new Udma_queue<Hyper_transfer>(fifo_depth);
    for(int j=0; j<fifo_depth; j++)
    {
      free_fifo[i]->push( new Hyper_transfer());
    }
  }

  this->cfg_setup = new bool[HYPER_NB_CHANNELS]{false};
  this->command_word = new bool[HYPER_NB_CHANNELS]{false};
}


void Hyper_periph_v3::reset(bool active)
{
  Udma_periph::reset(active);

  if (active)
  {
    memset(this->common_regs, 0, sizeof(unsigned int)*HYPER_NB_COMMON_REGS);
    this->clkdiv = 0;
    this->pending_tx = false;
    this->pending_rx = false;
    this->current_cmd = NULL;

    this->current_command = NULL;
    this->channel_id = 0;
  }
}


void Hyper_periph_v3::rx_sync(void *__this, int data)
{
  Hyper_periph_v3 *_this = (Hyper_periph_v3 *)__this;
  (static_cast<Hyper_v3_rx_channel *>(_this->channel0))->handle_rx_data(data);
}


void Hyper_periph_v3::handle_pending_word(void *__this, vp::clock_event *event)
{   
  Hyper_periph_v3 *_this = (Hyper_periph_v3 *)__this;
  uint8_t byte;
  int cs;
  int cs_value;
  bool send_byte = false;
  bool send_cs = false;
  bool end = false;

  if (_this->state == HYPER_STATE_IDLE)
  {
    if (_this->pending_bytes > 0)
    {
      _this->delay = _this->current_command->latency << _this->current_command->en_add_latency;
      if (_this->delay > 0)
      {
        _this->state = HYPER_STATE_DELAY;
      }
      else
      {
        _this->state = HYPER_STATE_CS;        
      }
      _this->ca_count = 6;
      _this->ca.low_addr = ARCHI_REG_FIELD_GET(_this->current_command->ex_addr, 0, 3);
      _this->ca.high_addr = ARCHI_REG_FIELD_GET(_this->current_command->ex_addr, 3, 29);      
      _this->ca.burst_type = ARCHI_REG_FIELD_GET(_this->current_command->ca_setup, 0, 1);
      _this->ca.address_space = ARCHI_REG_FIELD_GET(_this->current_command->ca_setup, 1, 1);
      _this->ca.read = ARCHI_REG_FIELD_GET(_this->current_command->ca_setup, 2, 1); 

      if(!_this->command_mode)
      {
        if (_this->ca.read)
        {
          _this->transfer_size = _this->rx_channel->current_cmd->size;         
        }
        else
          _this->transfer_size = _this->tx_channel->current_cmd->size;
      }
      else
      {
        _this->transfer_size = 2;
      }
    }
  }
  else if (_this->state == HYPER_STATE_DELAY)
  {
    _this->delay--;
    if (_this->delay == 0)
      _this->state = HYPER_STATE_CS;
  }
  else if (_this->state == HYPER_STATE_CS)
  {
    _this->state = HYPER_STATE_CA;
    send_cs = true;
    _this->set_device(_this->current_command->mem_sel);
    cs = _this->mem_sel;
    cs_value = 1;
  }
  else if (_this->state == HYPER_STATE_CA)
  {
    send_byte = true;
    _this->ca_count--;
    byte = _this->ca.raw[_this->ca_count];
    if (_this->ca_count == 0)
    {
      _this->state = HYPER_STATE_DATA;
    }
  }
  else if (_this->state == HYPER_STATE_DATA && _this->pending_bytes > 0)
  {
    if(!_this->ca.read || _this->rx_channel->is_ready())
    {
      send_byte = true;
      byte = _this->pending_word & 0xff;
      _this->pending_word >>= 8;
      _this->pending_bytes--;
      _this->transfer_size--;

      if (_this->transfer_size == 0)
      {
        _this->pending_bytes = 0;
        _this->state = HYPER_STATE_CS_OFF;
        _this->ending = true;
      }
      if (_this->pending_bytes == 0)
      {
        end = true;
      }
    }
  }
  else if (_this->state == HYPER_STATE_CS_OFF)
  {
    _this->state = HYPER_STATE_IDLE;
    send_cs = true;
    cs = _this->mem_sel;
    cs_value = 0;
    if(_this->get_nb_tran(_this->channel_id) == 0)
    {
      _this->set_busy_reg(_this->channel_id, 0);
      _this->common_regs[(TRANS_ID_ALLOC_OFFSET)/4] = _this->update_trans_id_alloc();
      _this->trace.msg("Current transfer is finished\n");
      if (!_this->ca.read)
      {
        _this->top->trigger_event(ARCHI_SOC_EVENT_HYPER_EOT_TX);
      }
      else
      {
        _this->top->trigger_event(ARCHI_SOC_EVENT_HYPER_EOT_RX);
      }
    }
    _this->ending = false;
  }

  if (send_byte || send_cs)
  {
    if (!_this->hyper_itf.is_bound())
    {
      _this->top->get_trace()->warning("%d: Trying to send to HYPER interface while it is not connected\n", _this->channel_id);
    }
    else
    {
      _this->next_bit_cycle = _this->top->get_periph_high_speed_clock()->get_cycles() + _this->clkdiv;
      if (send_byte)
      {
        _this->top->get_trace()->msg("%d: Sending byte (value: 0x%x)\n", _this->channel_id, byte);
        _this->hyper_itf.sync_cycle(byte);
      }
      else
      {
        _this->top->get_trace()->msg("%d: Updating CS (cs: %d, value: %d)\n", _this->channel_id, cs, cs_value);
        _this->hyper_itf.cs_sync(cs, cs_value);
      }
    }
  }

  if (end)
  {
    if(_this->ending)
    {
      _this->free_fifo[_this->channel_id]->push(_this->current_command);
      _this->current_command = NULL;
      _this->update_nb_tran(_this->channel_id, -1);
    }

    if(!_this->command_mode)
    {
      if (!_this->ca.read)
      {
        _this->pending_tx = false;
        _this->tx_channel->handle_ready_req_end(_this->pending_req);
        _this->tx_channel->handle_ready_reqs();
      }
      else
        _this->pending_rx = false;
    }
  }

  _this->check_state();

}

void Hyper_periph_v3::check_state()
{
  if (this->pending_bytes == 0 && !this->ending)
  {
    if(this->current_command == NULL)
    {
      this->fetch_from_fifos();
    }
    else if(!this->command_mode)
    {
      if (!this->tx_channel->ready_reqs->is_empty() && (this->pending_tx || !this->pending_rx))
      {
        this->pending_tx = true;
        vp::io_req *req = this->tx_channel->ready_reqs->pop();
        this->pending_req = req;
        this->pending_word = *(uint32_t *)req->get_data();
        this->pending_bytes = req->get_size();
      }
      else if (this->rx_channel->current_cmd && (this->pending_rx || !this->pending_tx))
      {
        this->pending_rx = true;
        this->pending_bytes = this->rx_channel->current_cmd->size;
      }
    }
  }
  if (this->pending_bytes != 0 || this->ending)
  {
    if (!this->pending_word_event->is_enqueued())
    {
      int latency = 1;
      int64_t cycles = this->top->get_periph_high_speed_clock()->get_cycles();
      if (this->next_bit_cycle > cycles)
        latency = this->next_bit_cycle - cycles;

      this->top->get_periph_high_speed_clock()->enqueue_ext(this->pending_word_event, latency);
    }
  }
}


void Hyper_periph_v3::handle_ready_reqs()
{
  this->check_state();
}

void Hyper_periph_v3::fetch_from_fifos()
{
  int id = transfer_arbiter(channel_id);
  channel_id = id;

  if(!pending_fifo[channel_id]->is_empty())
  {
    current_command = pending_fifo[channel_id]->pop();
    if(channel_id != current_command->channel_id)
    {
      vp_warning_always(&trace, "Wrong request. Fetching from %d but it is request from %d\n", channel_id, current_command->channel_id);        
    }
    trace.msg("Fetching new request from %d (cfg_setup: %d, nb_tran: %d)\n", channel_id, current_command->cfg_setup, get_nb_tran(id));

    command_mode = current_command->cfg_setup;
    if(command_mode)
    {
      pending_word = current_command->data;    
      pending_bytes = current_command->size; 
    }
    else
    {
      if(current_command->is_write)
      {
        channel1->build_reqs_and_enqueue(current_command);
      }
      else
      {
        channel0->build_reqs_and_enqueue(current_command);
      }
    }
  }
}

vp::io_req_status_e Hyper_periph_v3::hyper_cfg_req(vp::io_req *req, int id)
{
  uint32_t *data = (uint32_t *)req->get_data();
  continuous_mode = ARCHI_REG_FIELD_GET(*data, UDMA_CHANNEL_CFG_CONT_BIT, 1);
  transfer_size_mode = ARCHI_REG_FIELD_GET(*data, UDMA_CHANNEL_CFG_SIZE_BIT, 1);
  bool channel_enabled = ARCHI_REG_FIELD_GET(*data, UDMA_CHANNEL_CFG_EN_BIT, 1);
  bool channel_clear = ARCHI_REG_FIELD_GET(*data, UDMA_CHANNEL_CFG_CLEAR_BIT, 1);
  trace.msg("Setting cfg register (continuous: %d, size: %s, enable: %d, clear: %d)\n",
    continuous_mode, transfer_size_mode == 0 ? "8bits" : transfer_size_mode == 1 ? "16bits" : "32bits",
    channel_enabled, channel_clear);

  if (channel_clear)
  {
    trace.msg("UNIMPLEMENTED AT %s %d\n", __FILE__, __LINE__);
    return vp::IO_REQ_INVALID;
  }

  if (channel_enabled)
  {
    enqueue_transfer(id);
  }

  return vp::IO_REQ_OK;
}

vp::io_req_status_e Hyper_periph_v3::access_common_regs(vp::io_req *req, uint64_t offset, int id)
{
  int reg_id = offset/ 4;
  if (reg_id >= HYPER_NB_COMMON_REGS)
  {
    return vp::IO_REQ_INVALID;
  }
  if (!req->get_is_write())
  {
    *(uint32_t *)(req->get_data()) = this->common_regs[reg_id];
  }
  else 
    this->common_regs[reg_id] = *(uint32_t *)(req->get_data());  

  return vp::IO_REQ_OK;
}

vp::io_req_status_e Hyper_periph_v3::access_private_regs(vp::io_req *req, uint64_t offset, int id)
{
  int reg_id = offset/4;
  if (reg_id >= HYPER_NB_CONFIG_REGS)
  {
    return vp::IO_REQ_INVALID;
  }
  if (!req->get_is_write())
    *(uint32_t *)(req->get_data()) = this->regs[id][reg_id];
  else 
    this->regs[id][reg_id] = *(uint32_t *)(req->get_data());

  if (offset == REG_RX_SADDR(0))
  {
    if(channel0 == NULL)
    {
      trace.warning("Trying to access non-existing RX channel\n");
      return vp::IO_REQ_INVALID;
    }
    trace.msg("Accessing RX start address register (value: 0x%x, id: %d)\n", *(uint32_t *)(req->get_data()), id);    
    write_trans = false;
  }
  else if (offset == REG_RX_SIZE(0))
  {
    if(channel0 == NULL)
    {
      trace.warning("Trying to access non-existing RX channel\n");
      return vp::IO_REQ_INVALID;
    }
    trace.msg("Accessing RX size register (value: 0x%x, id: %d)\n", *(uint32_t *)(req->get_data()), id);
  }
  else if (offset == REG_RX_CFG(0))
  {
    if(channel0 == NULL)
    {
      trace.warning("Trying to access non-existing RX channel\n");
      return vp::IO_REQ_INVALID;
    }
    trace.msg("Accessing RX cfg register (value: 0x%x, id: %d)\n", *(uint32_t *)(req->get_data()), id);

    return hyper_cfg_req(req, id);
  }
  else if (offset == REG_TX_SADDR(0))
  {
    if(channel1 == NULL)
    {
      trace.warning("Trying to access non-existing RX channel\n");
      return vp::IO_REQ_INVALID;
    }
    trace.msg("Accessing TX start address register (value: 0x%x, id: %d)\n", *(uint32_t *)(req->get_data()), id);
    write_trans = true;  
  }
  else if (offset == REG_TX_SIZE(0))
  {
    if(channel1 == NULL)
    {
      trace.warning("Trying to access non-existing RX channel\n");
      return vp::IO_REQ_INVALID;
    }
    trace.msg("Accessing TX size register (value: 0x%x, id: %d)\n", *(uint32_t *)(req->get_data()), id);
  }
  else if (offset == REG_TX_CFG(0))
  {
    if(channel1 == NULL)
    {
      trace.warning("Trying to access non-existing RX channel\n");
      return vp::IO_REQ_INVALID;
    }
    trace.msg("Accessing TX cfg register (value: 0x%x, id: %d)\n", *(uint32_t *)(req->get_data()), id);

    return hyper_cfg_req(req, id);
  }
  else if (offset == HYPER_CA_SETUP(0))
  {
    trace.msg("Accessing CA setup register (value: 0x%x, id: %d)\n", *(uint32_t *)(req->get_data()), id);  
    command_word[id] = true;
  }
  else if (offset == REG_HYPER_CFG(0))
  {
    trace.msg("Accessing hyper cfg register (value: 0x%x, id: %d)\n", *(uint32_t *)(req->get_data()), id);
    cfg_setup[id] = true;  
  }

  return vp::IO_REQ_OK;
}

vp::io_req_status_e Hyper_periph_v3::custom_req(vp::io_req *req, uint64_t offset, int id)
{
  if (req->get_size() != 4)
  {
    return vp::IO_REQ_INVALID;
  }

  if(id == HYPER_NB_CHANNELS)
  {
    access_common_regs(req, offset - (HYPER_COMMON_REGS_OFFSET), id);
  }
  else
  {
    access_private_regs(req, offset - (HYPER_CHANNELS_ID_OFFSET(id)), id);
  }

  return vp::IO_REQ_OK;
}

int Hyper_periph_v3::get_nb_tran(int id)
{
  return ARCHI_REG_FIELD_GET(this->regs[id][(STATUS(0))/4], HYPER_NB_TRAN_OFFSET, HYPER_NB_TRAN_SIZE);
}

void Hyper_periph_v3::update_nb_tran(int id, int value)
{
  uint8_t nb_tran = get_nb_tran(id);
  this->regs[id][(STATUS(0))/4] = ARCHI_REG_FIELD_SET(this->regs[id][(STATUS(0))/4], (nb_tran + value), HYPER_NB_TRAN_OFFSET, HYPER_NB_TRAN_SIZE);
}

void Hyper_periph_v3::set_busy_reg(int id, int value)
{
  this->regs[id][(STATUS(0))/4] = ARCHI_REG_FIELD_SET(this->regs[id][(STATUS(0))/4], value, HYPER_BUSY_OFFSET, HYPER_BUSY_SIZE);
}

int Hyper_periph_v3::get_busy_reg(int id)
{
  return ARCHI_REG_FIELD_GET(this->regs[id][(STATUS(0))/4], HYPER_BUSY_OFFSET, HYPER_BUSY_SIZE);
}

void Hyper_periph_v3::set_device(int cs)
{
  int dev = cs;
  switch (dev)
  {
    case 0:
      trace.msg("Choosing HyperRam\n");
      this->mem_sel = dev;
    break;
    case 1:
      trace.msg("Choosing HyperFlash\n");
      this->mem_sel = dev;
    break;
    case 2:
      trace.msg("Choosing PSRAM(x8)\n");
      this->mem_sel = dev;
    break;
    case 3:
      trace.msg("Choosing PSRAM(x16)\n");
      this->mem_sel = dev;
    break;
    default:
      trace.warning("Choosing invalid device. Default is set to 0\n");
      this->mem_sel = 0;
    break;
  }
}

void Hyper_periph_v3::enqueue_transfer(int id)
{
  if (free_fifo[id]->is_empty())
  {
    vp_warning_always(&this->trace, "Trying to enqueue transfer on %d while already %d are pending. Transaction will be ignored\n", id, HYPER_FIFO_DEPTH);
    return;
  }

  Hyper_transfer *req = free_fifo[id]->pop();

  update_nb_tran(id, 1);

  if(get_cfg_status(id))
  {
    req->cfg_setup = true;
    req->size = 4;
  }
  else
  {
    req->cfg_setup = false;
    if(!write_trans)
    {
      req->addr = regs[id][(REG_RX_SADDR(0))/4];
      req->current_addr = regs[id][(REG_RX_SADDR(0))/4];
      req->size = regs[id][(REG_RX_SIZE(0))/4];
      req->remaining_size = regs[id][(REG_RX_SIZE(0))/4];
      req->channel = channel0;
    }
    else
    {
      req->addr = regs[id][(REG_TX_SADDR(0))/4];
      req->current_addr = regs[id][(REG_TX_SADDR(0))/4];
      req->size = regs[id][(REG_TX_SIZE(0))/4];
      req->remaining_size = regs[id][(REG_TX_SIZE(0))/4];
      req->channel = channel1;
    }
  }

  req->transfer_size = transfer_size_mode;      
  req->received_size = 0;
  req->continuous_mode = continuous_mode;
  req->is_write = write_trans;

  req->ex_addr = this->regs[id][(REG_HYPER_ADDR(0))/4];
  req->mem_sel = this->common_regs[(MEM_SEL_OFFSET)/4];
  req->ca_setup = this->regs[id][(HYPER_CA_SETUP(0))/4];
  req->data = this->regs[id][(REG_HYPER_CFG(0))/4];
  req->latency = this->common_regs[(REG_T_LATENCY_ACCESS_OFFSET)/4];
  req->en_add_latency = this->common_regs[(REG_EN_LATENCY_ADD_OFFSET)/4];
  req->channel_id = id;

  pending_fifo[id]->push(req);

  cfg_setup[id] = false;
  command_word[id] = false;

  set_busy_reg(id, 1);
  this->common_regs[(TRANS_ID_ALLOC_OFFSET)/4] = update_trans_id_alloc();

  check_state();
}

int Hyper_periph_v3::unpack(int original_size)
{
  int page_bound = common_regs[REG_PAGE_BOUND_OFFSET/4];

  switch (page_bound)
  {
    case 4:
      trace.msg("Page bound is not considered\n");
      return 0;
    break;

    default:
      if(original_size < page_bound)
      {
        trace.msg("Page bound is considered (value: %d) but unpacking is not necessary (value: %d)\n", 
          page_bound == 0 ? 128 : page_bound == 1 ? 256 : page_bound == 2 ? 512 : page_bound == 3 ? 1024 : 0, original_size);
        return original_size;
      }
      else
      {
        trace.msg("Page bound is considered (value: %d) and unpacking is necessary (original: %d, new: %d)\n", 
          page_bound == 0 ? 128 : page_bound == 1 ? 256 : page_bound == 2 ? 512 : page_bound == 3 ? 1024 : 0, original_size, page_bound - original_size);
        return page_bound - original_size;
      }
    break;
  }
  return 0;
}

void Hyper_periph_v3::transfer_splitter()
{

}

void Hyper_periph_v3::width_modulator_16b_32b()
{

}

void Hyper_periph_v3::width_modulator_32b_16b()
{

}

int Hyper_periph_v3::update_trans_id_alloc()
{
  int trans_id_alloc;

  for(int i=0; i<HYPER_NB_CHANNELS; i++)
  {
    if(get_busy_reg(i) == 0)
    {
      trans_id_alloc = i;
      return trans_id_alloc;
    }
  }

  return HYPER_NB_CHANNELS;
}

int Hyper_periph_v3::transfer_arbiter(int id)
{
  int new_id = id;

  id++;
  if (id == HYPER_NB_CHANNELS)
  {
    id = 0;
  }

  for(int i=0; i<HYPER_NB_CHANNELS; i++)
  {
    if(!pending_fifo[id]->is_empty())
    {
      new_id = id;
      return new_id;
    }
    else
    {
      id++;
      if(id == HYPER_NB_CHANNELS)
      {
        id = 0;
      }
    }
  }

  return new_id;
}

vp::io_req_status_e Hyper_periph_v3::req(vp::io_req *req, uint64_t offset)
{
  if (!get_periph_status())
  {
    trace.msg("Trying to access periph while it is off (periph: %d)\n", id);
    //return vp::IO_REQ_INVALID;
    // TODO should dump the warning but the himax driver is buggy
    return vp::IO_REQ_OK;
  }

  int id;

  for(int i=1; i<HYPER_NB_CHANNELS + 1; i++)
  {
    if(offset < HYPER_CHANNELS_ID_OFFSET(i))
    {
      id = i - 1;
      i = HYPER_NB_CHANNELS;
    }
    else if(i == HYPER_NB_CHANNELS)
    {
      id = i;
    }
  }

  custom_req(req, offset, id);

  return vp::IO_REQ_OK;
}





Hyper_v3_tx_channel::Hyper_v3_tx_channel(udma *top, Hyper_periph_v3 *periph, int id, string name)
: Udma_tx_channel(top, id, name), periph(periph)
{

}

void Hyper_v3_tx_channel::reset(bool active)
{
  Udma_tx_channel::reset(active);
}

void Hyper_v3_tx_channel::handle_ready_reqs()
{
  this->periph->handle_ready_reqs();
}






Hyper_v3_rx_channel::Hyper_v3_rx_channel(udma *top, Hyper_periph_v3 *periph, int id, string name) : Udma_rx_channel(top, id, name), periph(periph)
{
}

void Hyper_v3_rx_channel::reset(bool active)
{
  Udma_rx_channel::reset(active);
}

void Hyper_v3_rx_channel::handle_rx_data(int data)
{
  this->push_data((uint8_t *)&data, 1);
}

void Hyper_v3_rx_channel::handle_ready()
{
  this->periph->handle_ready_reqs();
}
