/*
 * Copyright (C) 2020 ETH Zurich and University of Bologna
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
/* Driver to control the soc control clock divider used in some PULP based
 * chips/IPs. This driver is mutually exclusive with the fll driver
 */
/* Author: Robert Balas (balasr@iis.ee.ethz.ch)
 */

#ifndef __CLKDIV_H__
#define __CLKDIV_H__
/* soc clock division register offset */
#define CLKDIV_FC_OFFSET 0xf00
/* cluster clock division register offset */
#define CLKDIV_CL_OFFSET 0xf08
/* peripheral clock division register offset */
#define CLKDIV_PERIPH_OFFSET 0xf10

#endif /* __CLKDIV_H__ */
