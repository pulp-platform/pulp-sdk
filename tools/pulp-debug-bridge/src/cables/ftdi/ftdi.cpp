/*
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
 * Authors: Andreas Traber
 */

/*
 * Olimex ARM-USB-OCD, ARM-USB-OCD-H, ARM-USB-TINY and ARM-USB-TINY-H pinout
 *
 * GPIO     Function      Comment
 *  0       TCK           Set as output but do not drive manually
 *  1       TDI           Set as output but do not drive manually
 *  2       TDO           Set as input, should not be driven
 *  3       TMS           Set as output but do not drive manually
 *  4       ??            GPIOL0 of FT2232H, set as output 
 *  5       ??            GPIOL1 of FT2232H, set as input, should not be driven
 *  6       ??            GPIOL2 of FT2232H, set as input, should not be driven
 *  7       RTCK          GPIOL3 of FT2232H, set as input, should not be driven
 *  8       TRST*)        Set as output
 *  9       SRST**)       Set as output
 * 10       ??            GPIOH2 of FT2232H, set as output
 * 11       Red LED       Set as output
 * 12       ??            GPIOH4 of FT2232H, set as input, should not be driven
 * 13       ??            GPIOH5 of FT2232H, set as input, should not be driven
 * 14       ??            GPIOH6 of FT2232H, set as input, should not be driven
 * 15       ??            GPIOH7 of FT2232H, set as input, should not be driven
 *
 *  *)  Pin is driven by Olimex in an active-high way, i.e. 0: driven to GND by Olimex
 *                                                          1: driven to VRef by Olimex
 *  **) Pin is driven by Olimex in an active-low way, i.e.  0: not driven by Olimex -> needs to be driven to high from DUT side (V_reset)
 *                                                          1: driven to GND by Olimex
 *
 * Use command SET_BITS_LOW for GPIO 0 - 7 (byte0: value, byte1: direction)
 * Use command SET_BITS_HIGH for GPIO 8 - 15 (byte0: value, byte1: direction)
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ftdi.hpp"
#include "cables/log.h"


#ifndef min
#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#endif

//-----------------------------------------------------------------------------

Ftdi::Ftdi(js::config *config, Log* log, FTDIDeviceID id) : Cable(config), log (log), m_id (id)
{
  if (config->get("**/vendor") != NULL && config->get("**/product") != NULL)
  {
    m_descriptors.push_back((struct device_desc){(unsigned int)config->get_child_int("**/vendor"), (unsigned int)config->get_child_int("**/product")});
  }
  else
  {
    // add all our known devices to the map
    m_descriptors.push_back((struct device_desc){0x15ba, 0x002a});
    m_descriptors.push_back((struct device_desc){0x15ba, 0x002b});

    m_descriptors.push_back((struct device_desc){0x0403, 0x6010}); // ftdi2232 Gapuino
    m_descriptors.push_back((struct device_desc){0x1d6b, 0x0002}); // ftdi2232 Gapuino
  }
}

Ftdi::~Ftdi()
{
  ftdi_usb_close(&m_ftdic);
  ftdi_deinit(&m_ftdic);

  if (m_params.send_buf) free(m_params.send_buf);
  if (m_params.recv_buf) free(m_params.recv_buf);
}

bool
Ftdi::connect(js::config *config)
{
  char buf[256];
  std::list<struct device_desc> dev_desc = m_descriptors;
  int error;
  bool result;
  int err;
  const char *description = NULL;
  js::config *user_gpios = config->get("user_gpios");

  if (config && config->get("description") != NULL)
  {
    description = config->get("description")->get_str().c_str();
  }

  m_params.send_buf_len   = FTDX_MAXSEND;
  m_params.send_buffered  = 0;
  m_params.send_buf       = (char*)malloc(m_params.send_buf_len);
  m_params.recv_buf_len   = FTDI_MAXRECV;
  m_params.to_recv        = 0;
  m_params.recv_write_idx = 0;
  m_params.recv_read_idx  = 0;
  m_params.recv_buf       = (char*)malloc(m_params.recv_buf_len);

  if (!m_params.send_buf || !m_params.recv_buf) {
    log->error("ftdi2232: Can't allocate memory for ftdi context structures\n");
    goto fail;
  }

  ftdi_init(&m_ftdic);

  if (config->get("bus") != NULL)
  {
#ifdef FTDI_1_4
    if (ftdi_usb_open_bus_addr(&m_ftdic, config->get_child_int("**/bus"), config->get_child_int("**/device")))
    {
      error = 1;
    }
#else
    log->error("Specifying device through USB address is only supported with at least version 1.4 of libftdi\n");
    goto fail;
#endif
  }
  else
  {
    //---------------------------------------------------------------------------
    // Device Selection
    if (description == NULL) {
      std::list<struct device_desc> dev_available;
      struct device_desc dev;

      for (std::list<struct device_desc>::iterator it = dev_desc.begin();
           it != dev_desc.end(); it++) {
        struct ftdi_device_list* devlist;
        int n = ftdi_usb_find_all(&m_ftdic, &devlist, it->vid, it->pid);

        if (n > 0) {
          for(int i = 0; i < n; ++i) {
            if (dev_try_open(it->vid, it->pid, i)) {
              log->user("Found ftdi device i:0x%X:0x%X:%d\n",
                         it->vid, it->pid, i);
              dev_available.push_back({.vid = it->vid, .pid = it->pid, .index = (unsigned int)i});
              break;
            }
          }
        }

        ftdi_list_free2(devlist);
      }

      if (dev_available.size() == 0) {
        log->error("ft2232: Connection failed\n");
        goto fail;
      }

      dev = dev_available.front();
      log->user("Connecting to ftdi device i:0x%X:0x%X:%d\n",
                 dev.vid, dev.pid, dev.index);
      error = ftdi_usb_open_desc_index(&m_ftdic, dev.vid, dev.pid, NULL, NULL, dev.index);
    } else {
      log->user("Connecting to ftdi device %s\n", description);
      error = ftdi_usb_open_string(&m_ftdic, description);
    }
  }

  if (error != 0) {
    if      (error ==  -1) log->debug("usb_find_busses() failed\n");
    else if (error ==  -2) log->debug("usb_find_devices() failed\n");
    else if (error ==  -3) log->debug("usb device not found\n");
    else if (error ==  -4) log->debug("unable to open device\n");
    else if (error ==  -5) log->debug("unable to claim device\n");
    else if (error ==  -6) log->debug("reset failed\n");
    else if (error ==  -7) log->debug("set baudrate failed\n");
    else if (error ==  -8) log->debug("get product description failed\n");
    else if (error ==  -9) log->debug("get serial number failed\n");
    else if (error == -10) log->debug("unable to close device\n");
    else if (error == -11) log->debug("ftdi context invalid\n");
    else if (error == -12) log->debug("libusb_get_device_list() failed\n");
    else if (error == -13) log->debug("libusb_get_device_descriptor() failed\n");

    log->warning("ft2232: Connection failed\n");

    goto fail;
  }

  if(!ft2232_mpsse_open()) {
    log->error("ft2232: Open MPSSE mode failed\n");
    goto fail;
  }

  log->debug("Connected to libftdi driver.\n");

  //---------------------------------------------------------------------------
  // Setup layout for different devices

  int buf_len;
  this->reverse_reset = config->get_child_bool("reverse_reset");
  bits_value = 0;
  bits_direction = 0x1b;

  if (user_gpios != NULL)
  {
    for (auto x:user_gpios->get_elems())
    {
      this->user_gpios.push_back(x->get_int());
      set_bit_direction(x->get_int(), 1);
    }
  }

  if (config->get("system_reset_gpio") != NULL)
  {
    this->system_reset_gpio = config->get_int("system_reset_gpio");
    set_bit_direction(this->system_reset_gpio, 1);
    if (reverse_reset)
      bits_value |= 1<<this->system_reset_gpio;
  }

  if (config->get("jtag_reset_gpio") != NULL)
  {
    this->jtag_reset_gpio = config->get_int("jtag_reset_gpio");
    set_bit_direction(this->jtag_reset_gpio, 1);
    bits_value |= 1<<this->jtag_reset_gpio;
  }


  buf[0] = SET_BITS_LOW;  // Set value & direction of ADBUS lines
  buf[1] = bits_value & 0xff;          // values
  buf[2] = 0x1b;          // direction (1 == output)
  //buf[3] = 0x8a;   // Activate this command to disabled the default divider by 5, otherwise by default we can just go up to 6MHz instead of 30MHz
  buf[3] = TCK_DIVISOR;
  buf[4] = 0x02;         // The divisor has been put to 2 as is not reliable on silicon with less
  // than that
  buf[5] = 0x00;
  buf[6] = SEND_IMMEDIATE;

  buf_len = 7;

  if(ft2232_write(buf, buf_len, 0) != buf_len) {
    log->error("ft2232: Initial write failed\n");
    goto fail;
  }

  flush();

  return true;

fail:
  if (m_params.send_buf) free(m_params.send_buf);
  if (m_params.recv_buf) free(m_params.recv_buf);

  return false;
}


bool Ftdi::chip_config(uint32_t config)
{
  for (auto x:this->user_gpios)
  {
    this->set_bit_value(x, config & 1);
    config >>= 1;
  }

}

bool Ftdi::chip_reset(bool active, int duration)
{
  if (this->system_reset_gpio != -1)
  {
    if (this->reverse_reset)
      active = !active;

    if (!set_bit_value(this->system_reset_gpio, active))
      return false;
  }

  if (duration > 0)
    usleep(duration / 1000);

  return true;
}

int
Ftdi::ft2232_seq_purge(int purge_rx, int purge_tx) {
  int ret = 0;
  unsigned char buf;

  ret = ftdi_usb_purge_buffers(&m_ftdic);
  if (ret < 0) {
    log->warning("ft2232: ftdi_usb_purge_buffers() failed\n");
    return -1;
  }

  ret = ftdi_read_data(&m_ftdic, &buf, 1);
  if (ret < 0) {
    log->warning("ft2232: ftdi_read_data() failed\n");
    return -1;
  }

  return 0;
}

int
Ftdi::ft2232_seq_reset() {
  if (ftdi_usb_reset(&m_ftdic) < 0) {
    log->warning("ft2232: ftdi_usb_reset() failed\n");
    return -1;
  }

  if(ft2232_seq_purge(1, 1) < 0) {
    log->warning("ft2232: Could not purge\n");
    return -1;
  }

  return 0;
}

int
Ftdi::flush() {
  unsigned int xferred;
  unsigned int recvd = 0;

  if (m_params.send_buffered == 0)
    return 0;

  if ((xferred = ftdi_write_data(&m_ftdic, (uint8_t*)m_params.send_buf, m_params.send_buffered)) < 0) {
    log->warning("ft2232: ftdi_write_data() failed\n");
    return -1;
  }

  if (xferred < m_params.send_buffered) {
    log->warning("Written fewer bytes than requested.\n");
    return -1;
  }

  m_params.send_buffered = 0;

  /* now read all scheduled receive bytes */
  if (m_params.to_recv) {
    if (m_params.recv_write_idx + m_params.to_recv > m_params.recv_buf_len) {
      /* extend receive buffer */
      m_params.recv_buf_len = m_params.recv_write_idx + m_params.to_recv;
      if (m_params.recv_buf)
        m_params.recv_buf = (char*)realloc(m_params.recv_buf, m_params.recv_buf_len);
    }

    assert(m_params.recv_buf != NULL);

    while (recvd == 0) {
      recvd = ftdi_read_data(&m_ftdic, (uint8_t*)&(m_params.recv_buf[m_params.recv_write_idx]), m_params.to_recv);
      if (recvd < 0)
        log->warning("Error from ftdi_read_data()\n");
    }

    if (recvd < m_params.to_recv)
      log->warning("Received less bytes than requested.\n");

    m_params.to_recv -= recvd;
    m_params.recv_write_idx += recvd;
  }

  return xferred < 0 ? -1 : xferred;
}

int
Ftdi::ft2232_read(char* buf, int len) {
  int cpy_len;
  int recvd = 0;

  /* flush send buffer to get all scheduled receive bytes */
  if (flush() < 0) {
    log->warning("ft2232: Could not read any data after a flush\n");
    return -1;
  }

  if (len == 0) {
    log->warning("ft2232: Please don't read 0 bits\n");
    return 0;
  }

  /* check for number of remaining bytes in receive buffer */
  cpy_len = m_params.recv_write_idx - m_params.recv_read_idx;

  if (cpy_len > len)
    cpy_len = len;

  len -= cpy_len;

  if (cpy_len > 0) {
    // get data from the receive buffer
    memcpy(buf, &(m_params.recv_buf[m_params.recv_read_idx]), cpy_len);
    m_params.recv_read_idx += cpy_len;

    if (m_params.recv_read_idx == m_params.recv_write_idx)
      m_params.recv_read_idx = m_params.recv_write_idx = 0;
  }

  if (len > 0) {
    /* need to get more data directly from the device */
    while (recvd == 0) {
      recvd = ftdi_read_data(&m_ftdic, (uint8_t*)&(buf[cpy_len]), len);
      if (recvd < 0)
        log->warning("ft2232: Error from ftdi_read_data()\n");
    }
  }

  return recvd < 0 ? -1 : (cpy_len + len);
}

int Ftdi::ft2232_write(char *buf, int len, int recv) 
{
  int xferred = 0;

  // this write function will try to buffer write data
  // buffering will be ceased and a flush triggered in two cases.

  // Case A: max number of scheduled receive bytes will be exceeded
  // with this write
  // Case B: max number of scheduled send bytes has been reached
  if ((m_params.to_recv + recv > FTDI_MAXRECV) || ((m_params.send_buffered > FTDX_MAXSEND) && (m_params.to_recv == 0)))
    xferred = flush();

  if (xferred < 0) {
    log->warning("ft2232: Flush before write failed\n");
    return -1;
  }

  // now buffer this write
  if (m_params.send_buffered + len > m_params.send_buf_len) {
    m_params.send_buf_len = m_params.send_buffered + len;

    if (m_params.send_buf)
      m_params.send_buf = (char*)realloc(m_params.send_buf, m_params.send_buf_len);
  }

  assert(m_params.send_buf);

  memcpy( &(m_params.send_buf[m_params.send_buffered]), buf, len);
  m_params.send_buffered += len;
  if (recv > 0)
    m_params.to_recv += recv;

  if (recv < 0) {
    // immediate write requested, so flush the buffered data
    xferred = flush();
  }

  return xferred < 0 ? -1 : len;
}

bool Ftdi::ft2232_mpsse_open()
{
  char buf[3];
  int ret;

  // This sequence might seem weird and containing superfluous stuff.
  // However, it's built after the description of JTAG_InitDevice
  // Ref. FTCJTAGPG10.pdf
  // Intermittent problems will occur when certain steps are skipped.

  ret = ft2232_seq_reset();
  if (ret < 0)
    goto fail;

  ret = ft2232_seq_purge(1, 0);
  if (ret < 0)
    goto fail;

  ret = ftdi_write_data_set_chunksize(&m_ftdic, FTDX_MAXSEND_MPSSE);
  if (ret < 0) {
    log->warning("ft2232: Got error %s\n", ftdi_get_error_string(&m_ftdic));
    goto fail;
  }

  ret = ftdi_read_data_set_chunksize(&m_ftdic, FTDX_MAXSEND_MPSSE);
  if (ret < 0) {
    log->warning("ft2232: Got error %s\n", ftdi_get_error_string(&m_ftdic));
    goto fail;
  }

  /* set a reasonable latency timer value
     if this value is too low then the chip will send intermediate result data
     in short packets (suboptimal performance) */
  ret = ftdi_set_latency_timer(&m_ftdic, 1);
  if (ret < 0) {
    log->warning("ft2232: ftdi_set_latency_timer() failed\n");
    goto fail;
  }

  ret = ftdi_set_bitmode(&m_ftdic, 0x0b, BITMODE_MPSSE);
  if (ret < 0) {
    log->warning("ft2232: ftdi_set_bitmode() failed\n");
    goto fail;
  }

  ret = ftdi_usb_reset(&m_ftdic);
  if (ret < 0) {
    log->warning("ft2232: ftdi_usb_reset() failed\n");
    goto fail;
  }

  ret = ft2232_seq_purge(1, 0);
  if (ret < 0) {
    log->warning("ft2232: Could not purge\n");
    goto fail;
  }

  // set TCK Divisor
  buf[0] = TCK_DIVISOR;
  buf[1] = 0x2;
  buf[2] = 0x0;
  ret = ft2232_write(buf, 3, 0 );
  if (ret < 0) {
    log->warning("ft2232: Failed to set TCK divisor\n");
    goto fail;
  }

  // switch off loopback
  buf[0] = LOOPBACK_END;
  ret = ft2232_write(buf, 1, 0);
  if (ret < 0) {
    log->warning("ft2232: Failed to switch off loopback\n");
    goto fail;
  }

  ret = flush();
  if (ret < 0) {
    goto fail;
  }

  ret = ftdi_usb_reset(&m_ftdic);
  if (ret < 0) {
    log->warning("ft2232: ftdi_usb_reset() failed\n");
    goto fail;
  }

  ret = ft2232_seq_purge(1, 0);
  if (ret < 0) {
    log->warning("ft2232: Could not purge\n");
    goto fail;
  }

  return true;

fail:
  log->warning("ft2232: Opening device in mpsse mode failed\n");

  ftdi_usb_close(&m_ftdic);
  ftdi_deinit(&m_ftdic);

  return false;
}

bool Ftdi::dev_try_open(unsigned int vid, unsigned int pid, unsigned int index) const
{
  struct ftdi_context ftdic;
  int error;

  ftdi_init(&ftdic);

  error = ftdi_usb_open_desc_index(&ftdic, vid, pid, NULL, NULL, index);
  if (error != 0) {
    ftdi_deinit(&ftdic);
    return false;
  }

  ftdi_usb_close(&ftdic);
  ftdi_deinit(&ftdic);

  return true;
}

bool Ftdi::bit_out(char outbit, bool last)
{
  return stream_out_internal(&outbit, 1, false, last);
}

bool Ftdi::bit_inout(char* inbit, char outbit, bool last)
{
  return stream_inout(inbit, &outbit, 1, last);
}

bool Ftdi::stream_out(char* outstream, unsigned int n_bits, bool last)
{
  return stream_out_internal(outstream, n_bits, false, last);
}

bool Ftdi::stream_out_internal(char* outstream, unsigned int n_bits, bool postread, bool last)
{
  unsigned int len_bytes;
  unsigned int len_bits;
  unsigned int len_tms_bits;
  char buf;

  len_tms_bits = last ? 1 : 0;
  len_bytes    = (n_bits - len_tms_bits) / 8;
  len_bits     = (n_bits - len_tms_bits) % 8;

  if(len_bytes > 0) {
    if (ft2232_write_bytes(outstream, len_bytes, postread) < 0) {
      log->warning("ft2232: ftdi_stream_out has failed\n");
      return false;
    }
  }

  if(len_bits > 0) {
    if (ft2232_write_bits(&(outstream[len_bytes]), len_bits, postread, 0) < 0) {
      log->warning("ft2232: ftdi_stream_out has failed\n");
      return false;
    }
  }

  if(len_tms_bits > 0) {
    buf = outstream[len_bytes] >> len_bits;
    if (ft2232_write_bits(&buf, 1, postread, 1) < 0) {
      log->warning("ft2232: ftdi_stream_out has failed\n");
      return false;
    }
  }

  return true;
}

bool Ftdi::stream_in(char* instream, unsigned int n_bits, bool last)
{
  int len_bytes;
  int len_bits;
  int len_tms_bits;

  len_tms_bits = last ? 1 : 0;
  len_bytes    = (n_bits - len_tms_bits) / 8;
  len_bits     = (n_bits - len_tms_bits) % 8;

  if(len_bytes > 0) {
    if (ft2232_read_packed_bits(instream, len_bytes, 8, 0) < 0) {
      log->warning("ft2232: fdti_stream_in has failed\n");
      return false;
    }
  }

  if(len_bits > 0) {
    if (ft2232_read_packed_bits(instream, 1, len_bits, len_bytes * 8) < 0) {
      log->warning("ft2232: fdti_stream_in has failed\n");
      return false;
    }
  }

  if(len_tms_bits > 0) {
    if (ft2232_read_packed_bits(instream, 1, 1, (len_bits + (len_bytes * 8))) < 0) {
      log->warning("ft2232: fdti_stream_in has failed\n");
      return false;
    }
  }

  return true;
}

bool Ftdi::stream_inout(char* instream, char* outstream, unsigned int n_bits, bool last)
{
  if (outstream)
  {
    if (!stream_out_internal(outstream, n_bits, instream != NULL, last)) {
      log->warning("ft2232: ftdi_stream_inout has failed\n");
      return false;
    }
  }
  else
  {
    int bytes = (n_bits + 7) / 8;
    char buffer[bytes];
    ::memset(buffer, 0, bytes);
    if (!stream_out_internal(buffer, n_bits, instream != NULL, last)) {
      log->warning("ft2232: ftdi_stream_inout has failed\n");
      return false;
    }
  }

  if (instream && !stream_in(instream, n_bits, last)) {
    log->warning("ft2232: ftdi_stream_inout has failed\n");
    return false;
  }

  return true;
}

int Ftdi::ft2232_write_bytes(char *buf, int len, bool postread)
{
  int cur_command_size;
  int max_command_size;
  int cur_chunk_len;
  int recv;
  int xferred;
  char *mybuf;

  if(len == 0)
    return 0;

  recv = 0;
  max_command_size = min(len, 65536)+3;
  mybuf = (char*) malloc(max_command_size);

  /// Command OPCODE: write bytes
  mybuf[0] = MPSSE_DO_WRITE | MPSSE_LSB | MPSSE_WRITE_NEG;
  if(postread) // if postread is enabled it will buffer incoming bytes
    mybuf[0] = mybuf[0] | MPSSE_DO_READ;

  // We divide the transmitting stream of bytes in chunks with a maximun length of 65536 bytes each.
  while(len > 0) {
    cur_chunk_len = min(len, 65536);
    len = len - cur_chunk_len;
    cur_command_size = cur_chunk_len + 3;

    /// Low and High bytes of the length field
    mybuf[1] = (unsigned char) ( cur_chunk_len - 1);
    mybuf[2] = (unsigned char) ((cur_chunk_len - 1) >> 8);

    /// The rest of the command is filled with the bytes that will be transferred
    memcpy(&(mybuf[3]), buf, cur_chunk_len );
    buf = buf + cur_chunk_len;

    /// Finally we can transmit the command
    xferred = ft2232_write(mybuf, cur_command_size, (postread ? cur_chunk_len : 0) );
    if(xferred != cur_command_size) {
      log->warning("ft2232: could not transmit command\n");
      free(mybuf);
      return -1;
    }

    // If OK, the update the number of incoming bytes that are being buffered for a posterior read
    if(postread)
      recv = recv + cur_chunk_len;
  }

  free(mybuf);

  // Returns the number of buffered incoming bytes
  return recv;
}

int Ftdi::ft2232_write_bits(char *buf, int len, bool postread, bool with_tms)
{
  int max_command_size;
  int max_chunk_len;
  int cur_chunk_len;
  int recv;
  int xferred;
  int i;
  char mybuf[3];

  if(len == 0)
    return 0;

  max_command_size = 3;

  if(!with_tms) {
    /// Command OPCODE: write bits (can write up to 8 bits in a single command)
    max_chunk_len = 8;
    mybuf[0] = MPSSE_DO_WRITE | MPSSE_LSB | MPSSE_WRITE_NEG | MPSSE_BITMODE;
  }
  else {
    /// Command OPCODE: 0x4B write bit with tms (can write up to 1 bits in a single command)
    max_chunk_len = 1;
    mybuf[0] = MPSSE_WRITE_TMS|MPSSE_LSB|MPSSE_BITMODE|MPSSE_WRITE_NEG;
  }

  if(postread) // (OPCODE += 0x20) if postread is enabled it will buffer incoming bits
    mybuf[0] = mybuf[0] | MPSSE_DO_READ;

  // We divide the transmitting stream of bytes in chunks with a maximun length of max_chunk_len bits each.
  i = 0;
  recv = 0;
  while(len > 0) {
    cur_chunk_len = min(len, max_chunk_len);
    len = len - cur_chunk_len;

    // Bit length field
    mybuf[1] = (char) (cur_chunk_len - 1);


    if(!with_tms) {
      /// The last byte of the command is filled with the bits that will be transferred
      mybuf[2] = buf[i/8];
      i += 8;
    }
    else {
      mybuf[2] = 0x01 | ((buf[(i/8)] >> (i%8)) << 7);
      i++;
    }

    // Finally we can transmit the command
    xferred = ft2232_write(mybuf, max_command_size, (postread ? 1 : 0) );
    if(xferred != max_command_size) {
      log->warning("ft2232: ftdi write has failed\n");
      return -1;
    }

    // If OK, the update the number of incoming bytes that are being buffered for a posterior read
    if(postread)
      recv = recv + 1;
  }

  if (flush() < 0)
    return -1;

  return recv;
}

int Ftdi::ft2232_read_packed_bits(char *buf, int packet_len, int bits_per_packet, int offset)
{
  char *mybuf;
  unsigned char dst_mask;
  unsigned char src_mask;
  int row_offset;
  int dst_row;
  int dst_col;
  int src_row;
  int src_col;
  int i;

  if(packet_len == 0 || bits_per_packet == 0)
    return 0;

  if (offset == 0 && bits_per_packet == 8)
  {
    if(ft2232_read(buf, packet_len) < 0) {
      log->warning("Read failed\n");
      return -1;
    }
  }
  else
  {
    mybuf = (char*) malloc(packet_len);
    if(ft2232_read(mybuf, packet_len) < 0) {
      log->warning("Read failed\n");
      free(mybuf);
      return -1;
    }

    if(bits_per_packet < 8) {
      for(i=0; i < packet_len; i++) { // rotate bits to the left side
        mybuf[i] = (mybuf[i] >> (8-bits_per_packet));
      }

      for(i = offset; i < (packet_len*bits_per_packet+offset); i++) {
        dst_row = i / 8;
        dst_col = i % 8;
        src_row = (i-offset) / bits_per_packet;
        src_col = (i-offset) % bits_per_packet;
        dst_mask = ~(1 << dst_col);
        src_mask = (1 << src_col);
        buf[dst_row] = (buf[dst_row] & dst_mask) | ((mybuf[src_row] & src_mask) >> (dst_col - src_col));
      }
    } else if(bits_per_packet == 8) {
      row_offset = offset / 8;
      memcpy( &(buf[row_offset]), mybuf, packet_len);
    } else {
      free(mybuf);
      return -1;
    }

    free(mybuf);
  }

  return 0;
}

bool Ftdi::set_bit_value(int bit, int value)
{

  char buf[4];

  bits_value = (bits_value & ~(1<<bit)) | (value << bit);
  if (bit >= 8)
  {
    buf[0] = SET_BITS_HIGH;
    buf[1] = bits_value >> 8;
    buf[2] = bits_direction >> 8;
    buf[3] = SEND_IMMEDIATE;
  }
  else
  {
    buf[0] = SET_BITS_LOW;
    buf[1] = bits_value;
    buf[2] = bits_direction;
    buf[3] = SEND_IMMEDIATE;
  }

  if (ft2232_write(buf, 4, 0) != 4) return false;
  flush();
  return true;
}

bool Ftdi::set_bit_value_del(int bit, int value, int del)
{

  char buf[4];

  bits_value = (bits_value & ~(1<<bit)) | (value << bit);
  if (bit >= 8)
  {
    buf[0] = SET_BITS_HIGH;
    buf[1] = bits_value >> 8;
    buf[2] = bits_direction >> 8;
    buf[3] = SEND_IMMEDIATE;
  }
  else
  {
    buf[0] = SET_BITS_LOW;
    buf[1] = bits_value;
    buf[2] = bits_direction;
    buf[3] = SEND_IMMEDIATE;
  }

  if (ft2232_write(buf, 4, 0) != 4) return false;
  usleep(del);
  flush();
  return true;
}

bool Ftdi::set_bit_direction(int bit, int isout)
{
  bits_direction = (bits_direction & ~(1<<bit)) | (isout << bit);
}

bool Ftdi::jtag_reset(bool active)
{
  if (this->jtag_reset_gpio != -1)
    return set_bit_value(this->jtag_reset_gpio, !active);
  return true;
}

