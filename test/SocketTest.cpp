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

#include <boost/test/unit_test.hpp>

#include <cgpr/net/interface.h>
#include <cgpr/net/socket.h>

BOOST_AUTO_TEST_CASE(BindAddrTest)
{
  int cgUdpPort = 1900;

  CGNetworkInterfaceList* netIfList = cg_net_interfacelist_new();

  BOOST_REQUIRE(0 < cg_net_gethostinterfaces(netIfList));

  if (cg_net_gethostinterfaces(netIfList) <= 0) {
    cg_net_interfacelist_delete(netIfList);
    return;
  }

  CGNetworkInterface* netIf = cg_net_interfacelist_gets(netIfList);
  const char* bindAddr = cg_net_interface_getaddress(netIf);

  CGSocket* sock = cg_socket_dgram_new();
  CGSocketOption* opt = cg_socket_option_new();

  // Unicast binding

  cg_socket_option_setbindinterface(opt, true);
  cg_socket_option_setreuseaddress(opt, true);
  cg_socket_option_setmulticastloop(opt, false);

  BOOST_REQUIRE(cg_socket_bind(sock, cgUdpPort, bindAddr, opt));
  BOOST_REQUIRE(cg_socket_close(sock));

  // Multicast binding

  cg_socket_option_setbindinterface(opt, false);
  cg_socket_option_setreuseaddress(opt, true);
  cg_socket_option_setmulticastloop(opt, true);

  BOOST_REQUIRE(cg_socket_bind(sock, cgUdpPort, bindAddr, opt));
  BOOST_REQUIRE(cg_socket_close(sock));

  cg_socket_option_delete(opt);
  cg_socket_delete(sock);

  cg_net_interfacelist_delete(netIfList);
}
