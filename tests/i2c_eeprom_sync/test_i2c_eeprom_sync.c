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
 * nico.orlando@studio.unibo.it
 */

/*
 * Test 24FC1025 eeprom (located at i2c address 0x50 in bus 0)
 */

#include "pmsis.h"
#include <bsp/bsp.h>

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
	int res = 0;
	res = pi_i2c_write(&i2c, tx, sizeof(tx),
			       PI_I2C_XFER_START | PI_I2C_XFER_STOP);
	if (res != PI_OK) {
		printf("pi_i2c_write failed\n");
		exit(1);
	}

	/* Wait for write to finish. It takes 5 million ns = 5 ms to finish. */
	for (volatile int i = 0; i < 100000; ++i)
		i++;

	printf("reading eeprom\n");
	uint8_t eeprom_addr[] = {
		0x00, /* addr msb */
		0x00, /* addr lsb */
	};

	res = pi_i2c_write(&i2c, eeprom_addr, sizeof(eeprom_addr),
			   PI_I2C_XFER_START | PI_I2C_XFER_STOP);
	if (res != PI_OK) {
		printf("pi_i2c_write for eeprom addr failed\n");
		exit(1);
	}
	res = pi_i2c_read(&i2c, rx, sizeof(rx),
			       PI_I2C_XFER_START | PI_I2C_XFER_STOP);
	if (res != PI_OK) {
		printf("pi_i2c_read failed\n");
		exit(1);
	}

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
	if (error != 0){
    	printf("Test NO success\n");
		exit(1);
	}
	printf("Test success\n");
	exit(0);
}

/* Program Entry. */
int main(void)
{
	printf("\n\n\t *** Pulp-SDK Test I2C sync *** \n\n");
	return pmsis_kickoff((void *)eeprom);
}
