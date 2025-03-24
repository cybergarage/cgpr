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

#include <string.h>

#include <cgpr/net/socket.h>

/****************************************
 * cg_socket_datagram_packet_new
 ****************************************/

CGDatagramPacket* cg_socket_datagram_packet_new(void)
{
  CGDatagramPacket* dgm_pkt;

  dgm_pkt = (CGDatagramPacket*)malloc(sizeof(CGDatagramPacket));

  if (!dgm_pkt)
    return NULL;

  dgm_pkt->data = NULL;
  dgm_pkt->dataLen = 0;

  dgm_pkt->localAddr = cg_string_new();
  cg_socket_datagram_packet_setlocalport(dgm_pkt, 0);

  dgm_pkt->remoteAddr = cg_string_new();
  cg_socket_datagram_packet_setremoteport(dgm_pkt, 0);

  return dgm_pkt;
}

/****************************************
 * cg_socket_datagram_packet_delete
 ****************************************/

void cg_socket_datagram_packet_delete(CGDatagramPacket* dgm_pkt)
{
  if (!dgm_pkt)
    return;

  cg_socket_datagram_packet_clear(dgm_pkt);

  cg_string_delete(dgm_pkt->localAddr);
  cg_string_delete(dgm_pkt->remoteAddr);

  free(dgm_pkt);
}

/****************************************
 * cg_socket_datagram_packet_setdata
 ****************************************/

bool cg_socket_datagram_packet_setdata(CGDatagramPacket* dgm_pkt, const byte* data, size_t dataLen)
{
  if (!dgm_pkt)
    return false;

  cg_socket_datagram_packet_clear(dgm_pkt);

  if (!data || (dataLen <= 0))
    return true;

  dgm_pkt->data = malloc(dataLen);
  if (!dgm_pkt->data)
    return false;

  memcpy(dgm_pkt->data, data, dataLen);
  dgm_pkt->dataLen = dataLen;

  return true;
}

/****************************************
 * cg_socket_datagram_packet_clear
 ****************************************/

bool cg_socket_datagram_packet_clear(CGDatagramPacket* dgm_pkt)
{
  if (!dgm_pkt)
    return false;

  if (dgm_pkt->data) {
    free(dgm_pkt->data);
    dgm_pkt->data = NULL;
  }
  dgm_pkt->dataLen = 0;

  return true;
}

/****************************************
 * cg_socket_datagram_packet_copy
 ****************************************/

bool cg_socket_datagram_packet_copy(CGDatagramPacket* dst_dgm_pkt, CGDatagramPacket* src_dgm_pkt)
{
  if (!dst_dgm_pkt || !src_dgm_pkt)
    return false;

  cg_socket_datagram_packet_setdata(dst_dgm_pkt, cg_socket_datagram_packet_getdata(src_dgm_pkt), cg_socket_datagram_packet_getlength(src_dgm_pkt));
  cg_socket_datagram_packet_setlocalAddr(dst_dgm_pkt, cg_socket_datagram_packet_getlocalAddr(src_dgm_pkt));
  cg_socket_datagram_packet_setlocalport(dst_dgm_pkt, cg_socket_datagram_packet_getlocalport(src_dgm_pkt));
  cg_socket_datagram_packet_setremoteAddr(dst_dgm_pkt, cg_socket_datagram_packet_getremoteAddr(src_dgm_pkt));
  cg_socket_datagram_packet_setremoteport(dst_dgm_pkt, cg_socket_datagram_packet_getremoteport(src_dgm_pkt));

  return true;
}
