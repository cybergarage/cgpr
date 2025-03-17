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

#include <cgpr/util/thread.h>

/****************************************
 * cg_threadlist_new
 ****************************************/

CGThreadList* cg_threadlist_new(void)
{
  CGThreadList* threadList;

  threadList = (CGThreadList*)malloc(sizeof(CGThreadList));

  if (!threadList)
    return NULL;

  cg_list_header_init((CGList*)threadList);
  threadList->runnableFlag = false;
  threadList->action = NULL;
  threadList->userData = NULL;

  return threadList;
}

/****************************************
 * cg_threadlist_delete
 ****************************************/

void cg_threadlist_delete(CGThreadList* threadList)
{
  if (!threadList)
    return;

  cg_threadlist_clear(threadList);
  free(threadList);
}

/****************************************
 * cg_threadlist_start
 ****************************************/

bool cg_threadlist_start(CGThreadList* threadList)
{
  CGThreadList* thread;

  if (!threadList)
    return false;

  for (thread = cg_threadlist_gets(threadList); thread != NULL;
      thread = cg_thread_next(thread)) {
    cg_thread_start(thread);
  }

  return true;
}

/****************************************
 * cg_threadlist_stop
 ****************************************/

bool cg_threadlist_stop(CGThreadList* threadList)
{
  CGThreadList* thread;

  if (!threadList)
    return false;

  for (thread = cg_threadlist_gets(threadList); thread != NULL;
      thread = cg_thread_next(thread))
    cg_thread_stop(thread);

  return true;
}
