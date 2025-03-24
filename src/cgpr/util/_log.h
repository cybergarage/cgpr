
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

#ifndef __CGPR_UTIL_LOG_H_
#define __CGPR_UTIL_LOG_H_

#include <cgpr/util/log.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LOG_BUF 2048

#define CG_LOG_ERROR_S "ERROR"
#define CG_LOG_WARN_S "WARN"
#define CG_LOG_INFO_S "INFO"
#define CG_LOG_DEBUG_S "DEBUG"

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __CGPR_UTIL_LOG_H_
