/*
 * Copyright 2020 Greenwaves Technologies
 * Copyright 2020 ETH Zurich
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
 * Author: Germain Hagou
 *         Robert Balas (balasr@iis.ee.ethz.ch)
 */

/*
 * Test if we can write to spi using pmsis
 */

#include "pmsis.h"
#include <bsp/bsp.h>

#if !defined(SYNC_CS_AUTO) && !defined(ASYNC_CS_AUTO) &&                       \
	!defined(SYNC_CS_KEEP) && !defined(ASYNC_CS_KEEP)
#define ASYNC_CS_AUTO 1
#endif

//#define DEBUG 1
/**
 #ifdef DEBUG
 #define DEBUG_PRINTF printf
 #define DBG_PRINTF   printf
 #else
 #define DEBUG_PRINTF(...) ((void)0)
 #define DBG_PRINTF(...)	  ((void)0)
 #endif /* DEBUG */

  #define DEBUG_PRINTF(...) ((void)0)
 #define DBG_PRINTF(...)	  ((void)0)

#define TOTAL_SIZE (256)
//#define TOTAL_SIZE (8192*8 + 256)
#define TOTAL_SIZE_RTL (256)

#define NB_BUFFERS 1

// S25FL256S Instruction Set
#define CMD_WRR	    0x01 // Write REG 1
#define CMD_RDID    0x9F // Read ID (JEDEC Manufacturer & JEDEC CFI
#define CMD_RDSR1   0x05 // Read status register-1
#define CMD_WREN    0x06 // Write enable
#define CMD_4P4E    0x21 // 4KB Erase
#define CMD_4PP	    0x12 // Page program (4 bytes address)
#define CMD_4QPP    0x34 // Page program QSPI (4 bytes address)
#define CMD_4READ   0x13 // Read (4 bytes address)
#define CMD_4QOREAD 0x6C // Read QSPI(4 bytes address)

#define ID_CFI_SIZE 10

static inline void get_info(int *buffer_size)
{
#if !defined(PI_PLATFORM_RTL)
	*buffer_size = TOTAL_SIZE;
#else
	if (pi_platform() == PI_PLATFORM_RTL) {
		*buffer_size = TOTAL_SIZE_RTL;
	} else {
		*buffer_size = TOTAL_SIZE;
	}
#endif
}
#undef PI_L2
#define PI_L2

PI_L2 int32_t cmd_buffer[4][2];
PI_L2 int32_t rx_cmd_buffer[4][2];

PI_L2 uint8_t *tx_buffer;
PI_L2 uint8_t *rx_buffer;

static pi_task_t buf_event[NB_BUFFERS];
static pi_task_t rx_buf_event[NB_BUFFERS];
static pi_task_t cmd_event[NB_BUFFERS];
static pi_task_t rx_cmd_event[NB_BUFFERS];

static void set_spim_verif_command(struct pi_device *spim, int cmd, int addr,
				   int size, int32_t *cmd_buffer,
				   pi_task_t *task)
{
	/*
	** The command moves it to the first 8 bits, and to the last 8 it puts
	** the size.
	** Size is the size in bytes of the command.
	*/
	cmd_buffer[0] = (cmd << 24) | (size * 8);
	cmd_buffer[1] = addr;

	DBG_PRINTF("%s:%s:%d cmd_buffer[0] = 0x%x)\n", __FILE__, __func__,
		   __LINE__, cmd_buffer[0]);
	DBG_PRINTF("%s:%s:%d cmd_buffer[1] = 0x%x)\n", __FILE__, __func__,
		   __LINE__, cmd_buffer[1]);

	if (task)
		pi_spi_send_async(spim, cmd_buffer, 8 * 8, PI_SPI_CS_AUTO,
				  task);
	else
		pi_spi_send(spim, cmd_buffer, 8 * 8, PI_SPI_CS_AUTO);
}

static int test_entry()
{
	struct pi_spi_conf conf;
	struct pi_device spim;

	PI_L2 uint8_t add_buffer[] = {0x00, 0x00, 0x00, 0x00, 0x00};

	int total_size;
	get_info(&total_size);
	int buffer_size = total_size / NB_BUFFERS;

	pi_spi_conf_init(&conf);

	conf.wordsize = PI_SPI_WORDSIZE_8;
	conf.big_endian = 1;
	conf.max_baudrate = 10000000;
	conf.polarity = 0;
	conf.phase = 0;
	conf.itf = 0;
	conf.cs = 0;

	pi_open_from_conf(&spim, &conf);

	if (pi_spi_open(&spim))
		return -1;

	printf("malloc tx buffer\n");
	tx_buffer = pmsis_l2_malloc(total_size);
	if (tx_buffer == NULL)
		return -1;

	printf("malloc rx buffer\n");
	rx_buffer = pmsis_l2_malloc(total_size);
	if (rx_buffer == NULL)
		return -1;

	printf("tx buffer init\n");
	for (int i = 0; i < total_size; i++) {
		tx_buffer[i] = i;
	}

	uint8_t cmd = CMD_WREN;


	printf("async cs auto\n");

    for (int i = 0; i < NB_BUFFERS; i++) {
        // Set write enabled
        cmd = CMD_WREN;
        DBG_PRINTF("%s:%s:%d ...pi_spi_send_async(&spim, cmd, 8, PI_SPI_CS_AUTO, pi_task_block(&event_wren))...\n", __FILE__, __func__, __LINE__);
        pi_spi_send(&spim, &cmd, (1*8), PI_SPI_CS_AUTO);

        // send page address
        add_buffer[0] = CMD_4PP;
        DBG_PRINTF("%s:%s:%d ...pi_spi_send_async(&spim, add_buffer, (sizeof(add_buffer)*8), PI_SPI_CS_KEEP, NULL)...\n", __FILE__, __func__, __LINE__);
        pi_spi_send(&spim, add_buffer, (sizeof(add_buffer)*8), PI_SPI_CS_KEEP);

        // send data
        DBG_PRINTF("%s:%s:%d ...pi_spi_send_async(&spim, tx_buffer, BUFFER_SIZE * 8, PI_SPI_CS_AUTO, pi_task_block(&event_tx))...\n", __FILE__, __func__, __LINE__);
        pi_spi_send(&spim, tx_buffer + buffer_size * i, (buffer_size*8), PI_SPI_CS_AUTO);

        // wait until program operation is in progress
        DBG_PRINTF("%s:%s:%d ...start -> wip read...\n", __FILE__, __func__, __LINE__);
        cmd = CMD_RDSR1; // command to read status register 1
        volatile uint8_t status = 0xFF;
        do
        {
            //DBG_PRINTF("%s:%s:%d ...pi_spi_send_async(&spim, cmd, 1, PI_SPI_CS_AUTO, pi_task_block(&event_wip_write))...\n", __FILE__, __func__, __LINE__);
            pi_spi_send(&spim, &cmd, (1*8), PI_SPI_CS_KEEP);
            pi_task_t event_wip_read;
            //DBG_PRINTF("%s:%s:%d ...pi_spi_receive_async(&spim, &status, 1, PI_SPI_CS_AUTO, pi_task_block(&event_wip_read))...\n", __FILE__, __func__, __LINE__);
            pi_spi_receive(&spim, &status, (1*8), PI_SPI_CS_AUTO);
            //DBG_PRINTF("%s:%s:%d ...pi_task_wait_on(&event_wip_read)...\n", __FILE__, __func__, __LINE__);
            printf("WIP Register: %d\n", status);
        }while (status & 0x01 != 0); // flash is buzy if status != 0
        DBG_PRINTF("%s:%s:%d ...end -> wip read...\n", __FILE__, __func__, __LINE__);

        // send page address
        add_buffer[0] = CMD_4READ;
        DBG_PRINTF( "%s:%s:%d ...pi_spi_send_async(&spim, add_buffer, (sizeof(add_buffer)*8), PI_SPI_CS_KEEP, NULL)...\n", __FILE__, __func__, __LINE__);
        pi_spi_send(&spim, add_buffer, (sizeof(add_buffer)*8), PI_SPI_CS_KEEP);
        // read data
        DBG_PRINTF( "%s:%s:%d ...pi_spi_receive_async(&spim, rx_buffer, BUFFER_SIZE * 8, PI_SPI_CS_AUTO, pi_task_block(&event_rx))...\n", __FILE__, __func__, __LINE__);
        pi_spi_receive(&spim, rx_buffer + buffer_size * i, (buffer_size*8), PI_SPI_CS_AUTO);
        DBG_PRINTF("%s:%s:%d ...pi_task_wait_on(&event_rx)...\n", __FILE__, __func__, __LINE__);
    }

	printf("starting error check\n");
	int error = 0;
	for (int i = 0; i < total_size; i++) {
		if (rx_buffer[i] != tx_buffer[i]) {
			if (error == 0)
				printf("First error at index %d, expected 0x%x, got 0x%x at %p\n",
				       i, tx_buffer[i], rx_buffer[i],
				       &rx_buffer[i]);
			error++;
			return -1;
		}
	}

	if (error) {
		printf("Got %d errors\n", error);
	} else {
		printf("Test success\n");
	}
	pi_spi_close(&spim);
	return error;
}

static void test_kickoff(void *arg)
{
	int ret = test_entry();
	pmsis_exit(ret);
}

/* Program Entry. */
int main(void)
{
	printf("\n\n\t *** Pulp-SDK Hello World *** \n\n");
	return pmsis_kickoff((void *)test_kickoff);
}

