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
 * i3c.c
 *
 * Cadence I3C Master Interface Core Driver.
 *
 ******************************************************************************
 */

// parasoft-begin-suppress METRICS-36-3 Allow common functions to be called from more than 5 places
#include <mipi_i3c_regs.h>
#include <i3c_if.h>
#include <i3c_structs_if.h>
#include <i3c_priv.h>
#include <i3c_obj_if.h>
#include <i3c_sanity.h>
#include <i3c_common.h>

#include "command_list.h"

#include <cdn_stdtypes.h>
#include <cdn_errno.h>
#include <cps.h>
#include <cdn_log.h>
#include <string.h>

#include <assert.h>
#include <stddef.h>

/**
 * Returns greater of given values.
 */
static uint16_t min(uint16_t a, uint16_t b)
{
    return (a < b) ? a : b;
}

/**
 * Size of the TX FIFO word width.
 */
#define TX_FIFO_WORD_WIDTH 4U
/**
 * Size of the RX FIFO word width.
 */
#define RX_FIFO_WORD_WIDTH 4U
/**
 * Size of the CMD FIFO word width.
 */
#define CMD_FIFO_WORD_WIDTH 4U
/**
 * The maximum number of checks if Core became idle.
 */
#define WAIT_FOR_CORE_BUSY_TIMEOUT 1000U
/**
 * DDR TX transfer payload overhead (Command + CRC words)
 */
#define DDR_TX_PAYLOAD_OVERHEAD 2U
/**
 * DDR RX transfer payload overhead (CRC word)
 */
#define DDR_RX_PAYLOAD_OVERHEAD 1U

/*
 * Helper inline funtcions for storing and obtaining some DDR Command properties
 * to and from the I3C Command private data field.
 */

/**
 * Returns DDR Command Word
 * @param cmd_priv_ priv field of the I3C_cmd_t structure.
 * @param cmd_ 8-bit DDR Command.
 * @return cmd_priv value.
 */
static inline uint32_t cmd_priv_ddr_set_cmd(uint32_t cmd_priv, uint32_t cmd)
{
    return ((cmd_priv & ~0xFFU) | (cmd & 0xFFU));
}

/**
 * Obtains DDR Command Word from the Command's private data field.
 * @param cmd_priv_ priv field of the I3C_cmd_t structure.
 * @return cmd_priv value.
 */
static inline uint8_t cmd_priv_ddr_get_cmd(uint32_t cmd_priv)
{
    return ((uint8_t)(cmd_priv & 0xFFU));
}

/**
 * Returns 7-bit I3C Device address (without parity bit)
 *  data field.
 * @param cmd_priv_ priv field of the I3C_cmd_t structure.
 * @param addr_ 7-bit I3C Device address
 * @return cmd_priv value.
 */
static inline uint32_t cmd_priv_ddr_set_dev_addr(uint32_t cmd_priv, uint32_t addr)
{
    return ((cmd_priv & ~((uint32_t)0x7FU << 8)) | ((addr & 0x7FU) << 8));
}

/**
 * Obtains 7-bit I3C Device Address (without parity bit) from the Command's
 *  private data field.
 * @param cmd_priv_ priv field of the I3C_cmd_t structure.
 * @return cmd_priv value.
 */
static inline uint8_t cmd_priv_ddr_get_dev_addr(uint32_t cmd_priv)
{
    return ((uint8_t)((cmd_priv >> 8) & 0x7FU));
}

/**
 * Returns 5-bit CRC.
 * @param cmd_priv_ priv field of the I3C_cmd_t structure.
 * @param crc_ 5-bit CRC value.
 * @return cmd_priv value.
 */
static inline uint32_t cmd_priv_ddr_set_crc(uint32_t cmd_priv, uint32_t crc)
{
    return (cmd_priv & ~((uint32_t)0x1FU << 16)) | ((crc & 0x1FU) << 16);
}

/*
 * Private helper functions.
 */

/**
 * Checks if the I3C Core is idle.
 * @param pd [in] Pointer to I3C Core Driver's private data structure.
 * @return true if I3C Core is idle.
 * @return false if I3C Core is busy.
 */
static inline bool core_idle(I3C_PrivData *pd)
{
    uint32_t reg_val;

    assert(pd);

    reg_val = CPS_REG_READ(&pd->regs_base->MST_STATUS0);

    return (CPS_FLD_READ(MIPI__MST_STATUS0, IDLE, reg_val) != 0U);
}

/**
 * Waits for the I3C Core enter into idle state.
 * @param pd [in] Pointer to I3C Core Driver's private data structure.
 * @return 0 if I3C Core is in idle state.
 * @return EIO if I3C Core did not enter idle state before timeout
 *  (WAIT_FOR_CORE_BUSY_TIMEOUT).
 */
static inline uint32_t wait_for_core_idle(I3C_PrivData *pd)
{
    uint32_t result = EIO;
    uint32_t iterations_left = WAIT_FOR_CORE_BUSY_TIMEOUT;

    assert(pd);

    while ((iterations_left != 0U) && (!core_idle(pd))) {
        --iterations_left;
    }

    if (iterations_left != 0U) {
        // Iterations left, it means core is in idle state
        result = 0U;
    }

    return (result);
}

/**
 * Checks if the I3C Core is enabled.
 * @param pd [in] Pointer to I3C Core Driver's private data structure.
 * @return true if I3C Core is enabled.
 * @return false if I3C Core is disabled.
 */
static inline bool core_enabled(I3C_PrivData *pd)
{
    uint32_t reg_val;

    assert(pd);

    reg_val = CPS_REG_READ(&pd->regs_base->CTRL);

    return (CPS_FLD_READ(MIPI__CTRL, DEV_EN, reg_val) != 0U);
}

/**
 * Enables manual command start.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 */
static void enable_mcs(I3C_PrivData *pd)
{
    uint32_t reg_val;
    reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
    reg_val = CPS_FLD_SET(MIPI__CTRL, MCS_EN, reg_val);
    CPS_REG_WRITE(&pd->regs_base->CTRL, reg_val);
}

/**
 * Starts command execution stored in HW CMD fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 */
static void manual_command_start(I3C_PrivData *pd)
{
    uint32_t reg_val;
    reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
    reg_val = CPS_FLD_SET(MIPI__CTRL, MCS, reg_val);
    CPS_REG_WRITE(&pd->regs_base->CTRL, reg_val);

    cDbgMsg(DBG_GEN_MSG, DBG_HIVERB, "Command execution started\n");
}

/**
 * Disables manual command start.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 */
static void disable_mcs(I3C_PrivData *pd)
{
    uint32_t reg_val;
    reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
    reg_val = CPS_FLD_SET(MIPI__CTRL, MCS_EN, reg_val);
    CPS_REG_WRITE(&pd->regs_base->CTRL, reg_val);
}
/**
 * Calculates Open-Drain SCL Timing.
 * @param[in] cfg Pointer to I3C Config structure.
 * @return prescaler value.
 */
static uint8_t get_open_drain_low_period(I3C_Config const *cfg)
{
    return (uint8_t) ((((cfg->sclLowPeriod * cfg->sysClk) / 1000000000U)
                       / (cfg->sysClk / (cfg->sdrFreq * 4U))) - 2U);
}

/**
 * Calculates Asymetric Push-Pull SCL Timing.
 * @param[in] cfg Pointer to I3C Config structure.
 * @return prescaler value.
 */
static uint8_t get_asymmetric_push_pull_low(I3C_Config const *cfg)
{
    return (uint8_t) (((cfg->sysClk)
                       / (cfg->maxSclFreq * (cfg->sysClk / (cfg->sdrFreq * 4U)))) - 4U);
}

/**
 * Calculates I2C base frequency.
 * @param[in] cfg Pointer to I3C Config structure.
 * @return prescaler value.
 */
static uint16_t get_i2c_base_frequency(I3C_Config const *cfg)
{
    return (uint16_t) ((cfg->sysClk / (cfg->i2cFreq * 5U)) - 1U);
}

/**
 * Calculates I3C base frequency.
 * @param[in] cfg Pointer to I3C Config structure.
 * @return prescaler value.
 */
static uint16_t get_sdr_base_frequency(I3C_Config const *cfg)
{
    return (uint16_t) ((cfg->sysClk / (cfg->sdrFreq * 4U)) - 1U);
}

/**
 * Returns index of device with given address
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] da Dynamic address
 * @return Device index
 */
static inline uint8_t get_dev_index(const I3C_PrivData *pd,
                                    uint16_t            da)
{
    uint8_t result = 0U;
    uint8_t i;

    // Look for device with da in all descriptors
    for (i = 0; i < pd->max_devs; ++i) {
        if (pd->devs[i].addr == da) {
            result = i;
            break;
        }
    }

    return (result);
}

/**
 * Prints all devices stored in RR registers.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 */
static void print_devices(const I3C_PrivData *pd)
{
    uint32_t i;
    for (i = 0; i < pd->max_devs; ++i) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Device %d: addr 0x%0.4X, i2cIdx 0x%x, i2cFmPlusSpeed 0x%x, i2cRsvd 0x%x, i2c10bAddr 0x%x, legacyI2CDev 0x%x, hdrCapable 0x%x, bcr 0x%0.2x, dcr 0x%0.2x, provIdHi 0x%0.4x, provIdLo 0x%0.8x isActive 0x%x\n",
               i,
               pd->devs[i].addr,
               pd->devs[i].i2cIdx,
               pd->devs[i].i2cFmPlusSpeed,
               pd->devs[i].i2cRsvd,
               pd->devs[i].i2c10bAddr,
               pd->devs[i].legacyI2CDev,
               pd->devs[i].hdrCapable,
               pd->devs[i].bcr,
               pd->devs[i].dcr,
               pd->devs[i].provIdHi,
               pd->devs[i].provIdLo,
               pd->devs[i].isActive);
    }
}

/**
 * Updates device descriptor with information stored in RR registers.
 * @param[in] dev Pointer to Device Descriptor structure.
 * @param[in] rr0_val Value of RR0 register corresponding to this device.
 * @param[in] rr0_val Value of RR2 register corresponding to this device.
 */
static void update_i3c_device_info(I3C_DeviceDescriptor *dev,
                                   uint32_t              rr0_val,
                                   uint32_t              rr2_val)
{
    dev->i2cIdx = 0;
    dev->i2cFmPlusSpeed = 0;
    dev->i2cRsvd = 0;
    dev->i2c10bAddr = 0;
    dev->legacyI2CDev = 0;

    dev->addr = (uint16_t)(CPS_FLD_READ(MIPI__DEV_ID0_RR0, DEV_ADDR, rr0_val) >> 1);

    dev->bcr = (uint8_t)CPS_FLD_READ(MIPI__DEV_ID0_RR2, BCR, rr2_val);
    // For I3C devices DCR
    dev->dcr = (uint8_t)CPS_FLD_READ(MIPI__DEV_ID0_RR2, DCR_LVR, rr2_val);
    dev->hdrCapable = (CPS_FldRead(I3C_BCR_HDR_CAPABLE_MASK, I3C_BCR_HDR_CAPABLE_SHIFT, dev->bcr) != 0U);
}

/**
 * Updates device descriptor with information stored in RR registers.
 * @param[in] dev Pointer to Device Descriptor structure.
 * @param[in] rr0_val Value of RR0 register corresponding to this device.
 * @param[in] rr0_val Value of RR2 register corresponding to this device.
 */
static void update_i2c_device_info(I3C_DeviceDescriptor *dev,
                                   uint32_t              rr0_val,
                                   uint32_t              rr2_val)
{
    // For I2C devices LVR
    dev->i2cIdx = (uint8_t)(CPS_FLD_READ(MIPI__DEV_ID0_RR2, DCR_LVR, rr2_val) & I2C_LVR_INDEX_MASK);
    dev->i2cFmPlusSpeed = (((CPS_FLD_READ(MIPI__DEV_ID0_RR2, DCR_LVR, rr2_val) >> I2C_LVR_MODE_OFFSET) & 0x1U) != 0U);
    dev->i2cRsvd = (uint8_t)((CPS_FLD_READ(MIPI__DEV_ID0_RR2, DCR_LVR, rr2_val) >> I2C_LVR_RSVD_OFFSET) & I2C_LVR_RSVD_MASK);
    dev->i2c10bAddr = (CPS_FLD_READ(MIPI__DEV_ID0_RR0, LVR_EXT_ADDR, rr0_val) != 0U);
    dev->legacyI2CDev = 1;

    dev->addr = (uint16_t)((CPS_FLD_READ(MIPI__DEV_ID0_RR0, DEV_ADDR, rr0_val) >> 1)
                           | (CPS_FLD_READ(MIPI__DEV_ID0_RR0, LVR_SA_MSB, rr0_val) << I2C_LVR_SA_MSB_SHIFT));
    dev->bcr = 0;
    dev->dcr = 0;
    dev->hdrCapable = 0;
}

/**
 * Updates device sescriptors with informations stored in RR registers.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 */
static void update_device_info(I3C_PrivData *pd)
{
    volatile uint32_t *rr0_reg;
    volatile uint32_t *rr1_reg;
    volatile uint32_t *rr2_reg;

    uint32_t rr0_val;
    uint32_t rr1_val;
    uint32_t rr2_val;

    uint32_t off;
    uint32_t i;

    assert(pd);
    assert(I3C_DEVS_OFF);

    // Address of first RR register
    rr0_reg = (volatile uint32_t *)(&(pd->regs_base->DEV_ID0_RR0));
    // Offset between two RR registers
    off = I3C_DEVS_OFF;

    // Active devices mask
    pd->devs_active = (uint16_t) CPS_REG_READ(&pd->regs_base->DEVS_CTRL);

    for (i = 0; i < pd->max_devs; ++i) {

        rr1_reg = &rr0_reg[1];
        rr2_reg = &rr0_reg[2];

        rr0_val = CPS_UncachedRead32(rr0_reg);
        rr1_val = CPS_UncachedRead32(rr1_reg);
        rr2_val = CPS_UncachedRead32(rr2_reg);

        // Separate update for I2C and I3C device
        if (CPS_FLD_READ(MIPI__DEV_ID0_RR0, IS_I3C, rr0_val) != 0U) {
            update_i3c_device_info(&pd->devs[i], rr0_val, rr2_val);
        } else {
            update_i2c_device_info(&pd->devs[i], rr0_val, rr2_val);
        }

        // Higher bits of PID
        pd->devs[i].provIdHi = (uint16_t)(rr1_val >> 16);

        // Lower bits of PID
        pd->devs[i].provIdLo = (uint32_t)((rr1_val << 16) |
                                          (uint16_t)(rr2_val >> 16));

        // parasoft-begin-suppress MISRA2012-RULE-12_2-2 RHS operand of shift will always be less than 32
        // because num_devs is checked against too big values, also CONF_STATUS0.devs_num
        // is encoded on 4 bits, it means that max value of this field is 15
        pd->devs[i].isActive = ((pd->devs_active & ((uint32_t)1UL << i)) != 0U);
        // parasoft-end-suppress MISRA2012-RULE-12_2-2

        advanceVolatileUint32Ptr(&rr0_reg, off);
    }
}

/**
 * Prepares slave descriptors for DEFSLVS command
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] slave_descs Pointer to I3C SLave Descriptors structure.
 */
static void get_slave_descriptors(const I3C_PrivData* pd, I3C_SlaveDescriptors* slave_descs)
{
    uint32_t i;
    slave_descs->slave_count = 0;

    for (i = 0; i < pd->max_devs; ++i) {
        if (pd->devs[i].isActive) {
            if (i > 0U) {        /* do not count master in */
                (slave_descs->slave_count)++;
            }
            // Legacy I2C Device address is described as static address
            if (pd->devs[i].legacyI2CDev) {
                slave_descs->descriptors[i].sa = i3c_dev_addr_with_parity(pd->devs[i].addr);
                slave_descs->descriptors[i].da = 0;
            } else {
                slave_descs->descriptors[i].sa = 0;
                slave_descs->descriptors[i].da = i3c_dev_addr_with_parity(pd->devs[i].addr);
            }
            slave_descs->descriptors[i].dcrType = pd->devs[i].dcr;
            slave_descs->descriptors[i].bcrType = pd->devs[i].bcr;
        }
    }
}

/**
 * Returns ID of next command.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @return Command ID
 */
static uint8_t get_next_cmd_id(I3C_PrivData *pd)
{
    uint8_t cmd_id = pd->next_cmd_id;

    // 0xFE and 0xFF are reserved
    if(pd->next_cmd_id >= I3C_CMD_ID_MAX) {
        pd->next_cmd_id = I3C_CMD_ID_FIRST;
    }
    else {
        pd->next_cmd_id++;
    }

    DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "CMD_ID: 0x%.2X\n", cmd_id);

    return cmd_id;
}

/**
 * Executes callback
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] callback Callback function
 */
static void i3c_callback_execute(I3C_PrivData *pd, void (*callback)(I3C_PrivData *pd))
{
    if(callback != NULL) {
        callback(pd);
    }
}
/**
 * Checks if actual command is completed, whole payload is written to fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 */
static void check_tx_cmd_is_in_progress(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    /* This function may be called during RX command (DDR preamble) */
    if(cmd->is_wr_cmd == true) {
        if(cmd->bytes_fared < cmd->payload_size) {
            pd->tx_cmd_in_progress = cmd;
            DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "TX payload write in progress, completed: %d.\n", cmd->bytes_fared);
        }
        else {
            // NULL indicates no commands in progress
            pd->tx_cmd_in_progress = NULL;
            DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "TX payload write completed, payload_size: %d.\n", cmd->bytes_fared);
        }
    }
}

/**
 * Checks if actual command is completed, whole payload is read from fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 */
static void check_rx_cmd_is_in_progress(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    if(cmd->bytes_fared < cmd->payload_size) {
        pd->rx_cmd_in_progress = cmd;
        DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "RX payload read in progress, completed: %d.\n", cmd->bytes_fared);
    }
    else {
        // NULL indicates no commands in progress
        pd->rx_cmd_in_progress = NULL;
        DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "RX payload read completed, payload_size: %d.\n", cmd->bytes_fared);
    }
}

/**
 * Returns number of free locations in Command HW fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @return Free space in the Command Queue.
 */
static inline uint16_t get_cmd_fifo_free_space(const I3C_PrivData *pd)
{
    return (pd->cmd_fifo_size - pd->cmd_fifo_used);
}

/**
 * Reads data of SDR/I2C private transfer from fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 */
static void sdr_read_rx_data(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    uint32_t data;
    uint16_t i;
    uint16_t bytes_to_read;
    uint16_t possible_to_read;
    uint16_t bytes_xferred = 0;

    // Ensure that pd is not null
    assert(pd);
    // Ensure that cmd is not null
    assert(cmd);
    // Ensure that payload_sdr is not null
    assert(cmd->payload_sdr);
    // Ensure that payload_size is not 0
    assert(cmd->payload_size);

    if (pd->rx_fifo_used > 0U) {
        possible_to_read = min(cmd->payload_size - cmd->bytes_fared,
                               pd->rx_fifo_used);
    } else {
        possible_to_read = cmd->payload_size - cmd->bytes_fared;
    }

    while (bytes_xferred < possible_to_read) {
        // Get number of bytes to read
        bytes_to_read = min(
            possible_to_read - bytes_xferred,
            RX_FIFO_WORD_WIDTH);

        data = CPS_REG_READ(&pd->regs_base->RX_FIFO);

        for (i = 0; i < bytes_to_read; ++i) {
            const uint16_t shift = (i * 8U);
            uint8_t one_byte = 0U;

            // parasoft-begin-suppress MISRA2012-RULE-12_2-2 RHS operand of shift will always be less than 32
            // because bytes_to_read will never exceed RX_FIFO_WORD_WIDTH, maximum is 8U * 3U
            one_byte = (uint8_t)(data >> shift);
            // parasoft-end-suppress MISRA2012-RULE-12_2-2

            cmd->payload_sdr[cmd->payload_iterator] = one_byte;
            cmd->payload_iterator++;
        }

        pd->rx_fifo_used -= RX_FIFO_WORD_WIDTH;
        bytes_xferred += bytes_to_read;
        cmd->bytes_fared += bytes_to_read;
    }

    // Check if command payload is read completely
    check_rx_cmd_is_in_progress(pd, cmd);

    DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "%d of %d bytes read.\n", cmd->bytes_fared, cmd->payload_size);
}

/**
 * Writes data of SDR/I2C private transfer to fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 */
static void sdr_write_tx_data(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    uint32_t data;
    uint16_t i;
    uint16_t bytes_to_write;
    uint16_t possible_to_write;

    // Ensure that pd is not null
    assert(pd);
    // Ensure that cmd is not null
    assert(cmd);
    // Ensure that payload_sdr is not null
    assert(cmd->payload_sdr);
    // Ensure that payload_size is not 0
    assert(cmd->payload_size);

    possible_to_write = min(cmd->payload_size,
                            ((pd->tx_fifo_size - pd->tx_fifo_used) + cmd->bytes_fared));

    while (cmd->bytes_fared < possible_to_write) {
        // Get number of bytes to write
        bytes_to_write = min(possible_to_write - cmd->bytes_fared, TX_FIFO_WORD_WIDTH);

        data = 0U;
        for (i = 0; i < bytes_to_write; ++i) {
            const uint16_t shift = (8U * i);
            // parasoft-begin-suppress MISRA2012-RULE-12_2-2 RHS operand of shift will always be less than 32
            // because bytes_to_wite will never exceed TX_FIFO_WORD_WIDTH, maximum is 8U * 3U
            data |= ((uint32_t)cmd->payload_sdr[cmd->payload_iterator] << shift);
            // parasoft-end-suppress MISRA2012-RULE-12_2-2
            cmd->payload_iterator++;
        }
        CPS_REG_WRITE(&pd->regs_base->TX_FIFO, data);

        pd->tx_fifo_used += TX_FIFO_WORD_WIDTH;
        cmd->bytes_fared += bytes_to_write;

        DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "%d of %d bytes written.\n", cmd->bytes_fared, cmd->payload_size);
    }

    // Check if command payload is written completely
    check_tx_cmd_is_in_progress(pd, cmd);
}

/**
 * Writes preamble of DDR private transfer to fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 * @param[in] is_read_command Indicates if command is read/!write
 */
static inline void ddr_write_tx_preamble(I3C_PrivData *pd, volatile I3C_cmd_t *cmd,
                                         bool is_read_command)
{
    uint8_t ddr_cmd;
    uint8_t pb;
    uint8_t da;
    uint16_t cmd_word;
    uint32_t ddr_word;

    ddr_cmd = cmd_priv_ddr_get_cmd(cmd->priv);
    da = cmd_priv_ddr_get_dev_addr(cmd->priv);
    cmd_word = i3c_ddr_cmd_word(ddr_cmd, da);
    pb = i3c_ddr_get_parity_bits(cmd_word);

    // Adjust parity pit in case of read command
    if (is_read_command) {
        cmd_word |= I3C_DDR_CMD_WORD_READ_PARITY_ADJ_BIT;
    }

    // Calculate CRC5
    cmd->ddr_crc = i3c_ddr_calc_crc_5(I3C_HDR_DDR_INITIAL_CRC_RX, cmd_word);
    ddr_word = i3c_ddr_word(I3C_DDR_PREAMB_CMD_FOLLOWS, cmd_word, pb);

    // Write word to FIFO
    CPS_REG_WRITE(&pd->regs_base->TX_FIFO, ddr_word);
}

/**
 * Writes word of DDR private transfer to fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 */
static inline void ddr_write_tx_word(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    uint8_t pb;
    uint32_t ddr_word;

    uint16_t ddr_payload = cmd->payload_ddr[cmd->payload_iterator];

    // Different preamble if this is first data word
    uint8_t preamble = (cmd->bytes_fared == TX_FIFO_WORD_WIDTH)
                       ? I3C_DDR_PREAMB_CMD_DATA_FOLLOWS
                       : I3C_DDR_PREAMB_WRITE_DATA_FOLLOWS;

    // Calculate CRC5
    cmd->ddr_crc = i3c_ddr_calc_crc_5(cmd->ddr_crc, ddr_payload);
    pb = i3c_ddr_get_parity_bits(ddr_payload);
    ddr_word = i3c_ddr_word(preamble, ddr_payload, pb);

    // Write word to FIFO
    CPS_REG_WRITE(&pd->regs_base->TX_FIFO, ddr_word);

    cmd->payload_iterator++;
}

/**
 * Writes CRC5 of DDR private transfer to fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 */
static inline void ddr_write_tx_crc(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    volatile I3C_cmd_t *local_cmd = cmd;
    uint8_t pb;
    uint16_t crc_word;
    uint32_t ddr_word;

    // Calculate CRC5
    crc_word = i3c_ddr_crc_word(local_cmd->ddr_crc, 0);
    pb = 0; // no parity bits for CRC Word
    ddr_word = i3c_ddr_word(I3C_HDR_DDR_INITIAL_CRC_TX, crc_word, pb);

    // Write word to FIFO
    CPS_REG_WRITE(&pd->regs_base->TX_FIFO, ddr_word);
}

/**
 * Checks if this is first part of DDR message
 * @param[in] cmd Pointer to I3C command structure.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 */
static bool is_first_message(volatile I3C_cmd_t *cmd, I3C_PrivData *pd) {
    bool result = false;
    volatile I3C_cmd_t *local_cmd = cmd;
    I3C_PrivData *local_pd = pd;
    /* if first message then write preamble*/
    if ((!local_cmd->is_wr_cmd) && (local_cmd->bytes_fared == 0U)) {
        ddr_write_tx_preamble(local_pd, cmd, true);
        cmd->priv = cmd_priv_ddr_set_crc(cmd->priv, cmd->ddr_crc);
        result = true;
    }
    return (result);
}
/**
 * Writes DDR private transfer payload to fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 */
static void ddr_write_tx_data(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    uint16_t bytes_possible_to_write;

    assert(pd);
    assert(cmd);
    assert(cmd->payload_ddr);
    assert(cmd->payload_size);

    /* if message is first, write preamble and increment fifo */
    if (is_first_message(cmd,pd) == true) {

        bytes_possible_to_write = TX_FIFO_WORD_WIDTH;

    } else {
        bytes_possible_to_write = min(cmd->payload_size,
                                      (pd->tx_fifo_size - pd->tx_fifo_used)  + cmd->bytes_fared);

        while (cmd->bytes_fared < bytes_possible_to_write) {
            if (cmd->bytes_fared == 0U) {
                /* Nothing has been sent. */
                ddr_write_tx_preamble(pd, cmd, false);
            } else if ((cmd->bytes_fared > 0U) && (cmd->bytes_fared < (cmd->payload_size - TX_FIFO_WORD_WIDTH))) {
                /* At least command preamble was sent and at least CRC was not sent. */
                ddr_write_tx_word(pd, cmd);
            } else {
                /* All is sent */
                ddr_write_tx_crc(pd, cmd);
                cmd->payload_iterator++;
            }
            /* Increase amount of sent bytes by 4 */
            cmd->bytes_fared += TX_FIFO_WORD_WIDTH;
        }
    }

    pd->tx_fifo_used += bytes_possible_to_write;
    check_tx_cmd_is_in_progress(pd, cmd);
}

/**
 * Returns number of bytes possible to read from fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 * @return Number of bytes possible to read
 */
static uint16_t get_bytes_possible_to_read(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    uint16_t bytes_possible_to_read;
    I3C_PrivData *local_pd = pd;
    volatile I3C_cmd_t *local_cmd = cmd;
    if (local_pd->rx_fifo_used > 0U) {
        bytes_possible_to_read = min(local_cmd->payload_size, local_pd->rx_fifo_used + local_cmd->bytes_fared);
    } else {
        bytes_possible_to_read = local_cmd->payload_size;
    }
    return (bytes_possible_to_read);
}
/**
 * Reads DDR private transfer payload from fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 * @return 0 on success
 * @return EPROTO if transfer error was detected.
 */
static inline uint32_t ddr_read_rx_data(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{

    uint32_t result = 0U;
    uint32_t ddr_word;
    uint16_t preamble;
    uint8_t crc_rec;

    uint16_t i;
    uint16_t bytes_fared;
    uint16_t bytes_possible_to_read;

    /* Get bytes possible to read, either size of payload or less, if RX FIFO was used */
    bytes_possible_to_read = get_bytes_possible_to_read(pd, cmd);

    bytes_fared = cmd->bytes_fared;
    /* Iterate through all 4 byte frames to be read from FIFO */
    for (i = bytes_fared;
         i < bytes_possible_to_read;
         i += RX_FIFO_WORD_WIDTH) {
        /* Get data from FIFO */
        ddr_word = CPS_REG_READ(&pd->regs_base->RX_FIFO);

        /* Get preamble from data word */
        preamble = i3c_ddr_word_get_preamble(ddr_word);
        /* Check which preamble was read */
        switch (preamble) {
        /* Data preamble, read data word */
        case (I3C_DDR_PREAMB_READ_ACK_DATA_FOLLOWS):
        case (I3C_DDR_PREAMB_READ_DATA_FOLLOWS):
            /* Check if iterator does not exceed range of payload */
            if (cmd->payload_iterator < cmd->payload_size) {
                /* Get payload from DDR word */
                cmd->payload_ddr[cmd->payload_iterator] = i3c_ddr_word_get_payload(ddr_word);

                /* Calculate CRC5 for DDR payload */
                cmd->ddr_crc = i3c_ddr_calc_crc_5(cmd->ddr_crc, cmd->payload_ddr[cmd->payload_iterator]);

                cmd->payload_iterator++;
            }
            else {
                /* Payload iterator points out of bound, report error and return protocol error */
                DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Payload iterator point outside payload. cmd->payload_iterator = %u, cmd->payload_size %u\n", cmd->payload_iterator, cmd->payload_size);
                result = EPROTO;
            }
            break;
        /* CRC preamble, read CRC word */
        case (I3C_DDR_PREAMB_READ_CRC_FOLLOWS):
            /* Get CRC value from DDR word */
            crc_rec = i3c_ddr_word_get_crc_value(ddr_word);
            cmd->payload_iterator++;
            /* Check if CRC received match calculated CRC */
            if (cmd->ddr_crc != crc_rec) {
                DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. DDR CRC mismatch. cmd->ddr_crc 0x%X crc_rec 0x%X\n", cmd->ddr_crc, crc_rec);
                result = EPROTO;
            }
            break;
        default:
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Bad DDR preamble.\n");
            result = EPROTO;
            break;
        }
        /* Stop reading data if error occurred */
        if(result != 0U) {
            break;
        }
        /* Everything OK. Increase amount of received bytes by 4 */
        cmd->bytes_fared += RX_FIFO_WORD_WIDTH;
    }

    check_rx_cmd_is_in_progress(pd, cmd);

    return (result);
}
/* START: Code duplicated from i3cs, blocked by DRV-3078 */
/**
 * Slave mode DDR read
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] buf_in Buffer for read payload
 * @param[in] num_words Number of words to read
 * @param[in] reg_val_in Value of first fifo word
 * @return 0 on success
 * @return EPROTO if transfer error was detected
 */
static uint32_t i3c_slave_mode_process_ddr_read(volatile I3C_PrivData *pd,
                                                uint16_t *buf_in, uint16_t num_words, uint32_t reg_val_in)
{
    uint32_t result;
    uint8_t crc = I3C_HDR_DDR_INITIAL_CRC_RX;
    uint16_t* buf = buf_in;
    uint32_t reg_val = reg_val_in;

    uint16_t preamble = i3c_ddr_word_get_preamble(reg_val);
    // Check if transfer starts with CMD preamble
    if (preamble != I3C_DDR_PREAMB_CMD_FOLLOWS) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Command preamble mismatch\n");
        result = EPROTO;
    } else {

        /* Get CRC for Command Word */
        crc = i3c_ddr_calc_crc_5(crc, i3c_ddr_word_get_payload(reg_val));

        /* Reading actual message */
        bool doContinue = true;
        uint16_t i;
        for (i = 0; i<num_words; ++i) {
            reg_val = CPS_REG_READ(&pd->regs_base->SLV_DDR_RX_FIFO);
            preamble = i3c_ddr_word_get_preamble(reg_val);
            if (preamble != ((i != 0U) ?
                             I3C_DDR_PREAMB_READ_DATA_FOLLOWS :
                             I3C_DDR_PREAMB_READ_ACK_DATA_FOLLOWS)) {
                DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Payload preamble mismatch\n");
                result = EPROTO;
                doContinue = false;
                break;
            }
            *buf = i3c_ddr_word_get_payload(reg_val);

            /* Update CRC for each Data Word */
            crc = i3c_ddr_calc_crc_5(crc, *buf);
            ++buf;
        }

        if (doContinue) {
            reg_val = CPS_REG_READ(&pd->regs_base->SLV_DDR_RX_FIFO);
            preamble = i3c_ddr_word_get_preamble(reg_val);
            // Check if transfer ends with CRC preamble
            if (preamble != I3C_DDR_PREAMB_READ_CRC_FOLLOWS) {
                result = EPROTO;
            } else if (i3c_ddr_word_get_crc_value(reg_val) != crc) { // Compare received CRC with calculated one
                DbgMsg(DBG_GEN_MSG, DBG_CRIT, "CRC mismatch.\n");
                result = EPROTO;
            } else {
                result = 0U;
            }
        }
    }

    return (result);
}

/**
 * Prepares initial CRC5 value. Slave Mode.
 * @param[in] da DDR private transfer dynamic address.
 * @return CRC value.
 */
static inline uint8_t i3c_slave_mode_ddr_initial_crc(uint8_t da)
{
    uint16_t cmd_word;
    uint8_t crc = I3C_HDR_DDR_INITIAL_CRC_RX;

    cmd_word = i3c_ddr_cmd_word(0x80, da) |
               I3C_DDR_CMD_WORD_READ_PARITY_ADJ_BIT;

    crc = i3c_ddr_calc_crc_5(crc, cmd_word);

    return crc;
}

/**
 * Writes DDR word. Slave Mode.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] preamble DDR Preamble
 * @param[in] word DDR word
 * @param[in] crc_in Value of CRC5
 * @return CRC value.
 */
static inline uint8_t i3c_slave_ddr_write_word(volatile I3C_PrivData* pd, uint8_t preamble, uint16_t word, uint8_t crc_in)
{
    uint32_t ddrw;
    uint8_t pb;
    uint8_t crc_out;

    // Calculate CRC5
    crc_out = i3c_ddr_calc_crc_5(crc_in, word);
    pb = i3c_ddr_get_parity_bits(word);
    ddrw = i3c_ddr_word(preamble, word, pb);

    // Write word slave DDR TX FIFO
    CPS_REG_WRITE(&pd->regs_base->SLV_DDR_TX_FIFO, ddrw);

    return crc_out;
}

/**
 * Writes DDR CRC5 word. Slave Mode.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] crc_in Value of CRC5
 */
static inline void i3c_slave_ddr_write_crc(volatile I3C_PrivData* pd, uint8_t crc_in)
{
    uint8_t pb = 0; /* No parity bits for CRC */
    uint16_t crcw = i3c_ddr_crc_word(crc_in, 1);
    uint32_t ddrw = i3c_ddr_word(I3C_DDR_PREAMB_WRITE_CRC_FOLLOWS, crcw, pb);

    CPS_REG_WRITE(&pd->regs_base->SLV_DDR_TX_FIFO, ddrw);
}
/* END: Code duplicated from i3cs, blocked by DRV-3078 */

/**
 *  Handler for Set Dynamic Address from Static Address CCC and
 *  Set New Dynamic Address CCC.
 *
 *  This function is called after Set Dynamic Address from Static
 *  Address CCC or Set New Dynamic Address CCC has been
 *  completed. This function is updating Retaining Registers for
 *  device that that has been assigned with Dynamic Address.
 *
 *  @param pd[in] Pointer to driver's private data.
 *  @param error[in] Value of command response error field
 *  @param cmd[in] Pointer to command structure.
 */
static void ccc_set_da_complete_handler(I3C_PrivData *      pd,
                                        volatile I3C_cmd_t* cmd,
                                        uint8_t             error)
{
    I3C_DeviceDescriptor *dev;
    volatile uint32_t *dev_cfg_reg;
    volatile I3C_cmd_t* local_cmd = cmd;
    uint32_t reg_val;
    uint8_t da;
    uint8_t dev_idx;
    // Ensure that pd is not null
    assert(pd);

    // Get da from 64-bit command
    da = (uint8_t) CPS_FLD_READ(MIPI__CMD0_FIFO, DEV_ADDR, (uint32_t)local_cmd->cmd);
    dev_idx = get_dev_index(pd, da);

    if (dev_idx != 0U) {
        dev = &(pd->devs[dev_idx]);

        // Ensure that device priv field is not null
        assert(dev->priv);

        if (error != 0U) {
            uint32_t dev_active;

            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to set DA. DA NACKed.\n");

            dev_active = CPS_REG_READ(&pd->regs_base->DEVS_CTRL);
            // Dev should be activated before issuing SETDASA CCC
            assert(dev_active & (1U << dev_idx));

            if (dev_idx < 32U) {
                // Deactivate the Device
                dev_active &= ~(1U << dev_idx);
            }
            CPS_REG_WRITE(&pd->regs_base->DEVS_CTRL, dev_active);
            pd->devs_active = (uint16_t) dev_active;
        }
        else {
            da = local_cmd->payload_sdr[0]; // dev_addr + parity bit
        }

        dev_cfg_reg = dev->priv;

        reg_val = CPS_REG_READ(dev_cfg_reg);
        reg_val = CPS_FLD_WRITE(MIPI__DEV_ID0_RR0, DEV_ADDR, reg_val, da);
        reg_val = CPS_FLD_WRITE(MIPI__DEV_ID0_RR0, IS_I3C, reg_val, true);
        CPS_REG_WRITE(dev_cfg_reg, reg_val);

        dev->addr = (uint16_t)da >> 1; // remove parity bit

    }
    else {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "No device descriptor for DA:0x%.2X!\n", da);
    }
}

/**
 * Function writes low half of SIR configuration register.
 * @param sir_map_val[in] current value of SIR register
 * @param ibi_sir_cfg[in] SIR configuration that will be written to low
 *	  half of register
 * @return modified SIR_MAP value
 */
static uint32_t modify_sir_map_low(uint32_t sir_map_val_in, const I3C_IbiSirConfiguration* ibi_sir_cfg)
{
    uint32_t sir_map_val = sir_map_val_in;

    // Low part of SIR register modification
    sir_map_val = CPS_FLD_WRITE(MIPI__SIR_MAP0, DEVID0_RESP, sir_map_val, ibi_sir_cfg->ibiResp);
    sir_map_val = CPS_FLD_WRITE(MIPI__SIR_MAP0, DEVID0_DA, sir_map_val, ibi_sir_cfg->addr);
    sir_map_val = CPS_FLD_WRITE(MIPI__SIR_MAP0, DEVID0_PL, sir_map_val, ibi_sir_cfg->ibiPayloadSize);
    sir_map_val = CPS_FLD_WRITE(MIPI__SIR_MAP0, DEVID0_SLOW, sir_map_val, ibi_sir_cfg->ibiPayloadSpeedLimit);
    sir_map_val = CPS_FLD_WRITE(MIPI__SIR_MAP0, DEVID0_ROLE, sir_map_val, ibi_sir_cfg->ibiReqDevRole);

    return sir_map_val;
}

/**
 * Function writes high half of SIR configuration register.
 * @param sir_map_val[in] current value of SIR register
 * @param ibi_sir_cfg[in] SIR configuration that will be written to high
 *	  half of register
 * @return modified SIR_MAP value
 */
static uint32_t modify_sir_map_high(uint32_t sir_map_val_in, const I3C_IbiSirConfiguration* ibi_sir_cfg)
{
    uint32_t sir_map_val = sir_map_val_in;

    // High part of SIR register modification
    sir_map_val = CPS_FLD_WRITE(MIPI__SIR_MAP0, DEVID1_RESP, sir_map_val, ibi_sir_cfg->ibiResp);
    sir_map_val = CPS_FLD_WRITE(MIPI__SIR_MAP0, DEVID1_DA, sir_map_val, ibi_sir_cfg->addr);
    sir_map_val = CPS_FLD_WRITE(MIPI__SIR_MAP0, DEVID1_PL, sir_map_val, ibi_sir_cfg->ibiPayloadSize);
    sir_map_val = CPS_FLD_WRITE(MIPI__SIR_MAP0, DEVID1_SLOW, sir_map_val, ibi_sir_cfg->ibiPayloadSpeedLimit);
    sir_map_val = CPS_FLD_WRITE(MIPI__SIR_MAP0, DEVID1_ROLE, sir_map_val, ibi_sir_cfg->ibiReqDevRole);

    return sir_map_val;
}

/*
 * Private helper functions end.
 */

/*
 * Base I3C_OBJ Functions.
 */

/**
 * Returns the memory requirements for a driver instance.
 * @param[in] config Driver and hardware configuration.
 * @param[out] sysReq Returns the memory requirements for given configuration.
 * @return 0 On success.
 * @return EINVAL If config contains invalid values or not supported configuration.
 */
uint32_t I3C_Probe(const I3C_Config *config, I3C_SysReq *sysReq)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_ProbeSF(config, sysReq) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        // I3C_PrivData + size of list of commands
        sysReq->memReq = (uint32_t)(sizeof(I3C_PrivData)
                                    + (I3C_CMD_BUFFER_SIZE * sizeof(I3C_cmd_t)));

        result = 0U;
    }

    return (result);
}

/**
 * Initializes fields needed for messaging
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] config Pointer to I3C Config structure
 */
static void init_messages_queue(I3C_PrivData *pd, const I3C_Config *config) {

    pd->cmd_empty = true;
    pd->cmd_abort = false;
    pd->cmd_in_progress = false;
    pd->imd_in_progress = false;
    pd->cmd_fifo_size = config->cmdFifoSize;
    pd->cmd_fifo_threshold = config->cmdFifoThreshold;
    pd->tx_fifo_size = config->txFifoSize;
    pd->rx_fifo_size = config->rxFifoSize;
    pd->cmd_fifo_used = 0;
    pd->tx_fifo_used = 0;
    pd->rx_fifo_used = 0;
    // Indicate that TX command is not in progress
    pd->tx_cmd_in_progress = NULL;
    // Indicate that RX command is not in progress
    pd->rx_cmd_in_progress = NULL;

}

/**
 * Configures devices in case of Master role, initializes messages,
 * configures thresholds, interrupts and command list.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] config Pointer to I3C Config structure
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
static uint32_t configure_devices(I3C_PrivData *pd, const I3C_Config *config){
    uint32_t result = 0U;
    DbgMsg(DBG_GEN_MSG, DBG_FYI, "Configuring Devices.\n");

    // Do not configure devices when device is in slave mode
    if ((pd->dev_role == I3C_DEV_ROLE_MASTER) &&
        (I3C_GetInstance()->configureDevices(pd, config->devs, config->numDevs) != 0U)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to allocate Devices.\n");
        result = EINVAL;
    } else {

        // I3C Messages setup

        DbgMsg(DBG_GEN_MSG, DBG_FYI, "Initializing Messages queue.\n");
        init_messages_queue(pd, config);

        result += I3C_GetInstance()->configureInterrupts(pd, &(config->interruptConfig));
        result += I3C_GetInstance()->configureThresholds(pd, &(config->thresholdConfig));
        result += CLInit(&(pd->cmd_list), pd->CMD_LIST_BUFFER, I3C_CMD_BUFFER_SIZE);
    }
    return (result);
}

/**
 * Custom validation is needed because dev_role and
 * bus_mode are configured during initialization
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] config Pointer to I3C Config structure.
 * @param[in] callbacks Callbacks.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
static uint32_t validate_init_input(I3C_PrivData *pd, const I3C_Config *config,
                                    const I3C_Callbacks *callbacks) {

    uint32_t result;
    I3C_PrivData *local_pd = pd;
    // Check only if params are not null, skip values
    if ((local_pd == NULL) || (config == NULL) || (callbacks == NULL)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        result = 0U;
    }
    return(result);
}

/**
 * Sets device role depending on HW configuration
 * Please make sure core is disabled.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] conf_status1_val Value of CONF_STATUS0 register
 */
static void set_dev_role(I3C_PrivData *pd, uint32_t conf_status0_val) {
    if (CPS_FLD_READ(MIPI__CONF_STATUS0, DEV_ROLE, conf_status0_val) == 0U) {
        pd->dev_role = I3C_DEV_ROLE_MASTER;
    }
    else {
        pd->dev_role = I3C_DEV_ROLE_SECONDARY_MASTER;
    }
}

/**
 * Sets IBI fifo size depending on HW configuration
 * Please make sure core is disabled.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] conf_status1_val Value of CONF_STATUS1 register
 */
static void set_ibi_fifo_size(I3C_PrivData *pd, uint32_t conf_status1_val) {
    uint32_t shift;

    // Get exponent of fifo size
    shift = (1U + CPS_FLD_READ(MIPI__CONF_STATUS1, IBI_MEM_DEPTH, conf_status1_val));

    // parasoft-begin-suppress MISRA2012-RULE-12_2-2 RHS operand of shift will always be less than
    // or equal to 8. IBI_MEM_DEPTH is encoded on 3-bit filed + 1U
    pd->ibi_fifo_size = ((uint16_t)1U << shift);
    // parasoft-end-suppress MISRA2012-RULE-12_2-2

    // If fifo size greater than allowed, reset it
    if(pd->ibi_fifo_size > 64U) {
        pd->ibi_fifo_size = 0U;
    }
}

/**
 * Sets CMD fifo size depending on HW configuration
 * Please make sure core is disabled.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] conf_status1_val Value of CONF_STATUS1 register
 */
static void set_cmd_fifo_size(I3C_PrivData *pd, uint32_t conf_status1_val) {
    uint32_t shift;

    // Get exponent of fifo size
    shift = (2U + CPS_FLD_READ(MIPI__CONF_STATUS1, CMD_MEM_DEPTH, conf_status1_val));

    // parasoft-begin-suppress MISRA2012-RULE-12_2-2 RHS operand of shift will always be less than
    // or equal to 5. CMD_MEM_DEPTH is encoded on 2-bit filed + 2U
    pd->cmd_fifo_size = ((uint16_t)1U << shift) * CMD_FIFO_WORD_WIDTH;
    // parasoft-end-suppress MISRA2012-RULE-12_2-2
}

/**
 * Sets Tx fifo size depending on HW configuration
 * Please make sure core is disabled.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] conf_status1_val Value of CONF_STATUS1 register
 */
static void set_tx_fifo_size(I3C_PrivData *pd, uint32_t conf_status1_val) {
    uint32_t shift;
    uint32_t fifo_size;

    // Get exponent of fifo size
    shift = (3U + CPS_FLD_READ(MIPI__CONF_STATUS1, TX_MEM_DEPTH, conf_status1_val));
    if(shift < 32U) {
        fifo_size = ((uint32_t)1U << shift) * TX_FIFO_WORD_WIDTH;
        // If fifo size greater than allowed, reset it
        if(fifo_size > 65536U) {
            pd->tx_fifo_size = 0U;
        }
        else {
            pd->tx_fifo_size = (uint16_t) fifo_size;
        }
    }
}

/**
 * Sets Rx fifo size depending on HW configuration
 * Please make sure core is disabled.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] conf_status1_val Value of CONF_STATUS1 register
 */
static void set_rx_fifo_size(I3C_PrivData *pd, uint32_t conf_status1_val) {
    uint32_t shift;
    uint32_t fifo_size;

    // Get exponent of fifo size
    shift = (3U + CPS_FLD_READ(MIPI__CONF_STATUS1, RX_MEM_DEPTH, conf_status1_val));
    if(shift < 32U) {
        fifo_size = ((uint32_t)1U << shift) * RX_FIFO_WORD_WIDTH;
        // If fifo size greater than allowed, reset it
        if(fifo_size > 65536U) {
            pd->rx_fifo_size = 0U;
        }
        else {
            pd->rx_fifo_size = (uint16_t) fifo_size;
        }
    }
}

/**
 * Configures the driver depending on HW configuration
 * Please make sure core is disabled.
 * @param[in] pd Pointer to driver's private data object.
 */
static void i3c_auto_configuration(I3C_PrivData *pd) {
    uint32_t reg_val;

    reg_val = CPS_REG_READ(&pd->regs_base->CONF_STATUS0);

    // Configurstion from CONF_STATUS0 register
    set_dev_role(pd, reg_val);
    pd->num_gpo = (uint8_t) CPS_FLD_READ(MIPI__CONF_STATUS0, GPO_NUM, reg_val);
    pd->num_gpi = (uint8_t) CPS_FLD_READ(MIPI__CONF_STATUS0, GPI_NUM, reg_val);
    pd->max_devs = (uint8_t) CPS_FLD_READ(MIPI__CONF_STATUS0, DEVS_NUM, reg_val);

    reg_val = CPS_REG_READ(&pd->regs_base->CONF_STATUS1);

    // Configurstion from CONF_STATUS1 register
    set_ibi_fifo_size(pd, reg_val);
    set_cmd_fifo_size(pd, reg_val);
    set_tx_fifo_size(pd, reg_val);
    set_rx_fifo_size(pd, reg_val);

    pd->max_ibi_devs = 1U + (uint8_t) CPS_FLD_READ(MIPI__CONF_STATUS1, IBI_HW_RES, reg_val);

    // If max IBI devices greater than allowed, reset it
    if(pd->max_ibi_devs > 11U) {
        pd->max_ibi_devs = 0U;
    }

    // If both Rx and Tx fifos are implemented, use it
    if((pd->tx_fifo_size > 0U) &&
       (pd->rx_fifo_size > 0U)) {
        pd->use_fifos = true;
    }
    else {
        pd->use_fifos = false;
    }
}

/**
 * Configures PRESCL_CTRL0 register, frequencies.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] config Specifies driver/hardware configuration.
 */
static void configure_base_frequencies(I3C_PrivData *pd, I3C_Config const *config) {
    uint32_t presc_reg = 0;
    uint16_t presc_ctl0_i2c = 0;
    uint16_t presc_ctl0_sdr = 0;
    /*
     * Calculate values for prescalers according to 3.2.1 chapter of the
     * I3C Master implementation specification v0.3
     */
    presc_ctl0_i2c = get_i2c_base_frequency(config);
    presc_ctl0_sdr = get_sdr_base_frequency(config);

    // Write base frequencies configuration
    presc_reg = CPS_FLD_WRITE(MIPI__PRESCL_CTRL0, I2C, presc_reg, presc_ctl0_i2c);
    presc_reg = CPS_FLD_WRITE(MIPI__PRESCL_CTRL0, I3C, presc_reg, presc_ctl0_sdr);

    CPS_REG_WRITE(&pd->regs_base->PRESCL_CTRL0, presc_reg);
}

/**
 * Configures PRESCL_CTRL1 register, timings.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] config Specifies driver/hardware configuration.
 */
static void configure_timings(I3C_PrivData *pd, I3C_Config const *config) {
    uint32_t presc_reg = 0;
    uint8_t presc_ctl1_od_low = 0;
    uint8_t presc_ctl1_pp_low = 0;

    presc_reg = CPS_REG_READ(&pd->regs_base->PRESCL_CTRL1);
    if (config->maxSclFreq != 0U) {
        /*
         * Calculate value for Asymetric Push-Pull SCL Timing
         */
        presc_ctl1_pp_low = get_asymmetric_push_pull_low(config);
        presc_reg = CPS_FLD_WRITE(MIPI__PRESCL_CTRL1, PP_LOW, presc_reg, presc_ctl1_pp_low);
    }

    if (config->sclLowPeriod != 0U) {
        /*
         * Calculate value for Open-Drain SCL Timing
         */
        presc_ctl1_od_low = get_open_drain_low_period(config);
        presc_reg = CPS_FLD_WRITE(MIPI__PRESCL_CTRL1, OD_LOW, presc_reg, presc_ctl1_od_low);
    }

    // Write timings configuration
    CPS_REG_WRITE(&pd->regs_base->PRESCL_CTRL1, presc_reg);
}

/**
 * Calculates prescaler values for Legacy I2C and SDR frequencies.
 * Please make sure core is disabled.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] config Specifies driver/hardware configuration.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigurePrescalers(I3C_PrivData *pd, I3C_Config const *config)
{
    uint32_t result = 0U;

    /*
     * Validate parameters passed function
     */
    if (I3C_ConfigurePrescalersSF(pd, config) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    }
    else if (core_enabled(pd)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Core enabled.\n");
        result = EIO;
    } else if ((config->i2cFreq == 0U) || (config->sdrFreq == 0U)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid input frequencies.\n");
        result = EINVAL;
    } else {

        // Configure base frequencies
        configure_base_frequencies(pd, config);

        // Configure timings
        configure_timings(pd, config);

    }

    return (result);
}

/**
 * Instantiates the I3C Core Driver, given the required blocks of
 * memory (this includes initializing the instance and the underlying
 * hardware). If a client configuration is required (likely to always
 * be true), it is passed in also. Returns an instance pointer, which
 * the client must maintain and pass to all other driver functions.
 * (except probe).
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] config Specifies driver/hardware configuration.
 * @param[in] callbacks Event Handlers and Callbacks.
 * @return 0 On success
 * @return EINVAL If illegal/inconsistent values in 'config' doesn't support feature(s) required by 'config' parameters.
 * @return EIO if operation failed
 */
uint32_t I3C_Init(I3C_PrivData *pd, const I3C_Config *config,
                  const I3C_Callbacks *callbacks)
{
    uint32_t result = 0U;

    /* Custom validation is needed because dev_role and
       bus_mode are configured during initialization */
    if (validate_init_input(pd, config, callbacks) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        (void)memset(pd, 0, sizeof(I3C_PrivData)); // cmds are cleared later

        pd->regs_base = config->regsBase;
        pd->callbacks = *callbacks;
        pd->isr_en = false;
        pd->bus_mode = I3C_BUS_MODE_MIXED_FAST;

        i3c_auto_configuration(pd);

        // Indicate that no TX/RX thresholded command is in progress
        pd->tx_cmd_in_progress = NULL;
        pd->rx_cmd_in_progress = NULL;
        pd->next_cmd_id = I3C_CMD_ID_INIT_VALUE;

        // Disable all interrupts
        CPS_REG_WRITE(&pd->regs_base->MST_IDR, 0xFFFFFFFFU);

        result = I3C_GetInstance()->configurePrescalers(pd, config);
        if (result != 0U) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to initialize prescalers.\n");
        }
        // Configure Devices
        else if ((config->numDevs != 0U) && (config->devs == NULL)) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Configuring Devices.\n"
                                          "Error. Invalid Device configuration.\n");
            result = EINVAL;
        } else if (configure_devices(pd, config) != 0U) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Cannot configure devices.\n");
            result = EINVAL;
        }
        else {
            DbgMsg(DBG_GEN_MSG, DBG_FYI, "Device initialized.\n");
        }
    }
    return (result);
}

/**
 * Executes callback if specified interrupt occured.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 * @param[in] callback Callback function
 * @param[in] mask Mask of interrupt
 */
static inline void i3c_process_isr_callback(I3C_PrivData *pd, uint32_t isr, void (*callback)(I3C_PrivData *pd), uint32_t mask)
{
    if ((isr & mask) != 0U) {
        i3c_callback_execute(pd, callback);
    }
}

/**
 * Processes protocol interrupt callbacks.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static inline void i3c_process_protocol_callbacks(I3C_PrivData *pd, uint32_t isr)
{
    i3c_process_isr_callback(pd, isr, pd->callbacks.mastershipDone, MIPI__MST_ISR__MR_DONE_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.immComplete, MIPI__MST_ISR__IMM_COMP_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.halted, MIPI__MST_ISR__HALTED_MASK);
}

/**
 * Processes command execution interrupt callbacks.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static inline void i3c_process_cmd_callbacks(I3C_PrivData *pd, uint32_t isr)
{
    // Command response
    i3c_process_isr_callback(pd, isr, pd->callbacks.cmdResponseOverflow, MIPI__MST_ISR__CMDR_OVF_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.cmdResponseThreshold, MIPI__MST_ISR__CMDR_THR_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.cmdResponseUnderflow, MIPI__MST_ISR__CMDR_UNF_MASK);
    // Command descriptor
    i3c_process_isr_callback(pd, isr, pd->callbacks.cmdDescriptorOverflow, MIPI__MST_ISR__CMDD_OVF_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.cmdDescriptorThreshold, MIPI__MST_ISR__CMDD_THR_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.cmdDescriptorEmpty, MIPI__MST_ISR__CMDD_EMP_MASK);
}

/**
 * Processes ibi interrupt callbacks.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static inline void i3c_process_ibi_callbacks(I3C_PrivData *pd, uint32_t isr)
{
    // IBI response
    i3c_process_isr_callback(pd, isr, pd->callbacks.ibiResponseOverflow, MIPI__MST_ISR__IBIR_OVF_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.ibiResponseThreshold, MIPI__MST_ISR__IBIR_THR_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.ibiResponseUnderflow, MIPI__MST_ISR__IBIR_UNF_MASK);

    // IBI data
    i3c_process_isr_callback(pd, isr, pd->callbacks.ibiDataUnderflow, MIPI__MST_ISR__IBID_UNF_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.ibiDataThreshold, MIPI__MST_ISR__IBID_THR_MASK);
}

/**
 * Processes TX/RX fifo interrupt callbacks.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static inline void i3c_process_fifo_isr_callbacks(I3C_PrivData *pd, uint32_t isr)
{
    i3c_process_isr_callback(pd, isr, pd->callbacks.txDataFifoOverflow, MIPI__MST_ISR__TX_OVF_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.rxDataFifoUnderflow, MIPI__MST_ISR__RX_UNF_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.txFifoThreshold, MIPI__MST_ISR__TX_THR_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.rxFifoThreshold, MIPI__MST_ISR__RX_THR_MASK);
}

/**
 * Processes all callbacks.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static void i3c_process_isr_callbacks(I3C_PrivData *pd, uint32_t isr)
{
    i3c_process_protocol_callbacks(pd, isr);
    i3c_process_fifo_isr_callbacks(pd, isr);
    i3c_process_cmd_callbacks(pd, isr);
    i3c_process_ibi_callbacks(pd, isr);
}

/**
 * Matches write command and executes SDR/DDR payload write
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 * @return 0 on success
 */
static uint32_t fetch_command_write(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    uint32_t result = 0U;

    if ((cmd->bytes_fared < cmd->payload_size) &&
        /* only write command */
        (cmd->is_wr_cmd == true) &&
        /* command with small payload or payload writing is in progress */
        ((pd->tx_cmd_in_progress == NULL) || (pd->tx_cmd_in_progress == cmd))
        ) {
        if (((uint32_t)cmd->cmd & I3C_CMD_DDR_MODE) != 0U) {
            ddr_write_tx_data(pd, cmd);
        } else {
            sdr_write_tx_data(pd, cmd);
        }
    }

    return (result);
}
/**
 * Processes TX FIFO threshold callback.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static void i3c_process_isr_tx_thr(I3C_PrivData *pd, uint32_t isr)
{

    if ((isr &  MIPI__MST_ISR__TX_THR_MASK) != 0U) {
        pd->tx_fifo_used = pd->threshold_config.txFifoThreshold;

        /* Following might occur. If sending 64 bytes of data while
         * tx_threshold is set to 32 bytes, two interrupts occur:
         * tx_threshold and comp. tx_threshold occurs first,
         * interrupted by comp, ends up being handled after comp. comp
         * handler removes element from the software buffer. If it was
         * the last element in the buffer tx_interrupt handler (this
         * function) will find that software command buffer is
         * empty. */

        if(CLTraverse(&(pd->cmd_list), pd, fetch_command_write) != 0U) {
            DbgMsg(DBG_GEN_MSG, DBG_FYI, "Command list is empty.\n");
        }
    }
}
/**
 * Matches write command and executes SDR/DDR payload read
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 * @return 0 on success
 */
static uint32_t fetch_command_read(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    uint32_t result = 0U;

    if ((cmd->bytes_fared < cmd->payload_size)  &&
        /* only read command */
        (cmd->is_wr_cmd == false) &&
        /* command with small payload or payload reading is in progress */
        ((pd->rx_cmd_in_progress == NULL) || (pd->rx_cmd_in_progress == cmd))
        ) {
        if (((uint32_t)cmd->cmd & I3C_CMD_DDR_MODE) != 0U) {
            result = ddr_read_rx_data(pd, cmd);
        } else {
            sdr_read_rx_data(pd, cmd);
        }
    }

    return (result);
}

/**
 * Processes RX FIFO threshold callback.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static void i3c_process_isr_rx_thr(I3C_PrivData *pd, uint32_t isr)
{
    if ((isr &  MIPI__MST_ISR__RX_THR_MASK) != 0U) {
        pd->rx_fifo_used = pd->threshold_config.rxFifoThreshold;

        /* Following might occur. If reading 64 bytes of data while
         * rx_threshold is set to 32 bytes, two interrupts occur:
         * rx_threshold and comp. rx_threshold occurs first,
         * interrupted by comp, ends up being handled after comp. comp
         * handler removes element from the software buffer. If it was
         * the last element in the buffer tx_interrupt handler (this
         * function) will find that software command buffer is
         * empty. */

        if(CLTraverse(&(pd->cmd_list), pd, fetch_command_read) != 0U) {
            DbgMsg(DBG_GEN_MSG, DBG_FYI, "Command list is empty.\n");
        }
        CPS_REG_WRITE(&pd->regs_base->MST_ICR, MIPI__MST_ISR__RX_THR_MASK);
    }
}

/**
 * Calls SDR/DDR read depending on command type.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 */
static void read_rx_data(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    uint32_t status;

    /* Check if command was RX and there was payload to read. */
    /* check if DDR */
    if (((uint32_t)cmd->cmd & I3C_CMD_DDR_MODE) != 0U) {
        /* Check if there is payload */
        if (cmd->payload_ddr != NULL) {
            /* DDR Write*/
            if (cmd->is_wr_cmd == false) {
                status = ddr_read_rx_data(pd, cmd);
                if (status != 0U) {
                    DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. DDR mode read error.\n");
                }
            }
        }
    }
    else { /* SDR */
           /* Check if there is payload */
        if (cmd->payload_sdr != NULL) {
            /* SDR write */
            if (cmd->is_wr_cmd == false) {
                sdr_read_rx_data(pd, cmd);
            }
        }
    }
}

/**
 * Swaps bytes of SDR/DDR command payload.
 * @param[in] cmd Pointer to I3C command structure.
 */
static void swap_bytes(volatile I3C_cmd_t *cmd)
{
    uint8_t ccc = ((uint8_t)((cmd->cmd) >> 32));
    /* Check if transfer is DDR */
    if (((uint32_t)cmd->cmd & I3C_CMD_DDR_MODE) != 0U) {
        if ((cmd->payload_ddr != NULL) &&
            ((ccc == (uint8_t)I3C_CCC_GETPID_DC) ||
             (ccc == (uint8_t)I3C_CCC_GETMRL_DC) ||
             (ccc == (uint8_t)I3C_CCC_GETMWL_DC))) {
            swap_bytes_ddr(cmd->payload_ddr, cmd->payload_size);
        }
    }
    else { /* transfer is SDR */
        if ((cmd->payload_sdr != NULL) &&
            ((ccc == (uint8_t)I3C_CCC_GETPID_DC) ||
             (ccc == (uint8_t)I3C_CCC_GETMRL_DC) ||
             (ccc == (uint8_t)I3C_CCC_GETMWL_DC))) {
            swap_bytes_sdr(cmd->payload_sdr, cmd->payload_size);
        }
    }
}

/**
 * Processes command when is completed.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 */
static void process_cmd(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    read_rx_data(pd, cmd);

    swap_bytes(cmd);

}

/**
 * Removes command from fifo and list
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 */
static void remove_cmd_from_fifo(I3C_PrivData *pd, volatile I3C_cmd_t *cmd)
{
    // Delete command from SW command list
    uint32_t result = CLDelete(&(pd->cmd_list), cmd->id);
    if (result != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Command not fount in SW buffer.\n");
    }

    if (pd->cmd_fifo_used > 0U) {
        pd->cmd_fifo_used--;
    }
    pd->cmd_in_progress = false;
    pd->tx_fifo_used = 0;

    /* Check if it was the last command */
    if (CLEmpty(&(pd->cmd_list))) {
        pd->cmd_empty = true;
        // Execute command buffer empty callback
        i3c_callback_execute(pd, pd->callbacks.commandBufferEmpty);
    }
}

/**
 * Processes DAA complete event.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] error Command response error
 */
static void ccc_daa_complete_handler(I3C_PrivData *pd,
                                     uint8_t       error)
{
    if(error == 0U) {
        update_device_info(pd);

        i3c_callback_execute(pd, pd->callbacks.daaComplete);
    }
}

/**
 * Processes command response event.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 * @param[in] error Command response error.
 * @param[in] xfer_bytes Number of bytes xferred.
 */
static void i3c_process_command_response(I3C_PrivData *      pd,
                                         volatile I3C_cmd_t* cmd,
                                         uint8_t             error,
                                         uint16_t            xfer_bytes)
{
    uint8_t ccc;

    if (cmd != NULL) {

        ccc = ((uint8_t)((cmd->cmd) >> 32U));
        if ((ccc == (uint8_t)I3C_CCC_SETDASA_DC) ||
            (ccc == (uint8_t)I3C_CCC_SETNEWDA_DC)) {

            ccc_set_da_complete_handler(pd, cmd, error);
        }
        else if(ccc == (uint8_t)I3C_CCC_ENTDAA_BC) {
            ccc_daa_complete_handler(pd, error);
        }
        else {
            DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "Unknown command type\n");
        }

        // Process command in case of read transfer
        process_cmd(pd, cmd);

        /* Check if number of bytes that were supposed to be
           transfered equals to number of bytes that were
           transfered by HW */
        if (cmd->payload_iterator != xfer_bytes) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Number of transferred bytes mismatch. SW transferred: %d, HW transferred: %d\n",
                   cmd->payload_iterator, xfer_bytes);
        }
        DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "ID: 0x%.2X, XB: %d, ERR: %d\n", cmd->id, xfer_bytes, error);
        if (error == 0U) {
            // Execute command complete callback
            i3c_callback_execute(pd, pd->callbacks.commandComplete);
        }

        // Remove command from command list if response is received
        if(cmd->in_cmd_fifo) {
            remove_cmd_from_fifo(pd, cmd);
        }
    }
}

/**
 * Processes command response.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] response Value of CMDR fifo word.
 * @param[in] immediate Indicates if command is immediate.
 */
static void i3c_process_cmd_response(I3C_PrivData *pd,
                                     uint32_t      response,
                                     bool          immediate)
{
    volatile I3C_cmd_t *cmd = NULL;
    uint8_t cmd_id;
    uint16_t xfer_bytes;
    uint8_t error;

    cmd_id = (uint8_t) CPS_FLD_READ(MIPI__CMDR, CMD_ID, response);
    xfer_bytes = (uint16_t) CPS_FLD_READ(MIPI__CMDR, XFER_BYTES, response);
    error = (uint8_t) CPS_FLD_READ(MIPI__CMDR, ERROR, response);

    if (true == immediate) {
        cmd = &(pd->imd_cmd);
        pd->imd_in_progress = false;

        // Process response of immediate command
        i3c_process_command_response(pd, cmd, error, xfer_bytes);

    }
    else if (cmd_id == I3C_CMD_ID_HW_ENTDAA) {
        // Hardware issued ENTDAA response
        uint32_t devs = CPS_REG_READ(&pd->regs_base->DEVS_CTRL);
        pd->hj_devices = (pd->devs_active ^ (uint16_t) devs);
        pd->devs_active = (uint16_t) devs;

        ccc_daa_complete_handler(pd, error);
    }
    else if (cmd_id == I3C_CMD_ID_HW_DISEC) {
        // Hardware issued DISEC response
        if(error == 0U) {
            i3c_callback_execute(pd, pd->callbacks.commandComplete);
        }
    }
    else {
        // Look for command in command list
        uint32_t status = CLSearch(&(pd->cmd_list), &cmd, cmd_id);
        if(status == 0U) {
            // Process response if command was found
            i3c_process_command_response(pd, cmd, error, xfer_bytes);
        }
        else {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Command ID 0x%.2X not found in SW command buffer, status: %d\n", cmd_id, status);
        }
    }
}

/**
 * Processes CMDR threshold interrupt.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static void i3c_process_isr_cmdr_thr(I3C_PrivData *pd, uint32_t isr)
{

    uint32_t response;
    uint8_t handled_responses = 0U;

    if ((isr &  MIPI__MST_ISR__CMDR_THR_MASK) != 0U) {
        // At least one response is stored in fifo if threshold ocurred
        do {
            response = CPS_REG_READ(&pd->regs_base->CMDR);
            bool immediate = ((isr & MIPI__MST_ISR__IMM_COMP_MASK) != 0U);
            i3c_process_cmd_response(pd, response, immediate);
            handled_responses++;
        } while (((CPS_REG_READ(&pd->regs_base->MST_STATUS0) &
                   MIPI__MST_STATUS0__CMDR_EMP_MASK) == 0U) &&
                 (handled_responses < pd->threshold_config.cmdrFifoThreshold));
    }
}

/**
 * Processes ibi response.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] response Value of IBIR fifo word.
 */
static void i3c_process_ibi_response(I3C_PrivData *pd, uint32_t response)
{
    I3C_Ibi ibi = {
        .ibi_type = (uint8_t) CPS_FLD_READ(MIPI__IBIR, IBI_TYPE, response),
        .slv_id = (uint8_t) CPS_FLD_READ(MIPI__IBIR, SLV_ID, response),
        .xfer_bytes = (uint8_t) CPS_FLD_READ(MIPI__IBIR, XFER_BYTES, response),
        .error = (CPS_FLD_READ(MIPI__IBIR, ERROR, response) > 0U),
        .acked = (CPS_FLD_READ(MIPI__IBIR, RESP, response) > 0U)
    };

    DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "IBI_TYPE: %d, SLV_ID: %d, ERR: %d\n",
           ibi.ibi_type, ibi.slv_id, ibi.error);

    if (true == ibi.error) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. IBI aborted.\n");
    } else if (false == ibi.acked) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. IBI nacked.\n");
    } else if (ibi.ibi_type == ((uint8_t) I3C_IBI_HOTJOIN)) {
        // Execute Hot-Join callback
        i3c_callback_execute(pd, pd->callbacks.hotjoin);
    } else if (ibi.ibi_type == ((uint8_t) I3C_IBI_MASTERSHIP)) {
        // Execute Mastership Request callback
        i3c_callback_execute(pd, pd->callbacks.mastershipRequest);
    } else {
        // Store last IBI in private data
        pd->ibi = ibi;
        i3c_callback_execute(pd, pd->callbacks.inbandInterrupt);
    }
}

/**
 * Processes IBIR threshold interrupt.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static void i3c_process_isr_ibir_thr(I3C_PrivData *pd, uint32_t isr)
{
    uint32_t response;
    uint8_t handled_ibis = 0U;

    if ((isr &  MIPI__MST_ISR__IBIR_THR_MASK) != 0U) {
        // At least one response is stored in fifo if threshold ocurred
        do {
            response = CPS_REG_READ(&pd->regs_base->IBIR);
            i3c_process_ibi_response(pd, response);
            handled_ibis++;
        } while (((CPS_REG_READ(&pd->regs_base->MST_STATUS0) &
                   MIPI__MST_STATUS0__IBIR_EMP_MASK) == 0U) &&
                 (handled_ibis < pd->threshold_config.ibirFifoThreshold));
    }
}

/**
 * Processes CMDD empty interrupt.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static void i3c_process_isr_cmd_fifo_empty(I3C_PrivData *pd, uint32_t isr)
{
    if ((isr &  MIPI__MST_ISR__CMDD_EMP_MASK) != 0U) {
        pd->cmd_fifo_empty = true;

        // Check if there is more commands in SW command lsit
        if (CLEmpty(&(pd->cmd_list)) == false) {
            pd->cmd_empty = false;
            // Execute commands
            (void)I3C_GetInstance()->cmdExec(pd);
        }
    }
}
/**
 * Forward interrupt processing.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static void process_isr_thr(I3C_PrivData *pd, uint32_t isr)
{
    i3c_process_isr_tx_thr(pd, isr);

    i3c_process_isr_rx_thr(pd, isr);

    i3c_process_isr_cmdr_thr(pd, isr);

    i3c_process_isr_ibir_thr(pd, isr);
}
/**
 * Clears interrupts and forwards interrupt processing
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static void i3c_process_isr(I3C_PrivData *pd, uint32_t isr)
{
    // Clear interrupts
    CPS_REG_WRITE(&pd->regs_base->MST_ICR, isr);

    process_isr_thr(pd, isr);

    i3c_process_isr_cmd_fifo_empty(pd, isr);

    i3c_process_isr_callbacks(pd, isr);
}

/**
 * Processes comp interrupt callbacks. Slave Mode.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static void i3cs_process_comp_isr(I3C_PrivData *pd, uint32_t isr)
{
    // Execute callbacks
    i3c_process_isr_callback(pd, isr, pd->callbacks.slaveSdrWrComplete, MIPI__SLV_ISR__SDR_WR_COMP_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.slaveSdrRdComplete, MIPI__SLV_ISR__SDR_RD_COMP_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.slaveDdrWrComplete, MIPI__SLV_ISR__DDR_WR_COMP_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.slaveDdrRdComplete, MIPI__SLV_ISR__DDR_RD_COMP_MASK);
    i3c_process_isr_callback(pd, isr, pd->callbacks.testMode, MIPI__SLV_ISR__TM_MASK);
}

/**
 * Clears interrupts, reads status to allow next private transfer processing
 * and forwards interrupts processing.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] isr ISR register value
 */
static void i3c_slave_process_isr(I3C_PrivData *pd, uint32_t isr)
{
    // Necessary to allow processing of next command, otherwise command is nacked
    (void)CPS_REG_READ(&pd->regs_base->SLV_STATUS0);

    CPS_REG_WRITE(&pd->regs_base->SLV_ICR, isr);

    i3cs_process_comp_isr(pd, isr);
}
/**
 * I3C Core Driver's ISR. Platform-specific code is responsible for
 * ensuring this gets called when the corresponding hardware's
 * interrupt is asserted. Registering the ISR should be done after
 * calling init, and before calling start. The driver's ISR will not
 * attempt to lock any locks, but will perform client callbacks. If
 * the client wishes to defer processing to non-interrupt time, it is
 * responsible for doing so. This function must not be called after
 * calling destroy and releasing private data memory.
 * @param[in] pd Pointer to driver's private data object filled by init.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_Isr(I3C_PrivData *pd)
{
    uint32_t result = 0U;
    uint32_t isr;

    /*
     * Validate parameters passed function
     */
    if (I3C_IsrSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if (pd->isr_en) {

        /*
         * Different ISR register is read depending on
         * device role.
         */
        if(pd->dev_role == I3C_DEV_ROLE_MASTER) {
            // ISR of master device
            isr = CPS_REG_READ(&pd->regs_base->MST_ISR);

            if (isr != 0U) {
                i3c_process_isr(pd, isr);
            }
        }
        else {
            // ISR of master in slave mode
            isr = CPS_REG_READ(&pd->regs_base->SLV_ISR);

            if (isr != 0U) {
                i3c_slave_process_isr(pd, isr);
            }
        }
    }
    else {
        cDbgMsg(DBG_GEN_MSG, DBG_FYI, "Interrupts disabled.\n");
    }

    return (result);
}

/**
 * Start the I3C driver, enabling interrupts. This is called after
 * the client has successfully initialized the driver and hooked the
 * driver's ISR (the isr member of this struct) to the IRQ.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_Start(I3C_PrivData *pd)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_StartSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        pd->isr_en = true;
        result = 0U;
    }

    return (result);
}

/**
 * The client may call this to disable the hardware (disabling its
 * IRQ at the source and disconnecting it if applicable). Also, a
 * best-effort is made to cancel any pending transactions.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_Stop(I3C_PrivData *pd)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_StopSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        pd->isr_en = false;
        result = 0U;
    }

    return (result);
}

/**
 * This performs an automatic stop and then de-initializes the
 * driver. The client may not make further requests on this instance.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_Destroy(I3C_PrivData *pd)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_DestroySF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        (void)I3C_GetInstance()->stop(pd);
        result = I3C_GetInstance()->disableCore(pd);
    }

    return (result);
}

/*
 * API Functions.
 */

/**
 * Enables the I3C Master Core.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_EnableCore(I3C_PrivData *pd)
{
    uint32_t result;
    uint32_t reg_val;

    DbgMsg(DBG_GEN_MSG, DBG_FYI, "Enabling Core.\n");

    /*
     * Validate parameters passed function
     */
    if (I3C_EnableCoreSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
        reg_val = CPS_FLD_SET(MIPI__CTRL, DEV_EN, reg_val);
        CPS_REG_WRITE(&pd->regs_base->CTRL, reg_val);

        result = 0U;
    }

    return (result);
}

/**
 * Waits for all operations to be completed and then it disables the
 * I3C Master Core.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_DisableCore(I3C_PrivData *pd)
{
    uint32_t result;
    uint32_t reg_val;

    DbgMsg(DBG_GEN_MSG, DBG_FYI, "Disabling Core.\n");

    /*
     * Validate parameters passed function
     */
    if (I3C_DisableCoreSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
        reg_val = CPS_FLD_CLEAR(MIPI__CTRL, DEV_EN, reg_val);
        CPS_REG_WRITE(&pd->regs_base->CTRL, reg_val);

        // Wait for Core to enter idle state
        result = wait_for_core_idle(pd);
    }

    return (result);
}

/**
 * Sets Bus Mode.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] mode Bus Mode.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SetBusMode(I3C_PrivData *pd, I3C_BusMode mode)
{
    uint32_t result;
    uint32_t reg_val;

    /*
     * Validate parameters passed function
     */
    if (I3C_SetBusModeSF(pd, mode) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if (I3C_GetInstance()->disableCore(pd) != 0U) {
        // Wait for Core to become idle and disable it
        result = EIO;
    } else {

        reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
        reg_val = CPS_FLD_WRITE(MIPI__CTRL, BUS_MODE, reg_val, mode);
        CPS_REG_WRITE(&pd->regs_base->CTRL, reg_val);
        pd->bus_mode = mode;

        result = I3C_GetInstance()->enableCore(pd);
    }

    return (result);
}

/**
 * Reads Bus Mode.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] mode Pointer to Bus Mode object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_GetBusMode(I3C_PrivData *pd, I3C_BusMode *mode)
{
    uint32_t result;
    uint32_t reg_val;
    uint32_t bus_mode;
    /*
     * Validate parameters passed function
     */
    if (I3C_GetBusModeSF(pd, mode) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
        bus_mode = CPS_FLD_READ(MIPI__CTRL, BUS_MODE, reg_val);
        if (bus_mode == 0U) {
            *mode = I3C_BUS_MODE_PURE;
        }
        else if (bus_mode == 1U) {
            *mode = I3C_BUS_MODE_INVALID;
        }
        else if (bus_mode == 2U) {
            *mode = I3C_BUS_MODE_MIXED_FAST;
        }
        else {
            /* if no other mode, it must be mixed slow mode */
            *mode = I3C_BUS_MODE_MIXED_SLOW;
        }
        result = 0U;
    }

    return (result);
}

/**
 * Sets BCR value in one of the retaining registers (DeviceIDx).
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] bcr BCR value.
 * @param[in] devId Retaining register number.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SetBcr(I3C_PrivData *pd, uint8_t bcr, uint8_t devId)
{
    uint32_t result;

    volatile uint32_t *dev_cfg_reg;
    volatile uint32_t *dev_bcr_reg;
    uint32_t reg_val;

    /*
     * Validate parameters passed function
     */
    if (I3C_SetBcrSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        // ID0_RR0 register stores information about this device
        dev_cfg_reg = getAdvanceVolatileUint32Ptr(&(pd->regs_base->DEV_ID0_RR0), devId * I3C_DEVS_OFF);
        dev_bcr_reg = getAdvanceVolatileUint32Ptr(dev_cfg_reg, 2U);

        reg_val = CPS_UncachedRead32(dev_bcr_reg);
        reg_val = CPS_FLD_WRITE(MIPI__DEV_ID0_RR2, BCR, reg_val, bcr);
        CPS_UncachedWrite32(dev_bcr_reg, reg_val);

        pd->devs[devId].bcr = bcr;
        pd->devs[devId].hdrCapable = (CPS_FldRead(I3C_BCR_HDR_CAPABLE_MASK, I3C_BCR_HDR_CAPABLE_SHIFT, bcr) != 0U);

        result = 0U;
    }

    return (result);
}

/**
 * Sets DCR value in one of the retaining registers (DeviceIDx).
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] dcr DCR value.
 * @param[in] devId Retaining register number.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SetDcr(I3C_PrivData *pd, uint8_t dcr, uint8_t devId)
{
    uint32_t result;
    volatile uint32_t *dev_cfg_reg;
    volatile uint32_t *dev_dcr_reg;

    uint32_t reg_val;

    /*
     * Validate parameters passed function
     */
    if (I3C_SetDcrSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        // ID0_RR0 register stores information about this device
        dev_cfg_reg = getAdvanceVolatileUint32Ptr(&(pd->regs_base->DEV_ID0_RR0), devId * I3C_DEVS_OFF);
        dev_dcr_reg = getAdvanceVolatileUint32Ptr(dev_cfg_reg, 2U);

        reg_val = CPS_UncachedRead32(dev_dcr_reg);
        reg_val = CPS_FLD_WRITE(MIPI__DEV_ID0_RR2, DCR_LVR, reg_val, dcr);
        CPS_UncachedWrite32(dev_dcr_reg, reg_val);

        pd->devs[devId].dcr = dcr;

        result = 0U;
    }

    return (result);
}

/**
 * Sets Provisional ID value in one of the retaining registers
 * (DeviceIDx).
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] pid Ponter to buffer containing 48-bit Provisional ID value.
 * @param[in] devId Retaining register number.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SetPid(I3C_PrivData *pd, const uint8_t *pid, uint8_t devId)
{
    uint32_t result;
    volatile uint32_t *dev_cfg_reg;
    volatile uint32_t *dev_pid_msb;
    volatile uint32_t *dev_pid_lsb;

    uint32_t reg_val;
    uint32_t pid_msb;
    uint16_t pid_lsb;

    /*
     * Validate parameters passed function
     */
    if (I3C_SetPidSF(pd, pid) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        // ID0_RR0 register stores information about this device
        dev_cfg_reg = getAdvanceVolatileUint32Ptr(&(pd->regs_base->DEV_ID0_RR0), devId * I3C_DEVS_OFF);
        dev_pid_msb = getAdvanceVolatileUint32Ptr(dev_cfg_reg, 1U);
        dev_pid_lsb = getAdvanceVolatileUint32Ptr(dev_cfg_reg, 2U);

        // MSB of PID
        pid_msb = ((uint32_t)pid[5] << 24) | ((uint32_t)pid[4] << 16) | ((uint32_t)pid[3] << 8) | pid[2];
        // LSB of PID
        pid_lsb = ((uint16_t)pid[1] << 8) | pid[0];

        CPS_UncachedWrite32(dev_pid_msb, pid_msb);
        reg_val = CPS_UncachedRead32(dev_pid_lsb);
        reg_val = CPS_FLD_WRITE(MIPI__DEV_ID0_RR2, PID_LSB, reg_val, pid_lsb);
        CPS_UncachedWrite32(dev_pid_lsb, reg_val);

        pd->devs[devId].provIdHi = (uint16_t)(pid_msb >> 16);
        pd->devs[devId].provIdLo = (uint32_t)((pid_msb << 16) | pid_lsb);

        result = 0U;
    }

    return (result);
}

/**
 * Reads values of RR registers for specified device.
 * @param[in] dev Pointer to Device Descriptor structure.
 * @param[in] dev_rr_val_array Pointer to array of RR values.
 * @return Pointer to array of RR values.
 */
static uint32_t * get_dev_rr_values(I3C_DeviceDescriptor * dev, uint32_t * dev_rr_val_array)
{
    I3C_DeviceDescriptor * local_dev = dev;
    uint8_t slave_addr;

    // Add parity bit to address
    slave_addr = i3c_dev_addr_with_parity(dev->addr);
    // Set parity bit
    slave_addr |= i3c_da_get_parity_bit((uint8_t) dev->addr);

    dev_rr_val_array[0] = CPS_FLD_WRITE(MIPI__DEV_ID0_RR0, DEV_ADDR, dev_rr_val_array[0], slave_addr);
    dev_rr_val_array[1] = CPS_FLD_WRITE(MIPI__DEV_ID0_RR1, PID_MSB, dev_rr_val_array[1],
                                        (((uint32_t)local_dev->provIdHi) << 16) |
                                        ((uint16_t)(local_dev->provIdLo >> 16)));
    dev_rr_val_array[2] = CPS_FLD_WRITE(MIPI__DEV_ID0_RR2, PID_LSB, dev_rr_val_array[2], (uint16_t)(local_dev->provIdLo));
    return (dev_rr_val_array);
}

/**
 * Writes values of RR registers corresponding to legacy I2C device.
 * @param[in] dev Pointer to Device Descriptor structure.
 * @param[in] dev_rr_val_array Pointer to array of RR values.
 */
static void configure_legacy_device(I3C_DeviceDescriptor * dev, uint32_t * dev_rr_val_array)
{
    I3C_DeviceDescriptor * local_dev = dev;
    uint8_t lvr;

    dev_rr_val_array[0] = CPS_FLD_WRITE(MIPI__DEV_ID0_RR0, LVR_EXT_ADDR, dev_rr_val_array[0], dev->i2c10bAddr ? 1U : 0U);

    if (local_dev->i2c10bAddr) {
        dev_rr_val_array[0] = CPS_FLD_WRITE(MIPI__DEV_ID0_RR0, LVR_SA_MSB, dev_rr_val_array[0],
                                            ((uint32_t)local_dev->addr >> I2C_LVR_SA_MSB_SHIFT) & I2C_LVR_SA_MSB_MASK);
    }

    lvr = (uint8_t) (((local_dev->i2cRsvd & I2C_LVR_RSVD_MASK) << I2C_LVR_RSVD_OFFSET)
                     | ((local_dev->i2cFmPlusSpeed ? 1U : 0U) << I2C_LVR_MODE_OFFSET)
                     | (local_dev->i2cIdx & I2C_LVR_INDEX_MASK));

    // RR2 register stores LVR for I2C device
    dev_rr_val_array[1] = CPS_FLD_WRITE(MIPI__DEV_ID0_RR2, DCR_LVR, dev_rr_val_array[1], lvr);

}

/**
 * Writes values of RR registers corresponding to I3C device.
 * @param[in] dev Pointer to Device Descriptor structure.
 * @param[in] dev_rr_val_array Pointer to array of RR values.
 * @return Pointer to array of RR values.
 */
static uint32_t * set_dev_rr_values(I3C_DeviceDescriptor * dev, uint32_t * dev_rr_val_array)
{
    I3C_DeviceDescriptor * local_dev = dev;
    dev_rr_val_array[0] = CPS_FLD_WRITE(MIPI__DEV_ID0_RR0, IS_I3C, dev_rr_val_array[0], 1U);
    dev_rr_val_array[1] = CPS_FLD_WRITE(MIPI__DEV_ID0_RR2, DCR_LVR, dev_rr_val_array[2], local_dev->dcr);
    dev_rr_val_array[2] = CPS_FLD_WRITE(MIPI__DEV_ID0_RR2, BCR, dev_rr_val_array[2], local_dev->bcr);
    return (dev_rr_val_array);
}

/**
 * Writes configuration when all devices are known to host.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] pure_bus Indicates if bus is pure.
 * @param[in] i2c_speed Indicates slowest I2C device speed.
 * @param[in] dev_active Active devices mask.
 */
static void write_configuration(I3C_PrivData *pd, bool pure_bus, uint8_t i2c_speed, uint32_t dev_active)
{
    uint32_t reg_val;

    // Write active devices mask
    CPS_REG_WRITE(&pd->regs_base->DEVS_CTRL, dev_active);
    pd->devs_active = (uint16_t) dev_active;

    if (pure_bus) {
        pd->bus_mode = I3C_BUS_MODE_PURE;
    } else {
        pd->bus_mode = (i2c_speed == I2C_SLOW_MODE_FM_PLUS)
                       ? I3C_BUS_MODE_MIXED_FAST
                       : I3C_BUS_MODE_MIXED_SLOW;
    }

    reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
    reg_val = CPS_FLD_WRITE(MIPI__CTRL, BUS_MODE, reg_val, pd->bus_mode);
    CPS_REG_WRITE(&pd->regs_base->CTRL, reg_val);

}

/**
 * Configures Device connected to the I3C Bus (including Master)
 * accordingly to the capabilities of this device. I3C Master Core
 * must be disabled before calling this function.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] dev Pointer to the structure containing configuration and capabilities of the Device.
 * @param[in] index Index of the device corresponding to index of devs array (field of the I3C_PrivData structure).
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigureDevice(I3C_PrivData *        pd,
                             I3C_DeviceDescriptor *dev,
                             uint8_t               index)
{
    uint32_t result = 0U;
    // Adresses of the DeviceID<n> retaining registers.
    volatile uint32_t *dev_rr0_reg;
    volatile uint32_t *dev_rr1_reg;
    volatile uint32_t *dev_rr2_reg;

    uint32_t dev_rr_val_array[3] = {0U, 0U, 0U};
    uint32_t * p_dev_arr = dev_rr_val_array;

    /*
     * Validate parameters passed function
     */
    if (I3C_ConfigureDeviceSF(pd, dev) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if (core_enabled(pd)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Core is enabled.\n");
        result = EIO;
    } else if (index >= pd->max_devs) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Index too big.\n");
        result = EINVAL;
    } else {
        dev_rr0_reg = getAdvanceVolatileUint32Ptr(&(pd->regs_base->DEV_ID0_RR0), index * I3C_DEVS_OFF);
        dev_rr1_reg = getAdvanceVolatileUint32Ptr(dev_rr0_reg, 1U);
        dev_rr2_reg = getAdvanceVolatileUint32Ptr(dev_rr0_reg, 2U);

        // Set priv value as address of RR0 register
        dev->priv = dev_rr0_reg;

        p_dev_arr = get_dev_rr_values(dev, p_dev_arr);

        // Different configuration function for I2C device
        if (dev->legacyI2CDev) {
            configure_legacy_device(dev, p_dev_arr);
        } else {
            p_dev_arr = set_dev_rr_values(dev, p_dev_arr);

        }

        CPS_UncachedWrite32(dev_rr0_reg, p_dev_arr[0]);
        CPS_UncachedWrite32(dev_rr1_reg, p_dev_arr[1]);
        CPS_UncachedWrite32(dev_rr2_reg, p_dev_arr[2]);
        // Set device descriptor
        pd->devs[index] = *dev;
    }

    return (result);
}

/**
 * Processes device configuration.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] devs Pointer to Device Descriptor structure.
 * @param[in] num_devs Number of devices.
 * @return 0 on success
 * @return EIO if operation failed
 */
static uint32_t i3c_process_configure_devices(I3C_PrivData *pd,
                                              I3C_DeviceDescriptor *devs, uint8_t num_devs)
{
    uint32_t result = EIO;
    uint32_t i;
    uint8_t i2c_speed = I2C_SLOW_MODE_FM_PLUS; // pick fastest at start
    bool pure_bus = true;
    uint32_t dev_active = CPS_REG_READ(&pd->regs_base->DEVS_CTRL);
    I3C_DeviceDescriptor *local_devs = devs;

    (void)memset(pd->devs, 0, sizeof(I3C_DeviceDescriptor) * I3C_ABSOLUTE_MAX_DEVS_NUM);

    for (i = 0; i < num_devs; ++i) {

        I3C_DeviceDescriptor *dev = &local_devs[i];

        if (I3C_GetInstance()->configureDevice(pd, dev, i) == 0U) {
            // Only I3C HDR capable devices can make up the Pure Bus.
            pure_bus = pure_bus && (dev->legacyI2CDev == false) &&
                       (dev->hdrCapable == true);
            if (dev->legacyI2CDev == true) {
                // parasoft-begin-suppress MISRA2012-RULE-12_2-2 RHS operand of shift will always be less than 32
                // because num_devs is checked against too big values, also CONF_STATUS0.devs_num
                // is encoded on 4 bits, it means that max value of this field is 15
                if (dev->isActive) {
                    dev_active |= ((uint32_t)1U << i);
                } else {
                    dev_active &= ~((uint32_t)1U << i);
                }
                // parasoft-end-suppress MISRA2012-RULE-12_2-2

                if (dev->i2cFmPlusSpeed == false) {
                    i2c_speed = I2C_SLOW_MODE_FM;
                }
            }
        }
    }

    if(I3C_GetInstance()->disableCore(pd) == 0U) {
        /* write device activity and set bus speed */
        write_configuration(pd, pure_bus, i2c_speed, dev_active);
        result = 0U;
    }

    return (result);
}

/**
 * Sets interrupt configuration mask.
 * @param[in] interrupt_config Interrupt configuration.
 * @param[out] p_reg_val Interrupt mask.
 */
static void set_data_fifo_interrupts(const I3C_InterruptConfig* interrupt_config, uint32_t * p_reg_val)
{

    // Tx/Rx fifo overflow/underflow interrupts
    if (true == interrupt_config->txDataFifoOverflow) {
        *p_reg_val |= MIPI__MST_IER__TX_OVF_MASK;
    }

    if (true == interrupt_config->rxDataFifoUnderflow) {
        *p_reg_val |= MIPI__MST_IER__RX_UNF_MASK;
    }

    // Tx/Rx fifo threshold interrupts
    if (true == interrupt_config->txFifoThreshold) {
        *p_reg_val |= MIPI__MST_IER__TX_THR_MASK;
    }

    if (true == interrupt_config->rxFifoThreshold) {
        *p_reg_val |= MIPI__MST_IER__RX_THR_MASK;
    }
}

/**
 * Sets interrupt configuration mask.
 * @param[in] interrupt_config Interrupt configuration.
 * @param[out] p_reg_val Interrupt mask.
 */
static void set_cmd_interrupts(const I3C_InterruptConfig* interrupt_config, uint32_t * p_reg_val)
{
    // Command response interrupts
    if (true == interrupt_config->cmdResponseOverflow) {
        *p_reg_val |= MIPI__MST_IER__CMDR_OVF_MASK;
    }

    if (true == interrupt_config->cmdResponseUnderflow) {
        *p_reg_val |= MIPI__MST_IER__CMDR_UNF_MASK;
    }

    if (true == interrupt_config->cmdResponseThreshold) {
        *p_reg_val |= MIPI__MST_IER__CMDR_THR_MASK;
    }

    // Command descriptor interrupts
    if (true == interrupt_config->cmdDescriptorOverflow) {
        *p_reg_val |= MIPI__MST_IER__CMDD_OVF_MASK;
    }

    if (true == interrupt_config->cmdDescriptorEmpty) {
        *p_reg_val |= MIPI__MST_IER__CMDD_EMP_MASK;
    }

    if (true == interrupt_config->cmdDescriptorThreshold) {
        *p_reg_val |= MIPI__MST_IER__CMDD_THR_MASK;
    }
}

/**
 * Sets interrupt configuration mask.
 * @param[in] interrupt_config Interrupt configuration.
 * @param[out] p_reg_val Interrupt mask.
 */
static void set_ibi_interrupts(const I3C_InterruptConfig* interrupt_config, uint32_t * p_reg_val)
{
    // IBI response related interrupts
    if (true == interrupt_config->ibiResponseOverflow) {
        *p_reg_val |= MIPI__MST_IER__IBIR_OVF_MASK;
    }

    if (true == interrupt_config->ibiResponseUnderflow) {
        *p_reg_val |= MIPI__MST_IER__IBIR_UNF_MASK;
    }

    if (true == interrupt_config->ibiResponseThreshold) {
        *p_reg_val |= MIPI__MST_IER__IBIR_THR_MASK;
    }

    // IBI data related interrupts
    if (true == interrupt_config->ibiDataUnderflow) {
        *p_reg_val |= MIPI__MST_IER__IBID_UNF_MASK;
    }

    if (true == interrupt_config->ibiDataThreshold) {
        *p_reg_val |= MIPI__MST_IER__IBID_THR_MASK;
    }
}

/**
 * Sets interrupt configuration mask and configures interrupts.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] interrupt_config Interrupt configuration.
 */
static void i3c_process_config_interrupts(I3C_PrivData*              pd,
                                          const I3C_InterruptConfig* interrupt_config)
{
    uint32_t reg_val = 0U;

    set_cmd_interrupts(interrupt_config, &reg_val);
    set_data_fifo_interrupts(interrupt_config, &reg_val);
    set_ibi_interrupts(interrupt_config, &reg_val);

    // Immediate command completed
    if (true == interrupt_config->immComplete) {
        reg_val |= MIPI__MST_IER__IMM_COMP_MASK;
    }

    if (true == interrupt_config->mastershipDone) {
        reg_val |= MIPI__MST_IER__MR_DONE_MASK;
    }

    if (true == interrupt_config->halted) {
        reg_val |= MIPI__MST_IER__HALTED_MASK;
    }

    // Enable specified interrupts
    CPS_REG_WRITE(&pd->regs_base->MST_IER, reg_val);

    // Store interrupts configuration in private data
    pd->interrupt_config = *interrupt_config;
}

/**
 * Configures thresholds
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] threshold_config Threshold configuration.
 */
static void i3c_process_config_thresholds(I3C_PrivData*              pd,
                                          const I3C_ThresholdConfig* threshold_config)
{
    uint32_t reg_val = 0;

    reg_val =
        CPS_FLD_WRITE(MIPI__TX_RX_THR_CTRL,
                      TX_THR,
                      0,
                      (uint16_t)(threshold_config->txFifoThreshold / TX_FIFO_WORD_WIDTH)) |
        CPS_FLD_WRITE(MIPI__TX_RX_THR_CTRL,
                      RX_THR,
                      0,
                      (uint16_t)(threshold_config->rxFifoThreshold / RX_FIFO_WORD_WIDTH));

    // Write Tx/Rx fifos threshold
    CPS_REG_WRITE(&pd->regs_base->TX_RX_THR_CTRL, reg_val);

    reg_val =
        CPS_FLD_WRITE(MIPI__CMD_IBI_THR_CTRL,
                      CMDR_THR,
                      0,
                      threshold_config->cmdrFifoThreshold) |
        CPS_FLD_WRITE(MIPI__CMD_IBI_THR_CTRL,
                      IBIR_THR,
                      0,
                      threshold_config->ibirFifoThreshold);

    pd->threshold_config = *threshold_config;

    // Write CMDR/IBIR fifos threshold
    CPS_REG_WRITE(&pd->regs_base->CMD_IBI_THR_CTRL, reg_val);
}

/**
 * Configures Devices connected to the I3C Bus (including Master)
 * accordingly to the capabilities of these Devices. Configuration of
 * the Devices must be provided in following order: Master, I3C
 * Slaves with Dynamic Address support, I3C Slaves with static
 * addresses and at the end Legacy I2C Devices. Position of the
 * Device configuration in each group determines priority of the
 * Device. Configuration of at least two Devices (including Master)
 * need to be provided. I3C Master Core must be disabled before
 * calling this function.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] devs Pointer to the structure containing configuration and capabilities of the Devices.
 * @param[in] numDevs Total number of the Devices.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigureDevices(I3C_PrivData *pd,
                              I3C_DeviceDescriptor *devs, uint8_t numDevs)
{
    uint32_t result;
    I3C_DeviceDescriptor *local_devs = devs;
    /*
     * Validate parameters passed function
     */
    if (I3C_ConfigureDevicesSF(pd, local_devs) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if (core_enabled(pd)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Core is enabled.\n");
        result = EIO;
    } else if (numDevs > (pd->max_devs + 1U)) { // + Master
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Too many Devices. Maximum supported: %d.\n",
               pd->max_devs + 1U);
        result = EINVAL;
    } else {
        result = i3c_process_configure_devices(pd, local_devs, numDevs);
    }

    return (result);
}

/**
 * This function clears retaining registers set for a chosen device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] device_no Number corresponding to Device position in retaining register.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ClearRrOfDevice(I3C_PrivData* pd, uint8_t device_no)
{
    uint32_t result;
    uint32_t reg_val;
    uint8_t shift = 16U + device_no;
    /*
     * Validate parameters passed function
     */
    if (I3C_ClearRrOfDeviceSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if (!core_enabled(pd)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Core is disabled.\n");
        result = EIO;
    } else {
        reg_val = CPS_REG_READ(&pd->regs_base->DEVS_CTRL);
        pd->devs_active = ((uint16_t)reg_val ^ device_no);
        if (shift < 32U) {
            // Set corresponding clear bit to 1
            reg_val = reg_val | ((uint32_t)1UL << (shift));
            // Write clear mask
            CPS_REG_WRITE(&pd->regs_base->DEVS_CTRL, reg_val);
            result = 0;
        }
        else {
            result = EINVAL;
        }
    }
    return (result);
}

/**
 * Enables interrupts chosen by user.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] interruptConfig Interrupt configuration.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigureInterrupts(I3C_PrivData*              pd,
                                 const I3C_InterruptConfig* interruptConfig)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_ConfigureInterruptsSF(pd, interruptConfig) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if (core_enabled(pd)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Core is enabled.\n");
        result = EIO;
    } else {
        i3c_process_config_interrupts(pd, interruptConfig);
        result = 0;
    }

    return (result);
}

/**
 * Sets thresholds to levels selected by user.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] thresholdConfig Threshold configuration.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigureThresholds(I3C_PrivData*              pd,
                                 const I3C_ThresholdConfig* thresholdConfig)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_ConfigureThresholdsSF(pd, thresholdConfig) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if (core_enabled(pd)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Core is enabled.\n");
        result = EIO;
    } else {
        i3c_process_config_thresholds(pd, thresholdConfig);
        result = 0;
    }

    return (result);
}

/**
 * Returns address of ibi issuer.
 * @param[in] reg_val SIR register value.
 * @param[in] low Indicates if low part of register has DA.
 * @return Address of IBI issuer.
 */
static uint8_t get_issuer_address(uint32_t reg_val, bool low)
{
    uint8_t address = EINVAL;
    if(low) {
        address = (uint8_t) CPS_FLD_READ(MIPI__SIR_MAP0, DEVID0_DA, reg_val);
    }
    else {
        address = (uint8_t) CPS_FLD_READ(MIPI__SIR_MAP0, DEVID1_DA, reg_val);
    }
    return address;
}

/**
 * This function extracts address of device that issued IBI.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on error
 * @return valid I3C address on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_IbiGetAddressOfIssuer(I3C_PrivData *pd)
{
    uint32_t address_of_ibi_issuer = EINVAL;
    uint32_t reg_val = 0U;

    /*
     * Validate parameters passed function
     */
    if (I3C_IbiGetAddressOfIssuerSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
    } else {
        // SLV_ID of last In-Band Interrupt
        switch(pd->ibi.slv_id) {
        // Every SIR_MAP register stores information about two devices
        case 0:
        case 1:
            reg_val = CPS_REG_READ(&pd->regs_base->SIR_MAP0);
            break;
        case 2:
        case 3:
            reg_val = CPS_REG_READ(&pd->regs_base->SIR_MAP1);
            break;
        case 4:
        case 5:
            reg_val = CPS_REG_READ(&pd->regs_base->SIR_MAP2);
            break;
        case 6:
        case 7:
            reg_val = CPS_REG_READ(&pd->regs_base->SIR_MAP3);
            break;
        case 8:
        case 9:
            reg_val = CPS_REG_READ(&pd->regs_base->SIR_MAP4);
            break;
        case 10:
        case 11:
            reg_val = CPS_REG_READ(&pd->regs_base->SIR_MAP5);
            break;
        default:
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Unknown slave ID.\n");
            break;
        }
        address_of_ibi_issuer = get_issuer_address(reg_val, (pd->ibi.slv_id % I3C_NUM_OF_SLVS_PER_SIR) == 0U);
    }

    return (address_of_ibi_issuer);
}

/**
 * This function configures devices issuing IBI.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] ibi_sir_cfg Pointer to array that will hold configuration of IBI related devices.
 * @param[in] num_sir_cfgs Size of configuration array.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_IbiConfigureDevices(const I3C_PrivData *           pd,
                                 const I3C_IbiSirConfiguration* ibi_sir_cfg,
                                 uint32_t                       num_sir_cfgs)
{
    uint32_t result = 0U;
    uint32_t i;
    volatile uint32_t *sir_map_reg = NULL;
    uint32_t sir_map_val = 0;

    /*
     * Validate parameters passed function
     */
    if (I3C_IbiConfigureDevicesSF(pd, ibi_sir_cfg) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if (num_sir_cfgs > pd->max_ibi_devs) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments. That many devices are not suported.\n");
        result = EINVAL;
    } else {

        /* set up devices -- one sir configuration register holds info
         * on two devices*/
        for (i = 0; i < (num_sir_cfgs / 2U); ++i) { /* for each pair of devices */
            sir_map_reg = NULL;
            sir_map_val = 0;

            sir_map_reg = getAdvanceVolatileUint32Ptr(&(pd->regs_base->SIR_MAP0), i * I3C_SIR_MAP_OFF);

            sir_map_val = modify_sir_map_low(sir_map_val, &ibi_sir_cfg[i * 2U]);
            sir_map_val = modify_sir_map_high(sir_map_val, &ibi_sir_cfg[(i * 2U) + 1U]);

            CPS_UncachedWrite32(sir_map_reg, sir_map_val);
        }

        if ((num_sir_cfgs % 2U) != 0U) { /* if there is one odd device -- with
                                          * no one to pair up with */
            sir_map_reg = NULL;
            sir_map_val = 0;
            sir_map_reg = getAdvanceVolatileUint32Ptr(&(pd->regs_base->SIR_MAP0), i * I3C_SIR_MAP_OFF);

            sir_map_val = modify_sir_map_low(sir_map_val, &ibi_sir_cfg[i * 2U]);

            CPS_UncachedWrite32(sir_map_reg, sir_map_val);
        }

    }

    return (result);
}

/**
 * This function modifies device configuration issuing IBI.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] ibi_sir_cfg Pointer to struct that holds configuration of IBI device.
 * @param[in] da Device DA.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return ENOENT if device with DA not found
 */
uint32_t I3C_IbiModifyDeviceConfig(const I3C_PrivData *           pd,
                                   const I3C_IbiSirConfiguration* ibi_sir_cfg,
                                   uint8_t                        da)
{
    uint32_t result = ENOENT;
    uint32_t i;
    volatile uint32_t *sir_map_reg = NULL;
    uint32_t sir_map_val = 0;

    /*
     * Validate parameters passed function
     */
    if (I3C_IbiModifyDeviceConfigSF(pd, ibi_sir_cfg) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        for (i = 0; i < pd->max_ibi_devs; i += 2U) {
            sir_map_reg = getAdvanceVolatileUint32Ptr(&(pd->regs_base->SIR_MAP0), (i / 2U) * I3C_SIR_MAP_OFF);
            sir_map_val = CPS_REG_READ(sir_map_reg);

            // Lower part of SIR_MAP
            if (((uint8_t) CPS_FLD_READ(MIPI__SIR_MAP0, DEVID0_DA, sir_map_val)) == da) {
                sir_map_val = modify_sir_map_low(sir_map_val, ibi_sir_cfg);
                CPS_REG_WRITE(sir_map_reg, sir_map_val);
                result = 0U;
            }
            // Higher part of SIR_MAP
            if (((uint8_t) CPS_FLD_READ(MIPI__SIR_MAP0, DEVID1_DA, sir_map_val)) == da) {
                sir_map_val = modify_sir_map_high(sir_map_val, ibi_sir_cfg);
                CPS_REG_WRITE(sir_map_reg, sir_map_val);
                result = 0U;
            }
        }
    }

    return (result);
}

/**
 * This function extracts IBI payload.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buf Pointer to buffer that will hold received payload.
 * @param[in] payload_size Size of payload.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_IbiGetData(I3C_PrivData *pd, uint8_t* buf, uint16_t payload_size)
{
    uint32_t result;
    uint32_t data;
    uint16_t possible_to_read;
    uint16_t bytes_to_read;
    uint16_t bytes_xferred;
    uint16_t i;

    /*
     * Validate parameters passed function
     */
    if (I3C_IbiGetDataSF(pd, buf) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        bytes_xferred = 0U;
        /* Get number of possible bytes to read in case of
           device transferred less than expected */
        possible_to_read = (uint16_t) (min(payload_size, (uint16_t)pd->ibi.xfer_bytes));

        while (bytes_xferred < possible_to_read) {
            // Bytes possible to read
            bytes_to_read = min(
                possible_to_read - bytes_xferred,
                RX_FIFO_WORD_WIDTH);

            // Read data from fifo
            data = CPS_REG_READ(&pd->regs_base->IBI_DATA_FIFO);

            for (i = 0; i < bytes_to_read; ++i) {
                const uint16_t shift = (i * 8U);
                uint8_t one_byte = 0U;

                // parasoft-begin-suppress MISRA2012-RULE-12_2-2 RHS operand of shift will always be less than 32
                // because bytes_to_wite will never exceed RX_FIFO_WORD_WIDTH, maximum is 8U * 3U
                one_byte = (uint8_t)(data >> shift);
                // parasoft-end-suppress MISRA2012-RULE-12_2-2

                buf[bytes_xferred + i] = one_byte;
            }

            bytes_xferred += bytes_to_read;
        }

        result = 0U;
    }

    return (result);
}

/**
 * This function configures response of I3C conftroller to Hot Join
 * request.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] hj_response Should I3C controller ACK or NACK Hot Join request.
 * @param[in] hj_disable_set Should I3C controller disable slaves from requesting Hot Join.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_HjConfigureResponse(I3C_PrivData* pd, I3C_HjResponse hj_response,
                                 I3C_HjDisableSet hj_disable_set)
{
    uint32_t result;
    uint32_t reg_val;

    /*
     * Validate parameters passed function
     */
    if (I3C_HjConfigureResponseSF(pd, hj_response, hj_disable_set) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
        // Master response to Hot-Join request
        if (hj_response == I3C_HJ_RESP_ACK) {
            CPS_REG_WRITE(&pd->regs_base->CTRL, CPS_FLD_SET(MIPI__CTRL, HJ_ACK, reg_val));
        } else {
            CPS_REG_WRITE(&pd->regs_base->CTRL, CPS_FLD_CLEAR(MIPI__CTRL, HJ_ACK, reg_val));
        }

        reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
        // Check whether disable HJ interrupts
        if (hj_disable_set == I3C_HJ_REQ_SLAVE_NOT_DISABLE) {
            CPS_REG_WRITE(&pd->regs_base->CTRL, CPS_FLD_CLEAR(MIPI__CTRL, HJ_DISEC, reg_val));
        } else {
            CPS_REG_WRITE(&pd->regs_base->CTRL, CPS_FLD_SET(MIPI__CTRL, HJ_DISEC, reg_val));
        }
        result = 0U;
    }

    return (result);
}

/**
 * Returns next command skeleton.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] immediate Indicates if command should be immediate.
 * @param[in] rsbc Indicates if repeated-start should be enabled.
 */
static inline I3C_cmd_t volatile *get_next_cmd(I3C_PrivData *pd, bool immediate, bool rsbc)
{
    uint32_t ret_val = 0;
    volatile I3C_cmd_t* command = NULL;
    uint8_t cmd_id;
    assert(pd);

    if (!immediate) {
        /* In case of error this will set result to NULL. */
        ret_val = CLAppend(&(pd->cmd_list), &command);
        if (ret_val != 0U) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Command buffer full.\n");
        }
        else {
            // Get next ID
            cmd_id = get_next_cmd_id(pd);
            command->id = cmd_id;
            command->cmd = i3c_cmd_id((uint64_t) cmd_id);

            // Repeated start between commands
            if(rsbc) {
                command->cmd |= I3C_CMD_RSBC;
            }
        }
    } else {
        if (pd->imd_in_progress) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Immediate Command already in progress.\n");
        } else {
            pd->imd_in_progress = true;
            // Reset all imd_cmd fields
            (void)memset(&pd->imd_cmd, 0, sizeof(I3C_cmd_t));
            command = &pd->imd_cmd;
            // Get next ID
            cmd_id = get_next_cmd_id(pd);
            command->id = cmd_id;
            command->cmd = i3c_cmd_id((uint64_t) cmd_id);
        }
    }

    return (command);
}

/**
 * Returns number of queued Commands.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 if input parameters are invalid or if there are no queued Commands
 */
uint32_t I3C_CmdCount(const I3C_PrivData *pd)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdCountSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = 0U;
    } else {
        result = pd->cmd_fifo_used;
    }

    return (result);
}

/**
 * Returns maximum number of Commands that can be put in queue.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 if input parameters are invalid or if there are no queued Commands
 */
uint32_t I3C_CmdMaxCount(const I3C_PrivData *pd)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdMaxCountSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = 0U;
    } else {
        result = pd->cmd_fifo_size;
    }

    return (result);
}

/**
 * Clears Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @return EINVAL if input parameters are invalid
 * @return 0 on success
 */
uint32_t I3C_CmdClearAll(I3C_PrivData *pd)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdClearAllSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        pd->cmd_in_progress = false;
        pd->cmd_empty = true;
        pd->cmd_abort = false;
        pd->cmd_fifo_used = 0;
        pd->tx_fifo_used = 0;
        pd->rx_fifo_used = 0;
        // Indicate that TX command is not in progress
        pd->tx_cmd_in_progress = NULL;
        // Indicate that RX command is not in progress
        pd->rx_cmd_in_progress = NULL;

        result = 0U;
    }

    return (result);
}

/**
 * Prints info on I3C devices on the bus.
 * @param[in] pd Pointer to driver's private data object.
 * @return EINVAL if input parameters are invalid
 * @return 0 on success
 */
uint32_t I3C_DevPrint(const I3C_PrivData *pd) {

    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_DevPrintSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        print_devices(pd);
        result = 0U;
    }

    return (result);
}

/**
 * Executes payload load to HW fifo depending on DDR or SDR command.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 */
static void i3c_cmd_exec_load_payload(I3C_PrivData *pd, volatile I3C_cmd_t* cmd)
{

    if (cmd->payload_size != 0U) {
        // Write Payload
        if (((uint32_t)cmd->cmd & I3C_CMD_DDR_MODE) != 0U) {
            // Both, DDR read and DDR write, have payload to send
            ddr_write_tx_data(pd, cmd);
        } else if (cmd->is_wr_cmd) {
            sdr_write_tx_data(pd, cmd);
        } else {
            // All 'if ... else if' constructs shall be terminated with an 'else' statement
            // (MISRA2012-RULE-15_7-3)
        }
    }
}

/**
 * Validates if command can be executed.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 * @return 0 on success
 * @return EINVAL if Command is invalid for execution
 */
static uint32_t validate_cmd_exec(I3C_PrivData *pd, volatile I3C_cmd_t* cmd)
{
    volatile I3C_cmd_t * local_cmd = cmd;
    I3C_PrivData * local_pd = pd;
    uint32_t result;

    if (((local_pd->cmd_fifo_size - local_pd->cmd_fifo_used) > 0U) &&
        (
            // write command and tx fifo not busy by tresholded command
            ((local_cmd->is_wr_cmd == true) && ((local_pd->tx_cmd_in_progress == NULL) || (local_pd->tx_cmd_in_progress == local_cmd)))
            ||
            // read command and rx fifo not busy by tresholded command
            ((local_cmd->is_wr_cmd == false) && ((local_pd->rx_cmd_in_progress == NULL) || (local_pd->rx_cmd_in_progress == local_cmd)))
            ||
            // command already in fifo
            (local_cmd->bytes_fared == local_cmd->payload_size)
        ))
    {
        result = 0U;
    }
    else {
        result = EINVAL;
    }

    return (result);
}

/**
 * Writes commands to HW fifo.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
static uint32_t i3c_cmd_exec_execute_commands(I3C_PrivData *pd, volatile I3C_cmd_t* cmd)
{
    uint32_t result = 0U;
    uint32_t cmd0;
    uint32_t cmd1;

    /*
     * Validate parameters passed function
     */
    if (validate_cmd_exec(pd, cmd) == 0U) {
        cmd0 = (uint32_t)cmd->cmd;
        cmd1 = (uint32_t)(cmd->cmd >> 32);

        // Write Command
        CPS_REG_WRITE(&pd->regs_base->CMD1_FIFO, cmd1);
        CPS_REG_WRITE(&pd->regs_base->CMD0_FIFO, cmd0);
        cDbgMsg(DBG_GEN_MSG, DBG_HIVERB, "CMD written to HW, CCC: 0x%.2x, payload_size: %d, R/W: %d\n", ((uint8_t) cmd1), cmd->payload_size, cmd->is_wr_cmd);

        // lock next read command in case of large payload
        if ((cmd->is_wr_cmd == false) && (cmd->payload_size > 0U)) {
            pd->rx_cmd_in_progress = cmd;
        }

        if (((uint32_t)cmd->cmd & I3C_CMD_DDR_MODE) != 0U) {
            result = I3C_GetInstance()->manualCommandStart(pd);
            (void)I3C_GetInstance()->disableMcs(pd);
        }
        pd->cmd_fifo_used++;
        cmd->in_cmd_fifo = true;
        pd->cmd_fifo_empty = false;
    }

    return (result);
}

/**
 * Takes command which are not in HW fifo yet, calls payload load
 * and then writes command to hardware.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 * @param[in] cmd Pointer to I3C command structure.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
static uint32_t i3c_cmd_exec_execute(I3C_PrivData *pd, volatile I3C_cmd_t* cmd)
{
    uint32_t result = 0U;
    if ((!cmd->in_cmd_fifo) &&
        (get_cmd_fifo_free_space(pd) > 0U)) {
        // Write only first command's payload to tx fifo
        if (pd->tx_cmd_in_progress == NULL) {
            i3c_cmd_exec_load_payload(pd, cmd);
        }
        if (get_cmd_fifo_free_space(pd) > 0U) {
            result = i3c_cmd_exec_execute_commands(pd, cmd);
            if (result != 0U) {
                DbgMsg(DBG_GEN_MSG, DBG_CRIT, "%s, Error. Could not execute command.\n", __func__);
            }
        } else {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "%s, Error. Could not write payload of command.\n", __func__);
        }

    }
    return result;
}

/**
 * Executes all queued Commands.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if Commands are already being processed
 * @return EOPNOTSUPP if Command list is empty
 */
uint32_t I3C_CmdExec(I3C_PrivData *pd)
{
    uint32_t result = 0U;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdExecSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if (pd->cmd_in_progress) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Commands execution already in progress.\n");
        result = EBUSY;
    } else {
        uint16_t iter;
        for (iter = 0; iter < get_cmd_fifo_free_space(pd); ++iter) {
            /* Find command that has not been send yet. */
            size_t number_of_used_locations = CLGetNumberOfUsedLocations(&(pd->cmd_list));
            if (number_of_used_locations == 0U) {
                break;
            }
            result = CLTraverse(&(pd->cmd_list), pd, i3c_cmd_exec_execute);
        }
    }

    return (result);
}

/**
 * Writes payload of immediate command to IMD_DATA register.
 * @param[in] pd Pointer to I3C Core Driver's private data structure.
 */
static void cmd_immediate_write_payload(I3C_PrivData *pd)
{
    uint32_t data = 0;
    uint16_t i;

    for (i = 0; i < pd->imd_cmd.payload_size; ++i) {
        const uint16_t shift = i * 8U;

        // parasoft-begin-suppress MISRA2012-RULE-12_2-2 RHS operand of shift will always be less than 32
        // because maximum payload_size of immediate command is 4, assert in cmdExecImmediate
        if (((uint32_t)pd->imd_cmd.cmd & I3C_CMD_DDR_MODE) != 0U) {
            data |= ((uint32_t)pd->imd_cmd.payload_ddr[pd->imd_cmd.payload_iterator] << shift);
        }
        else {
            data |= ((uint32_t)pd->imd_cmd.payload_sdr[pd->imd_cmd.payload_iterator] << shift);
        }
        // parasoft-begin-suppress MISRA2012-RULE-12_2-2

        pd->imd_cmd.payload_iterator++;
    }

    // Write payload
    if (pd->imd_cmd.payload_size > 0U) {
        CPS_REG_WRITE(&pd->regs_base->IMD_DATA, data);
        pd->imd_cmd.bytes_fared = pd->imd_cmd.payload_size;
    }
}

/**
 * Executes Immediate Command.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdExecImmediate(I3C_PrivData *pd)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdExecImmediateSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        // Ensure that payload_size is less than 4 bytes
        assert(pd->imd_cmd.payload_size <= 4);

        if(pd->imd_cmd.is_wr_cmd == true) {
            // Write payload in case of write command
            cmd_immediate_write_payload(pd);
        }

        CPS_REG_WRITE(&pd->regs_base->IMD_CMD1, (uint32_t)(pd->imd_cmd.cmd >> 32));
        CPS_REG_WRITE(&pd->regs_base->IMD_CMD0, (uint32_t)pd->imd_cmd.cmd);

        result = 0U;
    }

    return (result);
}

/**
 * Enables Manual Command Start.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_EnableMcs(I3C_PrivData *pd)
{
    uint32_t result = 0U;
    /*
     * Validate parameters passed function
     */
    if(I3C_EnableMcsSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    }
    else {
        enable_mcs(pd);
    }
    return result;
}

/**
 * Disabled Manual Command Start.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if Commands are already being processed
 */
uint32_t I3C_DisableMcs(I3C_PrivData *pd)
{
    uint32_t result = 0U;
    /*
     * Validate parameters passed function
     */
    if(I3C_DisableMcsSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    }
    else {
        disable_mcs(pd);
    }
    return result;
}

/**
 * Executes commands stored in HW.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if Commands are already being processed
 */
uint32_t I3C_ManualCommandStart(I3C_PrivData *pd)
{
    uint32_t result = 0U;
    /*
     * Validate parameters passed function
     */
    if(I3C_ManualCommandStartSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    }
    else {
        manual_command_start(pd);
    }
    return result;
}

/**
 * Returns csrmode for CMD purpose.
 * @param[in] csr_data Pointer to I3C Csr Data structure.
 * @return 64-bit value of command
 */
static uint64_t i3c_get_csr_mode(const I3C_CsrData *csr_data)
{
    uint32_t csr_mode = 0U;
    uint32_t sbca = 0U;

    csr_mode = CPS_FLD_WRITE(MIPI__CMD1_FIFO, CCC_CSRADDR0, csr_mode,
                             (uint8_t) csr_data->csr);

    if (csr_data->csr16 != false) {
        csr_mode = CPS_FLD_WRITE(MIPI__CMD1_FIFO, CSRADDR1, sbca,
                                 (uint8_t)(csr_data->csr >> MIPI__CMD1_FIFO__CSRADDR1_SHIFT));
        sbca = CPS_FLD_SET(MIPI__CMD0_FIFO, SBCA, 1U);
    }

    // Set CMD bits
    return ((((uint64_t)csr_mode) << 32) | (uint64_t)sbca);
}

/* SDR/DDR/I2C functions section start */

/**
 * Adds SDR Mode private Read Command to the Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address from which data will be transferred.
 * @param[in] csr_data CSR information with CSR address and address lenght.
 * @param[in] payload_data Pointer to structure with payload information
 * @param[in] xmitMmode Transmission Mode.
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
static uint32_t i3c_process_cmd_add_priv_read(I3C_PrivData *pd, uint16_t da,
                                              const I3C_CsrData *csr_data,
                                              const I3C_PayloadData *payload_data,
                                              I3C_TransmitMode xmit_mode)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;
    I3C_DeviceDescriptor *dev;
    uint8_t dev_idx;

    /*
     * Extra byte is required for modes 00b (single CSR) and 11b (NCA)
     * to avoid invalid report of rd_abort interrupt.
     */
    uint32_t xtra_pl_byte;

    // Get device index
    dev_idx = get_dev_index(pd, da);

    if (dev_idx != 0U) {

        dev = &(pd->devs[dev_idx]);
        cmd = get_next_cmd(pd, false, false);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");
            result = EBUSY;
        } else {

            // Extra payload byte
            if (dev->legacyI2CDev) {
                xtra_pl_byte = 0;
            } else if ((xmit_mode == I3C_CMD_XMIT_MODE_SINGLE_CSR)
                       || (xmit_mode == I3C_CMD_XMIT_MODE_NCA)) {
                xtra_pl_byte = 1;
            } else {
                xtra_pl_byte = 0;
            }

            cmd->cmd |= (uint64_t) I3C_CMD_DIR_READ
                        | i3c_cmd_get_payload_length((uint64_t)payload_data->payloadSize + (uint64_t)xtra_pl_byte)
                        | i3c_cmd_dev_addr(da)
                        | i3c_get_csr_mode(csr_data)
                        | ((((uint64_t)xmit_mode) & 0x03U) << 27U);
            cmd->payload_sdr = payload_data->sdr_payload;
            cmd->payload_size = payload_data->payloadSize;
            cmd->bytes_fared = 0;
            cmd->in_cmd_fifo = false;
            cmd->is_wr_cmd = false;

            // I2C 10-bits addressing
            if (dev->legacyI2CDev && dev->i2c10bAddr) {
                cmd->cmd |= i3c_cmd_dev_addr_msb(((uint64_t)da) >> I2C_LVR_SA_MSB_SHIFT);
                // Set HAS_MSB bit
                cmd->cmd |= I3C_CMD_DEV_ADDR_HAS_MSB;
            }
        }
    }
    else {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Device with addr 0x%.2X not found!\n", da);
        result = EINVAL;
    }

    return (result);
}

/**
 * Adds SDR Mode private Read Command to the Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address from which data will be transferred.
 * @param[in] csr_data CSR information with CSR address and address lenght.
 * @param[in] payload_data Pointer to structure with payload information
 * @param[in] xmitMmode Transmission Mode.
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddPrivRead(I3C_PrivData *pd, uint16_t da,
                            const I3C_CsrData *csr_data,
                            const I3C_PayloadData *payload_data,
                            I3C_TransmitMode xmitMmode)
{
    uint32_t result;

    DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "Adding SDR read command.\n");

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddPrivReadSF(pd, csr_data, payload_data, xmitMmode) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if ((!pd->use_fifos) && (payload_data->payloadSize > RX_FIFO_WORD_WIDTH)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid payload size.\n");
        result = EINVAL;
    } else {
        result = i3c_process_cmd_add_priv_read(pd, da, csr_data,
                                               payload_data, xmitMmode);
    }

    return (result);
}

/**
 * Adds SDR Mode private Write Command to the Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address to which data will be transferred.
 * @param[in] csr_data CSR information with CSR address and address lenght.
 * @param[in] payload_data Pointer to structure with payload information
 * @param[in] xmitMmode Transmission Mode.
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
static uint32_t i3c_process_cmd_add_priv_write(I3C_PrivData *pd, uint16_t da,
                                               const I3C_CsrData *csr_data,
                                               const I3C_PayloadData *payload_data,
                                               I3C_TransmitMode xmit_mode)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;
    I3C_DeviceDescriptor *dev;
    uint8_t dev_idx;

    // Get device index
    dev_idx = get_dev_index(pd, da);

    if (dev_idx != 0U) {

        dev = &(pd->devs[dev_idx]);
        cmd = get_next_cmd(pd, false, false);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");
            result = EBUSY;
        } else {
            cmd->cmd |= I3C_CMD_DIR_WRITE
                        | i3c_cmd_dev_addr(da)
                        | i3c_cmd_get_payload_length((uint64_t)payload_data->payloadSize)
                        | i3c_get_csr_mode(csr_data)
                        | ((((uint64_t)xmit_mode) & 0x03U) << 27U);
            cmd->payload_sdr = payload_data->sdr_payload;
            cmd->payload_size = payload_data->payloadSize;
            cmd->bytes_fared = 0;
            cmd->in_cmd_fifo = false;
            cmd->is_wr_cmd = true;

            // I2C 10-bit addressing
            if (dev->legacyI2CDev && dev->i2c10bAddr) {
                cmd->cmd |= i3c_cmd_dev_addr_msb(((uint64_t)da) >> I2C_LVR_SA_MSB_SHIFT);
                // Set HAS_MSB bit
                cmd->cmd |= I3C_CMD_DEV_ADDR_HAS_MSB;
            }
        }
    }
    else {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Device with addr 0x%.2X not found!\n", da);
        result = EINVAL;
    }

    return (result);
}

/**
 * Adds SDR Mode private Write Command to the Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address to which data will be transferred.
 * @param[in] csr_data CSR information with CSR address and address lenght.
 * @param[in] payload_data Pointer to structure with payload information
 * @param[in] xmitMmode Transmission Mode.
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddPrivWrite(I3C_PrivData *pd, uint16_t da,
                             const I3C_CsrData *csr_data,
                             const I3C_PayloadData *payload_data,
                             I3C_TransmitMode xmitMmode)
{
    uint32_t result;

    DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "Adding SDR write command.\n");

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddPrivWriteSF(pd, csr_data, payload_data, xmitMmode) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if ((!pd->use_fifos) && (payload_data->payloadSize > TX_FIFO_WORD_WIDTH)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid payload size.\n");
        result = EINVAL;
    } else {
        result = i3c_process_cmd_add_priv_write(pd, da, csr_data, payload_data, xmitMmode);
    }

    return (result);
}

/**
 * Adds Legacy I2C SDR Mode private Read Command to the Commands
 * queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address from which data will be transferred.
 * @param[in] payload Pointer to buffer containing payload to be sent.
 * @param[in] payloadSize Size of the payload [B].
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddPrivI2CRead(I3C_PrivData *pd, uint16_t da,
                               uint8_t *payload, uint16_t payloadSize)
{
    uint32_t result;
    I3C_CsrData csr_data = {
        .csr = 0U,
        .csr16 = false
    };

    I3C_PayloadData payload_data = {
        .sdr_payload = payload,
        .ddr_payload = NULL,
        .payloadSize = payloadSize
    };

    DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "Adding Legacy I2C read command.\n");

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddPrivI2CReadSF(pd, payload) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        result = I3C_GetInstance()->cmdAddPrivRead(
            pd, da, &csr_data, &payload_data, I3C_CMD_XMIT_MODE_NCA
            );
    }

    return (result);
}

/**
 * Adds Legacy I2C SDR Mode private Write Command to the Commands
 * queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address to which data will be transferred.
 * @param[in] payload Pointer to buffer containing payload to be sent.
 * @param[in] payloadSize Size of the payload [B].
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddPrivI2CWrite(I3C_PrivData *pd, uint16_t da,
                                uint8_t *payload, uint16_t payloadSize)
{
    uint32_t result;
    I3C_CsrData csr_data = {
        .csr = 0U,
        .csr16 = false
    };

    I3C_PayloadData payload_data = {
        .sdr_payload = payload,
        .ddr_payload = NULL,
        .payloadSize = payloadSize
    };

    DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "Adding Legacy I2C write command.\n");

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddPrivI2CWriteSF(pd, payload) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        result = I3C_GetInstance()->cmdAddPrivWrite(
            pd, da,&csr_data, &payload_data, I3C_CMD_XMIT_MODE_NCA
            );
    }

    return (result);
}

/**
 * Adds DDR Mode private Write Command to the Commands queue. I3C
 * Core Driver will automatically calculate CRC, Preambles and Parity
 * Bits for the DDR Command and its Payload.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address to which data will be transferred.
 * @param[in] command DDR Command.
 * @param[in] payload Pointer to payload data.
 * @param[in] payload_size Size of payload.
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
static uint32_t i3c_process_cmd_add_ddr_write(I3C_PrivData *pd, uint8_t da,
                                              uint8_t command, uint16_t *payload, uint16_t payload_size)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;
    I3C_DeviceDescriptor *dev;
    uint8_t dev_idx;

    // Get device index
    dev_idx = get_dev_index(pd, da);

    if (dev_idx != 0U) {

        dev = &(pd->devs[dev_idx]);
        if (dev->legacyI2CDev) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Legacy I2C Devices doesn't support the DDR transfers.\n");
            result = EINVAL;
        } else if (!dev->isActive) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Device with DA: 0x%.2X is not active..\n", da);
            result = EINVAL;
        } else {
            // Get next command
            cmd = get_next_cmd(pd, false, false);
            if (cmd == NULL) {
                DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");
                result = EBUSY;
            } else {

                cmd->cmd |= (I3C_CMD_DIR_WRITE
                             | i3c_cmd_dev_addr(da)
                             // DDR transfers have some protocol overhead
                             | i3c_cmd_get_payload_length(((uint64_t)payload_size / 2U) + DDR_TX_PAYLOAD_OVERHEAD)
                             | I3C_CMD_DDR_MODE);
                cmd->payload_ddr = payload;
                cmd->payload_size = payload_size;
                cmd->bytes_fared = 0;
                cmd->in_cmd_fifo = false;
                cmd->is_wr_cmd = true;
                cmd->priv = 0;

                // Set command code in private data for CRC calculation purpose
                cmd->priv = cmd_priv_ddr_set_cmd(cmd->priv, command);
                // Set DA in private data for CRC calculation purpose
                cmd->priv = cmd_priv_ddr_set_dev_addr(cmd->priv, da);
            }
        }
    } else {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Device with addr 0x%.2X not found!\n", da);
        result = EINVAL;
    }

    return (result);
}

/**
 * Adds DDR Mode private Write Command to the Commands queue. I3C
 * Core Driver will automatically calculate CRC, Preambles and Parity
 * Bits for the DDR Command and its Payload.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address to which data will be transferred.
 * @param[in] command DDR Command.
 * @param[in] hdrMode Specifies which one of the HDR modes to use.
 * @param[in] payload_data Pointer to structure with payload information
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddDdrWrite(I3C_PrivData *pd, uint8_t da,
                            uint8_t command, I3C_HdrMode hdrMode,
                            I3C_PayloadData *payload_data)
{
    uint32_t result;

    DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "Adding DDR write command.\n");

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddDdrWriteSF(pd, hdrMode, payload_data) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        uint16_t payloadSize = payload_data->payloadSize;
        /* Transform that to bytes (including preamble and crc words)
           -- final number of bytes that will be pushed through a
           FIFO */
        payloadSize = (payloadSize * TX_FIFO_WORD_WIDTH) +
                      (DDR_TX_PAYLOAD_OVERHEAD * TX_FIFO_WORD_WIDTH);

        if ((!pd->use_fifos) && (payloadSize > TX_FIFO_WORD_WIDTH)) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid payload size.\n");
            result = EINVAL;
        } else if (command > I3C_HDR_DDR_TX_CMD_CODE_MAX) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid DDR Write Command.\n");
            result = EINVAL;
        } else {

            // Prepare DDR write command
            result = i3c_process_cmd_add_ddr_write(pd, da, command,
                                                   payload_data->ddr_payload, payloadSize);

        }
    }

    return (result);
}

/**
 * Adds DDR Mode private Read Command to the Commands queue. I3C Core
 * Driver will automatically check CRC, Preambles and Parity Bits for
 * the DDR Command and its Payload.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address from which data will be transferred.
 * @param[in] command DDR Command.
 * @param[in] payload Pointer to payload data
 * @param[in] max_payload_size Maximum payload size
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
static uint32_t i3c_process_cmd_add_ddr_read(I3C_PrivData *pd, uint8_t da,
                                             uint8_t command, uint16_t *payload, uint16_t max_payload_size_in)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;
    I3C_DeviceDescriptor *dev;
    uint16_t payload_size;
    uint8_t dev_idx;

    // Get device index
    dev_idx = get_dev_index(pd, da);

    if (dev_idx != 0U) {

        dev = &(pd->devs[dev_idx]);
        if (dev->legacyI2CDev) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Legacy I2C Devices doesn't support the DDR transfers.\n");
            result = EINVAL;
        } else if (!dev->isActive) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Device with DA: 0x%.2X is not active..\n", da);
            result = EINVAL;
        } else {
            // Get next command
            cmd = get_next_cmd(pd, false, false);
            if (cmd == NULL) {
                DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");
                result = EBUSY;
            } else {

                /* Transform that to bytes (including preamble and crc words)
                   -- final number of bytes that will be pushed through a
                   FIFO */
                payload_size = (max_payload_size_in * RX_FIFO_WORD_WIDTH) +
                               // DDR transfers have some protocol overhead
                               (DDR_RX_PAYLOAD_OVERHEAD * TX_FIFO_WORD_WIDTH);
                cmd->cmd |= (I3C_CMD_DIR_READ
                             | i3c_cmd_dev_addr(da)
                             | i3c_cmd_get_payload_length(1U) // command word only
                             | I3C_CMD_DDR_MODE);
                cmd->payload_ddr = payload;
                cmd->payload_size = payload_size;
                cmd->bytes_fared = 0;
                cmd->in_cmd_fifo = false;
                cmd->is_wr_cmd = false;
                cmd->priv = 0;
                // Reset CRC value
                cmd->ddr_crc = 0;

                // Set command code in private data for CRC calculation purpose
                cmd->priv = cmd_priv_ddr_set_cmd(cmd->priv, command);
                // Set DA in private data for CRC calculation purpose
                cmd->priv = cmd_priv_ddr_set_dev_addr(cmd->priv, da);
            }
        }
    } else {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Device with addr 0x%.2X not found!\n", da);
        result = EINVAL;
    }

    return (result);
}

/**
 * Adds DDR Mode private Read Command to the Commands queue. I3C Core
 * Driver will automatically check CRC, Preambles and Parity Bits for
 * the DDR Command and its Payload.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address from which data will be transferred.
 * @param[in] command DDR Command.
 * @param[in] hdrMode Specifies which one of the HDR modes to use.
 * @param[in] payload_data Pointer to structure with payload information
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddDdrRead(I3C_PrivData *pd, uint8_t da,
                           uint8_t command, I3C_HdrMode hdrMode,
                           I3C_PayloadData *payload_data)
{
    uint32_t result;

    DbgMsg(DBG_GEN_MSG, DBG_HIVERB, "Adding DDR read command.\n");

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddDdrReadSF(pd, hdrMode, payload_data) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if ((!pd->use_fifos) && (payload_data->payloadSize > RX_FIFO_WORD_WIDTH)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid payload size.\n");
        result = EINVAL;
    } else if ((command > I3C_HDR_DDR_RX_CMD_CODE_MAX) ||
               (command < I3C_HDR_DDR_RX_CMD_CODE_MIN)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid DDR Read Command.\n");
        result = EINVAL;
    } else {
        // Process DDR read command
        result = i3c_process_cmd_add_ddr_read(pd, da, command,
                                              payload_data->ddr_payload, payload_data->payloadSize);
    }

    return (result);
}

/* SDR/DDR/I2C functions section end */

/* CCC functions section start */

/**
 * Validates slave events mask
 * @param[in] slave_events_mask Mask
 * @return 1U if valid
 * @return 0U if invalid
 */
static uint32_t validate_slave_event_mask (uint8_t slave_events_mask)
{
    uint32_t result;

    uint8_t validation_template =
        (uint8_t)((uint8_t)I3C_SLAVE_EVENT_INTERRUPT_REQS
                  | (uint8_t)I3C_SLAVE_EVENT_MASTERSHIP_REQ
                  | (uint8_t)I3C_SLAVE_EVENT_HOT_JOIN);

    if ((slave_events_mask & ~validation_template) != 0U) {
        result = 1U;
    } else {
        result = 0U;
    }

    return result;
}

/**
 * Executes command if it is immediate
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] immediate Indicates immediate command
 * @return 0 If success
 */
static uint32_t start_immediate_cmd(I3C_PrivData *pd, bool immediate)
{
    uint32_t result = 0;
    if (immediate == 1) {
        result = I3C_GetInstance()->cmdExecImmediate(pd);
    }
    return (result);
}

/**
 * Adds to queue Command to that sends message to I3C Device (or to
 * all I3C Devices in case of a broadcast) which event interrupts
 * should be enabled or disabled.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] eventsMask Mask of Events to be enabled or disabled.
 * @param[in] enable If true, specified events will be enabled; if false, specified events will be disabled.
 * @param[in] devAddr Address of the I3C Device in case of Direct Message.
 * @param[in] tx_params Transmission parameters - command descriptors
 * @return 0 on success
 * @return EPROTO if slave event mask is not valid
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 */
uint32_t I3C_CmdAddSetSlaveEvents(I3C_PrivData *pd,
                                  uint8_t eventsMask, bool enable,
                                  uint8_t devAddr, const I3C_TransmissionParameters *tx_params)
{
    uint32_t result;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddSetSlaveEventsSF(pd, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");
            result = EBUSY;
        } else {

            if (validate_slave_event_mask(eventsMask) != 0U) {
                DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Slave event mask is not valid.\n");
                result = EPROTO;
            } else {

                if (tx_params->broadcast) {
                    // Broadcast command
                    cmd->cmd |= enable
                                ? i3c_cmd_ccc((uint64_t)I3C_CCC_ENEC_BC)
                                : i3c_cmd_ccc((uint64_t)I3C_CCC_DISEC_BC);
                } else {
                    // Direct command
                    cmd->cmd |= enable
                                ? i3c_cmd_ccc((uint64_t)I3C_CCC_ENEC_DC)
                                : i3c_cmd_ccc((uint64_t)I3C_CCC_DISEC_DC);
                    cmd->cmd |= i3c_cmd_dev_addr(devAddr);
                }

                cmd->cmd |= i3c_cmd_get_payload_length(1U);
                cmd->payload_size = 1;
                cmd->payload_sdr = (volatile uint8_t *)&cmd->priv;
                cmd->payload_sdr[0] = eventsMask;
                cmd->is_wr_cmd = true;

                // Start command if it is immediate
                result = start_immediate_cmd(pd, tx_params->immediate);

            }
        }
    }
    return (result);
}

/**
 * Creates Enter Activity State Command to be sent to the I3C Slaves.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] state Activity State value.
 * @param[in] devAddr Address of the I3C Device in case of Direct Message.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 */
uint32_t I3C_CmdAddEnterActivityState(I3C_PrivData *                    pd,
                                      I3C_ActivityState                 state,
                                      uint8_t                           devAddr,
                                      const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddEnterActivityStateSF(pd,
                                       state, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            if (tx_params->broadcast) {
                // Broadcast command
                cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_ENTAS0_BC + (uint64_t)state);
            } else {
                // Direct command
                cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_ENTAS0_DC + (uint64_t)state);
                cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            }

            cmd->payload_size = 0U;
            cmd->payload_sdr = NULL;
            cmd->payload_ddr = NULL;
            cmd->is_wr_cmd = false;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);
        }
    }
    return (result);
}

/**
 * This function adds CCC for Dynamic Address Assignment process to
 * the Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] devAddr Address of the I3C Device in case of Direct Command. This parameter is ignored in case of Broadcast Command.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddResetDaa(I3C_PrivData *                    pd,
                            uint8_t                           devAddr,
                            const I3C_TransmissionParameters *tx_params)
{
    uint32_t result;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddResetDaaSF(pd, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            if (tx_params->broadcast) {
                // Broadcast command
                cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_RSTDAA_BC);
            } else {
                // Direct command
                cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_RSTDAA_DC)
                            | i3c_cmd_dev_addr(devAddr);
            }

            cmd->payload_sdr = NULL;
            cmd->payload_ddr = NULL;
            cmd->payload_size = 0;
            cmd->is_wr_cmd = false;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);
        }
    }
    return (result);
}

/**
 * This function adds CCC for Dynamic Address Assignment process to
 * the Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddEnterDaa(I3C_PrivData *pd, const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddEnterDaaSF(pd, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            // ENTDAA broadcast message
            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_ENTDAA_BC);
            cmd->payload_sdr = NULL;
            cmd->payload_ddr = NULL;
            cmd->payload_size = 0;
            cmd->bytes_fared = 0;
            cmd->in_cmd_fifo = false;
            cmd->is_wr_cmd = false;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }
    return (result);
}

/**
 * This function adds CCC for setting the Maximum Write Length for
 * all or specific device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] length Maximum Write Length to be set.
 * @param[in] devAddr Address of the I3C Device in case of Direct Message.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddSetMaxWriteLength(I3C_PrivData *                    pd,
                                     uint16_t                          length,
                                     uint8_t                           devAddr,
                                     const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0;
    volatile I3C_cmd_t *cmd;

    // Ensure that size of length is greater or equal to 2
    assert(sizeof(length) >= 2);

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddSetMaxWriteLengthSF(pd, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            if (tx_params->broadcast) {
                // Broadcast command
                cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_SETMWL_BC);
            } else {
                // Direct command
                cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_SETMWL_DC);
                cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            }

            cmd->cmd |= i3c_cmd_get_payload_length(2U);
            cmd->payload_size = 2;
            cmd->payload_sdr = (volatile uint8_t *)&cmd->priv;
            cmd->payload_sdr[0] = (uint8_t)(length >> 8);
            cmd->payload_sdr[1] = (uint8_t)length;
            cmd->bytes_fared = 0;
            cmd->in_cmd_fifo = false;
            cmd->is_wr_cmd = true;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);
        }
    }
    return (result);
}

/**
 * This function adds CCC for obtaining the Maximum Write Length
 * value for a specific device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] length Pointer to 16-bit variable to which Maximum Write Length will be written.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetMaxWriteLength(I3C_PrivData *                    pd,
                                     uint16_t *                        length,
                                     uint8_t                           devAddr,
                                     const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    // Ensure that size of length is greater or equal 2
    assert(sizeof(*length) >= 2);

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddGetMaxWriteLengthSF(pd,
                                      length, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_GETMWL_DC);
            cmd->cmd |= i3c_cmd_get_payload_length(2U);
            cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            cmd->cmd |= I3C_CMD_DIR_READ;
            cmd->payload_size = 2;
            cmd->payload_sdr = (uint8_t *)length;
            cmd->is_wr_cmd = false;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }

    return (result);
}

/**
 * This function adds CCC for setting the Maximum Read Length for all
 * or specific device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] max_read_length Maximum Read Length to be set.
 * @param[in] length_size Number of bytes required to store Maximum Read Length. Alloved values are 2 or 3.
 * @param[in] devAddr Address of the I3C Device in case of Direct Message.
 * @param[in] tx_params Transmission parameters - command descriptors
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddSetMaxReadLength(I3C_PrivData *pd,
                                    I3C_MaxReadLength max_read_length,
                                    uint8_t length_size,
                                    uint8_t devAddr, const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddSetMaxReadLengthSF(pd, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");
            result = EBUSY;
        } else {

            if (tx_params->broadcast) {
                // Broadcast command
                cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_SETMRL_BC);
            } else {
                // Direct command
                cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_SETMRL_DC);
                cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            }

            cmd->cmd |= i3c_cmd_get_payload_length((uint64_t)length_size);
            cmd->payload_size = length_size;
            cmd->payload_sdr = (volatile uint8_t *)&cmd->priv;

            // Check if read length contains IBI payload length
            if (length_size == 2U) {
                cmd->payload_sdr[0] = (uint8_t)(max_read_length.payload_length >> 8);
                cmd->payload_sdr[1] = (uint8_t)max_read_length.payload_length;
            } else {
                cmd->payload_sdr[0] = (uint8_t)(max_read_length.payload_length >> 8);
                cmd->payload_sdr[1] = (uint8_t)max_read_length.payload_length;
                cmd->payload_sdr[2] = (uint8_t)max_read_length.ibi_payload_length;
            }

            cmd->is_wr_cmd = true;
            // Start command if it is immediate
            result =  start_immediate_cmd(pd, tx_params->immediate);
        }
    }
    return (result);
}

/**
 * This function adds CCC for obtaining the Maximum Read Length value
 * for a specific device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] max_read_length Pointer to a structure where Maximum Read Length.
 * @param[in] length_size Number of bytes required to store Maximum Read Length. Alloved values are 2 or 3.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetMaxReadLength(I3C_PrivData *                    pd,
                                    I3C_MaxReadLength *               max_read_length,
                                    uint8_t                           length_size,
                                    uint8_t                           devAddr,
                                    const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddGetMaxReadLengthSF(pd,
                                     max_read_length, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        // Get next command
        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_GETMRL_DC);
            cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            cmd->cmd |= I3C_CMD_DIR_READ;
            cmd->cmd |= i3c_cmd_get_payload_length((uint64_t)length_size);
            cmd->payload_size = length_size;
            cmd->payload_sdr = (uint8_t *)max_read_length;
            cmd->is_wr_cmd = false;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }

    return (result);
}

/**
 * This function adds CCC for obtaining the Maximum Data Speed (SCL
 * Frequency of the slave)
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] max_data_speed Pointer to structure with max data speeds.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetMaxDataSpeed(I3C_PrivData *                    pd,
                                   I3C_MaxDataSpeed *                max_data_speed,
                                   uint8_t                           devAddr,
                                   const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddGetMaxDataSpeedSF(pd,
                                    max_data_speed,
                                    tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");
            result = EBUSY;
        } else {

            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_GETMXDS_DC);
            cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            cmd->cmd |= I3C_CMD_DIR_READ;
            // Size of GETMXDS command payload is 2
            cmd->cmd |= i3c_cmd_get_payload_length(2U);
            cmd->payload_size = 2U;
            cmd->payload_sdr = (uint8_t *)max_data_speed;
            cmd->is_wr_cmd = false;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }

    return (result);
}

/**
 * This function gets information about Slaves present on the bus.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] slaveDescs Pointer the structure of Slave descriptors. Memory will be written with slave count and slave data.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_GetSlavesList(const I3C_PrivData *  pd,
                           I3C_SlaveDescriptors *slaveDescs)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_GetSlavesListSF(pd, slaveDescs) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        get_slave_descriptors(pd, slaveDescs);
        result = 0;
    }

    return (result);
}

/**
 * This function adds CCC for informing Secondary Masters about
 * Slaves present on the bus.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] slaveDescs Pointer the structure of Slave descriptors.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddDefineSlavesList(I3C_PrivData *                    pd,
                                    I3C_SlaveDescriptors *            slaveDescs,
                                    const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddDefineSlavesListSF(pd,
                                     slaveDescs, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");
            result = EBUSY;
        } else {

            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_DEFSLVS_BC);
            cmd->cmd |= I3C_CMD_DIR_WRITE;
            /* count master in */
            /* +1 for slave number */
            cmd->payload_size = (uint16_t)((( (uint16_t)(slaveDescs->slave_count) + 1U) * (uint16_t)sizeof(I3C_SlaveDescriptor)) + 1U);
            cmd->cmd |= i3c_cmd_get_payload_length((uint64_t)(cmd->payload_size));
            cmd->payload_sdr = (volatile uint8_t *)(slaveDescs);
            cmd->is_wr_cmd = true;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }

    return (result);
}

/**
 * Prepares CCC broadcast Command for all I3C Devices that the Master
 * is entering specific Test Mode.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] mode Test mode code. 0x00 - Exit Test Mode. 0x01 - Vendor Test Mode. 0x02 - General Test Mode. 0x03-0x0F - MIPI Reserved. 0x10-0xFF - Vendor Definable.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddEnterTestMode(I3C_PrivData *pd, uint8_t mode,
                                 const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddEnterTestModeSF(pd, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_ENTTM_BC);
            // Size of ENTTM command payload is 1
            cmd->cmd |= i3c_cmd_get_payload_length(1U);
            cmd->payload_size = 1;
            cmd->payload_sdr = (volatile uint8_t *)&cmd->priv;
            cmd->payload_sdr[0] = (uint8_t)mode;
            cmd->is_wr_cmd = true;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }

    return (result);
}

/**
 * Sends broadcast message to all I3C Devices that the Bus is being
 * switched over to the indicated HDR Mode.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] mode
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddEnterHdrMode(I3C_PrivData *                    pd,
                                I3C_HdrMode                       mode,
                                const I3C_TransmissionParameters* tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddEnterHdrModeSF(pd, mode, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            (void)I3C_GetInstance()->enableMcs(pd);
            // Send ENTHDRx broadcast message
            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_ENTHDR0_BC + (uint64_t)mode);
            cmd->payload_size = 0;
            cmd->payload_sdr = NULL;
            cmd->payload_ddr = NULL;
            cmd->is_wr_cmd = true;
            cmd->bytes_fared = 0;
            cmd->in_cmd_fifo = false;
            cmd->priv = 0;
            cmd->ddr_crc = 0;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }

    return (result);
}

/**
 * Prepares SETDASA command
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] pd Pointer to I3C command object.
 * @param[in] address Dynamic Address to be set.
 */
static volatile I3C_cmd_t * prepare_cmd_set_da_from_sa(I3C_PrivData *pd, volatile I3C_cmd_t *cmd, uint8_t address, uint8_t devAddr)
{
    uint8_t cmd_id = get_next_cmd_id(pd);
    cmd->id = cmd_id;
    cmd->cmd = i3c_cmd_id((uint64_t) cmd_id);
    cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_SETDASA_DC);
    cmd->cmd |= i3c_cmd_dev_addr(devAddr);
    // Pyload size of SETDASA is 1
    cmd->cmd |= i3c_cmd_get_payload_length(1U);
    cmd->payload_size = 1;
    cmd->payload_sdr = (volatile uint8_t *)&cmd->priv;
    // New device address as 0 byte of payload
    cmd->payload_sdr[0] = i3c_dev_addr_with_parity((uint8_t)address);
    cmd->bytes_fared = 0;
    cmd->in_cmd_fifo = false;
    cmd->is_wr_cmd = true;
    return (cmd);
}
/**
 * Processes SETDASA CCC command
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] address Dynamic Address to be set.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
static uint32_t i3c_process_cmd_setdasa(I3C_PrivData *                    pd,
                                        uint8_t                           address,
                                        uint8_t                           da,
                                        const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;
    uint8_t dev_idx;
    uint32_t dev_active;

    // Get device index
    dev_idx = get_dev_index(pd, da);

    if (dev_idx != 0U) {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");
            result = EBUSY;
        } else {
            cmd = prepare_cmd_set_da_from_sa(pd, cmd, address, da);

            dev_active = CPS_REG_READ(&pd->regs_base->DEVS_CTRL);

            // parasoft-begin-suppress MISRA2012-RULE-12_2-2 RHS operand of shift will always be less than 32
            // because num_devs is checked against too big values, also CONF_STATUS0.devs_num
            // is encoded on 4 bits, it means that max value of this field is 15
            if ((dev_active & ((uint32_t)1U << dev_idx)) == 0U) {
                dev_active |= ((uint32_t)1U << dev_idx);
                // Write active device mask
                CPS_REG_WRITE(&pd->regs_base->DEVS_CTRL, dev_active);
                pd->devs_active = (uint16_t) dev_active;
            }
            // parasoft-end-suppress MISRA2012-RULE-12_2-2
            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);
        }
    }
    else {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to find Slave with DA %d\n", da);
        result = EINVAL;
    }

    return result;
}

/**
 * Sends Command that addresses specific Device with Dynamic Address
 * using its I2C Address.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] address Dynamic Address to be set.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdSetDaFromSa(I3C_PrivData *                    pd,
                            uint8_t                           address,
                            uint8_t                           devAddr,
                            const I3C_TransmissionParameters *tx_params)
{
    uint32_t result;
    /*
     * Validate parameters passed function
     */
    if (I3C_CmdSetDaFromSaSF(pd, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        result = i3c_process_cmd_setdasa(pd, address, devAddr, tx_params);
    }
    return (result);
}

/**
 * Sends Command that sets a new Dynamic Address for specific I3C
 * Slave Device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Dynamic Address to be set.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddSetNewDa(I3C_PrivData *                    pd,
                            uint8_t                           da,
                            uint8_t                           devAddr,
                            const I3C_TransmissionParameters *tx_params)
{
    uint32_t result;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddSetNewDaSF(pd, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");
            result = EBUSY;
        } else {

            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_SETNEWDA_DC);
            cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            // Payload size of SETNEWDA command is 1
            cmd->cmd |= i3c_cmd_get_payload_length(1U);
            cmd->payload_size = 1;
            cmd->payload_sdr = (volatile uint8_t *)&cmd->priv;
            cmd->payload_sdr[0] = i3c_dev_addr_with_parity((uint8_t)da);
            cmd->is_wr_cmd = true;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);
        }
    }
    return (result);
}

/**
 * This function adds CCC for obtaining provisional ID of device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buff Pointer to variable to which 48-bit Provisional ID will be written.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetProvisionalId(I3C_PrivData *                    pd,
                                    uint8_t *                         buff,
                                    uint8_t                           devAddr,
                                    const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddGetProvisionalIdSF(pd, buff, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_GETPID_DC);
            cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            cmd->cmd |= I3C_CMD_DIR_READ;
            // Payload size of GETPID command is 6
            cmd->cmd |= i3c_cmd_get_payload_length(6U);
            cmd->payload_size = 6;
            cmd->payload_sdr = (volatile uint8_t *)buff;
            cmd->is_wr_cmd = false;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }

    return (result);
}

/**
 * This function adds CCC for obtaining the Bus Characteristics
 * Register.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] bcr Pointer to variable to which 8-bit DCR value will be written.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetBcr(I3C_PrivData *                    pd,
                          uint8_t *                         bcr,
                          uint8_t                           devAddr,
                          const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddGetBcrSF(pd, bcr, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_GETBCR_DC);
            cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            cmd->cmd |= I3C_CMD_DIR_READ;
            // Payload size of GETBCR command is 1
            cmd->cmd |= i3c_cmd_get_payload_length(1U);
            cmd->payload_size = 1;
            cmd->payload_sdr = bcr;
            cmd->bytes_fared = 0;
            cmd->in_cmd_fifo = false;
            cmd->is_wr_cmd = false;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }

    return (result);
}

/**
 * This function adds CCC for obtaining the Device Characteristics
 * Register.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] dcr Pointer to variable to which 8-bit BCR value will be written.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetDcr(I3C_PrivData *                    pd,
                          uint8_t *                         dcr,
                          uint8_t                           devAddr,
                          const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddGetDcrSF(pd, dcr, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_GETDCR_DC);
            cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            cmd->cmd |= I3C_CMD_DIR_READ;
            // Payload size of GETDCR command is 1
            cmd->cmd |= i3c_cmd_get_payload_length(1U);
            cmd->payload_size = 1;
            cmd->payload_sdr = dcr;
            cmd->is_wr_cmd = false;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }

    return (result);
}

/**
 * This function adds CCC for obtaining status of the I3C Device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] status Pointer to variable to which 16-bit status value will be written.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetStatus(I3C_PrivData *                    pd,
                             uint16_t *                        status,
                             uint8_t                           devAddr,
                             const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddGetStatusSF(pd, status, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_GETSTATUS_DC);
            cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            cmd->cmd |= I3C_CMD_DIR_READ;
            // Payload size of GETSTATUS command is 2
            cmd->cmd |= i3c_cmd_get_payload_length(2U);
            cmd->payload_size = 2;
            cmd->payload_sdr = (uint8_t *)status;
            cmd->is_wr_cmd = false;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }

    return (result);
}

/**
 * This function adds CCC for granting Mastership.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] recData Pointer to data received from device that GETACCMST CCC is sent to. After command execution this should hold address of receiving device.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetAccMst(I3C_PrivData *                    pd,
                             uint8_t *                         recData,
                             uint8_t                           devAddr,
                             const I3C_TransmissionParameters *tx_params)
{
    uint32_t result = 0U;
    volatile I3C_cmd_t *cmd;

    /*
     * Validate parameters passed function
     */
    if (I3C_CmdAddGetAccMstSF(pd, recData, tx_params) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        cmd = get_next_cmd(pd, tx_params->immediate, tx_params->rsbc);
        if (cmd == NULL) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Failed to add Command.\n");

            result = EBUSY;
        } else {

            cmd->cmd |= i3c_cmd_ccc((uint64_t)I3C_CCC_GETACCMST_DC);
            cmd->cmd |= i3c_cmd_dev_addr(devAddr);
            cmd->cmd |= I3C_CMD_DIR_READ;
            // Payload size of GETACCMST command is 1
            cmd->cmd |= i3c_cmd_get_payload_length(1U);
            cmd->payload_size = 1;
            cmd->payload_sdr = recData;
            cmd->is_wr_cmd = false;

            // Start command if it is immediate
            result = start_immediate_cmd(pd, tx_params->immediate);

        }
    }
    return (result);
}

/**
 * Checks controller operation mode
 * @param[in] pd Pointer to driver's private data object.
 * @param[out] opMode Pointer to OperationMode structure.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CheckOperationMode(const I3C_PrivData* pd, I3C_OperationMode* opMode)
{
    uint32_t result = 0U;
    /*
     * Validate parameters passed function
     */
    if (I3C_CheckOperationModeSF(pd, opMode) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        uint32_t reg_val = CPS_REG_READ(&pd->regs_base->MST_STATUS0);
        if (CPS_FLD_READ(MIPI__MST_STATUS0, OP_MODE, reg_val) == 0U) {
            *opMode = I3C_OP_MODE_SLAVE;
        }
        else {
            *opMode = I3C_OP_MODE_MASTER;
        }
    }
    return result;
}
/* Master in Slave mode */

/**
 * Configures Hardware and Software to operate in Slave mode.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] slaveInterruptConfig Pointer to slave mode interrupt configuration.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SlaveModeConfigure(I3C_PrivData *                  pd,
                                const I3C_SlaveInterruptConfig *slaveInterruptConfig)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_SlaveModeConfigureSF(pd, slaveInterruptConfig) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {

        //Disable all slave interrupts
        CPS_REG_WRITE(&pd->regs_base->SLV_IDR, 0xFFFFFFFFU);

        pd->slaveInterruptConfig = *slaveInterruptConfig;
        result = I3C_GetInstance()->configureSlaveInterrupts(pd, slaveInterruptConfig);
    }

    return (result);
}

static void i3cs_config_transfer_intrrupts(uint32_t*                       reg_val,
                                           const I3C_SlaveInterruptConfig* interrupt_config) {

    if (true == interrupt_config->sdrWrComplete) {
        //enable sdrWrComplete interrupt
        *reg_val |= MIPI__SLV_IER__SDR_WR_COMP_MASK;
    }

    if (true ==interrupt_config->sdrRdComplete) {
        //enable sdrRdComplete interrupt
        *reg_val |= MIPI__SLV_IER__SDR_RD_COMP_MASK;
    }

    if (true ==interrupt_config->ddrWrComplete) {
        //enable ddrWrComplete interrupt
        *reg_val |= MIPI__SLV_IER__DDR_WR_COMP_MASK;
    }

    if (true ==interrupt_config->ddrRdComplete) {
        //enable ddrRdComplete interrupt
        *reg_val |= MIPI__SLV_IER__DDR_RD_COMP_MASK;
    }

}

static void i3cs_config_fifo_intrrupts(uint32_t*                       reg_val,
                                       const I3C_SlaveInterruptConfig* interrupt_config) {

    if (true ==interrupt_config->sdrTxDataFifoOverflow) {
        //enable sdrTxDataFifoOverflow interrupt
        *reg_val |= MIPI__SLV_IER__SDR_TX_OVF_MASK;
    }

    if (true ==interrupt_config->sdrRxDataFifoUnderflow) {
        //enable sdrRxDataFifoUnderflow interrupt
        *reg_val |= MIPI__SLV_IER__SDR_RX_UNF_MASK;
    }

    if (true ==interrupt_config->ddrTxDataFifoOverflow) {
        //enable ddrTxDataFifoOverflow interrupt
        *reg_val |= MIPI__SLV_IER__DDR_TX_OVF_MASK;
    }

    if (true ==interrupt_config->ddrRxDataFifoUnderflow) {
        //enable ddrRxDataFifoUnderflow interrupt
        *reg_val |= MIPI__SLV_IER__DDR_RX_UNF_MASK;
    }

}

static void i3cs_config_thr_intrrupts(uint32_t*                       reg_val,
                                      const I3C_SlaveInterruptConfig* interrupt_config) {

    if (true ==interrupt_config->sdrTxFifoThreshold) {
        //enable sdrTxFifoThreshold interrupt
        *reg_val |= MIPI__SLV_IER__SDR_TX_THR_MASK;
    }

    if (true ==interrupt_config->sdrRxFifoThreshold) {
        //enable sdrRxFifoThreshold interrupt
        *reg_val |= MIPI__SLV_IER__SDR_RX_THR_MASK;
    }

    if (true ==interrupt_config->ddrTxFifoThreshold) {
        //enable ddrTxFifoThreshold interrupt
        *reg_val |= MIPI__SLV_IER__DDR_TX_THR_MASK;
    }

    if (true ==interrupt_config->ddrRxFifoThreshold) {
        //enable ddrRxFifoThreshold interrupt
        *reg_val |= MIPI__SLV_IER__DDR_RX_THR_MASK;
    }

}

static void i3cs_config_hj_intrrupts(uint32_t*                       reg_val,
                                     const I3C_SlaveInterruptConfig* interrupt_config) {
    if (true ==interrupt_config->hotJoinDone) {
        //enable hotJoinDone interrupt
        *reg_val |= MIPI__SLV_IER__HJ_DONE_MASK;
    }
}

static void i3cs_process_config_interrupts(I3C_PrivData*                   pd,
                                           const I3C_SlaveInterruptConfig* interrupt_config) {
    uint32_t reg_val = 0U;

    //process transfer interrupts
    i3cs_config_transfer_intrrupts(&reg_val, interrupt_config);
    //process fifo over/underflow interrupts
    i3cs_config_fifo_intrrupts(&reg_val, interrupt_config);
    //process thresholds interrupts
    i3cs_config_thr_intrrupts(&reg_val, interrupt_config);
    //process hotjoin interrupts
    i3cs_config_hj_intrrupts(&reg_val, interrupt_config);

    if (true ==interrupt_config->masterReadAbort) {
        //enable masterReadAbort interrupt
        reg_val |= MIPI__SLV_IER__M_RD_ABORT_MASK;
    }

    if (true ==interrupt_config->ddrFail) {
        //enable ddrFail interrupt
        reg_val |= MIPI__SLV_IER__DDR_FAIL_MASK;
    }

    if (true ==interrupt_config->sdrFail) {
        //enable sdrFail interrupt
        reg_val |= MIPI__SLV_IER__SDR_FAIL_MASK;
    }

    if (true ==interrupt_config->dynamicAddrUpdated) {
        //enable dynamicAddrUpdated interrupt
        reg_val |= MIPI__SLV_IER__DA_UPDATE_MASK;
    }

    if (true ==interrupt_config->eventUpdate) {
        //enable eventUpdate interrupt
        reg_val |= MIPI__SLV_IER__EVENT_UP_MASK;
    }

    if (true ==interrupt_config->protocolError) {
        //enable protocolError interrupt
        reg_val |= MIPI__SLV_IER__ERROR_MASK;
    }

    if (true ==interrupt_config->testMode) {
        //enable testMode interrupt
        reg_val |= MIPI__SLV_IER__TM_MASK;
    }

    CPS_REG_WRITE(&(pd->regs_base->SLV_IER), reg_val);
}

/**
 * This function configures response of I3C conftroller to Hot Join
 * request.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] hj_response Should I3C controller ACK or NACK Hot Join request.
 * @param[in] hj_disable_set Should I3C controller disable slaves from requesting Hot Join.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigureSlaveInterrupts(I3C_PrivData*                   pd,
                                      const I3C_SlaveInterruptConfig* slaveInterruptConfig)
{
    uint32_t result;

    /*
     * Validate parameters passed function
     */
    if (I3C_ConfigureSlaveInterruptsSF(pd, slaveInterruptConfig) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else if (core_enabled(pd)) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Core is enabled.\n");
        result = EIO;
    } else {
        i3cs_process_config_interrupts(pd, slaveInterruptConfig);
        result = 0;
    }

    return (result);
}

/**
 * Adds SDR Read request to the queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buf Pointer to buffer where data will be stored.
 * @param[in] num_bytes Number of bytes to read.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SlaveModeReqSdrRead(I3C_PrivData *pd, uint8_t* buf, uint16_t num_bytes)
{
    uint32_t result;
    uint16_t i;

    /*
     * Validate parameters passed function
     */
    if (I3C_SlaveModeReqSdrReadSF(pd, buf) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        /* START: Code duplicated from i3cs, blocked by DRV-3078 */
        for (i = 0U; i < num_bytes; ++i) {
            buf[i] = (uint8_t)CPS_REG_READ(&pd->regs_base->RX_FIFO);
        }
        /* END: Code duplicated from i3cs, blocked by DRV-3078 */

        result = 0;
    }

    return (result);
}

/**
 * Adds SDR Write request to the queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buf Pointer to buffer with data to be sent.
 * @param[in] num_bytes Number of bytes to send.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SlaveModeReqSdrWrite(I3C_PrivData *pd, const uint8_t* buf, uint16_t num_bytes)
{
    uint32_t result;
    uint16_t i;

    /*
     * Validate parameters passed function
     */
    if (I3C_SlaveModeReqSdrWriteSF(pd, buf) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        /* START: Code duplicated from i3cs, blocked by DRV-3078 */
        for (i = 0; i < num_bytes; ++i) {
            CPS_REG_WRITE(&pd->regs_base->TX_FIFO, buf[i]);
        }
        /* END: Code duplicated from i3cs, blocked by DRV-3078 */

        result = 0;
    }

    return (result);
}

/**
 * Adds DDR Read request to the queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buf Pointer to buffer where data will be stored.
 * @param[in] num_words Number of bytes to read.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return ENOENT if no DDR data is available for read
 * @return EPROTO if reveived command code is not to DDR Write
 */
uint32_t I3C_SlaveModeReqDdrRead(I3C_PrivData *pd, uint16_t* buf, uint16_t num_words)
{
    uint32_t result;
    uint32_t reg_val;
    uint8_t command_code;

    /*
     * Validate parameters passed function
     */
    if (I3C_SlaveModeReqDdrReadSF(pd, buf) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        /* START: Code duplicated from i3cs, blocked by DRV-3078 */
        /* Check if there is data in DDR RX FIFO */

        if (CPS_FLD_READ(MIPI__SLV_STATUS1, DDRRX_EMPTY,
                         CPS_REG_READ(&pd->regs_base->SLV_STATUS1)) != 0U) {
            DbgMsg(DBG_GEN_MSG, DBG_CRIT, "No DDR data available for read.\n");
            result = ENOENT;
        } else {

            /* Check if there is Write commad (Master's point of view) in
             * first FIFO buffer */
            reg_val = CPS_REG_READ(&pd->regs_base->SLV_DDR_RX_FIFO);
            command_code = i3c_ddr_word_get_cmd_code(reg_val);
            if (command_code > I3C_HDR_DDR_TX_CMD_CODE_MAX) {
                DbgMsg(DBG_GEN_MSG, DBG_CRIT,
                       "Received Command Code is not corresponding to DDR Write transfer\n");
                result = EPROTO;
            } else {
                result = i3c_slave_mode_process_ddr_read(pd, buf, num_words, reg_val);
            }
        }
        /* END: Code duplicated from i3cs, blocked by DRV-3078 */
    }

    return (result);
}

/**
 * Adds DDR Write request to the queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buf_in Pointer to buffer with data to be sent.
 * @param[in] num_words_in Number of 16-bit words to send.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return ENOTSUP if DDR FIFOs are not implemented
 */
uint32_t I3C_SlaveModeReqDdrWrite(I3C_PrivData *  pd,
                                  const uint16_t* buf_in,
                                  uint16_t        num_words_in)
{
    uint32_t result;
    uint16_t const *buf = buf_in;
    uint16_t num_words = num_words_in;

    /*
     * Validate parameters passed function
     */
    if (I3C_SlaveModeReqDdrWriteSF(pd, buf) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        /* START: Code duplicated from i3cs, blocked by DRV-3078 */
        uint32_t status1_reg_val = 0;
        uint8_t da = 0;
        uint8_t crc = 0;

        status1_reg_val = CPS_REG_READ(&pd->regs_base->SLV_STATUS1);

        if (CPS_FLD_READ(MIPI__SLV_STATUS1, HAS_DA, status1_reg_val) != 0U) {
            da = (uint8_t)CPS_FLD_READ(MIPI__SLV_STATUS1, DA, status1_reg_val);

            crc = i3c_slave_mode_ddr_initial_crc(da);
            crc = i3c_slave_ddr_write_word(pd, I3C_DDR_PREAMB_READ_ACK_DATA_FOLLOWS, *buf, crc);

            ++buf;
            --num_words;

            while (num_words > 0U) {
                crc = i3c_slave_ddr_write_word(pd, I3C_DDR_PREAMB_READ_DATA_FOLLOWS, *buf, crc);

                ++buf;
                --num_words;
            }

            i3c_slave_ddr_write_crc(pd, crc);
            result = 0;

        } else {
            result = ENOTSUP;
        }
        /* END: Code duplicated from i3cs, blocked by DRV-3078 */
    }

    return (result);
}

/**
 * Sends Hot Join request.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SlaveModeRequestHotJoin(I3C_PrivData *pd)
{
    uint32_t result;
    uint32_t reg_val;

    /*
     * Validate parameters passed function
     */
    if (I3C_SlaveModeRequestHotJoinSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        /* START: Code duplicated from i3cs, blocked by DRV-3078 */
        reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
        reg_val = CPS_FLD_SET(MIPI__CTRL, HJ_INIT, reg_val);
        CPS_REG_WRITE(&pd->regs_base->CTRL, reg_val);
        result = 0U;
        /* END: Code duplicated from i3cs, blocked by DRV-3078 */
    }

    return (result);
}

/**
 * Sends Mastership request.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SlaveModeMastershipReq(I3C_PrivData *pd)
{
    uint32_t result;
    uint32_t reg_val;

    /*
     * Validate parameters passed function
     */
    if (I3C_SlaveModeMastershipReqSF(pd) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        reg_val = CPS_REG_READ(&pd->regs_base->CTRL);
        reg_val = CPS_FLD_SET(MIPI__CTRL, MST_INIT, reg_val);
        CPS_REG_WRITE(&pd->regs_base->CTRL, reg_val);
        result = 0U;
    }

    return (result);
}

/**
 * Retrieves ASF information from I3C controller.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] asf_info Pointer to ASF information structure.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_GetAsfInfo(I3C_PrivData* pd, I3C_AsfInfo* asf_info)
{
    uint32_t result = 0U;
    I3C_PrivData* local_pd = pd;

    /*
     * Validate parameters passed function
     */
    if (I3C_GetAsfInfoSF(local_pd, asf_info) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        asf_info->regs_base = &(local_pd->regs_base->asf_int_status);
    }

    return (result);
}

/**
 * Returns devices which joined (by HJ) the bus since last check.
 * @param[in] pd Pointer to driver's private data object.
 * @param[out] hj_devices This mask indicates which devices joined the bus.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_GetNewDevices(I3C_PrivData* pd, uint16_t* hj_devices)
{
    uint32_t result = 0U;

    /*
     * Validate parameters passed function
     */
    if (I3C_GetNewDevicesSF(pd, hj_devices) != 0U) {
        DbgMsg(DBG_GEN_MSG, DBG_CRIT, "Error. Invalid arguments.\n");
        result = EINVAL;
    } else {
        *hj_devices = pd->hj_devices;
        pd->hj_devices = 0U;
    }

    return (result);
}
// parasoft-end-suppress METRICS-36-3
