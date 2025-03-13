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

#include <cgpr/util/_log.h>
#include <cgpr/util/logs.h>
#include <stdio.h>
#include <string.h>

/****************************************
 * cg_net_socket_log
 ****************************************/

void cg_net_socket_log(int severity, const char* prefix, const char* from_addr, const char* to_addr, const byte* msg_bytes, size_t msg_len)
{
  char buf[256];
  size_t offset;
  size_t n;

  if (msg_len <= 0)
    return;

  offset = 0;
  if (prefix && (0 < strlen(prefix))) {
    snprintf(buf, sizeof(buf), "%s ", prefix);
    offset = strlen(buf);
  }
  snprintf((buf + offset), (sizeof(buf) - offset), "%-15s -> %-15s ", from_addr, to_addr);
  offset = strlen(buf);
  for (n = 0; n < msg_len; n++) {
    snprintf((buf + offset), (sizeof(buf) - offset), "%02X", msg_bytes[n]);
    offset += 2;
  }

  cg_log_debug("%s", buf);
}

/****************************************
 * cg_net_socket_*
 ****************************************/

void cg_net_socket_debug(const char* prefix, const char* from_addr, const char* to_addr, const byte* pkt_bytes, size_t pkt_len)
{
  return cg_net_socket_log(CG_LOG_DEBUG, prefix, from_addr, to_addr, pkt_bytes, pkt_len);
}

void cg_net_socket_error(const char* prefix, const char* from_addr, const char* to_addr, const byte* pkt_bytes, size_t pkt_len)
{
  return cg_net_socket_log(CG_LOG_ERROR, prefix, from_addr, to_addr, pkt_bytes, pkt_len);
}

/****************************************
 * cg_net_datagram_packet_*
 ****************************************/

// void cg_net_datagram_packet_error(const char* prefix, CGDatagramPacket* dpkt)
// {
//   if (!dpkt)
//     return;

//   return cg_net_socket_error(
//       prefix,
//       cg_socket_datagram_packet_getremoteaddress(dpkt),
//       cg_socket_datagram_packet_getlocaladdress(dpkt),
//       cg_socket_datagram_packet_getdata(dpkt),
//       cg_socket_datagram_packet_getlength(dpkt));
// }

// void cg_net_datagram_packet_debug(const char* prefix, CGDatagramPacket* dpkt)
// {
//   if (!dpkt)
//     return;

//   return cg_net_socket_debug(
//       prefix,
//       cg_socket_datagram_packet_getremoteaddress(dpkt),
//       cg_socket_datagram_packet_getlocaladdress(dpkt),
//       cg_socket_datagram_packet_getdata(dpkt),
//       cg_socket_datagram_packet_getlength(dpkt));
// }

/****************************************
 * cg_message_error
 ****************************************/

// void cg_message_error(const char* prefix, CGMessage* msg)
// {
//   if (!msg)
//     return;

//   return cg_net_socket_log(
//       CG_LOG_ERROR,
//       prefix,
//       cg_message_getsourceaddress(msg),
//       cg_message_getdestinationaddress(msg),
//       cg_message_getbytes(msg),
//       cg_message_size(msg));
// }
