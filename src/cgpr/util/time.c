/******************************************************************
 *
 * mUPnP for C
 *
 * Copyright (C) Satoshi Konno 2005
 * Copyright (C) 2006 Nokia Corporation. All rights reserved.
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <stdlib.h>
#include <limits.h>

#include <cgpr/util/log.h>
#include <cgpr/util/time.h>

#if defined(WIN32) && !defined(ITRON) && !defined(WINCE)
#include <windows.h>
#elif defined(WIN32) && defined(WINCE)
#include <windows.h>
#elif defined(BTRON)
#include <btron/clk.h>
#include <btron/proctask.h>
#elif defined(ITRON)
#include <kernel.h>
#elif defined(TENGINE) && !defined(PROCESS_BASE)
#include <tk/tkernel.h>
#elif defined(TENGINE) && defined(PROCESS_BASE)
#include <btron/proctask.h>
#include <tk/tkernel.h>
#else
#include <unistd.h>
#endif

/****************************************
 * cg_time_wait
 ****************************************/

void cg_wait(CGTime mtime)
{
#if defined(WIN32) && !defined(ITRON)
  Sleep(mtime);
#elif defined(BTRON)
  slp_tsk(mtime);
#elif defined(ITRON)
  tslp_tsk(mtime);
#elif defined(TENGINE) && !defined(PROCESS_BASE)
  tk_slp_tsk(mtime);
#elif defined(TENGINE) && defined(PROCESS_BASE)
  b_slp_tsk(mtime);
#else
  usleep(((useconds_t)(mtime * 1000)));
#endif
}

/****************************************
 * cg_waitrandom
 ****************************************/

void cg_waitrandom(CGTime mtime)
{
  double factor;
  long waitTime;

  factor = (double)rand() / (double)RAND_MAX;
  waitTime = (long)((double)mtime * factor);
  cg_wait(waitTime);
}

/****************************************
 * cg_getcurrentsystemtime
 ****************************************/

CGTime cg_getcurrentsystemtime(void)
{
#if defined(BTRON)
  STIME CGTime;
  TIMEZONE tz;
  STIME localtime;
  if (get_tim(&CGTime, &tz) != 0)
    return 0;
  localtime = CGTime - tz.adjust + (tz.dst_flg ? (tz.dst_adj * 60) : 0);
#elif defined(ITRON)
  static bool initialized = false;
  SYSTIM sysTim;
  if (initialized == false) {
    sysTim.utime = 0;
    sysTim.ltime = 0;
    set_tim(&sysTim);
  }
  get_tim(&sysTim);
#endif

#if defined(BTRON)
  return localtime;
#elif defined(ITRON)
  return ((sysTim.utime / 1000) << 32) + (sysTim.ltime / 1000);
#else
  return time((time_t*)NULL);
#endif
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
