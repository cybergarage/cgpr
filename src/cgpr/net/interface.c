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
#include <string.h>

/****************************************
 * cg_net_interface_new
 ****************************************/

CGNetworkInterface* cg_net_interface_new(void)
{
  CGNetworkInterface* netIf;

  netIf = (CGNetworkInterface*)malloc(sizeof(CGNetworkInterface));

  if (!netIf)
    return NULL;

  cg_list_node_init((CGList*)netIf);
  netIf->name = cg_string_new();
  netIf->ipaddr = cg_string_new();
  netIf->netmask = cg_string_new();
  cg_net_interface_setindex(netIf, 0);
  memset(netIf->macaddr, 0, (size_t)CG_NET_MACADDR_SIZE);

  return netIf;
}

/****************************************
 * cg_net_interface_delete
 ****************************************/

void cg_net_interface_delete(CGNetworkInterface* netIf)
{
  if (!netIf)
    return;

  cg_list_remove((CGList*)netIf);
  cg_string_delete(netIf->name);
  cg_string_delete(netIf->ipaddr);
  cg_string_delete(netIf->netmask);
  free(netIf);
}

/****************************************
 * cg_net_interface_getany
 ****************************************/

CGNetworkInterface* cg_net_interface_getany(void)
{
  CGNetworkInterface* netIf;

  netIf = cg_net_interface_new();
  cg_net_interface_setname(netIf, "INADDR_ANY");
  cg_net_interface_setaddress(netIf, "0.0.0.0");

  return netIf;
}

/****************************************
 * cg_net_interface_setname
 ****************************************/

void cg_net_interface_setname(CGNetworkInterface* netIf, char* name)
{
  if (!netIf)
    return;

  cg_string_setvalue(netIf->name, name);
}

/****************************************
 * cg_net_interface_getname
 ****************************************/

char* cg_net_interface_getname(CGNetworkInterface* netIf)
{
  if (!netIf)
    return NULL;

  return cg_string_getvalue(netIf->name);
}

/****************************************
 * cg_net_interface_setaddress
 ****************************************/

void cg_net_interface_setaddress(CGNetworkInterface* netIf, char* value)
{
  if (!netIf)
    return;

  cg_string_setvalue(netIf->ipaddr, value);
}

/****************************************
 * cg_net_interface_getaddress
 ****************************************/

char* cg_net_interface_getaddress(CGNetworkInterface* netIf)
{
  if (!netIf)
    return NULL;

  return cg_string_getvalue(netIf->ipaddr);
}

/****************************************
 * cg_net_interface_setnetmask
 ****************************************/

void cg_net_interface_setnetmask(CGNetworkInterface* netIf, char* value)
{
  if (!netIf)
    return;

  cg_string_setvalue(netIf->netmask, value);
}

/****************************************
 * cg_net_interface_getnetmask
 ****************************************/

char* cg_net_interface_getnetmask(CGNetworkInterface* netIf)
{
  if (!netIf)
    return NULL;

  return cg_string_getvalue(netIf->netmask);
}
