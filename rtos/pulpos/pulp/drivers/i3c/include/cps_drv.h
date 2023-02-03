/******************************************************************************
 *
 * Copyright (C) 2014-2018 Cadence Design Systems, Inc.
 * All rights reserved worldwide
 * The material contained herein is the proprietary and confidential
 * information of Cadence or its licensors, and is supplied subject to, and may
 * be used only by Cadence's customer in accordance with a previously executed
 * license and maintenance agreement between Cadence and that customer.
 *
 ******************************************************************************
 * cps_drv.h
 * Interface for the Register Access Layer of Cadence Platform Service (CPS)
 ******************************************************************************
 */

#ifndef CPS_DRV_H_
#define CPS_DRV_H_

#include "cps.h"

// parasoft-begin-suppress MISRA2012-DIR-4_9-4 "function-like macro"
// parasoft-begin-suppress MISRA2012-RULE-20_10-4 "## preprocessor operator"
// parasoft-begin-suppress METRICS-36-3 "A function should not be called from more than 5 different functions"

/**
 *  \brief    Read a 32-bit value from memory.
 *  \param    reg   address of the memory mapped hardware register
 *  \return   the value at the given address
 */
#define CPS_REG_READ(reg) (CPS_RegRead((volatile uint32_t*)(reg)))

/**
 *  \brief   Write a 32-bit address value to memory.
 *  \param   reg     address of the memory mapped hardware register
 *  \param   value   unsigned 32-bit value to write
 */
#define CPS_REG_WRITE(reg, value) (CPS_RegWrite((volatile uint32_t*)(reg), (uint32_t)(value)))

/**
 *  \brief    Read a value of bit-field from the register value.
 *  \param    reg         register name
 *  \param    fld         field name
 *  \param    reg_value   register value
 *  \return   bit-field value
 */
#define CPS_FLD_READ(reg, fld, reg_value) (CPS_FldRead((uint32_t)(reg##__##fld##_MASK),  \
                                                       (uint32_t)(reg##__##fld##_SHIFT), \
                                                       (uint32_t)(reg_value)))

/**
 *  \brief    Write a value of the bit-field into the register value.
 *  \param    reg         register name
 *  \param    fld         field name
 *  \param    reg_value   register value
 *  \param    value       value to be written to bit-field
 *  \return   modified register value
 */
#define CPS_FLD_WRITE(reg, fld, reg_value, value) (CPS_FldWrite((uint32_t)(reg##__##fld##_MASK),  \
                                                                (uint32_t)(reg##__##fld##_SHIFT), \
                                                                (uint32_t)(reg_value), (uint32_t)(value)))

/**
 *  \brief    Set bit within the register value.
 *  \param    reg         register name
 *  \param    fld         field name
 *  \param    reg_value   register value
 *  \return   modified register value
 */
#define CPS_FLD_SET(reg, fld, reg_value) (CPS_FldSet((uint32_t)(reg##__##fld##_WIDTH), \
                                                     (uint32_t)(reg##__##fld##_MASK),  \
                                                     (uint32_t)(reg##__##fld##_WOCLR), \
                                                     (uint32_t)(reg_value)))

/**
 *  \brief    Clear bit within the register value.
 *  \param    reg         register name
 *  \param    fld         field name
 *  \param    reg_value   register value
 *  \return   modified register value
 */
#define CPS_FLD_CLEAR(reg, fld, reg_value) (CPS_FldClear((uint32_t)(reg##__##fld##_WIDTH), \
                                                         (uint32_t)(reg##__##fld##_MASK),  \
                                                         (uint32_t)(reg##__##fld##_WOSET), \
                                                         (uint32_t)(reg##__##fld##_WOCLR), \
                                                         (uint32_t)(reg_value)))

/**
 *  \brief    Read a 32-bit value from memory.
 *  \param    reg   address of the memory mapped hardware register
 *  \return   the value at the given address
 */
static inline uint32_t CPS_RegRead(volatile uint32_t* reg)
{
    return (CPS_ReadReg32(reg));
}

/**
 *  \brief   Write a 32-bit address value to memory.
 *  \param   reg     address of the memory mapped hardware register
 *  \param   value   unsigned 32-bit value to write
 */
static inline void CPS_RegWrite(volatile uint32_t* reg, uint32_t value)
{
    CPS_WriteReg32(reg, value);
}

/**
 *  \brief    Read a value of bit-field from the register value.
 *  \param    mask        mask for the bit-field
 *  \param    shift       bit-field shift from LSB
 *  \param    reg_value   register value
 *  \return   bit-field value
 */
static inline uint32_t CPS_FldRead(uint32_t mask, uint32_t shift, uint32_t reg_value)
{
    // parasoft-begin-suppress MISRA2012-RULE-12_2-2 "shift ranges"
    uint32_t result = (reg_value & mask) >> shift;
    // parasoft-end-suppress MISRA2012-RULE-12_2-2

    return (result);
}

/**
 *  \brief    Write a value of the bit-field into the register value.
 *  \param    mask        mask for the bit-field
 *  \param    shift       bit-field shift from LSB
 *  \param    reg_value   register value
 *  \param    value       value to be written to bit-field
 *  \return   modified register value
 */
static inline uint32_t CPS_FldWrite(uint32_t mask, uint32_t shift, uint32_t reg_value, uint32_t value)
{
    // parasoft-begin-suppress MISRA2012-RULE-12_2-2 "shift ranges"
    uint32_t new_value = (value << shift) & mask;
    // parasoft-end-suppress MISRA2012-RULE-12_2-2

    new_value = (reg_value & ~mask) | new_value;
    return (new_value);
}

/**
 *  \brief    Set bit within the register value.
 *  \param    width       width of the bit-field
 *  \param    mask        mask for the bit-field
 *  \param    is_woclr    is bit-field has 'write one to clear' flag set
 *  \param    reg_value   register value
 *  \return   modified register value
 */
static inline uint32_t CPS_FldSet(uint32_t width, uint32_t mask, uint32_t is_woclr, uint32_t reg_value)
{
    uint32_t new_value = reg_value;
    if ((width == 1U) && (is_woclr == 0U)) {
        new_value |= mask;
    }

    return (new_value);
}

/**
 *  \brief    Clear bit within the register value.
 *
 *  \param    width       Width of the bit-field.
 *  \param    mask        Mask for the bit-field.
 *  \param    is_woset    Is bit-field has 'write one to set' flag set.
 *  \param    is_woclr    Is bit-field has 'write one to clear' flag set.
 *  \param    reg_value   Register value.
 *
 *  \return   Modified register value.
 */
static inline uint32_t CPS_FldClear(uint32_t width, uint32_t mask, uint32_t is_woset, uint32_t is_woclr,  uint32_t reg_value)
{
    uint32_t new_value = reg_value;
    if ((width == 1U) && (is_woset == 0U)) {
        new_value = (new_value & ~mask) | ((is_woclr != 0U) ? mask : 0U);
    }

    return (new_value);
}

// parasoft-end-suppress MISRA2012-RULE-20_10-4
// parasoft-end-suppress MISRA2012-DIR-4_9-4
// parasoft-end-suppress METRICS-36-3

#endif /* CPS_DRV_H_ */

