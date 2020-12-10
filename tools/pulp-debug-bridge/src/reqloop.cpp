/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
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
 * Authors: Germain Haugou, ETH (germain.haugou@iis.ee.ethz.ch)
 */

#include <stdio.h>
#include <thread>
#include "cable.hpp"
#include "cables/log.h"
#include "debug_bridge/debug_bridge.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mutex>
#include <queue>
#include <condition_variable>

#if defined(__USE_SDL__)
#include <SDL.h>
#endif

typedef enum 
{
  TARGET_SYNC_FSM_STATE_INIT,
  TARGET_SYNC_FSM_STATE_WAIT_INIT,
  TARGET_SYNC_FSM_STATE_WAIT_AVAILABLE,
  TARGET_SYNC_FSM_STATE_WAIT_REQUEST,
  TARGET_SYNC_FSM_STATE_WAIT_ACK
} target_sync_fsm_state_e;

class Target_req
{
public:
  bool done;

  hal_bridge_req_t target_req;
};

class Reqloop
{
public:
  Reqloop(Cable *cable, unsigned int debug_struct_addr);
  void reqloop_routine();
  int stop(bool kill);
  void activate();

  void efuse_access(bool write, int id, uint32_t value, uint32_t mask);
  int eeprom_access(uint32_t itf, uint32_t cs, bool write, uint32_t addr, uint32_t buffer, uint32_t size);
  int flash_access(int32_t type, uint32_t itf, uint32_t cs, bool write, uint32_t addr, uint32_t buffer, uint32_t size);
  int flash_erase(int32_t type, uint32_t itf, uint32_t cs, uint32_t addr, int32_t size);
  int flash_erase_sector(int32_t type, uint32_t itf, uint32_t cs, uint32_t addr);
  int flash_erase_chip(int32_t type, uint32_t itf, uint32_t cs);
  void buffer_free(uint32_t addr, uint32_t size);
  uint32_t buffer_alloc(uint32_t size);

private:
  void reply_req(hal_debug_struct_t *debug_struct, hal_bridge_req_t *target_req, hal_bridge_req_t *req);
  bool handle_req_connect(hal_debug_struct_t *debug_struct, hal_bridge_req_t *target_req, hal_bridge_req_t *req);
  bool handle_req_open(hal_debug_struct_t *debug_struct, hal_bridge_req_t *target_req, hal_bridge_req_t *req);
  bool handle_req_read(hal_debug_struct_t *debug_struct, hal_bridge_req_t *target_req, hal_bridge_req_t *req);
  bool handle_req_write(hal_debug_struct_t *debug_struct, hal_bridge_req_t *target_req, hal_bridge_req_t *req);
  bool handle_req_close(hal_debug_struct_t *debug_struct, hal_bridge_req_t *target_req, hal_bridge_req_t *req);
  bool handle_req_fb_open(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req);
  bool handle_req_fb_update(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req);
  bool handle_req_target_status_sync(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req);
  bool handle_req_disconnect(hal_debug_struct_t *debug_struct, hal_bridge_req_t *target_req, hal_bridge_req_t *req);
  bool handle_req_reply(hal_debug_struct_t *debug_struct, hal_bridge_req_t *target_req, hal_bridge_req_t *req);
  bool handle_req(hal_debug_struct_t *debug_struct, hal_bridge_req_t *target_req, hal_bridge_req_t *req);
  void update_target_status(hal_debug_struct_t *debug_struct);
  void wait_target_available(hal_debug_struct_t *debug_struct);

  void notif_target(hal_debug_struct_t *debug_struct);
  void handle_target_req(hal_debug_struct_t *debug_struct, Target_req *target_req);
  void handle_bridge_to_target_reqs(hal_debug_struct_t *debug_struct);

  bool wait_target_request();
  unsigned int get_target_state();
  void send_target_ack();
  void clear_target_ack();

  Log *log;
  std::thread *thread;
  Cable *cable;
  bool end = false;
  unsigned int debug_struct_addr;
  int status = 0;
  bool connected = false;   // Set to true once the applicatoin has sent the connect request
  bool target_connected;

  hal_target_state_t target;

  std::queue<Target_req *> target_reqs;

  std::mutex mutex;
  std::condition_variable cond;

  target_sync_fsm_state_e target_sync_fsm_state;
  unsigned int jtag_val;

  hal_debug_struct_t *debug_struct = NULL;

  bool target_jtag_sync;

  int confreg_instr;
};

class Framebuffer
{
public:
  Framebuffer(Cable *cable, std::string name, int width, int height, int format);
  void update(uint32_t addr, int posx, int posy, int width, int height);
  bool open();

private:
  void fb_routine();

  std::string name;
  int width;
  int height;
  int format;
  int pixel_size = 1;
  Cable *cable;
  std::thread *thread;
  uint32_t *pixels;
#if defined(__USE_SDL__)
  SDL_Surface *screen;
  SDL_Texture * texture;
  SDL_Renderer *renderer;
  SDL_Window *window;
#endif
};

Framebuffer::Framebuffer(Cable *cable, std::string name, int width, int height, int format)
: name(name), width(width), height(height), format(format), cable(cable)
{
}

void Framebuffer::fb_routine()
{
#if defined(__USE_SDL__)
  bool quit = false;
  SDL_Event event;

  while (!quit)
  {
    SDL_WaitEvent(&event);
    switch (event.type)
    {
      case SDL_QUIT:
      quit = true;
      break;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
#endif
}


bool Framebuffer::open()
{
#if defined(__USE_SDL__)

  if (format == HAL_BRIDGE_REQ_FB_FORMAT_GRAY)
  {
    pixel_size = 1;
  }
  else if (format == HAL_BRIDGE_REQ_FB_FORMAT_RGB)
  {
    pixel_size = 4;
  }
  else if (format == HAL_BRIDGE_REQ_FB_FORMAT_RAW)
  {
    pixel_size = 1;
  }
  else
  {
    printf("Unsupported format: %d\n", format);
  }

  pixels = new uint32_t[width*height];
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow(name.c_str(),
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  texture = SDL_CreateTexture(renderer,
      SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);

  memset(pixels, 255, width * height * sizeof(Uint32));

  SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  thread = new std::thread(&Framebuffer::fb_routine, this);
  return true;

#else
  printf("Trying to open framebuffer while bridge has not been compiled with SDL support\n");
  return false;
#endif
}

void Framebuffer::update(uint32_t addr, int posx, int posy, int width, int height)
{
#if defined(__USE_SDL__)

  if (posx == -1)
  {
    posx = posy = 0;
    width = this->width;
    height = this->height;
  }

  int size = width*height*pixel_size;

  if (this->format == HAL_BRIDGE_REQ_FB_FORMAT_GRAY)
  {
    uint8_t buffer[size];
    this->cable->access(false, addr, size, (char*)buffer);

#if 0
    static int count = 0;
    char name[64];
    sprintf(name, "fb%d", count++);
    FILE *file = fopen(name, "w");
    fprintf(file, "P5\n%d %d\n255\n", this->width, this->height);
    fwrite(buffer, 1, size, file);
    fclose(file);
#endif

    for (int j=0; j<height; j++)
    {
      for (int i=0; i<width; i++)
      {
        unsigned int value = buffer[j*width+i];
        pixels[(j+posy)*this->width + i + posx] = (0xff << 24) | (value << 16) | (value << 8) | value;
      }
    }
  }
  else if (this->format == HAL_BRIDGE_REQ_FB_FORMAT_RAW)
  {
    uint8_t buffer[size];
    this->cable->access(false, addr, size, (char*)buffer);

    for (int j=0; j<height; j++)
    {
      for (int i=0; i<width; i++)
      {
        int shift;
        int line = height - j - 1;
        if (line & 1)
        {
          if (i & 1)
            shift = 16;
          else
            shift = 8;
        }
        else
        {
          if (i & 1)
            shift = 8;
          else
            shift = 0;
        }

        unsigned int value = buffer[j*width+i];

        pixels[(j+posy)*this->width + i + posx] = (0xff << 24) | (value << shift);
      }
    }
  }
  else
  {
    uint32_t buffer[size];
    this->cable->access(false, addr, size*4, (char*)buffer);

    for (int j=0; j<height; j++)
    {
      for (int i=0; i<width; i++)
      {
        unsigned int value = buffer[j*width+i];
        pixels[(j+posy)*this->width + i + posx] = (0xff << 24) | value;
      }
    }
  }


  SDL_UpdateTexture(texture, NULL, pixels, this->width * sizeof(Uint32));

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
#endif
}



int Reqloop::stop(bool kill)
{
  if (kill) end = true;
  thread->join();
  return status;
}

void Reqloop::activate()
{
  if (this->debug_struct == NULL)
  {
    cable->access(false, debug_struct_addr, 4, (char*)&this->debug_struct);

    if (this->debug_struct != NULL)
    {
      uint32_t protocol_version;
      cable->access(false, (unsigned int)(long)&this->debug_struct->protocol_version, 4, (char*)&protocol_version);
      
      if (protocol_version != PROTOCOL_VERSION_4)
      {
        this->log->error("Protocol version mismatch between bridge and runtime (bridge: %d, runtime: %d)\n", PROTOCOL_VERSION_4, protocol_version);
        throw std::logic_error("Unable to connect to runtime");
      }

      int32_t is_connected;
      this->cable->access(false, (unsigned int)(long)&this->debug_struct->target.connected, 4, (char*)&is_connected);
      this->connected = is_connected;

      // The binary has just started, we need to tell him we want to watch for requests
      unsigned int value = 0;

      uint32_t connected = 1;
      cable->access(true, (unsigned int)(long)&this->debug_struct->bridge.connected, 4, (char*)&connected);
      cable->access(true, (unsigned int)(long)&this->debug_struct->use_internal_printf, 4, (char*)&value);
    }
  }
}



void Reqloop::notif_target(hal_debug_struct_t *debug_struct)
{
  uint32_t notif_req_addr;
  uint32_t notif_req_value;
  cable->access(false, (unsigned int)(long)&debug_struct->notif_req_addr, 4, (char*)&notif_req_addr);
  cable->access(false, (unsigned int)(long)&debug_struct->notif_req_value, 4, (char*)&notif_req_value);

  cable->access(true, (unsigned int)(long)notif_req_addr, 4, (char*)&notif_req_value);
}

void Reqloop::reply_req(hal_debug_struct_t *debug_struct, hal_bridge_req_t *target_req, hal_bridge_req_t *req)
{
  uint32_t value = 1;
  this->cable->access(true, (unsigned int)(long)&target_req->done, sizeof(target_req->done), (char*)&value);

  this->notif_target(debug_struct);
}

static int transpose_code(int code)
{
  int alt = 0;

  if ((code & 0x0) == 0x0) alt |= O_RDONLY;
  if ((code & 0x1) == 0x1) alt |= O_WRONLY;
  if ((code & 0x2) == 0x2) alt |= O_RDWR;
  if ((code & 0x8) == 0x8) alt |= O_APPEND;
  if ((code & 0x200) == 0x200) alt |= O_CREAT;
  if ((code & 0x400) == 0x400) alt |= O_TRUNC;
  if ((code & 0x800) == 0x800) alt |= O_EXCL;
  if ((code & 0x2000) == 0x2000) alt |= O_SYNC;
  if ((code & 0x4000) == 0x4000) alt |= O_NONBLOCK;
  if ((code & 0x8000) == 0x8000) alt |= O_NOCTTY;

  return alt;
}

bool Reqloop::handle_req_connect(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req)
{
  this->connected = true;
  this->reply_req(debug_struct, target_req, req);
  return false;
}

bool Reqloop::handle_req_reply(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req)
{
  Target_req *bridge_req = (Target_req *)req->bridge_data;

  this->mutex.lock();
  bridge_req->done = true;
  this->notif_target(debug_struct);
  this->cond.notify_all();

  // Copy the target information to the local request in case some returned
  // information are needed
  memcpy((void *)&bridge_req->target_req, (void *)req, sizeof(hal_bridge_req_t));

  this->mutex.unlock();
  return false;
}

bool Reqloop::handle_req_disconnect(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req)
{
  this->connected = false;
  this->reply_req(debug_struct, target_req, req);
  return true;
}

bool Reqloop::handle_req_open(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req)
{
  char name[req->open.name_len+1];
  cable->access(false, (unsigned int)(long)req->open.name, req->open.name_len+1, (char*)name);

  int res = open(name, req->open.flags, req->open.mode);

  cable->access(true, (unsigned int)(long)&target_req->open.retval, 4, (char*)&res);

  this->reply_req(debug_struct, target_req, req);
  return false;
}

bool Reqloop::handle_req_read(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req)
{
  char buffer[4096];
  int size = req->read.len;
  char *ptr = (char *)(long)req->read.ptr;
  int res = 0;
  while (size)
  {
    int iter_size = size;
    if (iter_size > 4096)
      iter_size = 4096;

    iter_size = read(req->read.file, (void *)buffer, iter_size);

    if (iter_size <= 0) {
      if (iter_size == -1 && res == 0) res = -1;
      break;
    }

    cable->access(true, (unsigned int)(long)ptr, iter_size, (char*)buffer);

    res += iter_size;
    ptr += iter_size;
    size -= iter_size;
  }

  cable->access(true, (unsigned int)(long)&target_req->read.retval, 4, (char*)&res);

  this->reply_req(debug_struct, target_req, req);
  return false;
}

bool Reqloop::handle_req_write(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req)
{
  char buffer[4096];
  int size = req->write.len;
  char *ptr = (char *)(long)req->write.ptr;
  int res = 0;
  while (size)
  {
    int iter_size = size;
    if (iter_size > 4096)
      iter_size = 4096;

    cable->access(false, (unsigned int)(long)ptr, iter_size, (char*)buffer);

    iter_size = write(req->write.file, (void *)buffer, iter_size);

    if (iter_size <= 0)
      break;

    res += iter_size;
    ptr += iter_size;
    size -= iter_size;
  }

  if (res == 0)
    res = -1;

  cable->access(true, (unsigned int)(long)&target_req->write.retval, 4, (char*)&res);

  this->reply_req(debug_struct, target_req, req);
  return false;
}

bool Reqloop::handle_req_close(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req)
{
  int res = close(req->close.file);
  cable->access(true, (unsigned int)(long)&target_req->write.retval, 4, (char*)&res);
  this->reply_req(debug_struct, target_req, req);
  return false;
}

bool Reqloop::handle_req_fb_open(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req)
{
  char name[req->fb_open.name_len+1];
  cable->access(false, (unsigned int)(long)req->fb_open.name, req->fb_open.name_len+1, (char*)name);

  int res = 0;
  Framebuffer *fb = new Framebuffer(cable, name, req->fb_open.width, req->fb_open.height, req->fb_open.format);



  if (!fb->open()) 
  {
    res = -1;
    delete fb;
    fb = NULL;
  }

  cable->access(true, (unsigned int)(long)&target_req->fb_open.screen, 8, (char*)&fb);

  this->reply_req(debug_struct, target_req, req);
  return false;
}

void Reqloop::update_target_status(hal_debug_struct_t *debug_struct)
{
  this->cable->access(false, (unsigned int)(long)&debug_struct->target, sizeof(this->target), (char *)&this->target);
}

bool Reqloop::handle_req_target_status_sync(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req)
{
  this->update_target_status(debug_struct);
  this->reply_req(debug_struct, target_req, req);
  return false;
}

bool Reqloop::handle_req_fb_update(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req)
{
#if defined(__USE_SDL__)
  Framebuffer *fb = (Framebuffer *)req->fb_update.screen;

  fb->update(
    req->fb_update.addr, req->fb_update.posx, req->fb_update.posy, req->fb_update.width, req->fb_update.height
  );
#endif

  this->reply_req(debug_struct, target_req, req);
  return false;
}

bool Reqloop::handle_req(hal_debug_struct_t *debug_struct, hal_bridge_req_t *req, hal_bridge_req_t *target_req)
{
  switch (req->type)
  {
    case HAL_BRIDGE_REQ_CONNECT:    return this->handle_req_connect(debug_struct, req, target_req);
    case HAL_BRIDGE_REQ_DISCONNECT: return this->handle_req_disconnect(debug_struct, req, target_req);
    case HAL_BRIDGE_REQ_OPEN:       return this->handle_req_open(debug_struct, req, target_req);
    case HAL_BRIDGE_REQ_READ:       return this->handle_req_read(debug_struct, req, target_req);
    case HAL_BRIDGE_REQ_WRITE:      return this->handle_req_write(debug_struct, req, target_req);
    case HAL_BRIDGE_REQ_CLOSE:      return this->handle_req_close(debug_struct, req, target_req);
    case HAL_BRIDGE_REQ_FB_OPEN:    return this->handle_req_fb_open(debug_struct, req, target_req);
    case HAL_BRIDGE_REQ_FB_UPDATE:  return this->handle_req_fb_update(debug_struct, req, target_req);
    case HAL_BRIDGE_REQ_TARGET_STATUS_SYNC:    return this->handle_req_target_status_sync(debug_struct, req, target_req);
    case HAL_BRIDGE_REQ_REPLY:    return this->handle_req_reply(debug_struct, req, target_req);
    default:
      this->log->print(LOG_ERROR, "Received unknown request from target (type: %d)\n", req->type);
  }
  return false;
}


unsigned int Reqloop::get_target_state()
{
  unsigned int value;
  if (this->confreg_instr == 6)
  {
    this->cable->jtag_get_reg((6<<5)|(0x1f), 9, &value, this->jtag_val, 9);
    value = (value >> 1) & 0xf;
  }
  else
  {
    this->cable->jtag_get_reg(7, 4, &value, this->jtag_val);
    value &= 0xf;
  }
  return value >> 1;
}

void Reqloop::send_target_ack()
{
  unsigned int value;
  this->jtag_val = 0x7 << 1;

  if (this->confreg_instr == 6)
  {
    this->cable->jtag_set_reg((6<<5)|(0x1f), 9, 0x7 << 2, 9);
  }
  else
  {
    this->cable->jtag_set_reg(7, 4, 0x7 << 1);
  }
}

void Reqloop::clear_target_ack()
{
  unsigned int value;
  this->jtag_val = 0x0 << 1;

  if (this->confreg_instr == 6)
  {
    this->cable->jtag_set_reg((6<<5)|(0x1f), 9, 0x0 << 2, 9);
  }
  else
  {
    this->cable->jtag_set_reg(7, 4, 0x0 << 1);    
  }
}

bool Reqloop::wait_target_request()
{
  // In case the target does not support synchronization through the JTAG
  // register, just consider that the target is always available.
  if (!this->target_jtag_sync)
    return true;

  switch (this->target_sync_fsm_state)
  {
    case TARGET_SYNC_FSM_STATE_INIT: {
      //printf("STATE INIT\n");
      // This state is used in case the bridge is not yet connected to the
      // target (e.g. if it boots from flash).
      // Wait until the target becomes available and ask for the connection.
      unsigned int state = this->get_target_state();
      if (state == 4)
      {
        //printf("RECEIVED CONNECTION REQ\n");
        // Target is asking for connection, acknowledge it and go to next step
        this->send_target_ack();
        this->target_sync_fsm_state = TARGET_SYNC_FSM_STATE_WAIT_INIT;
      }
      return false;
    }

    case TARGET_SYNC_FSM_STATE_WAIT_INIT: {
      //printf("STATE WAIT INIT\n");
      // This state is used to wait for the target acknoledgment, after
      // it asked for connection
      unsigned int state = this->get_target_state();
      if (state != 4)
      {
        // Once the target acknowledged the connection, activate the bridge
        // clear the acknowledge and wait for target availability
        //printf("RECEIVED CONNECTION ACK\n");
        this->activate();
        this->clear_target_ack();
        this->target_sync_fsm_state = TARGET_SYNC_FSM_STATE_WAIT_REQUEST;
      }
      return true;
    }

    case TARGET_SYNC_FSM_STATE_WAIT_AVAILABLE: {
      //printf("STATE WAIT AVAILABLE\n");
      // We are in the state when the target cannot be accessed.
      // Wait until we see something different from 0.
      // We also need to clear the request acknowledgement to let the target
      // know that we got it, in case we come from state
      // TARGET_SYNC_FSM_STATE_WAIT_ACK.
      this->clear_target_ack();
      unsigned int state = this->get_target_state();
      if (state == 0)
        return false;

      //printf("TARGET AVAILABLE\n");
      this->target_sync_fsm_state = TARGET_SYNC_FSM_STATE_WAIT_REQUEST;
      return false;
    }

    case TARGET_SYNC_FSM_STATE_WAIT_REQUEST: {
      //printf("STATE WAIT REQUEST\n");
      // The target became available, now wait for a request
      unsigned int state = this->get_target_state();

      if (state == 1)
      {
        // The target is still available with no request, stay in this state
        return false;
      }

      if (state == 2)
      {
      //printf("SHUTDOWN REQ\n");
        // The target is requesting a shutdown, acknowledge it and stop
        // accessing the target
        this->send_target_ack();
        this->target_sync_fsm_state = TARGET_SYNC_FSM_STATE_WAIT_ACK;
        return false;
      }

      //printf("OTHER REQ\n");
      // The target is requesting something else, just report true so that
      // the caller now checks what is requested through JTAG and stay in
      // this state.
      return true;
    }

    case TARGET_SYNC_FSM_STATE_WAIT_ACK: {
      //printf("STATE WAIT ACK\n");
      // The chip is not accessible, just wait until it becomes available
      // again
      unsigned int state = this->get_target_state();
      if (state != 0)
        return false;

      //printf("AVAILABLE AFTER SHUTDOWN\n");
      this->target_sync_fsm_state = TARGET_SYNC_FSM_STATE_WAIT_AVAILABLE;
      return false;
    }
  }
}


void Reqloop::handle_target_req(hal_debug_struct_t *debug_struct, Target_req *target_req)
{
  // First get a request from the target
  hal_bridge_req_t *req = NULL;

  this->cable->access(false, (unsigned int)(long)&debug_struct->first_bridge_free_req, 4, (char*)&req);

  if (req == NULL)
  {
    this->log->error("Unable to allocate bridge to target request");
    throw std::logic_error("Unable to allocate bridge to target request");
  }

  uint32_t next;
  this->cable->access(false, (unsigned int)(long)&req->next, 4, (char*)&next);
  this->cable->access(true, (unsigned int)(long)&debug_struct->first_bridge_free_req, 4, (char*)&next);
  this->cable->access(true, (unsigned int)(long)req, sizeof(hal_bridge_req_t), (char*)&target_req->target_req);
  this->cable->access(true, (unsigned int)(long)&req->bridge_data, sizeof(target_req), (char*)&target_req);


  // Store it to the debug structure
  this->cable->access(true, (unsigned int)(long)&debug_struct->target_req, 4, (char*)&req);

  // And notify the target so that it is processed
  this->notif_target(debug_struct);
}

void Reqloop::handle_bridge_to_target_reqs(hal_debug_struct_t *debug_struct)
{
  if (!this->connected)
    return;

  while(this->target_reqs.size())
  {
    // Runtime can only handle one request, first check if no request is already
    // pushed.
    uint32_t target_req;
    this->cable->access(false, (unsigned int)(long)&debug_struct->target_req, 4, (char*)&target_req);
    if (target_req)
      break;

    this->mutex.lock();
    Target_req *bridge_target_req = this->target_reqs.front();
    this->target_reqs.pop();
    this->handle_target_req(debug_struct, bridge_target_req);
    this->mutex.unlock();
  }
}

void Reqloop::reqloop_routine()
{
  // In case the birdge is not yet connected, do extra init steps to
  // connect once the target becomes available
  this->target_sync_fsm_state = this->debug_struct ? TARGET_SYNC_FSM_STATE_WAIT_AVAILABLE : TARGET_SYNC_FSM_STATE_INIT;

  this->jtag_val = 0;

  if (debug_struct_addr) {

    // In case the debug struct pointer is found, iterate to receive IO requests
    // from runtime
    while(!end)
    {
      uint32_t value;

      // Wait until the target is available and has a request.
      // This will poll the target through the JTAG register.
      if (!this->wait_target_request())
      {
        // If not, just wait a bit and retry
        usleep(100);
        continue;
      }

      // First check if the application has exited
      cable->access(false, (unsigned int)(long)&debug_struct->exit_status, 4, (char*)&value);
      if (value >> 31) {
        status = ((int)value << 1) >> 1;
        printf("Detected end of application, exiting with status: %d\n", status);
        return;
      }

      // Check printf
      // The target application should quickly dumps the characters, so we can loop on printf
      // until we don't find anything
      while(1) {
        cable->access(false, (unsigned int)(long)&debug_struct->pending_putchar, 4, (char*)&value);
        if (value == 0) break;
        char buff[value+1];
        cable->access(false, (unsigned int)(long)&debug_struct->putc_buffer, value, (char*)buff);
        unsigned int zero = 0;
        cable->access(true, (unsigned int)(long)&debug_struct->pending_putchar, 4, (char*)&zero);
        for (int i=0; i<value; i++) putchar(buff[i]);
        fflush(NULL);
      }

      // Handle target to bridge requests
      while(1) {
        hal_bridge_req_t *first_bridge_req=NULL, *last_req=NULL, *next=NULL, *next_next=NULL;

        if (!cable->access(false, (unsigned int)(long)&debug_struct->first_bridge_req, 4, (char*)&first_bridge_req)) goto end;

        if (first_bridge_req == NULL)
          break;

        hal_bridge_req_t req;
        if (!this->cable->access(false, (unsigned int)(long)first_bridge_req, sizeof(hal_bridge_req_t), (char*)&req)) goto end;

        value = 1;
        if (!cable->access(true, (unsigned int)(long)&first_bridge_req->popped, sizeof(first_bridge_req->popped), (char*)&value)) goto end;
        if (!cable->access(true, (unsigned int)(long)&debug_struct->first_bridge_req, 4, (char*)&req.next)) goto end;

        if (this->handle_req(debug_struct, &req, first_bridge_req))
          return;
      }

      // Handle bridge to target requests
      this->handle_bridge_to_target_reqs(debug_struct);

      if (!this->target_jtag_sync)
      {
        // Small sleep to not poll too often
        usleep(500);
      }
    }
  }
  else
  {
    log->warning("Trying to launch request loop (command reqloop) while no binary is provided\n");
  }

  return;

end:
  log->warning("Got access error in reqloop\n");
}

void Reqloop::efuse_access(bool write, int id, uint32_t value, uint32_t mask)
{
  Target_req *req = new Target_req();
  req->done = false;

  req->target_req.type = HAL_BRIDGE_TARGET_REQ_EFUSE_ACCESS;
  req->target_req.efuse_access.is_write = write;
  req->target_req.efuse_access.index = id;
  req->target_req.efuse_access.value = value;
  req->target_req.efuse_access.mask = mask;

  std::unique_lock<std::mutex> lock(this->mutex);

  this->target_reqs.push(req);

  while(!req->done)
  {
    this->cond.wait(lock);
  }

  lock.unlock();
}

int Reqloop::eeprom_access(uint32_t itf, uint32_t cs, bool write, uint32_t addr, uint32_t buffer, uint32_t size)
{
  Target_req *req = new Target_req();
  req->done = false;

  req->target_req.type = HAL_BRIDGE_TARGET_REQ_EEPROM_ACCESS;
  req->target_req.eeprom_access.itf = itf;
  req->target_req.eeprom_access.cs = cs;
  req->target_req.eeprom_access.is_write = write;
  req->target_req.eeprom_access.addr = addr;
  req->target_req.eeprom_access.buffer = buffer;
  req->target_req.eeprom_access.size = size;

  std::unique_lock<std::mutex> lock(this->mutex);
  this->target_reqs.push(req);

  while(!req->done)
  {
    this->cond.wait(lock);
  }

  int retval = req->target_req.eeprom_access.retval;

  delete req;

  lock.unlock();

  return retval;
}

int Reqloop::flash_access(int type, uint32_t itf, uint32_t cs, bool write, uint32_t addr, uint32_t buffer, uint32_t size)
{
  Target_req *req = new Target_req();
  req->done = false;

  req->target_req.type = HAL_BRIDGE_TARGET_REQ_FLASH_ACCESS;
  req->target_req.flash_access.type = type;
  req->target_req.flash_access.itf = itf;
  req->target_req.flash_access.cs = cs;
  req->target_req.flash_access.is_write = write;
  req->target_req.flash_access.addr = addr;
  req->target_req.flash_access.buffer = buffer;
  req->target_req.flash_access.size = size;

  std::unique_lock<std::mutex> lock(this->mutex);
  this->target_reqs.push(req);

  while(!req->done)
  {
    this->cond.wait(lock);
  }

  int retval = req->target_req.flash_access.retval;

  delete req;

  lock.unlock();

  return retval;
}

int Reqloop::flash_erase(int type, uint32_t itf, uint32_t cs, uint32_t addr, int32_t size)
{
  Target_req *req = new Target_req();
  req->done = false;

  req->target_req.type = HAL_BRIDGE_TARGET_REQ_FLASH_ERASE;
  req->target_req.flash_erase.type = type;
  req->target_req.flash_erase.itf = itf;
  req->target_req.flash_erase.cs = cs;
  req->target_req.flash_erase.addr = addr;
  req->target_req.flash_erase.size = size;

  std::unique_lock<std::mutex> lock(this->mutex);
  this->target_reqs.push(req);

  while(!req->done)
  {
    this->cond.wait(lock);
  }

  int retval = req->target_req.flash_erase.retval;

  delete req;

  lock.unlock();

  return retval;
}

int Reqloop::flash_erase_sector(int type, uint32_t itf, uint32_t cs, uint32_t addr)
{
  Target_req *req = new Target_req();
  req->done = false;

  req->target_req.type = HAL_BRIDGE_TARGET_REQ_FLASH_ERASE_SECTOR;
  req->target_req.flash_erase_sector.type = type;
  req->target_req.flash_erase_sector.itf = itf;
  req->target_req.flash_erase_sector.cs = cs;
  req->target_req.flash_erase_sector.addr = addr;

  std::unique_lock<std::mutex> lock(this->mutex);
  this->target_reqs.push(req);

  while(!req->done)
  {
    this->cond.wait(lock);
  }

  int retval = req->target_req.flash_erase_sector.retval;

  delete req;

  lock.unlock();

  return retval;
}

int Reqloop::flash_erase_chip(int type, uint32_t itf, uint32_t cs)
{
  Target_req *req = new Target_req();
  req->done = false;

  req->target_req.type = HAL_BRIDGE_TARGET_REQ_FLASH_ERASE_CHIP;
  req->target_req.flash_erase_chip.type = type;
  req->target_req.flash_erase_chip.itf = itf;
  req->target_req.flash_erase_chip.cs = cs;

  std::unique_lock<std::mutex> lock(this->mutex);
  this->target_reqs.push(req);

  while(!req->done)
  {
    this->cond.wait(lock);
  }

  int retval = req->target_req.flash_erase_chip.retval;

  delete req;

  lock.unlock();

  return retval;
}

void Reqloop::buffer_free(uint32_t addr, uint32_t size)
{
  Target_req *req = new Target_req();
  req->done = false;

  req->target_req.type = HAL_BRIDGE_TARGET_REQ_BUFFER_FREE;
  req->target_req.buffer_free.size = size;
  req->target_req.buffer_free.buffer = addr;

  std::unique_lock<std::mutex> lock(this->mutex);
  this->target_reqs.push(req);

  while(!req->done)
  {
    this->cond.wait(lock);
  }

  delete req;

  lock.unlock();
}

uint32_t Reqloop::buffer_alloc(uint32_t size)
{
  Target_req *req = new Target_req();
  req->done = false;

  req->target_req.type = HAL_BRIDGE_TARGET_REQ_BUFFER_ALLOC;
  req->target_req.buffer_alloc.size = size;

  std::unique_lock<std::mutex> lock(this->mutex);
  this->target_reqs.push(req);

  while(!req->done)
  {
    this->cond.wait(lock);
  }

  uint32_t addr = req->target_req.buffer_alloc.buffer;

  delete req;

  lock.unlock();

  return addr;
}

Reqloop::Reqloop(Cable *cable, unsigned int debug_struct_addr) : cable(cable), debug_struct_addr(debug_struct_addr), target_connected(false)
{
  log = new Log();

  js::config *config = cable->get_config();

  this->target_jtag_sync = config->get_child_bool("**/debug_bridge/target_jtag_sync");
  
  if (config->get("**/pulp_tap/confreg_instr") != NULL)
  {
    this->confreg_instr = config->get_int("**/pulp_tap/confreg_instr");
  }
  else
  {
    this->confreg_instr = 7;
  }

  std::string chip = config->get("**/chip/name")->get_str();

  // Try to connect the bridge now before the execution is started so
  // that the target sees the bridge as soon as it starts.
  // Otherwise, the bridge will get connected later on when the target
  // becomes available
  this->activate();
  thread = new std::thread(&Reqloop::reqloop_routine, this);
}

extern "C" void *bridge_reqloop_open(void *cable, unsigned int debug_struct_addr)
{
  return (void *)new Reqloop((Cable *)cable, debug_struct_addr);
}

extern "C" void bridge_reqloop_close(void *arg, int kill)
{
  Reqloop *reqloop = (Reqloop *)arg;
  reqloop->stop(kill);
}

extern "C" void bridge_reqloop_efuse_access(void *arg, bool write, int id, uint32_t value, uint32_t mask)
{
  Reqloop *reqloop = (Reqloop *)arg;
  reqloop->efuse_access(write, id, value, mask);
}


extern "C" int bridge_reqloop_eeprom_access(void *arg, uint32_t itf, uint32_t cs, bool write, uint32_t addr, uint32_t buffer, uint32_t size)
{
  Reqloop *reqloop = (Reqloop *)arg;
  return reqloop->eeprom_access(itf, cs, write, addr, buffer, size);
}


extern "C" int bridge_reqloop_flash_access(void *arg, int type, uint32_t itf, uint32_t cs, bool write, uint32_t addr, uint32_t buffer, uint32_t size)
{
  Reqloop *reqloop = (Reqloop *)arg;
  return reqloop->flash_access(type, itf, cs, write, addr, buffer, size);
}



extern "C" int bridge_reqloop_flash_erase(void *arg, int type, uint32_t itf, uint32_t cs, uint32_t addr, int size)
{
  Reqloop *reqloop = (Reqloop *)arg;
  return reqloop->flash_erase(type, itf, cs, addr, size);
}



extern "C" int bridge_reqloop_flash_erase_sector(void *arg, int type, uint32_t itf, uint32_t cs, uint32_t addr)
{
  Reqloop *reqloop = (Reqloop *)arg;
  return reqloop->flash_erase_sector(type, itf, cs, addr);
}



extern "C" int bridge_reqloop_flash_erase_chip(void *arg, int type, uint32_t itf, uint32_t cs)
{
  Reqloop *reqloop = (Reqloop *)arg;
  return reqloop->flash_erase_chip(type, itf, cs);
}


extern "C" uint32_t bridge_reqloop_buffer_alloc(void *arg, uint32_t size)
{
  Reqloop *reqloop = (Reqloop *)arg;
  return reqloop->buffer_alloc(size);
}


extern "C" void bridge_reqloop_buffer_free(void *arg, uint32_t addr, uint32_t size)
{
  Reqloop *reqloop = (Reqloop *)arg;
  reqloop->buffer_free(addr, size);
}


