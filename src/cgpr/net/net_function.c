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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cgpr/net/interface.h>

/****************************************
 * global variables
 ****************************************/

static bool cgNetUseOnlyIpV4Addr = false;
static bool cgNetUseOnlyIpV6Addr = false;

////////////////////////////////////////////////
// cg_net_isuseaddress
////////////////////////////////////////////////

bool cg_net_isuseaddress(char* addr)
{
  if (cgNetUseOnlyIpV6Addr == true) {
    if (cg_net_isipv6address(addr) == false)
      return false;
  }
  if (cgNetUseOnlyIpV4Addr == true) {
    if (cg_net_isipv6address(addr) == true)
      return false;
  }
  return true;
}

////////////////////////////////////////////////
// cg_net_getipv6address
////////////////////////////////////////////////

bool cg_net_isipv6address(const char* addr)
{
  if (addr == NULL)
    return false;

  if (0 < cg_strchr(addr, ":", 1))
    return true;

  return false;
}

////////////////////////////////////////////////
// cg_net_getipv6address
////////////////////////////////////////////////

int cg_net_getipv6scopeid(const char* addr)
{
  ssize_t perIdx;
  char scopeIdBuf[8 + 1];

  if (cg_net_isipv6address(addr) == false)
    return 0;

  perIdx = cg_strchr(addr, "%", 1);
  if (perIdx < 0)
    return 0;

  cg_strncpy(scopeIdBuf, (addr + perIdx + 1), sizeof(scopeIdBuf) - 1);
  scopeIdBuf[sizeof(scopeIdBuf) - 1] = '\0';

  return atoi(scopeIdBuf);
}
