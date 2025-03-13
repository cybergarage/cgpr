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

#ifndef _CGPR_UTIL_LOG_H_
#define _CGPR_UTIL_LOG_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  CG_LOG_NONE = 0x00,
  CG_LOG_ERROR = 0x01,
  CG_LOG_WARN = 0x02,
  CG_LOG_INFO = 0x04,
  CG_LOG_DEBUG = 0x08,
  CG_LOG_ALL = 0xFF,
} CGLogLevel;

void cg_log_setlevel(CGLogLevel level);

#if defined(__USE_ISOC99)
#define cg_log(severity, format, ...) \
  cg_log_output(severity, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#define cg_log_error(format, ...) \
  cg_log_output(CG_LOG_ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#define cg_log_warn(format, ...) \
  cg_log_output(CG_LOG_WARN, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#define cg_log_info(format, ...) \
  cg_log_output(CG_LOG_INFO, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#define cg_log_debug(format, ...) \
  cg_log_output(CG_LOG_DEBUG, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#else
#define cg_log(severity, format...) \
  cg_log_output(severity, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#define cg_log_error(format...) \
  cg_log_output(CG_LOG_ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#define cg_log_warn(format...) \
  cg_log_output(CG_LOG_WARN, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#define cg_log_info(format...) \
  cg_log_output(CG_LOG_INFO, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#define cg_log_debug(format...) \
  cg_log_output(CG_LOG_DEBUG, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#endif

void cg_log_output(int severity, const char* file, int line_n, const char* function, const char* format, ...);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _CGPR_UTIL_LOG_H_ */
