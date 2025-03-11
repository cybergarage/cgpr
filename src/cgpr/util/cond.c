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

#include <cgpr/util/cond.h>

#include <errno.h>

/****************************************
 * cg_cond_new
 ****************************************/

CGCond* cg_cond_new(void)
{
  CGCond* cond;

  cond = (CGCond*)malloc(sizeof(CGCond));

  if (!cond)
    return NULL;

  pthread_mutex_init(&cond->mutexID, NULL);
  pthread_cond_init(&cond->cond_id, NULL);

  return cond;
}

/****************************************
 * cg_cond_delete
 ****************************************/

bool cg_cond_delete(CGCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_destroy(&cond->mutexID);
  pthread_cond_destroy(&cond->cond_id);

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

  pthread_mutex_lock(&cond->mutexID);
  pthread_cond_wait(&cond->cond_id, &cond->mutexID);
  pthread_mutex_unlock(&cond->mutexID);

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

  pthread_mutex_lock(&cond->mutexID);
  c = pthread_cond_timedwait(&cond->cond_id, &cond->mutexID, &to);
  pthread_mutex_unlock(&cond->mutexID);

  return (c == 0) ? true : false;
}

/****************************************
 * cg_cond_signal
 ****************************************/

bool cg_cond_signal(CGCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_lock(&cond->mutexID);
  pthread_cond_signal(&cond->cond_id);
  pthread_mutex_unlock(&cond->mutexID);

  return true;
}
