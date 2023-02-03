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
 * command_list.c
 *
 * This file holds implementation of linked list.
 *
 ******************************************************************************
 */

#include "command_list.h"

/**
 * Instantiates the Command List
 * @param[in] cmd_list Pointer to I3C_cmd_list object.
 * @param[in] buffer Pointer to buffer for command stored on list
 * @param[in] capacity Maximum capacity of the list
 * @return EOK On success
 * @return EINVAL If cmd_list points to NULL
 */
uint32_t CLInit(I3C_cmd_list* cmd_list, I3C_cmd_list_element* buffer,
        size_t capacity) {
    uint32_t ret_val = EINVAL;
    size_t i;

    if (cmd_list != NULL) {
        cmd_list->buffer = buffer;
        cmd_list->capacity = capacity;
        cmd_list->count = 0U;
        // All buffer elements are unused
        for (i = 0U; i < cmd_list->capacity; i++) {
            cmd_list->buffer[i].used = false;
        }
        ret_val = EOK;
    }
    return ret_val;
}

/**
 * Clear all fields
 * @param[in] cmd_list Pointer to I3C_cmd_list object.
 */
void CLDestroy(I3C_cmd_list* cmd_list) {
    cmd_list->buffer = NULL;
    cmd_list->count = 0U;
    cmd_list->capacity = 0U;
}

/**
 * Returns unused buffer element
 * @param[in] cmd_list Pointer to I3C_cmd_list object.
 * @return Buffer element
 */
static I3C_cmd_list_element* get_free_node(const I3C_cmd_list* cmd_list) {
    static const I3C_cmd_t zeroed_descriptor = { 0 };
    I3C_cmd_list_element* elem = NULL;
    size_t i;
    for (i = 0U; i < cmd_list->capacity; i++) {
        if (cmd_list->buffer[i].used == false) {
            elem = &cmd_list->buffer[i];
            // Mark this element as used
            elem->used = true;
            // Reset all fields
            elem->cmd = zeroed_descriptor;
            break;
        }
    }
    return elem;
}

/**
 * Adds element to the list and returns pointer to added command
 * @param[in] cmd_list Pointer to I3C_cmd_list object.
 * @param[out] descriptor Pointer to command added to list
 * @return EOK On success
 * @return ENOMEM If list is full
 */
uint32_t CLAppend(I3C_cmd_list* cmd_list, volatile I3C_cmd_t** descriptor) {
    uint32_t ret_val;
    I3C_cmd_list_element* elem;

    if (cmd_list->count >= cmd_list->capacity) {
        ret_val = ENOMEM;
    } else {
        elem = get_free_node(cmd_list);
        // Check if returned element is not null
        if (elem != NULL) {
            *descriptor = &(elem->cmd);
            // Increment number of list elements
            cmd_list->count++;
            ret_val = EOK;
        } else {
            ret_val = ENOMEM;
        }
    }

    return ret_val;
}

/**
 * Removes element from the list
 * @param[in] cmd_list Pointer to I3C_cmd_list object.
 * @param[in] cmd_id ID of command to delete
 * @return EOK On success
 * @return ENOENT If list command with given ID was not found
 * @return EOPNOTSUPP If list is empty
 */
uint32_t CLDelete(I3C_cmd_list* cmd_list, uint8_t cmd_id) {
    uint32_t ret_val;
    size_t i;

    // Don't do anything if list is empty
    if (cmd_list->count == 0U) {
        ret_val = EOPNOTSUPP;
    } else {
	// No entry if command with given ID not found
        ret_val = ENOENT;

        for (i = 0U; i < cmd_list->capacity; i++) {
            if ((cmd_list->buffer[i].used == true)
                    && (cmd_list->buffer[i].cmd.id == cmd_id)) {
        	// Free this element, reset when allocated again
                cmd_list->buffer[i].used = false;
                ret_val = EOK;
                cmd_list->count--;
            }
        }
    }

    return ret_val;
}

/**
 * Search and returns command with given ID
 * @param[in] cmd_list Pointer to I3C_cmd_list object.
 * @param[out] descriptor Pointer to command added to list
 * @param[in] cmd_id ID of command to delete
 * @return EOK On success
 * @return ENOENT If list command with given ID was not found
 * @return EOPNOTSUPP If list is empty
 */
uint32_t CLSearch(const I3C_cmd_list* cmd_list, volatile I3C_cmd_t** descriptor,
        uint8_t cmd_id) {
    uint32_t ret_val;
    size_t i;

    // Don't do anything if list is empty
    if (cmd_list->count == 0U) {
        ret_val = EOPNOTSUPP;
    } else {
	// No entry if command with given ID not found
        ret_val = ENOENT;

        for (i = 0U; i < cmd_list->capacity; i++) {
            if ((cmd_list->buffer[i].used == true)
                    && (cmd_list->buffer[i].cmd.id == cmd_id)) {
                *descriptor = &(cmd_list->buffer[i].cmd);
                ret_val = EOK;
            }
        }
    }

    return ret_val;
}

/**
 * Returns number of elements on the list
 * @param[in] cmd_list Pointer to I3C_cmd_list object.
 * @return number of elements
 */
size_t CLGetNumberOfUsedLocations(const I3C_cmd_list* cmd_list) {
    return cmd_list->count;
}

/**
 * Returns if lsit is empty or not
 * @param[in] cmd_list Pointer to I3C_cmd_list object.
 * @return If list is empty
 */
bool CLEmpty(const I3C_cmd_list* cmd_list) {
    return (cmd_list->count == 0U);
}

/**
 * Allows to traverse all used elements of the list calling callback function
 * @param[in] cmd_list Pointer to I3C_cmd_list object.
 * @param[in] pd Pointer to I3C_PrivData object.
 * @patam[in] callback Callback function which is called for every command
 * @return EOK On success
 * @return EOPNOTSUPP If list is empty
 */
uint32_t CLTraverse(I3C_cmd_list* cmd_list, I3C_PrivData *pd,
        uint32_t (*callback)(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)) {
    uint32_t ret_val = EOK;
    size_t i;

    // Don't do anything if list is empty
    if (cmd_list->count == 0U) {
        ret_val = EOPNOTSUPP;
    } else {
        for (i = 0U; i < cmd_list->capacity; i++) {
            if (cmd_list->buffer[i].used == true) {
        	// Execute callback with command from list as a parameter
                ret_val = callback(pd, &(cmd_list->buffer[i].cmd));
            }
        }
    }

    return ret_val;
}

