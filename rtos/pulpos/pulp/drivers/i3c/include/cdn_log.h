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
 * cdn_log.h
 * System wide debug log messaging framework
 *****************************************************************************/

#ifndef INCLUDE_CDN_LOG_H

#define INCLUDE_CDN_LOG_H

#include "cdn_inttypes.h"


// parasoft-begin-suppress MISRA2012-RULE-8_6-2 "An identifier with external linkage shall have exactly one external definition"
// parasoft-begin-suppress MISRA2012-DIR-4_9-4 "A function should be used in preference to a function-like macro where they are interchangeable"

#ifdef DEBUG
 #if DEBUG
  #define CFP_DBG_MSG 1
 #endif
#endif

/**
 * Modules definitions
 */
#define CLIENT_MSG         0x01000000

#define DBG_GEN_MSG        0xFFFFFFFF

/**
 * Log level:
 * DBG_CRIT - critical
 * DBG_WARN - warning
 * DBG_FYI - fyi
 * DBG_HIVERB - highly verbose
 * DBG_INFLOOP - infinite loop debug
 */
#define DBG_CRIT 0
#define DBG_WARN 5
#define DBG_FYI 10
#define DBG_HIVERB 100
#define DBG_INFLOOP 200

/* module mask: */
#ifdef _HAVE_DBG_LOG_INT_
uint32_t g_dbg_enable_log  = 0;
#else
extern uint32_t g_dbg_enable_log;
#endif

/* level, counter, state: */
#ifdef _HAVE_DBG_LOG_INT_
uint32_t g_dbg_log_lvl = DBG_CRIT;
uint32_t g_dbg_log_cnt = 0;
uint32_t g_dbg_state = 0;
#else
extern uint32_t g_dbg_log_lvl;
extern uint32_t g_dbg_log_cnt;
extern uint32_t g_dbg_state;
#endif



/* There is possibility to provide custom logging functions instead of default
 *   - DbgMsg
 * by defining macros:
 *   - DEBUG for DbgMsg with custom print implementation
 *
 * There is also possibility to provide own implementation of printf() routine by
 * defining function:
 *   DbgPrint
 * that provides name of functions that replace printf().
 * Their prototype:
 *   void DbgPrint(const char *fmt, ...)
 *
 * If DEBUG symbol is defined, then appropriate prototype is declared within this header file.
 *
 * If DEBUG is not defined, print is not used. Existing macros will not print any data.
 */


/* For DEBUG build, use custom logging with own implementation of DbgPrint */
#ifdef DEBUG
extern void DbgPrint(const char *fmt, ...);
#define cDbgMsg(_t, _x, ...) ( ((_x)==  0) || \
                                (((_t) & g_dbg_enable_log) && ((_x) <= g_dbg_log_lvl)) ? \
                                  DbgPrint( __VA_ARGS__): 0 )
#else
#define cDbgMsg( _t, _x, ...)
#endif // DEBUG

#ifdef CFP_DBG_MSG
#define DbgMsg(t, x, ...)                                                      \
  cDbgMsg((t), (x), "PRI%2d: %s:%d\n", x, __func__, __LINE__);          \
  cDbgMsg((t), (x), __VA_ARGS__);                                       \


#else
#define DbgMsg( t, x, ...)
#endif


#define DEBUG_PREFIX "[%-20.20s %4d %4"PRId32"]-"

// ******** Default vDbgMsg ********
#  define vDbgMsg(log_lvl, module, msg, ...)    DbgMsg( (log_lvl), (module), (DEBUG_PREFIX msg), __func__,\
                                                    __LINE__, g_dbg_log_cnt++, __VA_ARGS__)

// ******** Default cvDbgMsg ********
#  define cvDbgMsg(log_lvl, module, msg, ...)   cDbgMsg( (log_lvl), (module), (DEBUG_PREFIX msg), __func__,\
                                                   __LINE__, g_dbg_log_cnt++, __VA_ARGS__)

// ******** Default cvDbgMsg ********
#  define evDbgMsg(log_lvl, module, msg, ...)   { cDbgMsg( (log_lvl), (module), (DEBUG_PREFIX msg), __func__,         \
                                                   __LINE__, g_dbg_log_cnt++, __VA_ARGS__); }

#define DbgMsgSetLvl( x ) (g_dbg_log_lvl = (x))
#define DbgMsgEnableModule( x ) (g_dbg_enable_log |= (x) )
#define DbgMsgDisableModule( x ) (g_dbg_enable_log &= ~( (uint32_t) (x) ))
#define DbgMsgClearAll( _x ) ( g_dbg_enable_log = (_x) )

#define SetDbgState( _x ) (g_dbg_state = (_x) )
#define GetDbgState       (g_dbg_state)

// parasoft-end-suppress MISRA2012-RULE-8_6-2
// parasoft-end-suppress MISRA2012-DIR-4_4-4

#endif // INCLUDE_CDN_LOG_H
