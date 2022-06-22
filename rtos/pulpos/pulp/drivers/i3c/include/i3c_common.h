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
* i3c_common.h
*
* This file holds declarations and definitions common to i3c master
* and i3c slave.
*
******************************************************************************
*/

#ifndef I3C_COMMON_H_
#define I3C_COMMON_H_

# include <assert.h>
# include <cdn_stdint.h>

// parasoft-begin-suppress METRICS-36-3 Allow common functions to be called from more than 5 places

/* BCR */

/* BCR [6:7] */
/* 0b00 – I3C Slave */
/* 0b01 – I3C Master */
/* 0b10 – Reserved for future definition by MIPI Sensor WG */
/* 0b11 – Reserved for future definition by MIPI Sensor WG */
#define I3C_BCR_DEVICE_ROLE_MASK 0xC0
#define I3C_BCR_DEVICE_ROLE_SHIFT 6U
#define I3C_BCR_DEVICE_ROLE_WIDTH 2U

/* BCR [5] */
/* 0 – SDR only */
/* 1 – HDR Capable */
#define I3C_BCR_HDR_CAPABLE_MASK 0x20
#define I3C_BCR_HDR_CAPABLE_SHIFT 5U
#define I3C_BCR_HDR_CAPABLE_WIDTH 1U

/* BCR [4] */
/* 0 – Not a Bridge Device */
/* 1 – Is a Bridge Device */
#define I3C_BCR_BRIDGE_IDENTIFIER_MASK 0x10
#define I3C_BCR_BRIDGE_IDENTIFIER_SHIFT 4U
#define I3C_BCR_BRIDGE_IDENTIFIER_WIDTH 1U

/* BCR [3] */
/* 0 – Device will always respond to I3C Bus commands */
/* 1 – Device will not always respond to I3C Bus commands */
#define I3C_BCR_OFFLINE_CAPABLE_MASK 0x08
#define I3C_BCR_OFFLINE_CAPABLE_SHIFT 3U
#define I3C_BCR_OFFLINE_CAPABLE_WIDTH 1U

/* BCR [2] */
/* 0 – No data byte follows the accepted IBI */
/* 1 – Mandatory one or more data bytes follow the accepted IBI. Data */
/* byte continuation is indicated by T-Bit, as described in Section */
/* 5.1.2.3.3 */
#define I3C_BCR_IBI_PAYLOAD_MASK 0x04
#define I3C_BCR_IBI_PAYLOAD_SHIFT 2U
#define I3C_BCR_IBI_PAYLOAD_WIDTH 1U

/* BCR [1] */
/* 0 – Not Capable */
/* 1 – Capable */
// parasoft-begin-suppress MISRA2012-RULE-1_1_b_c90-2 "too long name"
#define I3C_BCR_IBI_REQUEST_CAPABLE_MASK 0x02
#define I3C_BCR_IBI_REQUEST_CAPABLE_SHIFT 1U
#define I3C_BCR_IBI_REQUEST_CAPABLE_WIDTH 1U
// parasoft-end-suppress MISRA2012-RULE-1_1_b_c90-2

/* BCR [0] */
/* 0 – No Limitation */
/* 1 – Limitation */
// parasoft-begin-suppress MISRA2012-RULE-1_1_b_c90-2 "too long name"
#define I3C_BCR_MAX_DATA_SPEED_LIMIT_MASK 0x01
#define I3C_BCR_MAX_DATA_SPEED_LIMIT_SHIFT 0U
#define I3C_BCR_MAX_DATA_SPEED_LIMIT_WIDTH 1U
// parasoft-end-suppress MISRA2012-RULE-1_1_b_c90-2


/* DDR Word Preamble values */
// parasoft-begin-suppress MISRA2012-RULE-1_1_b_c90-2 "too long name"
#define I3C_DDR_PREAMB_CMD_FOLLOWS ((uint8_t) 0x1)
#define I3C_DDR_PREAMB_CMD_DATA_FOLLOWS ((uint8_t) 0x2)
#define I3C_DDR_PREAMB_READ_ACK_DATA_FOLLOWS ((uint8_t) 0x2)
#define I3C_DDR_PREAMB_READ_NACK ((uint8_t) 0x3)
#define I3C_DDR_PREAMB_READ_ABORT ((uint8_t) 0x2)
#define I3C_DDR_PREAMB_READ_DATA_FOLLOWS ((uint8_t) 0x3)
#define I3C_DDR_PREAMB_READ_CRC_FOLLOWS ((uint8_t) 0x1)
#define I3C_DDR_PREAMB_WRITE_DATA_FOLLOWS ((uint8_t) 0x3)
#define I3C_DDR_PREAMB_WRITE_CRC_FOLLOWS ((uint8_t) 0x1)
// parasoft-end-suppress MISRA2012-RULE-1_1_b_c90-2

#define I3C_SLAVE_ADDR_WIDTH 7U
#define I3C_MAX_ADDR 0x7FU

/**
 * DDR Read Command Word Parity adjustment bit.
 */
// parasoft-begin-suppress MISRA2012-RULE-1_1_b_c90-2 "too long name"
#define I3C_DDR_CMD_WORD_READ_PARITY_ADJ_BIT ((uint16_t)  0x0001)
// parasoft-end-suppress MISRA2012-RULE-1_1_b_c90-2

/**
 * Returns mask for given width and offset.
 */
static inline uint32_t get_bit_mask(uint32_t width, uint32_t offset)
{
    uint32_t result;

    if (offset >= 32U) {
        result = 0U; // Zero bit mask
    } else  if (width >= 32U) {
        result = 0xFFFFFFFFU;
    } else {
        result = ~(0xFFFFFFFFU << width) << offset;
    }

    return (result);
}

/* Defines for 32-bit DDR Word */

#define I3C_DDR_WORD_PREAMBLE_WIDTH ((uint32_t) 2)
#define I3C_DDR_WORD_PREAMBLE_OFFSET ((uint32_t) 18)

#define I3C_DDR_WORD_PAYLOAD_WIDTH ((uint32_t) 16)
#define I3C_DDR_WORD_PAYLOAD_OFFSET ((uint32_t) 2)

#define I3C_DDR_WORD_PARITY_BITS_WIDTH ((uint32_t) 2)
#define I3C_DDR_WORD_PARITY_BITS_OFFSET ((uint32_t) 0)

/* Defines for 16-bit DDR Command Word */

// parasoft-begin-suppress MISRA2012-RULE-1_1_b_c90-2 "too long name"
#define I3C_DDR_CMD_WORD_CODE_WIDTH ((uint32_t) 8)
#define I3C_DDR_CMD_WORD_CODE_OFFSET ((uint32_t) 8)

#define I3C_DDR_CMD_WORD_SLV_ADDR_WIDTH ((uint32_t) 7)
#define I3C_DDR_CMD_WORD_SLV_ADDR_OFFSET ((uint32_t) 1)
// parasoft-end-suppress MISRA2012-RULE-1_1_b_c90-2

/* Defines for 16-bit DDR Payload Word */
/* Only one 16-bit Payload Word exists for now */

/* Defines for DDR CRC Word */

// parasoft-begin-suppress MISRA2012-RULE-1_1_b_c90-2 "too long name"
#define I3C_DDR_CRC_WORD_TOKEN_VALUE ((uint16_t) 0xC)

#define I3C_DDR_CRC_WORD_TOKEN_WIDTH ((uint32_t) 4)
#define I3C_DDR_CRC_WORD_TOKEN_OFFSET ((uint32_t) 12)

#define I3C_DDR_CRC_WORD_CRC_WIDTH ((uint32_t) 5)
#define I3C_DDR_CRC_WORD_CRC_OFFSET ((uint32_t) 7)

#define I3C_DDR_CRC_WORD_SETUP_BITS_WIDTH ((uint32_t) 2)
#define I3C_DDR_CRC_WORD_SETUP_BITS_OFFSET ((uint32_t) 5)
// parasoft-end-suppress MISRA2012-RULE-1_1_b_c90-2

/* Defines for I2C */
#define I2C_LVR_INDEX_MASK ((uint8_t) 0x7U)

#define I2C_LVR_MODE_OFFSET 3

#define I2C_LVR_RSVD_MASK ((uint8_t) 0xFU)
#define I2C_LVR_RSVD_OFFSET 4

#define I2C_LVR_SA_MSB_MASK ((uint8_t) 0x7U)
#define I2C_LVR_SA_MSB_SHIFT 7

#define I2C_SLOW_MODE_FM 0U
#define I2C_SLOW_MODE_FM_PLUS 1U

/**
 * Creates 16-bit Command Word.
 *
 * @param cmd 8-bit DDR Command Word.
 * @param slave_addr 7-bit I3C Device address.
 *
 * @returns 16-bit DDR Command Word.
 */
static inline uint16_t i3c_ddr_cmd_word(uint8_t cmd, uint8_t slave_addr)
{
    uint32_t const cmd_mask = get_bit_mask(
        I3C_DDR_CMD_WORD_CODE_WIDTH,
        I3C_DDR_CMD_WORD_CODE_OFFSET
    );

    uint32_t const addr_mask = get_bit_mask(
        I3C_DDR_CMD_WORD_SLV_ADDR_WIDTH,
        I3C_DDR_CMD_WORD_SLV_ADDR_OFFSET
    );

    return (uint16_t)((((uint32_t)cmd << I3C_DDR_CMD_WORD_CODE_OFFSET) & cmd_mask)
        | (((uint32_t)slave_addr << I3C_DDR_CMD_WORD_SLV_ADDR_OFFSET) & addr_mask));
}

/**
 * Creates 16-bit DDR Data Word.
 * For now, it does actualy nothing.
 *
 * @param data 16-bit Payload
 *
 * @returns 16-bit DDR Data Word.
 */
static inline uint16_t i3c_ddr_data_word(uint16_t data)
{
    return data;
}

/**
 * Creates 16-bit DDR CRC Word.
 * CRC's 0xC token is added automatically.
 * Setting HDR Restart bit is not given as an option here, since HW
 *  sets it automatically if the following Command is also a DDR Command.
 *
 * @param crc 5-bit CRC5 value.
 *
 * @returns 16-bit DDR CRC Word.
 */
static inline uint16_t i3c_ddr_crc_word(uint8_t crc, uint8_t setup_bits)
{
    uint32_t const token_mask = get_bit_mask(
        I3C_DDR_CRC_WORD_TOKEN_WIDTH,
        I3C_DDR_CRC_WORD_TOKEN_OFFSET
    );

    uint32_t const crc_mask = get_bit_mask(
        I3C_DDR_CRC_WORD_CRC_WIDTH,
        I3C_DDR_CRC_WORD_CRC_OFFSET
    );

    uint32_t const setup_bits_mask = get_bit_mask(
        I3C_DDR_CRC_WORD_SETUP_BITS_WIDTH,
        I3C_DDR_CRC_WORD_SETUP_BITS_OFFSET
    );

    return (uint16_t)(((I3C_DDR_CRC_WORD_TOKEN_VALUE << I3C_DDR_CRC_WORD_TOKEN_OFFSET)
            & token_mask)
        | (((uint32_t)crc << I3C_DDR_CRC_WORD_CRC_OFFSET) & crc_mask)
        | (((uint32_t)setup_bits << I3C_DDR_CRC_WORD_SETUP_BITS_OFFSET) & setup_bits_mask));
}

/**
 * Creates 20-bit DDR Command Word.
 *
 * @param preamble 2-bit Preamble value.
 * @param payload 16-bit Payload Word.
 * @param parity_bits Two Parity bits P1 and P0 for DDR Word.
 *
 * @returns 20-bit DDR Command Word.
 */
static inline uint32_t i3c_ddr_word(uint8_t preamble, uint16_t payload,
    uint8_t parity_bits)
{
    uint32_t const preamble_mask = get_bit_mask(
        I3C_DDR_WORD_PREAMBLE_WIDTH,
        I3C_DDR_WORD_PREAMBLE_OFFSET
    );

    uint32_t const payload_mask = get_bit_mask(
        I3C_DDR_WORD_PAYLOAD_WIDTH,
        I3C_DDR_WORD_PAYLOAD_OFFSET
    );

    uint32_t const parity_bits_mask = get_bit_mask(
        I3C_DDR_WORD_PARITY_BITS_WIDTH,
        I3C_DDR_WORD_PARITY_BITS_OFFSET
    );

    return (((uint32_t)preamble << I3C_DDR_WORD_PREAMBLE_OFFSET) & preamble_mask)
        | (((uint32_t)payload << I3C_DDR_WORD_PAYLOAD_OFFSET) & payload_mask)
        | (((uint32_t)parity_bits << I3C_DDR_WORD_PARITY_BITS_OFFSET) & parity_bits_mask);
}

/**
 * Reads 2-bit Preamble value from the 20-bit DDR Word.
 *
 * @param ddr_word 20-bit DDR Word.
 *
 * @returns 2-bit Preamble value.
 */
static inline uint8_t i3c_ddr_word_get_preamble(uint32_t ddr_word)
{
    uint32_t const preamble_mask = get_bit_mask(
        I3C_DDR_WORD_PREAMBLE_WIDTH,
        I3C_DDR_WORD_PREAMBLE_OFFSET
    );

    return (uint8_t)((ddr_word & preamble_mask)
        >> I3C_DDR_WORD_PREAMBLE_OFFSET);
}

/**
 * Reads 16-bit Payload word from the 20-bit DDR Word.
 *
 * @param ddr_word 20-bit DDR Word.
 *
 * @returns 16-bit Payload word.
 */
static inline uint16_t i3c_ddr_word_get_payload(uint32_t ddr_word)
{
    uint32_t const payload_mask = get_bit_mask(
        I3C_DDR_WORD_PAYLOAD_WIDTH,
        I3C_DDR_WORD_PAYLOAD_OFFSET
    );

    return (uint16_t)((ddr_word & payload_mask)
        >> I3C_DDR_WORD_PAYLOAD_OFFSET);
}

/**
 * Reads two Parity Bits from the 20-bit DDR Word.
 *
 * @param ddr_word 20-bit DDR Word.
 *
 * @returns P1 (bit 1) and P0 (bit 0) parity bits.
 */
static inline uint8_t i3c_ddr_word_get_parity_bits(uint32_t ddr_word)
{
    uint32_t const parity_bits_mask = get_bit_mask(
        I3C_DDR_WORD_PARITY_BITS_WIDTH,
        I3C_DDR_WORD_PARITY_BITS_OFFSET
    );

    return (uint8_t)((ddr_word & parity_bits_mask)
        >> I3C_DDR_WORD_PARITY_BITS_OFFSET);
}

/**
 * Reads 16-bit DDR CRC Word from the 20-bit DDR Word.
 *
 * @param ddr_word 20-bit DDR Word.
 *
 * @returns 16-bit DDR CRC Word.
 */
static inline uint16_t i3c_ddr_word_get_crc_word(uint32_t ddr_word)
{
    return i3c_ddr_word_get_payload(ddr_word);
}

/**
 * Reads 8-bit Command Code value from 16-bit DDR Command Word.
 *
 * @param cmd_word 16-bit DDR Command Word.
 *
 * @returns 8-bit Command Code.
 */
static inline uint8_t i3c_ddr_cmd_word_get_code(uint16_t cmd_word)
{
    uint16_t const code_mask = (uint16_t)get_bit_mask(
        I3C_DDR_CMD_WORD_CODE_WIDTH,
        I3C_DDR_CMD_WORD_CODE_OFFSET
    );

    return (uint8_t)((cmd_word & code_mask)
        >> I3C_DDR_CMD_WORD_CODE_OFFSET);
}

/**
 * Reads 7-bit Slave Address from 16-bit DDR Command Word.
 *
 * @param cmd_word 16-bit DDR Command Word.
 *
 * @returns 7-bit Slave Address.
 */
static inline uint8_t i3c_ddr_cmd_word_get_slv_addr(uint16_t cmd_word)
{
    uint16_t const slv_addr_mask = (uint16_t)get_bit_mask(
        I3C_DDR_CMD_WORD_SLV_ADDR_WIDTH,
        I3C_DDR_CMD_WORD_SLV_ADDR_OFFSET
    );

    return (uint8_t)((cmd_word & slv_addr_mask)
        >> I3C_DDR_CMD_WORD_SLV_ADDR_OFFSET);
}

/**
 * Reads 5-bit CRC value from 16-bit DDR CRC Word.
 *
 * @param cmd_word 16-bit DDR CRC Word.
 *
 * @returns 7-bit Slave Address.
 */
static inline uint8_t i3c_ddr_crc_word_get_crc_value(uint16_t crc_word)
{
    uint16_t const crc_mask = (uint16_t)get_bit_mask(
        I3C_DDR_CRC_WORD_CRC_WIDTH,
        I3C_DDR_CRC_WORD_CRC_OFFSET
    );

    return (uint8_t)((crc_word & crc_mask)
        >> I3C_DDR_CRC_WORD_CRC_OFFSET);
}

/**
 * Reads two Setup Bits from 16-bit DDR CRC Word.
 *
 * @param cmd_word 16-bit DDR CRC Word.
 *
 * @returns Setup Bits.
 */
static inline uint8_t i3c_ddr_crc_word_get_setup_bits(uint16_t crc_word)
{
    uint16_t const crc_mask = (uint16_t)get_bit_mask(
        I3C_DDR_CRC_WORD_SETUP_BITS_WIDTH,
        I3C_DDR_CRC_WORD_SETUP_BITS_OFFSET
    );

    return (uint8_t)((crc_word & crc_mask)
        >> I3C_DDR_CRC_WORD_SETUP_BITS_OFFSET);
}

/**
 * Same as i3c_ddr_cmd_word_get_code but it extracts DDR Command Code directly
 * from the DDR Word.
 */
static inline uint8_t i3c_ddr_word_get_cmd_code(uint32_t ddr_word)
{
    uint16_t payload;

    payload = i3c_ddr_word_get_payload(ddr_word);

    return i3c_ddr_cmd_word_get_code(payload);
}

/**
 * Same as i3c_ddr_cmd_word_get_slv_addr but it extracts Slave Address
 * directrly from the DDR Word.
 */
static inline uint8_t i3c_ddr_word_get_cmd_slv_addr(uint32_t ddr_word)
{
    uint16_t payload;

    payload = i3c_ddr_word_get_payload(ddr_word);

    return i3c_ddr_cmd_word_get_slv_addr(payload);
}

/**
 * Same as i3c_ddr_crc_word_get_crc_value but it extracts CRC value
 * directly from the DDR Word.
 */
static inline uint8_t i3c_ddr_word_get_crc_value(uint32_t ddr_word)
{
    uint16_t payload;

    payload = i3c_ddr_word_get_payload(ddr_word);

    return i3c_ddr_crc_word_get_crc_value(payload);
}

/**
 * Same as i3c_ddr_crc_word_get_setup_bits but it extracts Setup Bits value
 * directrly from the DDR Word.
 */
static inline uint8_t i3c_ddr_word_get_crc_setup_bits(uint32_t ddr_word)
{
    uint16_t payload;

    payload = i3c_ddr_word_get_payload(ddr_word);

    return i3c_ddr_crc_word_get_setup_bits(payload);
}

/**
 * This function check parity for 7-bit I2C/I3C device address.
 *
 * @param addr 7-bit I3C/I2C Device address.
 *
 * @returns 1 if there's an even number of '1's in bit vector; otherwise 0.
 */
static inline uint8_t i3c_da_get_parity_bit(uint8_t addr)
{
    uint8_t i, n = 1;

    assert(!(addr & (1 << I3C_SLAVE_ADDR_WIDTH)));

    for (i = 0; i < I3C_SLAVE_ADDR_WIDTH; ++i) {
        n += ((addr & (1U << i)) != 0U) ? 1U : 0U;
    }

    return (n & 1U);
}

/**
 * Calculates and returns P1, P2 parity bits accordingly
 * to chapter 5.2.2.1 of the MIPI I3C Specification 0.7r14.
 *
 * @param v Command or Data word for which parity bits need to be returned.
 *
 * @returns Two parity bits. P1 as MSb and P0 as LSb.
 */
static inline uint8_t i3c_ddr_get_parity_bits(uint16_t v)
{
    uint16_t p1;
    uint16_t p0;

    // parity for odd index bits
    p1 = (v >> 15)
        ^ (v >> 13)
        ^ (v >> 11)
        ^ (v >> 9)
        ^ (v >> 7)
        ^ (v >> 5)
        ^ (v >> 3)
        ^ (v >> 1);

    // parity for even index bits
    p0 = (v >> 14)
        ^ (v >> 12)
        ^ (v >> 10)
        ^ (v >> 8)
        ^ (v >> 6)
        ^ (v >> 4)
        ^ (v >> 2)
        ^ v
        ^ 1U;

    return (uint8_t)(((p1 << 1) & 0x2U) | (p0 & 0x1U));
}

/**
 * Calculates CRC5 according to chapter 5.2.2.5 of the
 * MIPI I3C Specification 0.7r14
 *
 * @param [in] icrc - Initial CRC value.
 * @param [in] data_word Data word for which CRC is calculated.
 *
 * @returns 5-bit CRC value.
 */
static inline uint8_t i3c_ddr_calc_crc_5(uint8_t icrcIn, uint16_t data_word)
{
    uint8_t icrc = icrcIn;
    uint8_t crc0;
    uint8_t i;

    // crc0 = next_data_bit ^ crc[4]
    //              1         2            3       4
    // crc[4:0] = { crc[3:2], crc[1]^crc0, crc[0], crc0 }

    for (i = 0; i < 16U; ++i) {
	// parasoft-begin-suppress MISRA2012-RULE-12_2-2 RHS operand of shift will always be less than 16
        crc0 = (uint8_t)(((data_word >> (15U - i)) ^ ((uint16_t)icrc >> 4)) & 0x1U);
        // parasoft-end-suppress MISRA2012-RULE-12_2-2
        icrc = ((icrc << 1) & (0x18U | 0x2U)) // 1 and 3
            | (((icrc >> 1) ^ crc0) << 2) // 2
            | crc0; // 4
    }

    return (icrc & 0x1FU);
}

/**
 * Increments given pointer by given n
 */
static inline void advanceVolatileUint32Ptr(volatile uint32_t** ptr, uint32_t n)
{
    volatile uint32_t** local_ptr = ptr;
    *local_ptr = &((*local_ptr)[n]);
}

/**
 * Returns given pointer incremented by n
 */
static inline uint32_t volatile* getAdvanceVolatileUint32Ptr(uint32_t volatile* ptr, uint32_t n)
{
    uint32_t volatile* local_ptr = ptr;
    return (&(local_ptr[n]));
}

/**
 * Changes order of bytes for SDR transfers
 */
static inline void swap_bytes_sdr(volatile uint8_t* data, uint16_t data_size)
{
    uint16_t head;
    uint16_t tail;
    uint8_t temp;

    // Head is first element of the list
    head = 0;
    // Tail is last element of the list
    tail = data_size - 1U;
    while (head < tail) {
        temp = data[head];
        data[head] = data[tail];
        data[tail] = temp;
        ++head;
        --tail;
    }
}

/**
 * Changes order of bytes for DDR transfers
 */
static inline void swap_bytes_ddr(volatile uint16_t* data, uint16_t data_size)
{
    uint16_t head;
    uint16_t tail;
    uint16_t temp;

    // Head is first element of the list
    head = 0;
    // Tail is last element of the list
    tail = data_size - 1U;
    while (head < tail) {
        temp = data[head];
        data[head] = data[tail];
        data[tail] = temp;
        ++head;
        --tail;
    }
}

/* Returns number of data bytes to be transmitted */
static inline uint64_t i3c_cmd_get_payload_length(uint64_t length) {
    return (((length) & 0xFFFU) << 12U);
}

/* Returns Common Command Code */
static inline uint64_t i3c_cmd_ccc(uint64_t ccc) {
    return (((((uint64_t)((ccc) & (0xFFU))) << 32U ) | ((uint64_t)1U << 30U)));
}

/* Returns Command ID */
static inline uint64_t i3c_cmd_id(uint64_t cmd_id) {
    return (((uint64_t)((cmd_id) & (0xFFU))) << 56U);
}

/* Returns bits 10:8 of the Static or Dynamic device address for Legacy I2C Devices.*/
static inline uint64_t i3c_cmd_dev_addr_msb(uint64_t id_msb) {
    return (((uint64_t)((id_msb) & 0x7U) << 8U) | ((uint64_t)1U << 24U));
}

/* Returns device address  */
static inline uint64_t i3c_cmd_dev_addr(uint64_t dev_addr) {
    return (uint64_t)(((dev_addr) & 0x7FU) << 1U);
}

/* Returns address with parity bit at bit 0 */
static inline uint8_t i3c_dev_addr_with_parity(uint16_t dev_addr) {
    return (uint8_t)(((dev_addr) & 0x7FU) << 1U);
}

#endif

// parasoft-end-suppress METRICS-36-3
