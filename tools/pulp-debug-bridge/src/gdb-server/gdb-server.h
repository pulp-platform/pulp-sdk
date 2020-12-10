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
 * Authors: Andreas Traber
 */

#ifndef __GDB_SERVER_GDB_SERVER_H__
#define __GDB_SERVER_GDB_SERVER_H__

#include <list>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>


class Rsp {
  public:
    Rsp(int socket_ptr);
    //Rsp(int socket_port, MemIF* mem, LogIF *log, std::list<DbgIF*> list_dbgif, std::list<DbgIF*> list_dbg_cluster_if, BreakPoints* bp, DbgIF *main_if);

  private:
    bool open();
    void close();

    bool wait_client();
    bool loop();

    bool decode(char* data, size_t len);

    bool multithread(char* data, size_t len);

    bool cont(char* data, size_t len); // continue, reserved keyword, thus not used as function name
    bool step(char* data, size_t len);

    bool query(char* data, size_t len);
    bool v_packet(char* data, size_t len);

    bool regs_send();
    bool reg_read(char* data, size_t len);
    bool reg_write(char* data, size_t len);

    bool get_packet(char* data, size_t* len);

    bool signal();

    bool send(const char* data, size_t len);
    bool send_str(const char* data);


    // internal helper functions
    bool pc_read(unsigned int* pc);

    bool waitStop(DbgIF* dbgif);
    bool resume(bool step);
    bool resume(int tid, bool step);
    void resumeCoresPrepare(DbgIF *dbgif, bool step);
    void resumeCores();

    bool mem_read(char* data, size_t len);
    bool mem_write_ascii(char* data, size_t len);
    bool mem_write(char* data, size_t len);

    bool bp_insert(char* data, size_t len);
    bool bp_remove(char* data, size_t len);

    DbgIF* get_dbgif(int thread_id);

    int m_socket_port;
    int m_socket_in;
    int m_socket_client;

    int m_thread_sel;
    int m_thread_init;
    MemIF* m_mem;
    LogIF *log;
    BreakPoints* m_bp;
    std::list<DbgIF*> m_dbgifs;
    std::list<DbgIF*> m_dbg_cluster_ifs;
};

#endif