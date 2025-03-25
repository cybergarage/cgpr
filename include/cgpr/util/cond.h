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

#ifndef _CGPR_UTIL_COND_H_
#define _CGPR_UTIL_COND_H_

#include <cgpr/util/typedef.h>

#if defined(WIN32)
#include <winsock2.h>
#else
#include <pthread.h>
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Types
 ****************************************/

typedef struct UEchoCond {
  pthread_mutex_t mutexId;
  pthread_cond_t condId;
} CGCond;

/****************************************
 * Functions
 ****************************************/

CGCond* cg_cond_new(void);
bool cg_cond_delete(CGCond* cond);

bool cg_cond_wait(CGCond* cond);
bool cg_cond_timedwait(CGCond* cond, clock_t mtime);
bool cg_cond_signal(CGCond* cond);

#ifdef __cplusplus
}
#endif

#endif /* _CGPR_UTIL_COND_H_ */
