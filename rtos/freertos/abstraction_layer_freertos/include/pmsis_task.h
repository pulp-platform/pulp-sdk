/*
 * Copyright 2020 GreenWaves Technologies
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
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __PMSIS_TASK_H__
#define __PMSIS_TASK_H__

#include "pmsis_types.h"
#include "target.h"
#include "os.h"

#include <FreeRTOS.h>
#include <task.h>

/**
 * \brief Prepare a blocking event task.
 *
 * This function initializes an instance of event task.
 * A semaphore will be initialized.
 *
 * \param task           Pointer to event task.
 *
 * \return task This function returns the event task initialized.
 */
pi_task_t *__pi_task_block(pi_task_t *task);

/**
 * \brief Prepare an event task with callback.
 *
 * This function initializes an instance of event task.
 * This event task executes the callback given in argument.
 *
 * \param callback_task  Pointer to event task.
 * \param func           Callback function.
 * \param arg            Callback function argument.
 *
 * \return task This function returns the event task initialized.
 */
pi_task_t *__pi_task_callback(pi_task_t *callback_task, void (*func)(void *),
			      void *arg);

/**
 * \brief Wait on an event task.
 *
 * This function allows to wait on an event task until the event occurs.
 *
 * \param task           Pointer to event task.
 */
void __pi_task_wait_on(pi_task_t *task);

/**
 * \brief Push an event task.
 *
 * This function is used to push an event task to the event kernel.
 *
 * \param task           Pointer to event task.
 */
void __pi_task_push(pi_task_t *task);

/**
 * \brief Delete an event task.
 *
 * This function removes an event task.
 * It will delete the semaphore if it has been allocated.
 *
 * \param task           Pointer to event task.
 */
void __pi_task_destroy(pi_task_t *task);

static inline pi_task_t *pi_task_block(pi_task_t *task)
{
	return __pi_task_block(task);
}

/**
 * \brief Prepare a notification callback.
 *
 * This initializes a notification callback so that it is ready to be
 * triggered.
 * A notification callback can be used to trigger a function execution when
 * a certain action occurs, e.g. an end of transfer.
 *
 * \param task           Pointer to notification event.
 * \param callback       The callback which will be executed when the
 *                       notification is triggered.
 * \param arg            The argument to the callback.
 *
 * \return task          The notification event initialized.
 *
 * \note This structure is allocated by the caller and must be kept alive until
 *       the pi_task_wait_on returns.
 * \note If the same notification is re-used several times, it must be
 *       reinitialized everytime by calling this function or another variant.
 * \note A notification callback can not be used to block the caller execution
 *       with pi_task_wait_on.
 */
static inline pi_task_t *pi_task_callback(pi_task_t *task,
					  void (*callback)(void *), void *arg)
{
	return __pi_task_callback(task, callback, arg);
}

/**
 * \brief Wait until a notification event is triggered.
 *
 * This can be called to block the caller until the specified notification
 * event (created with pi_task_block) has been triggered.
 *
 * \param task           Pointer to notification event.
 *
 * \note The notification event is released just before returning from this call
 *       and must be reinitialized before it can be re-used.
 */
static inline void pi_task_wait_on(pi_task_t *task)
{
	__pi_task_wait_on(task);
}

/**
 * \brief Trigger a notification.
 *
 * This can be used to trigger the specified notification. If the notification
 * is a callback, this will schedule the callback execution. If the notification
 * is an event, this will trigger the event.
 *
 * \param task           Pointer to notification event.
 */
static inline void pi_task_push(pi_task_t *task)
{
	__pi_task_push(task);
}

static inline void pi_task_destroy(pi_task_t *task)
{
	__pi_task_destroy(task);
}

void pi_task_release(pi_task_t *task);

/**
 * \brief Trigger a notification.
 *
 * This can be used to trigger the specified notification after the specified
 * delay, given in micro-seconds. If the notification
 * is a callback, this will schedule the callback execution. If the notification
 * is an event, this will trigger the event.
 *
 * \param task           Pointer to notification event.
 * \param delay          The number of micro-seconds after which the
 * notification must be triggered.
 */
void pi_task_push_delayed_us(pi_task_t *task, uint32_t delay);


#endif /* __PMSIS_TASK_H__ */
