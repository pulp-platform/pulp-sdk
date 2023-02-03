/******************************************************************************
*
* Copyright 2014-2018 Cadence Design Systems, Inc.
*
******************************************************************************
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*
* command_list.h
*
* This file holds declarations and definitions for linked list
*
******************************************************************************
*/

#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#include <string.h>
#include <cdn_errno.h>
#include "i3c_structs_if.h"

uint32_t CLInit(I3C_cmd_list* cmd_list, I3C_cmd_list_element* buffer, size_t capacity);
void CLDestroy(I3C_cmd_list* cmd_list);
uint32_t CLAppend(I3C_cmd_list* cmd_list, volatile I3C_cmd_t** descriptor);
uint32_t CLDelete(I3C_cmd_list* cmd_list, uint8_t cmd_id);
uint32_t CLSearch(const I3C_cmd_list* cmd_list, volatile I3C_cmd_t** descriptor, uint8_t cmd_id);
size_t CLGetNumberOfUsedLocations(const I3C_cmd_list* cmd_list);
bool CLEmpty(const I3C_cmd_list* cmd_list);
uint32_t CLTraverse(I3C_cmd_list* cmd_list, I3C_PrivData *pd, uint32_t (*callback)(I3C_PrivData *pd, volatile I3C_cmd_t *cmd));


#endif /* COMMAND_LIST_H */
