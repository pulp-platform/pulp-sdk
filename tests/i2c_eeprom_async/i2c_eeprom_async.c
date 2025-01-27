/*
 * Copyright 2021 Greenwaves Technologies
 * Copyright 2021 ETH Zurich
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
 * Modify by Nico Orlando, nico.orlando@studio.unibo.it
 */

/*
 * Test 24FC1025 eeprom (located at i2c address 0x50 in bus 0)
 */
#ifdef USE_PULPOS_TEST
#include "pmsis.h"
#include <bsp/bsp.h>
#endif

#ifdef USE_FREERTOS_TEST
/* FreeRTOS kernel includes. */
#include <FreeRTOS.h>
#include <task.h>

/* c stdlib */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

/* system includes */
#include "system.h"
#include "timer_irq.h"
#include "fll.h"
#include "irq.h"
#include "gpio.h"
#include "i2c.h"
#include "udma.h"
#include "udma_i2c.h"
#include "pi_errno.h"
#include "pmsis_task.h"
#include "fc_event.h"
#include "freq.h"
#include "debug.h"
/* pmsis */
#include "target.h"
#include "os.h"
#include "pi_errno.h"

void vApplicationMallocFailedHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
#endif

/* I2C device/peripheral id */
#define I2C_DEV_ID 0
/* I2C address of the eeprom */
#define I2C_EEPROM_ADDR 0x50

static struct pi_device i2c;

void eeprom(void)
{
	/* initalize i2c */
	struct pi_i2c_conf i2c_conf;

	printf("opening eeprom on i2c bus %d\n", I2C_DEV_ID);

	pi_i2c_conf_init(&i2c_conf);
	i2c_conf.itf = I2C_DEV_ID;
	i2c_conf.max_baudrate = 100000;
	pi_i2c_conf_set_slave_addr(&i2c_conf, I2C_EEPROM_ADDR << 1, 0);
	/* pi_i2c_conf_set_wait_cycles(conf, 2048); */

	pi_open_from_conf(&i2c, &i2c_conf);

	if (pi_i2c_open(&i2c)) {
		printf("i2c open failed\n");
		exit(1);
	}

	uint8_t expected_rx[] = {
		0xca,
		0x00,
		0xde,
		0xca,
		0xc0,
		0xfe
	};
	uint8_t rx[sizeof(expected_rx)] = {0};

	uint8_t tx[] = {
		0x00,  /* addr msb */
		0x00,  /* addr lsb */
		expected_rx[0],
		expected_rx[1],
		expected_rx[2],
		expected_rx[3],
		expected_rx[4],
		expected_rx[5]
	};

	/* the address of our i2c eeprom is normally 0x50 if you want to
	 * specifically test that */
	printf("writing eeprom\n");

	pi_task_t task_write_buffer;	
	pi_i2c_write_async(&i2c, tx, sizeof(tx), PI_I2C_XFER_START | PI_I2C_XFER_STOP, pi_task_block(&task_write_buffer));
	pi_task_wait_on(&task_write_buffer);
	/* Wait for write to finish. It takes 5 million ns = 5 ms to finish. */
	for (volatile int i = 0; i < 100000; ++i)
		i++;

	printf("reading eeprom\n");
	uint8_t eeprom_addr[] = {
		0x00, /* addr msb */
		0x00, /* addr lsb */
	};

	pi_task_t task_write_addr;
	pi_i2c_write_async(&i2c, eeprom_addr, sizeof(eeprom_addr), PI_I2C_XFER_START | PI_I2C_XFER_NO_STOP, pi_task_block(&task_write_addr));	
	//pi_i2c_write(&i2c, eeprom_addr, sizeof(eeprom_addr), PI_I2C_XFER_START | PI_I2C_XFER_NO_STOP);	
	pi_task_t task_read_buffer;
	pi_i2c_read_async(&i2c, rx, sizeof(rx), PI_I2C_XFER_START | PI_I2C_XFER_STOP, pi_task_block(&task_read_buffer));
	pi_task_wait_on(&task_write_addr);
	pi_task_wait_on(&task_read_buffer);

	pi_task_destroy(&task_write_buffer);
	pi_task_destroy(&task_write_addr);
	pi_task_destroy(&task_read_buffer);

	pi_i2c_close(&i2c);

	printf("comparing\n");
	int error = 0;
	for (int i = 0; i < sizeof(rx); i++) {
		if (rx[i] != expected_rx[i]) {
			printf("rx[%d]=0x%0x differs from expected rx[%d]=0x%0x\n",
			       i, rx[i], i, expected_rx[i]);
			error++;
		} else {
			printf("rx[%d]=0x%0x ok\n", i, rx[i]);
		}
	}
	if (error != 0)
	{
		printf("Test I2C async -> Not Success\n");
		exit(1);
	}
	printf("Test I2C async -> Success\n");
	exit(0);
}

int main(void)
{
	#ifdef USE_FREERTOS_TEST
	/* Init board hardware. */
	system_init();
	printf("\n\n\t *** FreeRTOS Test I2C async *** \n\n");
	#else
	printf("\n\n\t *** PULP-OS Test I2C async *** \n\n");
	#endif

	return pmsis_kickoff((void *)eeprom);
}

#ifdef USE_FREERTOS_TEST
/* Some debugging help */
void vApplicationMallocFailedHook(void)
{
	taskDISABLE_INTERRUPTS();
	printf("error: application malloc failed\n");
	__asm volatile("ebreak");
	for (;;)
		;
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	(void)pcTaskName;
	(void)pxTask;

	taskDISABLE_INTERRUPTS();
	printf("error: stack overflow\n");
	__asm volatile("ebreak");
	for (;;)
		;
}
#endif

