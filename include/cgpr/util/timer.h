/******************************************************************
 *
 * Copyright (C) 2025 The Cyber Garage Portable Runtime for C Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef _CGPR_UTIL_TIME_H_
#define _CGPR_UTIL_TIME_H_

#include <cgpr/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Function
 ****************************************/

float cg_random(void);

void cg_wait(clock_t mtime);
void cg_waitrandom(clock_t mtime);

#define cg_sleep(val) cg_wait(val)
#define cg_sleeprandom(val) cg_waitrandom(val)

clock_t cg_getcurrentsystemtime(void);

#ifdef __cplusplus
}
#endif

#endif
