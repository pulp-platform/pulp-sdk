/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
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
 * Authors: Germain Haugou, ETH (germain.haugou@iis.ee.ethz.ch)
 */

#ifndef LOG_H
#define LOG_H

typedef enum
{
  LOG_ERROR = 0,
  LOG_WARNING = 1,
  LOG_INFO = 2,
  LOG_DEBUG = 3
} log_level_e;

class Log {
  public:
    void print(log_level_e, const char *str, ...);
    void error(const char *str, ...) ;
    void warning(const char *str, ...) ;
    void user(const char *str, ...) ;
    void debug(const char *str, ...) ;
};

#endif
