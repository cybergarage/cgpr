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

#include <cgpr/typedef.h>
#include <cgpr/util/mutex.h>

#if defined(WIN32) && !defined(ITRON)
#include <winsock2.h>
#elif defined(BTRON)
#include <btron/taskcomm.h>
#elif defined(ITRON)
#include <kernel.h>
#elif defined(TENGINE) && !defined(PROCESS_BASE)
#include <tk/tkernel.h>
#elif defined(TENGINE) && defined(PROCESS_BASE)
#include <btron/taskcomm.h>
#else
#include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Types
 ****************************************/

/**
 * \brief The generic wrapper struct for mUPnP's conds.
 *
 * This wrapper has been created to enable 100% code
 * compatibility for different platforms (Linux, Win32 etc..)
 */
typedef struct _CGCond {
#if defined(WIN32) && !defined(ITRON)
  HANDLE condID;
#elif defined(BTRON)
  WERR condID;
#elif defined(ITRON)
  ER_ID condID;
#elif defined(TENGINE) && !defined(PROCESS_BASE)
  ID condID;
#elif defined(TENGINE) && defined(PROCESS_BASE)
  WERR condID;
#else
  /** The cond entity */
  pthread_cond_t condID;
#endif
} CGCond;

/****************************************
 * Functions
 ****************************************/

/**
 * Create a new condition variable
 */
CGCond* cg_cond_new(void);

/**
 * Destroy a condition variable
 *
 * \param cond The cond to destroy
 */
bool cg_cond_delete(CGCond* cond);

/**
 * Wait for condition variable to be signalled.
 *
 * \param cond Cond to be waited
 * \param mutex Mutex used for synchronization
 * \param timeout Maximum time in seconds to wait, 0 to wait forever
 */
bool cg_cond_wait(CGCond* cond, CGMutex* mutex, unsigned long timeout);

/**
 * Signal a condition variable
 *
 * \param cond Cond to be signalled
 */
bool cg_cond_signal(CGCond* cond);

#ifdef __cplusplus

} /* extern "C" */

#endif

#endif
