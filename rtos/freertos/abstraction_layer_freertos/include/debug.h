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

#ifndef __DEBUG_H__
#define __DEBUG_H__

/* Define those flags in Makefile or command line to enable traces/logs. */

#if defined(TRACE_CPI)
#define CPI_TRACE(...)	   PI_LOG_DBG(__func__, __VA_ARGS__)
#define CPI_TRACE_ERR(...) PI_LOG_ERR(__func__, __VA_ARGS__)
#else
#define CPI_TRACE(...)	   ((void)0)
#define CPI_TRACE_ERR(...) ((void)0)
#endif /* TRACE_CPI */

#if defined(TRACE_I2S)
#define I2S_TRACE(...)	   PI_LOG_DBG(__func__, __VA_ARGS__)
#define I2S_TRACE_ERR(...) PI_LOG_ERR(__func__, __VA_ARGS__)
#else
#define I2S_TRACE(...)	   ((void)0)
#define I2S_TRACE_ERR(...) ((void)0)
#endif /* TRACE_I2S */

#if defined(TRACE_UART)
#define UART_TRACE(...)	    PI_LOG_DBG(__func__, __VA_ARGS__)
#define UART_TRACE_ERR(...) PI_LOG_ERR(__func__, __VA_ARGS__)
#else
#define UART_TRACE(...)	    ((void)0)
#define UART_TRACE_ERR(...) ((void)0)
#endif /* TRACE_UART */

#if defined(TRACE_I2C)
#define I2C_TRACE(...)             PI_LOG_DBG(__func__, __VA_ARGS__)
#define I2C_TRACE_ERR(...)         PI_LOG_ERR(__func__, __VA_ARGS__)
#else
#define I2C_TRACE(...)             ((void) 0)
#define I2C_TRACE_ERR(...)         ((void) 0)
#endif  /* TRACE_I2C */

/* Debug helper. */
#if defined (DEBUG_TASKS)
#define DEBUG_PRINTF printf
#else
#define DEBUG_PRINTF(...) ((void)0)
#endif /* DEBUG */


#endif /* __DEBUG_H__ */
