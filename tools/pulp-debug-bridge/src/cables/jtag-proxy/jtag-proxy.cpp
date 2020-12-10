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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>

#include "cables/log.h"
#include "jtag-proxy.hpp"
#include "debug_bridge/proxy.hpp"

Jtag_proxy::Jtag_proxy(Log* log) : Cable(NULL)
{
}   

bool Jtag_proxy::connect(js::config *config)
{
  struct sockaddr_in addr;
  struct hostent *he;

  js::config *proxy_config = config->get("jtag-proxy");

  if (proxy_config == NULL || proxy_config->get("port") == NULL)
  {
    fprintf(stderr, "Didn't find any information for JTAG proxy\n");
    return false;
  }

  int m_port = proxy_config->get("port")->get_int();
  char *m_server = (char *)"localhost";

  if((m_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "Unable to create socket (%s)\n", strerror(errno));
    return false;
  }

  if((he = gethostbyname(m_server)) == NULL) {
    perror("gethostbyname");
    return false;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(m_port);
  addr.sin_addr = *((struct in_addr *)he->h_addr_list[0]);
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

  if(::connect(m_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    fprintf(stderr, "Unable to connect to %s port %d (%s)\n", m_server, m_port,
            strerror(errno));
    return false;
  }
  return true;
}

bool Jtag_proxy::bit_inout(char* inbit, char outbit, bool last)
{
  return stream_inout(inbit, &outbit, 1, last);
}

bool Jtag_proxy::proxy_stream(char* instream, char* outstream, unsigned int n_bits, bool last, int bit)
{
  proxy_req_t req = { .type=DEBUG_BRIDGE_JTAG_REQ };
  req.jtag.bits = n_bits;
  req.jtag.tdo = instream != NULL;

  if (n_bits >= (1<<16)) return false;

  uint8_t buffer[n_bits];
  uint8_t value;
  if (outstream)
  {
    for (int i=0; i<n_bits; i++)
    {

      if ((i % 8) == 0)
      {
        value = *(unsigned char *)outstream;
        outstream++;
      }

      buffer[i] = (value & 1) << bit;
      if (bit != DEBUG_BRIDGE_JTAG_TRST) buffer[i] |= 1 << DEBUG_BRIDGE_JTAG_TRST;

      value >>= 1;
    }
  }
  else
  {
    ::memset(buffer, 0, n_bits); 
  }

  if (last)
  {
    buffer[n_bits-1] |= 1 << DEBUG_BRIDGE_JTAG_TMS;
  }

  ::send(m_socket, (void *)&req, sizeof(req), 0);
  ::send(m_socket, (void *)buffer, n_bits, 0);
  if (instream != NULL)
  {

    ::memset((void *)instream, 0, (n_bits + 7) / 8);
    if (::recv(m_socket, (void *)instream, (n_bits + 7) / 8, 0) != (n_bits + 7) / 8) return false;

  }
  return true;
}

bool Jtag_proxy::stream_inout(char* instream, char* outstream, unsigned int n_bits, bool last)
{
  return proxy_stream(instream, outstream, n_bits, last, DEBUG_BRIDGE_JTAG_TDI);
}

bool Jtag_proxy::jtag_reset(bool active)
{
  int value = !active;
  return proxy_stream(NULL, (char *)&value, 1, 0, DEBUG_BRIDGE_JTAG_TRST);
}

int Jtag_proxy::flush()
{
  return true;
}

bool Jtag_proxy::chip_reset(bool active, int duration)
{
  proxy_req_t req = { .type=DEBUG_BRIDGE_RESET_REQ };
  req.reset.active = active;
  req.reset.duration = duration;
  ::send(m_socket, (void *)&req, sizeof(req), 0);

  return true;
}

bool Jtag_proxy::chip_config(uint32_t config)
{
  proxy_req_t req = { .type=DEBUG_BRIDGE_CONFIG_REQ };
  req.config.value = config;
  ::send(m_socket, (void *)&req, sizeof(req), 0);
  return true;
}
