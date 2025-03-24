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

#include <stdlib.h>
#include <errno.h>

#include <cgpr/util/cond.h>

/****************************************
 * cg_cond_new
 ****************************************/

CGCond* cg_cond_new(void)
{
  CGCond* cond;

  cond = (CGCond*)malloc(sizeof(CGCond));

  if (!cond)
    return NULL;

  pthread_mutex_init(&cond->mutexId, NULL);
  pthread_cond_init(&cond->condId, NULL);

  return cond;
}

/****************************************
 * cg_cond_delete
 ****************************************/

bool cg_cond_delete(CGCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_destroy(&cond->mutexId);
  pthread_cond_destroy(&cond->condId);

  free(cond);

  return true;
}

/****************************************
 * cg_cond_wait
 ****************************************/

bool cg_cond_wait(CGCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_lock(&cond->mutexId);
  pthread_cond_wait(&cond->condId, &cond->mutexId);
  pthread_mutex_unlock(&cond->mutexId);

  return true;
}

/****************************************
 * cg_cond_timedwait
 ****************************************/

bool cg_cond_timedwait(CGCond* cond, clock_t mtime)
{
  struct timespec to;
  int c;

  if (!cond)
    return false;

  to.tv_sec = time(NULL) + (mtime / CLOCKS_PER_SEC);
  to.tv_nsec = 0;

  pthread_mutex_lock(&cond->mutexId);
  c = pthread_cond_timedwait(&cond->condId, &cond->mutexId, &to);
  pthread_mutex_unlock(&cond->mutexId);

  return (c == 0) ? true : false;
}

/****************************************
 * cg_cond_signal
 ****************************************/

bool cg_cond_signal(CGCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_lock(&cond->mutexId);
  pthread_cond_signal(&cond->condId);
  pthread_mutex_unlock(&cond->mutexId);

  return true;
}
