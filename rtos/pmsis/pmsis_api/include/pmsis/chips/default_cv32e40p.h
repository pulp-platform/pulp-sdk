/*
 * Copyright (C) 2020 GreenWaves Technologies
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

#ifndef __PMSIS_CHIPS_DEFAULT_H__
#define __PMSIS_CHIPS_DEFAULT_H__

/**
 * \ingroup groupChips
 */

/**
 * \enum pi_pad_e
 * \brief Pad numbers.
 *
 * This is used to identify pads.
 */
typedef enum
{
    PI_PAD_NONE
} pi_pad_e;

/**
 * \enum pi_gpio_e
 * \brief GPIO numbers.
 *
 * List of available GPIO pins.
 */
typedef enum
{
    PI_GPIO_NONE
} pi_gpio_e;

/** \enum pi_perf_event_e
 * \brief Performance event identifiers.
 *
 * This can be used to describe which performance event to monitor (cycles,
 * cache miss, etc).
 */
typedef enum {
  PI_PERF_CYCLES        = 17, /*!< Total number of cycles (also includes the
    cycles where the core is sleeping). Be careful that this event is using a
    timer shared within the cluster, so resetting, starting or stopping it on
    one core will impact other cores of the same cluster. */
  PI_PERF_ACTIVE_CYCLES = 0,  /*!< Counts the number of cycles the core was
    active (not sleeping). */
  PI_PERF_INSTR         = 1,  /*!< Counts the number of instructions executed.
  */
  PI_PERF_LD_STALL      = 2,  /*!< Number of load data hazards. */
  PI_PERF_JR_STALL      = 3,  /*!< Number of jump register data hazards. */
  PI_PERF_IMISS         = 4,  /*!< Cycles waiting for instruction fetches, i.e.
    number of instructions wasted due to non-ideal caching. */
  PI_PERF_LD            = 5,  /*!< Number of data memory loads executed.
    Misaligned accesses are counted twice. */
  PI_PERF_ST            = 6,  /*!< Number of data memory stores executed.
    Misaligned accesses are counted twice. */
  PI_PERF_JUMP          = 7,  /*!< Number of unconditional jumps (j, jal, jr,
    jalr). */
  PI_PERF_BRANCH        = 8,  /*!< Number of branches. Counts both taken and
    not taken branches. */
  PI_PERF_BTAKEN        = 9, /*!< Number of taken branches. */
  PI_PERF_RVC           = 10, /*!< Number of compressed instructions
    executed. */
  PI_PIPE_STALL         = 11, /*!< Number of cycles from stalled pipeline. */
  PI_APU_TYPE           = 12, /*!< Number of type conflicts on APU/FP. */
  PI_APU_CONT           = 13, /*!< Number of contentions on APU/FP. */
  PI_APU_DEP            = 14, /*!< Number of dependency stall on APU/FP. */
  PI_APU_WB             = 15  /*!< Number of write backs on APU/FP. */
} pi_perf_event_e;

/**
 * @}
 */

#endif  /* __PMSIS_CHIPS_GAP9_PAD_H__ */
