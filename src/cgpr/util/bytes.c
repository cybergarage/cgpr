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

#include <cgpr/util/bytes.h>

/****************************************
 * cg_bytes_toint
 ****************************************/

int cg_bytes_toint(byte* byteData, size_t byteSize)
{
  int idx;
  int n;
  int val = 0;

  for (n = 0; n < (int)byteSize; n++) {
    idx = ((int)byteSize - 1) - n;
    val += byteData[idx] << (n * 8);
  }

  return val;
}

/****************************************
 * cg_bytes_fromint
 ****************************************/

byte* cg_bytes_fromint(int val, size_t bytesSize)
{
  byte* bytesData;
  int idx;
  int n;

  bytesData = malloc(bytesSize);
  for (n = 0; n < (int)bytesSize; n++) {
    idx = ((int)bytesSize - 1) - n;
    bytesData[idx] = ((val >> (n * 8)) & 0xFF);
  }

  return bytesData;
}
