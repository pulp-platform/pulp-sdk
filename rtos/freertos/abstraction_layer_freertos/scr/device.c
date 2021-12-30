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

#include "device.h"

static inline pi_device_e get_pi_device_type(void *conf);

/* extract pi_device_type */
static inline pi_device_e get_pi_device_type(void *conf)
{
	pi_device_e *device_ptr = (pi_device_e *)conf;
	return *device_ptr;
}

void pi_open_from_conf(struct pi_device *device, void *conf)
{
#if 0
	pi_device_e device_type = get_pi_device_type(conf);
	switch (device_type) {
	case PI_DEVICE_CLUSTER_TYPE:
		device->config = conf;
		pi_cluster_open(device);
		break;
	case PI_DEVICE_CPI_TYPE:
	case PI_DEVICE_HYPERBUS_TYPE:
	case PI_DEVICE_I2C_TYPE:
	case PI_DEVICE_SPI_TYPE:
	case PI_DEVICE_GPIO_TYPE:
		device->config = conf;
		break;
	default:
		device->config = conf;
		// TODO: add debug warning here
		break;
	}
#else
	device->config = conf;
#endif
}
