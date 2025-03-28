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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <cgpr/util/_log.h>
#include <cgpr/util/mutex.h>

#if defined(WIN32)
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif

static CGLogLevel _gLogLevel = CG_LOG_NONE;
static const char* _gLogErrorS = CG_LOG_ERROR_S;
static const char* _gLogWarningS = CG_LOG_WARN_S;
static const char* _gLogInfoS = CG_LOG_INFO_S;
static const char* _gLogDebugS = CG_LOG_DEBUG_S;

static const char* cg_log_type2string(int type)
{
  switch (type) {
  case CG_LOG_ERROR:
    return _gLogErrorS;
    break;

  case CG_LOG_WARN:
    return _gLogWarningS;
    break;

  case CG_LOG_INFO:
    return _gLogInfoS;
    break;

  case CG_LOG_DEBUG:
    return _gLogDebugS;
    break;
  }

  return "";
}

void cg_log_setlevel(CGLogLevel level) { _gLogLevel = level; }

void cg_log_output(int severity, const char* file, int lineN, const char* function, const char* format, ...)
{
  va_list list;

  char msg[MAX_LOG_BUF], tsPrefix[MAX_LOG_BUF];
  time_t ts;
  struct tm* localts;
  size_t prefixLen = -1;

  if (_gLogLevel < severity)
    return;

  ts = time(NULL);
  localts = localtime(&ts);

  strftime(tsPrefix, MAX_LOG_BUF, "%F %T", localts);

  prefixLen = snprintf(msg, MAX_LOG_BUF, "%s : %s ", tsPrefix, cg_log_type2string(severity));

  va_start(list, format);
  vsnprintf(msg + prefixLen, MAX_LOG_BUF - prefixLen, format, list);
  va_end(list);

  printf("%s\n", msg);
}
