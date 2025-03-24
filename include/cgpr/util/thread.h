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

#include <cgpr/util/cond.h>
#include <cgpr/util/list.h>

#include <cgpr/util/time.h>
#if defined(WIN32) && !defined(ITRON)
#include <windows.h>
#elif defined(BTRON)
#include <btron/proctask.h>
#elif defined(ITRON)
#include <kernel.h>
#elif defined(TENGINE) && !defined(PROCESS_BASE)
#include <tk/tkernel.h>
#elif defined(TENGINE) && defined(PROCESS_BASE)
#include <btron/proctask.h>
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

#if defined(WINCE)
#define CG_THREAD_SHUTDOWN_ATTEMPTS 10
#define CG_THREAD_MIN_SLEEP 1000
/* ADD Fabrice Fontaine Orange 24/04/2007 */
/* Bug correction : Variable used to wait for thread termination by sleeping */
/* instead of joining */
#else
#define CG_THREAD_MIN_SLEEP 1000
/* ADD END Fabrice Fontaine Orange 24/04/2007 */
#endif

/****************************************
 * Data Type
 ****************************************/

/**
 * \brief The generic wrapper struct for mUPnP's threads.
 *
 * This wrapper has been created to enable 100% code
 * compatibility between different platforms (Linux, Win32 etc..)
 */
typedef struct _CGThread {
  CG_LIST_STRUCT_MEMBERS

  /** Indicates whether this thread is ready to run */
  bool runnableFlag;
#if defined WINCE
  /** serves as look ahead to have the thread manage its own delete(thread) on exit */
  bool isRunning;
  bool deletePending;
  CGTime sleep;
#endif // WINCE

#if defined DEBUG
  char friendlyName[32];
#endif

#if defined(WIN32) && !defined(ITRON)
  HANDLE hThread;
  DWORD threadID;
#elif defined(BTRON)
  W taskID;
#elif defined(ITRON)
  ER_ID taskID;
#elif defined(TENGINE) && !defined(PROCESS_BASE)
  ID taskID;
#elif defined(TENGINE) && defined(PROCESS_BASE)
  WERR taskID;
#else

  /** The POSIX thread handle */
  pthread_t pThread;

#endif

  /** Thread's worker function */
  void (*action)(struct _CGThread*);

  /** Arbitrary data pointer */
  void* userData;
} CGThread, CGThreadList;

/**
 * Prototype for the threads' worker functions
 */
typedef void (*CG_THREAD_FUNC)(CGThread*);

/****************************************
 * Function
 ****************************************/

/**
 * Create a new thread
 */
CGThread* cg_thread_new(void);

/**
 * Get a self reference to thread.
 */

CGThread* cg_thread_self(void);

/**
 * Stop and destroy a thread.
 *
 * \param thread Thread to destroy
 */
bool cg_thread_delete(CGThread* thread);

/**
 * Start a thread (must be created first with ch_thread_new())
 *
 * \param thread Thread to start
 */
bool cg_thread_start(CGThread* thread);

/**
 * Stop a running thread.
 *
 * \param thread Thread to stop
 */
bool cg_thread_stop(CGThread* thread);

/**
 * Stop the running thread and signal the given CGCond.
 */
bool cg_thread_stop_with_cond(CGThread* thread, CGCond* cond);

/**
 * Restart a thread. Essentially calls cg_thread_stop() and cg_thread_start()
 *
 * \param thread Thread to restart
 */
bool cg_thread_restart(CGThread* thread);

/**
 * Check if a thread has been started
 *
 * \param thread Thread to check
 */
bool cg_thread_isrunnable(CGThread* thread);

/**
 * Set the thread's worker function.
 *
 * \param thread Thread struct
 * \param actionFunc Function pointer to set as the worker function
 */
void cg_thread_setaction(CGThread* thread, CG_THREAD_FUNC actionFunc);

/**
 * Set the user data pointer
 *
 * \param thread Thread struct
 * \param data Pointer to user data
 */
void cg_thread_setuserdata(CGThread* thread, void* data);

/**
 * Get the user data pointer
 *
 * \param thread Thread from which to get the pointer
 */
void* cg_thread_getuserdata(CGThread* thread);

#if defined(WINCE)
void cg_thread_sleep(CGThread* thread);
void cg_thread_exit(DWORD exitCode);
#if defined DEBUG_MEM
void cg_thread_monitor(CGThread* thread);
#endif // DEBUG_MEM
#endif // WIN32_WCE

#define cg_thread_next(thread) (CGThread*)cg_list_next((CGList*)thread)
#define cg_thread_remove(thread) cg_list_remove((CGList*)thread)

/****************************************
 * Function (Thread List)
 ****************************************/

/**
 * Create a new thread list
 *
 * \return Thread list
 */
CGThreadList* cg_threadlist_new(void);

/**
 * Destroy a thread list
 *
 * \param threadList The thread list in question
 */
void cg_threadlist_delete(CGThreadList* threadList);

/**
 * Clear the contents of a thread list
 *
 * \param threadList Thread list in question
 */
#define cg_threadlist_clear(threadList) cg_list_clear((CGList*)threadList, (CG_LIST_DESTRUCTORFUNC)cg_thread_delete)

/**
 * Get the size of a thread list
 *
 * \param threadList The thread list in question
 */
#define cg_threadlist_size(threadList) cg_list_size((CGList*)threadList)

/**
 * Get the first actual item from a thread list to use as an iterator
 *
 * \param threadList The thread list in question
 */
#define cg_threadlist_gets(threadList) (CGThread*)cg_list_next((CGList*)threadList)

/**
 * Add a thread into a thread list
 *
 * \param threadList The thread list in question
 * \param thread The thread to add to the list
 */
#define cg_threadlist_add(threadList, thread) cg_list_add((CGList*)threadList, (CGList*)thread)

/**
 * Remove a thread from thread list
 *
 * \param threadList The thread list in question
 * \param thread The thread to be removed
 */
#define cg_threadlist_remove(thread) cg_list_remove((CGList*)thread)

/**

 * Start all threads in the thread list
 *
 * \param threadList The thread list in question
 */
bool cg_threadlist_start(CGThreadList* threadList);

/**
 * Stop all threads in the thread list
 *
 * \param threadList The thread list in question
 */
bool cg_threadlist_stop(CGThreadList* threadList);

#ifdef __cplusplus

} /* extern "C" */

#endif

#endif
