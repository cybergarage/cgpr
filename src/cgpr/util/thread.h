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

#ifndef _CGPR_UTIL_THREAD_H_
#define _CGPR_UTIL_THREAD_H_

#include <cgpr/util/list.h>

#include <cgpr/util/time.h>
#if defined(WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <signal.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#define CG_THREAD_MIN_SLEEP 200

/****************************************
 * Data Type
 ****************************************/

typedef struct _CGThread {
  CG_LIST_STRUCT_MEMBERS

      bool runnableFlag;

#if defined(WIN32)
  HANDLE hThread;
  DWORD threadID;
#else
  pthread_t pThread;
#endif

  void (*action)(struct _CGThread*);
  void* userData;
} CGThread, CGThreadList;

typedef void (*CG_THREAD_FUNC)(CGThread*);

/****************************************
 * Function
 ****************************************/

CGThread* cg_thread_new(void);
bool cg_thread_delete(CGThread* thread);

bool cg_thread_start(CGThread* thread);
bool cg_thread_stop(CGThread* thread);
bool cg_thread_restart(CGThread* thread);
bool cg_thread_isrunnable(CGThread* thread);
bool cg_thread_isrunning(CGThread* thread);

void cg_thread_setaction(CGThread* thread, CG_THREAD_FUNC action_func);
void cg_thread_setuserdata(CGThread* thread, void* data);
void* cg_thread_getuserdata(CGThread* thread);

#define cg_thread_next(thread) \
  (CGThread*)cg_list_next((CGList*)thread)
#define cg_thread_remove(thread) cg_list_remove((CGList*)thread)

/****************************************
 * Function (Thread List)
 ****************************************/

CGThreadList* cg_threadlist_new(void);
void cg_threadlist_delete(CGThreadList* thread_list);

#define cg_threadlist_clear(threadList) \
  cg_list_clear((CGList*)threadList,    \
      (CG_LIST_DESTRUCTORFUNC)cg_thread_delete)
#define cg_threadlist_size(threadList) \
  cg_list_size((CGList*)threadList)
#define cg_threadlist_gets(threadList) \
  (CGThread*)cg_list_next((CGList*)threadList)
#define cg_threadlist_add(threadList, thread) \
  cg_list_add((CGList*)threadList, (CGList*)thread)
#define cg_threadlist_remove(thread) \
  cg_list_remove((CGList*)thread)

bool cg_threadlist_start(CGThreadList* thread_list);
bool cg_threadlist_stop(CGThreadList* thread_list);

#ifdef __cplusplus

} /* extern "C" */

#endif

#endif
