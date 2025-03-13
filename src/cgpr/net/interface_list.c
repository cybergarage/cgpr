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

#include <cgpr/net/interface.h>
#include <cgpr/util/list.h>

/****************************************
 * cg_net_interfacelist_new
 ****************************************/

CGNetworkInterfaceList* cg_net_interfacelist_new(void)
{
  CGNetworkInterfaceList* netIf_list;

  netIf_list = (CGNetworkInterfaceList*)malloc(sizeof(CGNetworkInterfaceList));
  if (!netIf_list)
    return NULL;

  cg_list_header_init((CGList*)netIf_list);
  netIf_list->name = NULL;
  netIf_list->ipaddr = NULL;

  return netIf_list;
}

/****************************************
 * cg_net_interfacelist_delete
 ****************************************/

void cg_net_interfacelist_delete(CGNetworkInterfaceList* netIf_list)
{
  cg_net_interfacelist_clear(netIf_list);
  free(netIf_list);
}
