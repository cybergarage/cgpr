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

#ifndef _CGPR_UTIL_BYTES_H_
#define _CGPR_UTIL_BYTES_H_

#include <cgpr/util/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Function
 ****************************************/

int cg_bytes_toint(byte* byteData, size_t byteSize);
byte* cg_bytes_fromint(int val, size_t bytesSize);

#ifdef __cplusplus
}
#endif

#endif // _CGPR_UTIL_BYTES_H_
