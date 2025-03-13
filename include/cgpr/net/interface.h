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

#ifndef _CGPR_NET_CINTERFACE_H_
#define _CGPR_NET_CINTERFACE_H_

#include <cgpr/typedef.h>
#include <cgpr/util/list.h>
#include <cgpr/util/strings.h>

#if !defined(WIN32)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#define CG_NET_IPV4_ADDRSTRING_MAXSIZE ((3 * 4) + (1 * 3) + 1)
#define CG_NET_IPV6_ADDRSTRING_MAXSIZE (1 + (8 * 4) + (1 * 7) + 1 + 1)

#define CG_NET_IPV4_LOOPBACK "127.0.0.1"
#define CG_NET_IPV6_LOOPBACK "fixmelater"
#define CG_NET_MACADDR_SIZE 6

/****************************************
 * Data Type
 ****************************************/

typedef struct {
  CG_LIST_STRUCT_MEMBERS

  CGString* name;
  CGString* ipaddr;
  CGString* netmask;
  byte macaddr[CG_NET_MACADDR_SIZE];
  int index;
} CGNetworkInterface, CGNetworkInterfaceList;

/****************************************
 * Function (NetworkInterface)
 ****************************************/

CGNetworkInterface* cg_net_interface_new(void);
void cg_net_interface_delete(CGNetworkInterface* netIf);
CGNetworkInterface* cg_net_interface_getany(void);

#define cg_net_interface_next(netIf) (CGNetworkInterface*)cg_list_next((CGList*)netIf)
#define cg_net_interface_remove(netIf) cg_list_remove((CGList*)netIf)

void cg_net_interface_setname(CGNetworkInterface* netIf, char* name);
char* cg_net_interface_getname(CGNetworkInterface* netIf);
void cg_net_interface_setaddress(CGNetworkInterface* netIf, char* ipaddr);
char* cg_net_interface_getaddress(CGNetworkInterface* netIf);
void cg_net_interface_setnetmask(CGNetworkInterface* netIf, char* ipaddr);
char* cg_net_interface_getnetmask(CGNetworkInterface* netIf);
char* cg_net_selectaddr(struct sockaddr* remoteaddr);

#define cg_net_interface_setmacaddress(netIf, value) memcpy(netIf->macaddr, value, CG_NET_MACADDR_SIZE)
#define cg_net_interface_getmacaddress(netIf, buf) memcpy(buf, netIf->macaddr, CG_NET_MACADDR_SIZE)

#define cg_net_interface_setindex(netIf, value) (netIf->index = value)
#define cg_net_interface_getindex(netIf, buf) (netIf->index)

/****************************************
 * Function (NetworkInterfaceList)
 ****************************************/

CGNetworkInterfaceList* cg_net_interfacelist_new(void);
void cg_net_interfacelist_delete(CGNetworkInterfaceList* netIf_list);

#define cg_net_interfacelist_clear(netIfList) cg_list_clear((CGList*)netIfList, (CG_LIST_DESTRUCTORFUNC)cg_net_interface_delete)
#define cg_net_interfacelist_size(netIfList) cg_list_size((CGList*)netIfList)
#define cg_net_interfacelist_gets(netIfList) (CGNetworkInterface*)cg_list_next((CGList*)netIfList)
#define cg_net_interfacelist_add(netIfList, netIf) cg_list_add((CGList*)netIfList, (CGList*)netIf)

/****************************************
 * Function
 ****************************************/

size_t cg_net_gethostinterfaces(CGNetworkInterfaceList* netIf_list);

bool cg_net_isipv6address(const char* addr);
int cg_net_getipv6scopeid(const char* addr);

#ifdef __cplusplus
}
#endif

#endif
