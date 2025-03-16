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
  CGThreadList* thread_list;

  thread_list = (CGThreadList*)malloc(sizeof(CGThreadList));

  if (!thread_list)
    return NULL;

  cg_list_header_init((CGList*)thread_list);
  thread_list->runnableFlag = false;
  thread_list->action = NULL;
  thread_list->userData = NULL;

  return thread_list;
}

/****************************************
 * cg_threadlist_delete
 ****************************************/

void cg_threadlist_delete(CGThreadList* thread_list)
{
  if (!thread_list)
    return;

  cg_threadlist_clear(thread_list);
  free(thread_list);
}

/****************************************
 * cg_threadlist_start
 ****************************************/

bool cg_threadlist_start(CGThreadList* thread_list)
{
  CGThreadList* thread;

  if (!thread_list)
    return false;

  for (thread = cg_threadlist_gets(thread_list); thread != NULL;
       thread = cg_thread_next(thread)) {
    cg_thread_start(thread);
  }

  return true;
}

/****************************************
 * cg_threadlist_stop
 ****************************************/

bool cg_threadlist_stop(CGThreadList* thread_list)
{
  CGThreadList* thread;

  if (!thread_list)
    return false;

  for (thread = cg_threadlist_gets(thread_list); thread != NULL;
       thread = cg_thread_next(thread))
    cg_thread_stop(thread);

  return true;
}
