/* parasoft suppress item  MISRA2012-DIR-4_8 "Consider hiding implementation of structure" */
/**********************************************************************
 * Copyright (C) 2014-2018 Cadence Design Systems, Inc.
 * All rights reserved worldwide
 * The material contained herein is the proprietary and confidential
 * information of Cadence or its licensors, and is supplied subject to, and may
 * be used only by Cadence's customer in accordance with a previously executed
 * license and maintenance agreement between Cadence and that customer.
 **********************************************************************
 * WARNING: This file is auto-generated using api-generator utility.
 *          api-generator: 12.02.13bb8d5
 *          Do not edit it manually.
 **********************************************************************
 * Cadence Core Driver for I3C Master Interface
 **********************************************************************/

#ifndef I3C_PRIV_H
#define I3C_PRIV_H



/** @defgroup ConfigInfo  Configuration and Hardware Operation Information
 *  The following definitions specify the driver operation environment that
 *  is defined by hardware configuration or client code. These defines are
 *  located in the header file of the core driver.
 *  @{
 */

/**********************************************************************
* Defines
**********************************************************************/
/** Offset between two successive DeviceID<n>_reg registers (uint32_t as unit). */
#define	I3C_DEVS_OFF ((uint32_t)((offsetof(MIPI_I3cRegs, DEV_ID1_RR0) - offsetof(MIPI_I3cRegs, DEV_ID0_RR0)) / sizeof(volatile uint32_t)))

/** Offset between two successive SIR_MAP<n> registers. */
#define	I3C_SIR_MAP_OFF ((uint32_t)((offsetof(MIPI_I3cRegs, SIR_MAP1) - offsetof(MIPI_I3cRegs, SIR_MAP0)) / sizeof(volatile uint32_t)))

/**
 *  @}
 */


#endif	/* I3C_PRIV_H */
