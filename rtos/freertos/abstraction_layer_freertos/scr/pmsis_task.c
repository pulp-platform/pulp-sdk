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

#include "pmsis.h"
#include <soc.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>

pi_task_t *__pi_task_block(pi_task_t *callback_task)
{
	callback_task->id = PI_TASK_NONE_ID;
	callback_task->done = 0;
	pi_sem_init(&(callback_task->wait_on));
	/* lock the mutex so that task may be descheduled while waiting on it */
	callback_task->destroy = 1;
	callback_task->core_id = -1;
	return callback_task;
}

pi_task_t *__pi_task_callback(pi_task_t *callback_task, void (*func)(void *),
			      void *arg)
{
	callback_task->id = PI_TASK_CALLBACK_ID;
	callback_task->arg[0] = (uintptr_t)func;
	callback_task->arg[1] = (uintptr_t)arg;
	callback_task->done = 0;
	callback_task->wait_on.sem_object = (void *)NULL;
	callback_task->destroy = 0;
	callback_task->core_id = -1;
	return callback_task;
}

void __pi_task_destroy(pi_task_t *task)
{
	if (task->destroy) {
		task->destroy = 0;
		/* if the mutex is only virtual (e.g. wait on soc event) */
		hal_compiler_barrier();
		/* if the sched support semaphore/mutexes */
		if (task->wait_on.sem_object) {
			pi_sem_deinit(&task->wait_on);
			task->wait_on.sem_object = (void *)NULL;
		}
		hal_compiler_barrier();
	}
}

void __pi_task_wait_on(pi_task_t *task)
{
	while (!task->done) {
		/* if the underlying scheduler support it, deschedule the task
		 */
		if (task->wait_on.sem_object != NULL) {
			pi_sem_take(&task->wait_on);
		}
		DEBUG_PRINTF("[%s] waited on sem %p\n", __func__,
			     &(task->wait_on));
	}
	hal_compiler_barrier();
	__pi_task_destroy(task);
}

void __pi_task_push(pi_task_t *task)
{
	pi_task_push_delayed_us(task, 0);
}

pi_task_t *pi_task_callback_no_mutex(pi_task_t *callback_task,
				     void (*func)(void *), void *arg)
{
	return pi_task_callback(callback_task, func, arg);
}

pi_task_t *pi_task_block_no_mutex(pi_task_t *callback_task)
{
	callback_task->id = PI_TASK_NONE_ID;
	callback_task->done = 0;
	callback_task->wait_on.sem_object = (void *)NULL;
	callback_task->destroy = 0;
	callback_task->core_id = -1;
	/* lock the mutex so that task may be descheduled while waiting on it */
	return callback_task;
}

void pi_task_release(pi_task_t *task)
{
	DEBUG_PRINTF("[%s] releasing task %p\n", __func__, task);
	/* if the mutex is only virtual (e.g. wait on soc event)
	 * if the sched support semaphore/mutexes */
	if (task->wait_on.sem_object) {
		DEBUG_PRINTF("[%s] sem give %p\n", __func__, &task->wait_on);
		pi_sem_give(&(task->wait_on));
	}
	hal_compiler_barrier();
	task->done = 1;
	hal_compiler_barrier();
}

void pi_cl_pi_task_wait(pi_task_t *task)
{
}

void pi_cl_pi_task_notify_done(pi_task_t *task)
{
}

void pi_task_wait_on_no_mutex(pi_task_t *task)
{
	/* if the mutex is only virtual (e.g. wait on soc event) */
	while (!task->done) {
		hal_compiler_barrier();
		asm volatile("nop");
		/* FIXME: workaround for gcc bug */
		hal_compiler_barrier();
	}
}

void pi_task_push_delayed_us(pi_task_t *task, uint32_t delay)
{
	pi_time_wait_us(delay);
	uint32_t irq = disable_irq();
	/* TODO: unimplemented callback */
	/* pmsis_event_push(pmsis_event_get_default_scheduler(), task); */
	restore_irq(irq);
}
