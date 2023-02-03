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
 * cps_v3.h
 * Interface for Cadence Platform Services (CPS), version 3
 *
 * This is the "hardware abstraction layer" upon which all drivers are built.
 * It must be implemented for each platform.
 ******************************************************************************
 */
#ifndef CPS_V3_H
#define CPS_V3_H

#ifdef ERROR_INJECTION
#include "cps_ei.h"
#else

#include "cdn_stdtypes.h"

// parasoft-begin-suppress MISRA2012-RULE-8_6-2 "An identifier with external linkage shall have exactly one external definition"
// parasoft-begin-suppress METRICS-36-3 "A function should not be called from more than 5 different functions"
/****************************************************************************
 * Prototypes
 ***************************************************************************/

/**
 * Read a (32-bit) word
 * @param[in] address the address
 * @return the word at the given address
 */
extern uint32_t CPS_ReadReg32(volatile uint32_t* address);

/**
 * Write a (32-bit) word to memory
 * @param[in] address the address
 * @param[in] value the word to write
 */
extern void CPS_WriteReg32(volatile uint32_t* address, uint32_t value);

/**
 * Read a byte, bypassing the cache
 * @param[in] address the address
 * @return the byte at the given address
 */
extern uint8_t CPS_UncachedRead8(volatile uint8_t* address);

/**
 * Read a short, bypassing the cache
 * @param[in] address the address
 * @return the short at the given address
 */
extern uint16_t CPS_UncachedRead16(volatile uint16_t* address);

/**
 * Read a (32-bit) word, bypassing the cache
 * @param[in] address the address
 * @return the word at the given address
 */
extern uint32_t CPS_UncachedRead32(volatile uint32_t* address);

/**
 * Write a byte to memory, bypassing the cache
 * @param[in] address the address
 * @param[in] value the byte to write
 */
extern void CPS_UncachedWrite8(volatile uint8_t* address, uint8_t value);

/**
 * Write a short to memory, bypassing the cache
 * @param[in] address the address
 * @param[in] value the short to write
 */
extern void CPS_UncachedWrite16(volatile uint16_t* address, uint16_t value);

/**
 * Write a (32-bit) word to memory, bypassing the cache
 * @param[in] address the address
 * @param[in] value the word to write
 */
extern void CPS_UncachedWrite32(volatile uint32_t* address, uint32_t value);

/**
 * Write a (32-bit) address value to memory, bypassing the cache.
 * This function is for writing an address value, i.e. something that
 * will be treated as an address by hardware, and therefore might need
 * to be translated to a physical bus address.
 * @param[in] location the (CPU) location where to write the address value
 * @param[in] addrValue the address value to write
 */
extern void CPS_WritePhysAddress32(volatile uint32_t* location, uint32_t addrValue);

/**
 * Hardware specific memcpy.
 * @param[in] src  src address
 * @param[in] dst  destination address
 * @param[in] size size of the copy
 */
extern void CPS_BufferCopy(volatile uint8_t *dst, volatile const uint8_t *src, uint32_t size);

/**
* Invalidate the cache for the specified memory region.
* This function may be stubbed out if caching is disabled for memory regions
* as described in the driver documentation, or if the driver configuration does
* not require this function.
* @param[in] address Virtual address of memory region. (If an MMU is not in use,
* this will be equivalent to the physical address.) This address should be
* rounded down to the nearest cache line boundary.
* @param[in] size  size of memory in bytes.  This size should be rounded up to
* the nearest cache line boundary.  Use size UINTPTR_MAX to invalidate all
* memory cache.  A size of 0 should be ignored and the function should return
* immediately with no effect.
* @param[in] devInfo   This parameter can be used to pass implementation specific
* data to this function.  The content and use of this parameter is up to the
* implementor of this function to determine, and if not required it may be ignored.
*  For example, under Linux it can be used to pass a pointer to
* the device struct to be used in a call to dma_sync_single_for_device().  If
* used, the parameter should be passed to the core driver at initialisation as
* part of the configurationInfo struct.  Please
* see the core driver documentation for details of how to do this.
*/
extern void CPS_CacheInvalidate(void* address, size_t size, uintptr_t devInfo);

/**
* Flush the cache for the specified memory region
* This function may be stubbed out if caching is disabled for memory regions
* as described in the driver documentation, or if the driver configuration does
* not require this function.
* @param[in] address Virtual address of memory region. (If an MMU is not in use,
* this will be equivalent to the physical address.) This address should be
* rounded down to the nearest cache line boundary.
* @param[in] size  size of memory in bytes.  This size should be rounded up to
* the nearest cache line boundary.  Use size UINTPTR_MAX to flush all
* memory cache.  A size of 0 should be ignored and the function should return
* immediately with no effect.
* @param[in] devInfo   This parameter can be used to pass implementation specific
* data to this function.  The content and use of this parameter is up to the
* implementor of this function to determine, and if not required it may be ignored.
*  For example, under Linux it can be used to pass a pointer to
* the device struct to be used in a call to dma_sync_single_for_device().  If
* used, the parameter should be passed to the core driver at initialisation as
* part of the configurationInfo struct.  Please
* see the core driver documentation for details of how to do this.
*/
extern void CPS_CacheFlush(void* address, size_t size, uintptr_t devInfo);

/**
 * Delay software execution by a number of nanoseconds
 * @param[in] ns number of nanoseconds to delay software execution
 */
extern void CPS_DelayNs(uint32_t ns);

/**
 * Memory barrier
 * Waits until previous data accesses are finished
 */
extern void CPS_MemoryBarrier(void);

/**
 * Write Memory Barrier
 * Waits until previous data stores are finished
 */
extern void CPS_MemoryBarrierWrite(void);

/**
 * Read Memory barrier
 * Waits until previous data loads are finished
 */
extern void CPS_MemoryBarrierRead(void);

// parasoft-end-suppress METRICS-36-3
// parasoft-end-suppress MISRA2012-RULE-8_6-2

#endif /* ERROR_INJECTION */

#endif /* multiple inclusion protection */
