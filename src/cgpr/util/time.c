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

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#if defined(WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <cgpr/util/time.h>

/****************************************
 * cg_time_wait
 ****************************************/

void cg_wait(clock_t mtime)
{
#if defined(WIN32)
  Sleep(mtime);
#else
  usleep(((useconds_t)(mtime * 1000)));
#endif
}

/****************************************
 * cg_time_wait
 ****************************************/

void cg_waitrandom(clock_t mtime)
{
  double factor;
  long waitTime;

  factor = (double)rand() / (double)RAND_MAX;
  waitTime = (long)((double)mtime * factor);
  cg_wait(waitTime);
}

/****************************************
 * cg_time_wait
 ****************************************/

clock_t cg_getcurrentsystemtime(void)
{
  return (size_t)(time((time_t*)NULL));
}

/****************************************
 * cg_random
 ****************************************/

float cg_random(void)
{
  static bool seedDone = false;

  if (seedDone == false) {
    srand((int)(cg_getcurrentsystemtime() % INT_MAX));
    seedDone = true;
  }

  return (float)rand() / (float)RAND_MAX;
}
