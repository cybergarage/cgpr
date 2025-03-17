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

#ifndef _CGPR_LOG_FUNC_H_
#define _CGPR_LOG_FUNC_H_

#include <cgpr/typedef.h>
// #include <cgpr/_message.h>

#define CG_LOG_NET_PREFIX_SEND "S"
#define CG_LOG_NET_PREFIX_RECV "R"

void cg_net_socket_error(const char* prefix, const char* fromAddr, const char* toAddr, const byte* pktBytes, size_t pktLen);
void cg_net_socket_debug(const char* prefix, const char* fromAddr, const char* toAddr, const byte* pktBytes, size_t pktLen);

// void cg_net_datagram_packet_error(const char* prefix, CGDatagramPacket* dpkt);
// void cg_net_datagram_packet_debug(const char* prefix, CGDatagramPacket* dpkt);

// void cg_message_error(const char* prefix, CGMessage* msg);

#endif /* _CGPR_LOG_FUNC_H_ */
