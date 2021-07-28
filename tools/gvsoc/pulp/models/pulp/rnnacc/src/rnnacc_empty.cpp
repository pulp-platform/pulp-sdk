/*
 * Copyright (C) 2021 ETH Zurich, University of Bologna, GreenWaves Technologies
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
 * Authors: Gianna Paulin, ETH Zurich (pauling@iis.ee.ethz.ch)
 *          Francesco Conti, University of Bologna & GreenWaves Technologies (f.conti@unibo.it)
 *          Germain Haugou, GreenWaves Technologies (germain.haugou@greenwaves-technologies.com)
 */

#include "vp/vp.hpp"
#include <vp/itf/io.hpp>

class Rnnacc_v1 : public vp::component
{
    friend class Rnnacc_v1_base;

public:
    Rnnacc_v1(js::config *config);

    int build();
    void reset(bool active);


private:
    vp::io_master out;
    vp::io_slave in;
    vp::wire_master<bool> irq;

};

Rnnacc_v1::Rnnacc_v1(js::config *config)
    : vp::component(config)
{
}


void Rnnacc_v1::reset(bool active)
{
}


int Rnnacc_v1::build()
{
    this->new_master_port("out", &this->out);

    this->new_master_port("irq", &this->irq);

    this->new_slave_port("input", &this->in);

    return 0;
}

extern "C" vp::component *vp_constructor(js::config *config)
{
    return new Rnnacc_v1(config);
}
