/*
 * Copyright (C) 2019 GreenWaves Technologies
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
 */

#ifndef __PMSIS_IMPLEM_PERF_H__
#define __PMSIS_IMPLEM_PERF_H__

#include "archi/pulp.h"
#include "hal/pulp.h"

/// @cond IMPLEM

#if defined(TIMER_VERSION) && TIMER_VERSION >= 2

static inline void pi_perf_conf(unsigned events)
{
  if ((events >> PI_PERF_CYCLES) & 1)
  {
  }

  cpu_perf_conf_events(events);
}

static inline void pi_perf_cl_reset()
{
#ifdef ARCHI_HAS_CLUSTER
  timer_reset(timer_base_cl(0, 0, 0));
  #ifndef ARCHI_HAS_COREV
  cpu_perf_setall(0);
  #else
  cpu_perf_setall(MCOUNTINHIBIT_RESET); // 0xFFFF_FFFD resets performance counters in cv32e40p (mcountinhibit, processor specs pag. 43)
  #endif
#endif
}

static inline void pi_perf_fc_reset()
{
#ifdef ARCHI_HAS_FC
  timer_reset(timer_base_fc(0, 0));
  #ifndef ARCHI_HAS_COREV
  cpu_perf_setall(0);
  #else
  cpu_perf_setall(MCOUNTINHIBIT_RESET); // 0xFFFF_FFFD resets performance counters in cv32e40p (mcountinhibit, processor specs pag. 43)
  #endif
#endif
}

static inline void pi_perf_reset()
{
  if (hal_is_fc())
    pi_perf_fc_reset();
  else
    pi_perf_cl_reset();
}

static inline void pi_perf_cl_start()
{
#ifdef ARCHI_HAS_CLUSTER
  timer_start(timer_base_cl(0, 0, 0));
  #ifndef ARCHI_HAS_COREV
  cpu_perf_conf(PCMR_ACTIVE | PCMR_SATURATE);
  #else
  cpu_perf_conf(MCOUNTINHIBIT_ACTIVE);
  #endif
#endif
}

static inline void pi_perf_fc_start()
{
#ifdef ARCHI_HAS_FC
  timer_start(timer_base_fc(0, 0));
  #ifndef ARCHI_HAS_COREV
  cpu_perf_conf(PCMR_ACTIVE | PCMR_SATURATE);
  #else
  cpu_perf_conf(MCOUNTINHIBIT_ACTIVE);
  #endif
#endif
}

static inline void pi_perf_start()
{
  if (hal_is_fc())
    pi_perf_fc_start();
  else
    pi_perf_cl_start();
}

static inline void pi_perf_cl_stop()
{
#ifdef ARCHI_HAS_CLUSTER
  timer_conf_set(timer_base_cl(0, 0, 0), TIMER_CFG_LO_ENABLE(0));
  #ifndef ARCHI_HAS_COREV
  cpu_perf_conf(0);
  #else
  cpu_perf_conf(MCOUNTINHIBIT_RESET);
  #endif
#endif
}

static inline void pi_perf_fc_stop()
{
#ifdef ARCHI_HAS_FC
  timer_conf_set(timer_base_fc(0, 0), TIMER_CFG_LO_ENABLE(0));
  cpu_perf_conf(0);
#endif
}

static inline void pi_perf_stop()
{
  if (hal_is_fc())
    pi_perf_fc_stop();
  else
    pi_perf_cl_stop();
}

// get mhpmcounter{4, 31} events count with NUM_MHPMCOUNTERS = 16
static inline unsigned int pi_perf_cl_read(int event)
{
#ifdef ARCHI_HAS_CLUSTER
  if (event == PI_PERF_CYCLES)
  {
    return timer_count_get(timer_base_cl(0, 0, 0));
  }
  else
  {
#if __PLATFORM__ != ARCHI_PLATFORM_BOARD
    return cpu_perf_get(1 + event);
#else
    return cpu_perf_get(0);
#endif
  }
#else
  return 0;
#endif
}

static inline unsigned int pi_perf_fc_read(int event)
{
#ifdef ARCHI_HAS_FC
  if (event == PI_PERF_CYCLES)
  {
    return timer_count_get(timer_base_fc(0, 0));
  }
  else
  {
#if __PLATFORM__ != ARCHI_PLATFORM_BOARD
    return cpu_perf_get(1 + event);
#else
    return cpu_perf_get(0);
#endif
  }
#else
  return 0;
#endif
}

static inline unsigned int pi_perf_read(int event)
{
  if (hal_is_fc())
    return pi_perf_fc_read(event);
  else
    return pi_perf_cl_read(event);
}


#ifdef ARCHI_HAS_COREV

// CV32E40P-specific API functions for handling Performance Counters reads

// Get clock cycle count from mcycle counter
static inline unsigned int pi_perf_cl_read_mcycle()
{
#ifdef ARCHI_HAS_CLUSTER
  return cpu_perf_get_mcycle();
#else
  return 0;
#endif
}

static inline unsigned int pi_perf_fc_read_mcycle()
{
#ifdef ARCHI_HAS_FC
  return cpu_perf_get_mcycle();
#else
  return 0;
#endif
}

static inline unsigned int pi_perf_read_mcycle()
{
  if (hal_is_fc())
    return pi_perf_fc_read_mcycle();
  else
    return pi_perf_cl_read_mcycle();
}


// Get instructions count from minstret counter
static inline unsigned int pi_perf_cl_read_minstret()
{
#ifdef ARCHI_HAS_CLUSTER
  return cpu_perf_get_minstret();
#else
  return 0;
#endif
}

static inline unsigned int pi_perf_fc_read_minstret()
{
#ifdef ARCHI_HAS_FC
  return cpu_perf_get_minstret();
#else
  return 0;
#endif
}

static inline unsigned int pi_perf_read_minstret()
{
  if (hal_is_fc())
    return pi_perf_fc_read_minstret();
  else
    return pi_perf_cl_read_minstret();
}
#endif

#endif

/// @endcond

#endif
