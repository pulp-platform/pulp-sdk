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
#include <stdarg.h>
#include <signal.h>
#include <stdexcept>

#include "json.hpp"
#include "cables/log.h"
#include "cables/adv_dbg_itf/adv_dbg_itf.hpp"
#include "cables/jtag-proxy/jtag-proxy.hpp"
#ifdef __USE_FTDI__
#include "cables/ftdi/ftdi.hpp"
#endif
#include "gdb-server/gdb-server.hpp"

using namespace std;


static int bridge_verbose = 0;
static const char *bridge_error = NULL;
static js::config *system_config = NULL;

void Log::print(log_level_e level, const char *str, ...)
{
  if (bridge_verbose <= level) return;
  va_list va;
  va_start(va, str);
  vprintf(str, va);
  va_end(va);
}


void Log::user(const char *str, ...)
{
  if (bridge_verbose <= LOG_INFO) return;
  va_list va;
  va_start(va, str);
  vprintf(str, va);
  va_end(va);
}

void Log::debug(const char *str, ...)
{
  if (bridge_verbose <= LOG_DEBUG) return;
  va_list va;
  va_start(va, str);
  vprintf(str, va);
  va_end(va);
}

void Log::warning(const char *str, ...)
{
  if (bridge_verbose <= LOG_WARNING) return;
  va_list va;
  va_start(va, str);
  vprintf(str, va);
  va_end(va);
}

void Log::error(const char *str, ...)
{
  char buff[1024];
  va_list va;
  va_start(va, str);
  vsnprintf(buff, 1024, str, va);
  va_end(va);
  bridge_error = strdup(buff);

  if (bridge_verbose <= LOG_ERROR) return;
  va_start(va, str);
  vprintf(str, va);
  va_end(va);
}

extern "C" void *cable_new(const char *config_string, const char *system_config_string)
{
  const char *cable_name = NULL;
  js::config *config = NULL;
  js::config *system_config = js::import_config_from_string(std::string(system_config_string));

  if (config_string != NULL)
  {
    config = js::import_config_from_string(std::string(config_string));
    js::config *type_config = config->get("type");
    if (type_config != NULL)
    {
      cable_name = type_config->get_str().c_str();
    }
  }

  if (cable_name == NULL) {
    bridge_error = "No cable name specified";
    return NULL;
  }

  if (strncmp(cable_name, "ftdi", 4) == 0)
  {
#ifdef __USE_FTDI__
    Log *log = new Log();
    Ftdi::FTDIDeviceID id = Ftdi::Olimex;
    if (strcmp(cable_name, "ftdi@digilent") == 0) id = Ftdi::Digilent;
    if (strcmp(cable_name, "ftdi@generic") == 0) id = Ftdi::Generic;
    Adv_dbg_itf *adu = new Adv_dbg_itf(system_config, config, log, new Ftdi(system_config, log, id));
    return (void *)static_cast<Cable *>(adu);
#else
    fprintf(stderr, "Debug bridge has not been compiled with FTDI support\n");
    return NULL;
#endif
  }
  else if (strcmp(cable_name, "jtag-proxy") == 0)
  {
    Log *log = new Log();
    Adv_dbg_itf *adu = new Adv_dbg_itf(system_config, config, log, new Jtag_proxy(log));
    return (void *)static_cast<Cable *>(adu);
  }
  else
  {
    fprintf(stderr, "Unknown cable: %s\n", cable_name);
    return NULL;
  }
  
  return NULL;
}

extern "C" void cable_write(void *cable, unsigned int addr, int size, const char *data)
{
  Adv_dbg_itf *adu = (Adv_dbg_itf *)cable;
  adu->access(true, addr, size, (char *)data);
}

extern "C" void cable_read(void *cable, unsigned int addr, int size, const char *data)
{
  Adv_dbg_itf *adu = (Adv_dbg_itf *)cable;
  adu->access(false, addr, size, (char *)data);
}

extern "C" void cable_reg_write(void *cable, unsigned int addr, const char *data, int device)
{
  Adv_dbg_itf *adu = (Adv_dbg_itf *)cable;
  adu->reg_access(true, addr, (char *)data, device);
}

extern "C" void cable_reg_read(void *cable, unsigned int addr, const char *data, int device)
{
  Adv_dbg_itf *adu = (Adv_dbg_itf *)cable;
  adu->reg_access(false, addr, (char *)data, device);
}

extern "C" void chip_reset(void *handler, bool active, int duration)
{
  Adv_dbg_itf *cable = (Adv_dbg_itf *)handler;
  cable->chip_reset(active, duration);
}

extern "C" void chip_config(void *handler, uint32_t value)
{
  Adv_dbg_itf *cable = (Adv_dbg_itf *)handler;
  cable->chip_config(value);
}

extern "C" void jtag_reset(void *handler, bool active)
{
  Adv_dbg_itf *cable = (Adv_dbg_itf *)handler;
  cable->jtag_reset(active);
}

extern "C" void jtag_soft_reset(void *handler)
{
  Adv_dbg_itf *cable = (Adv_dbg_itf *)handler;
  cable->jtag_soft_reset();
}


extern "C" bool cable_jtag_set_reg(void *handler, unsigned int reg, int width, unsigned int value, int ir_len)
{
  Cable *cable = (Cable *)handler;
  return cable->jtag_set_reg(reg, width, value, ir_len);
}

extern "C" bool cable_jtag_get_reg(void *handler, unsigned int reg, int width, unsigned int *out_value, unsigned int value, int ir_len)
{
  Cable *cable = (Cable *)handler;
  return cable->jtag_get_reg(reg, width, out_value, value, ir_len);
}

extern "C" void cable_lock(void *handler)
{
  Cable *cable = (Cable *)handler;
  cable->lock();
}

extern "C" void cable_unlock(void *handler)
{
  Cable *cable = (Cable *)handler;
  cable->unlock();
}




static void init_sigint_handler(int s) {
  raise(SIGTERM);
}

extern "C" char * bridge_get_error()
{
  if (bridge_error == NULL) return strdup("unknown error");
  return strdup(bridge_error);
}

extern "C" void bridge_init(const char *config_string, int verbose)
{
  system_config = js::import_config_from_string(std::string(config_string));
  bridge_verbose = verbose;

  // This should be the first C method called by python.
  // As python is not catching SIGINT where we are in C world, we have to
  // setup a  sigint handler to exit in case control+C is hit.
  signal (SIGINT, init_sigint_handler);

}


extern "C" void *gdb_server_open(void *cable, int socket_port)
{
  return (void *)new Gdb_server(new Log(), (Cable *)cable, system_config, socket_port);
}

extern "C" void gdb_server_close(void *arg, int kill)
{
  Gdb_server *server = (Gdb_server *)arg;
  server->stop(kill);
}




#if 0

extern "C" void plt_exit(void *_bridge, bool status)
{
  Bridge *bridge = (Bridge *)_bridge;
  bridge->getMemIF()->exit(status);
}

extern "C" bool jtag_reset(void *_bridge)
{
  Bridge *bridge = (Bridge *)_bridge;
  bridge->getJtagIF()->jtag_reset();
}

extern "C" bool jtag_idle(void *_bridge)
{
  Bridge *bridge = (Bridge *)_bridge;
  bridge->getJtagIF()->jtag_idle();
}

extern "C" bool jtag_shift_ir(void *_bridge)
{
  Bridge *bridge = (Bridge *)_bridge;
  bridge->getJtagIF()->jtag_shift_ir();
}

extern "C" bool jtag_shift_dr(void *_bridge)
{
  Bridge *bridge = (Bridge *)_bridge;
  bridge->getJtagIF()->jtag_shift_dr();
}

extern "C" void jtag_shift(void *_bridge, int width, const char *datain, const char *dataout, int noex)
{
  Bridge *bridge = (Bridge *)_bridge;
  bridge->getJtagIF()->jtag_shift(width, (unsigned char *)datain, (unsigned char *)dataout, noex);
}

#endif
