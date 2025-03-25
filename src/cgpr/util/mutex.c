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

#include <errno.h>
#include <stdlib.h>

#include <cgpr/util/mutex.h>

/****************************************
 * cg_mutex_new
 ****************************************/

CGMutex* cg_mutex_new(void)
{
  CGMutex* mutex;

  mutex = (CGMutex*)malloc(sizeof(CGMutex));

  if (!mutex)
    return NULL;

#if defined(WIN32)
  mutex->mutexId = CreateMutex(NULL, false, NULL);
#else
  pthread_mutex_init(&mutex->mutexId, NULL);
#endif

  return mutex;
}

/****************************************
 * cg_mutex_delete
 ****************************************/

bool cg_mutex_delete(CGMutex* mutex)
{
  if (!mutex)
    return false;

#if defined(WIN32)
  CloseHandle(mutex->mutexId);
#else
  pthread_mutex_destroy(&mutex->mutexId);
#endif
  free(mutex);

  return true;
}

/****************************************
 * cg_mutex_lock
 ****************************************/

bool cg_mutex_lock(CGMutex* mutex)
{
  if (!mutex)
    return false;

#if defined(WIN32)
  WaitForSingleObject(mutex->mutexId, INFINITE);
#else
  pthread_mutex_lock(&mutex->mutexId);
#endif

  return true;
}

/****************************************
 * cg_mutex_unlock
 ****************************************/

bool cg_mutex_unlock(CGMutex* mutex)
{
  if (!mutex)
    return false;

#if defined(WIN32)
  ReleaseMutex(mutex->mutexId);
#else
  pthread_mutex_unlock(&mutex->mutexId);
#endif
  return true;
}
