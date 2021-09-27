/*
 * Copyright (C) 2020 ETH Zurich
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

#ifndef __CPU_ISS_PULP_ISS_WRAPPER_HPP
#define __CPU_ISS_PULP_ISS_WRAPPER_HPP

#include <vp/vp.hpp>
#include <iss_wrapper.hpp>

class pulp_iss_wrapper : public iss_wrapper
{
public:
    pulp_iss_wrapper(js::config *config);

    void target_open();
};


#endif
