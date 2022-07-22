/*
 * Copyright (C) 2020 GreenWaves Technologies, SAS, ETH Zurich and
 *                    University of Bologna
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

/*
 * Authors: Germain Haugou, GreenWaves Technologies (germain.haugou@greenwaves-technologies.com)
 */

#ifndef __VP_CLOCK_COMPONENT_CLOCK_HPP__
#define __VP_CLOCK_COMPONENT_CLOCK_HPP__

#include "vp/component.hpp"
#include "vp/itf/clk.hpp"
#include "vp/clock/clock_event.hpp"
#include "vp/itf/implem/wire_class.hpp"

#include <memory>

using namespace std;

namespace vp {

  class config;

  class clock_engine;
  class time_engine;

  class component_clock
  {

  public:

    static void clk_reg(component *_this, std::shared_ptr<clock_engine> clock);
    static void reset_sync(void *_this, bool active);

    inline void event_enqueue(std::shared_ptr<clock_event> event, int64_t cycles);

    inline void event_reenqueue(std::shared_ptr<clock_event> event, int64_t cycles);

    inline void event_enqueue_ext(std::shared_ptr<clock_event> event, int64_t cycles);

    inline void event_reenqueue_ext(std::shared_ptr<clock_event> event, int64_t cycles);

    inline void event_cancel(std::shared_ptr<clock_event> event);

    std::shared_ptr<clock_event> event_new(clock_event_meth_t *meth);

    std::shared_ptr<clock_event> event_new(void *_this, clock_event_meth_t *meth);

    inline std::shared_ptr<clock_event> event_new(clock_event_meth_t *meth, void *arg)
    {
      std::shared_ptr<clock_event> event = event_new(meth);
      event->get_args()[0] = arg;
      return event;
    }

    inline std::shared_ptr<clock_engine> get_clock();

    inline int64_t get_time();

    inline int64_t get_cycles();

    void pre_build(component *comp);

    inline int64_t get_period();

    inline int64_t get_frequency();

    inline std::shared_ptr<time_engine> get_engine();

  protected:
    std::shared_ptr<clock_engine> clock = NULL;

    clk_slave            clock_port;
    vp::wire_slave<bool> reset_port;

  };

};


#endif
