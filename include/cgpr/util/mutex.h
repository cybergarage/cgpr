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

#ifndef _CGPR_UTIL_MUTEX_H_
#define _CGPR_UTIL_MUTEX_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdbool.h>

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
 * \brief The generic wrapper struct for mUPnP's mutexes.
 *
 * This wrapper has been created to enable 100% code
 * compatibility for different platforms (Linux, Win32 etc..)
 */
typedef struct _CGMutex {
#if defined(WIN32) && !defined(ITRON)
  HANDLE mutexID;
#elif defined(BTRON)
  WERR mutexID;
#elif defined(ITRON)
  ER_ID mutexID;
#elif defined(TENGINE) && !defined(PROCESS_BASE)
  ID mutexID;
#elif defined(TENGINE) && defined(PROCESS_BASE)
  WERR mutexID;
#else
  /** The mutex entity */
  pthread_mutex_t mutexID;
#endif
} CGMutex;

#if defined(WITH_THREAD_LOCK_TRACE) && defined(__USE_ISOC99)
#include <stdlib.h>

typedef struct _CGLockInfo {
  struct _CGLockInfo* next;
  pthread_t thread_id;
  char *file, *function;
  int line, mutex_id;
} CGLockInfo;
#endif

/****************************************
 * Functions
 ****************************************/

/**
 * Create a new mutex
 */
CGMutex* cg_mutex_new(void);

/**
 * Destroy a mutex
 *
 * \param mutex The mutex to destroy
 */
bool cg_mutex_delete(CGMutex* mutex);

/**
 * Acquire a mutex lock
 *
 * \param mutex Mutex to lock
 */

#if defined(WITH_THREAD_LOCK_TRACE) && defined(__USE_ISOC99)
#define cg_mutex_lock(mutex) cg_mutex_lock_trace(__FILE__, __LINE__, __PRETTY_FUNCTION__, mutex)
#else
bool cg_mutex_lock(CGMutex* mutex);
#endif

/**
 * Release a locked mutex
 *
 * \param mutex Mutex to unlock
 */
#if defined(WITH_THREAD_LOCK_TRACE) && defined(__USE_ISOC99)
#define cg_mutex_unlock(mutex) cg_mutex_unlock_trace(__FILE__, __LINE__, __PRETTY_FUNCTION__, mutex)
#else
bool cg_mutex_unlock(CGMutex* mutex);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _CGPR_UTIL_MUTEX_H_ */
