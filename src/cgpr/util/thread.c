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

#if !defined(WIN32)
#include <signal.h>
#endif

#include <cgpr/util/thread.h>
#include <cgpr/util/timer.h>
#include <string.h>

static void cg_sig_handler(int sign);

/****************************************
 * Thread Function
 ****************************************/

#if defined(WIN32)
static DWORD WINAPI Win32ThreadProc(LPVOID lpParam)
{
  CGThread* thread;

  thread = (CGThread*)lpParam;
  if (thread->action != NULL)
    thread->action(thread);

  return 0;
}
#else
static void* posix_thread_proc(void* param)
{
  sigset_t set;
  struct sigaction actions;
  CGThread* thread = (CGThread*)param;

  sigfillset(&set);
  sigdelset(&set, SIGQUIT);
  pthread_sigmask(SIG_SETMASK, &set, NULL);

  memset(&actions, 0, sizeof(actions));
  sigemptyset(&actions.sa_mask);
  actions.sa_flags = 0;
  actions.sa_handler = cg_sig_handler;
  sigaction(SIGQUIT, &actions, NULL);

  if (thread->action != NULL)
    thread->action(thread);

  return 0;
}
#endif

/****************************************
 * cg_thread_new
 ****************************************/

CGThread* cg_thread_new(void)
{
  CGThread* thread;

  thread = (CGThread*)malloc(sizeof(CGThread));

  if (!thread)
    return NULL;

  cg_list_node_init((CGList*)thread);

  thread->runnableFlag = false;
  thread->action = NULL;
  thread->userData = NULL;

  return thread;
}

/****************************************
 * cg_thread_delete
 ****************************************/

bool cg_thread_delete(CGThread* thread)
{
  if (!thread)
    return false;

  if (thread->runnableFlag == true) {
    cg_thread_stop(thread);
  }

  cg_thread_remove(thread);

  free(thread);

  return true;
}

/****************************************
 * cg_thread_start
 ****************************************/

bool cg_thread_start(CGThread* thread)
{
  if (!thread)
    return false;

  thread->runnableFlag = true;

#if defined(WIN32)
  thread->hThread = CreateThread(NULL, 0, Win32ThreadProc, (LPVOID)thread, 0, &thread->threadID);
#else
  pthread_attr_t thread_attr;
  if (pthread_attr_init(&thread_attr) != 0) {
    thread->runnableFlag = false;
    return false;
  }

  if (pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) != 0) {
    thread->runnableFlag = false;
    pthread_attr_destroy(&thread_attr);
    return false;
  }

  if (pthread_create(&thread->pThread, &thread_attr, posix_thread_proc, thread) != 0) {
    thread->runnableFlag = false;
    pthread_attr_destroy(&thread_attr);
    return false;
  }
  pthread_attr_destroy(&thread_attr);
#endif

  return true;
}

/****************************************
 * cg_thread_stop
 ****************************************/

bool cg_thread_stop(CGThread* thread)
{
  if (!thread)
    return false;

  if (thread->runnableFlag == true) {
    thread->runnableFlag = false;
#if defined(WIN32)
    TerminateThread(thread->hThread, 0);
    WaitForSingleObject(thread->hThread, INFINITE);
#else
    pthread_kill(thread->pThread, 0);
    /* Now we wait one second for thread termination instead of using
     * pthread_join */
    cg_sleep(CG_THREAD_MIN_SLEEP);
#endif
  }

  return true;
}

/****************************************
 * cg_thread_restart
 ****************************************/

bool cg_thread_restart(CGThread* thread)
{
  cg_thread_stop(thread);
  return cg_thread_start(thread);
}

/****************************************
 * cg_thread_isrunnable
 ****************************************/

bool cg_thread_isrunnable(CGThread* thread)
{
  if (!thread)
    return false;

#if !defined(WIN32)
  pthread_testcancel();
#endif

  return thread->runnableFlag;
}

/****************************************
 * cg_thread_isrunning
 ****************************************/

bool cg_thread_isrunning(CGThread* thread)
{
  if (!thread)
    return false;

  return thread->runnableFlag;
}

/****************************************
 * cg_thread_setaction
 ****************************************/

void cg_thread_setaction(CGThread* thread, CG_THREAD_FUNC func)
{
  if (!thread)
    return;

  thread->action = func;
}

/****************************************
 * cg_thread_setuserdata
 ****************************************/

void cg_thread_setuserdata(CGThread* thread, void* value)
{
  if (!thread)
    return;

  thread->userData = value;
}

/****************************************
 * cg_thread_getuserdata
 ****************************************/

void* cg_thread_getuserdata(CGThread* thread)
{
  if (!thread)
    return NULL;

  return thread->userData;
}

/****************************************
 * cg_sig_handler
 ****************************************/

static void cg_sig_handler(int sign) { }
