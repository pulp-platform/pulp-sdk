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
#define NUM_DEVICE 128

static struct pi_device i2c;

static void i2c_init(uint8_t itf, uint8_t addr)
{
	struct pi_i2c_conf i2c_conf;

	pi_i2c_conf_init(&i2c_conf);
	i2c_conf.itf = itf;
	i2c_conf.max_baudrate = 100000;
	pi_i2c_conf_set_slave_addr(&i2c_conf, addr << 1, 0);
	/* pi_i2c_conf_set_wait_cycles(conf, 2048); */

	pi_open_from_conf(&i2c, &i2c_conf);

	if (pi_i2c_open(&i2c)) {
		printf("i2c open failed\n");
		exit(1);
	}
}

static void i2c_close(void)
{
	pi_i2c_close(&i2c);
}

static uint8_t i2c_write(uint8_t *buf, uint16_t size)
{
	int res = pi_i2c_write(&i2c, buf, size,
			       PI_I2C_XFER_START | PI_I2C_XFER_STOP);
                   
#ifndef CONFIG_UDMA_I2C_ACK
	#error "This test requires the I2C ACK feature support in the udma"
#endif

	if (res == PI_OK) {
		/* we received an ACK */
		return 1;
	}
	/* we received a NACK */
	return 0;
}

static int scan(uint8_t itf)
{
	printf("scanning i2c bus %d\n", itf);
	uint8_t buf[1] = {0};
	uint8_t peripherals[NUM_DEVICE] = {0};

	/* the address of our i2c eeprom is normally 0x50 if you want to
	 * specifically test that */
	int found = 0;
	for (int i = 0; i < NUM_DEVICE; i++) {
		i2c_init(itf, i);
		peripherals[i] = i2c_write(buf, 0);
		if (peripherals[i] != 0) {
			found = 1;
			printf("interface %d: *found* peripheral at addr 0x%x\n", itf, i);
		} else {
			printf("interface %d: nothing at addr 0x%x\n", itf, i);
		}
		i2c_close();
	}

	if (found) {
		int count = 0;
		printf("Found peripherals: ");
		for (int i = 0; i < NUM_DEVICE; i++) {
			if (peripherals[i] != 0) {
				count++;
				printf("0x%x, ", i);
			}
		}
		printf("\n");
		printf("interface %d: 0x%x peripherals found\n", itf, count);
	} else {
		printf("interface %d: No peripheral found\n", itf);
	}

#ifdef CHECK_EEPROM_PRESENT
	/* note that the eeprom appears on two addresses */
	printf("checking whether eeprom is present\n");
	if (peripherals[0x50] && peripherals[0x54]) {
		printf("ok\n");
		return 0;
	} else {
		printf("eeprom not found at addr 0x80 and 0x84\n");
		return 1;
	}
#else
	return 0;
#endif
}

static void test_kickoff(void *arg)
{
	int ret = scan(I2C_DEV_ID);
	pmsis_exit(ret);
}

int main(void)
{
#ifdef USE_FREERTOS_TEST
	/* Init board hardware. */
	system_init();
	printf("\n\n\t *** FreeRTOS Scan Test I2C *** \n\n");
#else
	printf("\n\n\t *** PULP-OS Scan Test I2C *** \n\n");
#endif

	printf("i2c scan test\n");
	return pmsis_kickoff((void *)test_kickoff);
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