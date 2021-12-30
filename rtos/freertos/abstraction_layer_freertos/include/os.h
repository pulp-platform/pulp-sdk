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

#ifndef __OS_H__
#define __OS_H__

#include <stdlib.h>
#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "riscv.h"
#include "properties.h"
#include "pmsis_types.h"


static inline void pmsis_exit(int err)
{
	exit(err);
}

static inline int pmsis_kickoff(void *arg)
{
	BaseType_t xTask;
	TaskHandle_t xHandler0 = NULL;

	xTask = xTaskCreate(arg, "main",
			    ((unsigned short)configMINIMAL_STACK_SIZE), NULL,
			    tskIDLE_PRIORITY + 1, &xHandler0);

	if (xTask != pdPASS) {
		printf("main is NULL !\n");
		pmsis_exit(-1);
	}

	__enable_irq();

	struct pmsis_event_kernel_wrap *wrap;
	/* TODO: blocks everything because priority too high */
	/* TODO: spawn event loop for callbacks or use FreeRTOS callback api */
	/* pmsis_event_kernel_init(&wrap, pmsis_event_kernel_main); */
	/* pmsis_event_set_default_scheduler(wrap); */

	/* TODO:handle case when uart is being used before initialized */
#ifdef DEBUG
	puts("pmsis_kickoff: starting scheduler\n");
#endif
	/* Start the kernel. From here on only tasks and interrupts will run. */
	vTaskStartScheduler();

	/* should never return here */
	return 0;
}

static inline void *pmsis_task_create(void (*entry)(void *), void *arg,
				      char *name, int priority)
{
	TaskHandle_t task_handle = NULL;
	BaseType_t task_ret;
	task_ret = xTaskCreate(entry, name, 2 * configMINIMAL_STACK_SIZE, arg,
			       tskIDLE_PRIORITY + 1 + priority, &task_handle);
	if (task_ret != pdPASS) {
		return NULL;
	} else {
		return task_handle;
	}
}

static inline void pmsis_task_suspend(void *task_handler)
{
	vTaskSuspend((TaskHandle_t)task_handler);
}

static inline void pi_yield()
{
	taskYIELD();
}

static inline uint32_t disable_irq(void)
{
	hal_compiler_barrier();
	return __disable_irq();
}

static inline void restore_irq(uint32_t irq_enable)
{
	hal_compiler_barrier();
	__restore_irq(irq_enable);
}

/* TODO: fix these functions, seems broken */
static inline void __os_native_api_sem_take(void *sem_object)
{
	uint32_t irq = __disable_irq();
	if (__get_mcause() & MCAUSE_IRQ_Msk) {
		/* This case should never happen ! */
		BaseType_t ret;
		xSemaphoreTakeFromISR(sem_object, &ret);
	} else {
		xSemaphoreTake(sem_object, portMAX_DELAY);
	}
	__restore_irq(irq);
}

static inline void __os_native_api_sem_give(void *sem_object)
{
	uint32_t irq = __disable_irq();
	if (__get_mcause() & MCAUSE_IRQ_Msk) {
		BaseType_t ret;
		xSemaphoreGiveFromISR(sem_object, &ret);
		portYIELD_FROM_ISR(ret);
	} else {
		BaseType_t ret;
		xSemaphoreGiveFromISR(sem_object, &ret);
	}
	__restore_irq(irq);
}


static inline int pi_sem_init(pi_sem_t *sem)
{
	hal_compiler_barrier();
	sem->sem_object = xSemaphoreCreateCounting(0xFFu, 0);
	if (sem->sem_object == NULL) {
		return -1;
	}
	sem->take = __os_native_api_sem_take;
	sem->give = __os_native_api_sem_give;
	return 0;
}

static inline int pi_sem_deinit(pi_sem_t *sem)
{
	hal_compiler_barrier();
	if (sem->sem_object == NULL) {
		return -1;
	}
	vSemaphoreDelete(sem->sem_object);
	sem->take = NULL;
	sem->give = NULL;
	sem->sem_object = (void *)NULL;
	return 0;
}

static inline void pi_sem_take(pi_sem_t *sem)
{
	hal_compiler_barrier();
	sem->take(sem->sem_object);
}

static inline void pi_sem_give(pi_sem_t *sem)
{
	sem->give(sem->sem_object);
	hal_compiler_barrier();
}

static inline void pmsis_mutex_take(pmsis_mutex_t *mutex)
{
	hal_compiler_barrier();
	mutex->take(mutex->mutex_object);
}

static inline void pmsis_mutex_release(pmsis_mutex_t *mutex)
{
	hal_compiler_barrier();
	mutex->release(mutex->mutex_object);
	hal_compiler_barrier();
}

static inline void __os_native_api_mutex_lock(void *mutex_object)
{
	xSemaphoreTake(mutex_object, portMAX_DELAY);
}

static inline void __os_native_api_mutex_release(void *mutex_object)
{
	uint32_t irq = __disable_irq();
	BaseType_t ret;
	xSemaphoreGiveFromISR(mutex_object, &ret);
	__restore_irq(irq);
}

static inline int pmsis_mutex_init(pmsis_mutex_t *mutex)
{
	hal_compiler_barrier();
	mutex->mutex_object = xSemaphoreCreateBinary();
	if (mutex->mutex_object == NULL) {
		return -1;
	}
	__os_native_api_mutex_release(mutex->mutex_object);
	mutex->take = __os_native_api_mutex_lock;
	mutex->release = __os_native_api_mutex_release;
	return 0;
}

static inline int pmsis_mutex_deinit(pmsis_mutex_t *mutex)
{
	hal_compiler_barrier();
	if (mutex->mutex_object == NULL) {
		return -1;
	}
	vSemaphoreDelete(mutex->mutex_object);
	mutex->take = NULL;
	mutex->release = NULL;
	mutex->mutex_object = (void *)NULL;
	return 0;
}

static inline void pmsis_spinlock_init(pmsis_spinlock_t *spinlock)
{
	hal_compiler_barrier();
	spinlock->lock = 0;
}

static inline void pmsis_spinlock_take(pmsis_spinlock_t *spinlock)
{
	uint32_t irq_enabled = disable_irq();
	hal_compiler_barrier();
	spinlock->lock = 1;
	hal_compiler_barrier();
	restore_irq(irq_enabled);
}

static inline void pmsis_spinlock_release(pmsis_spinlock_t *spinlock)
{
	uint32_t irq_enabled = disable_irq();
	hal_compiler_barrier();
	spinlock->lock = 0;
	hal_compiler_barrier();
	restore_irq(irq_enabled);
}

static void pi_time_wait_us(int time_us)
{
	/* Wait less than 1 ms. */
	if (time_us < 1000) {
		for (volatile int i = 0; i < time_us; i++)
			;
	} else {
		vTaskDelay(((uint32_t)time_us / 1000) / portTICK_PERIOD_MS);
	}
}

#endif /* __OS_H__ */
