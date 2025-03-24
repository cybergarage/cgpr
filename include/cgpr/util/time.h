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

#if !defined(BTRON) && !defined(ITRON)
#include <time.h>
#endif

#if defined(BTRON)
#include <btron/clk.h>
#include <btron/proctask.h>
#elif defined(ITRON)
#include <kernel.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#if defined(BTRON) || defined(ITRON)
typedef size_t CGTime;
#else
typedef long CGTime;
#endif

/****************************************
 * FIXME: XCode 13 might not include <time.h> normally
 ****************************************/

#if !defined(_TIME_H_) && !defined(_TIME_H)
struct tm {
  int tm_sec; /* seconds after the minute [0-60] */
  int tm_min; /* minutes after the hour [0-59] */
  int tm_hour; /* hours since midnight [0-23] */
  int tm_mday; /* day of the month [1-31] */
  int tm_mon; /* months since January [0-11] */
  int tm_year; /* years since 1900 */
  int tm_wday; /* days since Sunday [0-6] */
  int tm_yday; /* days since January 1 [0-365] */
  int tm_isdst; /* Daylight Savings Time flag */
  long tm_gmtoff; /* offset from UTC in seconds */
  char* tm_zone; /* timezone abbreviation */
};
#endif

/****************************************
 * Function
 ****************************************/

float cg_random(void);

void cg_wait(CGTime mtime);
void cg_waitrandom(CGTime mtime);

#define cg_sleep(val) cg_wait(val)
#define cg_sleeprandom(val) cg_waitrandom(val)

CGTime cg_getcurrentsystemtime(void);

#ifdef __cplusplus
}
#endif

#endif
