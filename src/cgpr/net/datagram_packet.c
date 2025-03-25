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
  CGDatagramPacket* dgmPkt;

  dgmPkt = (CGDatagramPacket*)malloc(sizeof(CGDatagramPacket));

  if (!dgmPkt)
    return NULL;

  dgmPkt->data = NULL;
  dgmPkt->dataLen = 0;

  dgmPkt->localAddr = cg_string_new();
  cg_socket_datagram_packet_setlocalport(dgmPkt, 0);

  dgmPkt->remoteAddr = cg_string_new();
  cg_socket_datagram_packet_setremoteport(dgmPkt, 0);

  return dgmPkt;
}

/****************************************
 * cg_socket_datagram_packet_delete
 ****************************************/

void cg_socket_datagram_packet_delete(CGDatagramPacket* dgmPkt)
{
  if (!dgmPkt)
    return;

  cg_socket_datagram_packet_clear(dgmPkt);

  cg_string_delete(dgmPkt->localAddr);
  cg_string_delete(dgmPkt->remoteAddr);

  free(dgmPkt);
}

/****************************************
 * cg_socket_datagram_packet_setdata
 ****************************************/

bool cg_socket_datagram_packet_setdata(CGDatagramPacket* dgmPkt, const byte* data, size_t dataLen)
{
  if (!dgmPkt)
    return false;

  cg_socket_datagram_packet_clear(dgmPkt);

  if (!data || (dataLen <= 0))
    return true;

  dgmPkt->data = malloc(dataLen);
  if (!dgmPkt->data)
    return false;

  memcpy(dgmPkt->data, data, dataLen);
  dgmPkt->dataLen = dataLen;

  return true;
}

/****************************************
 * cg_socket_datagram_packet_clear
 ****************************************/

bool cg_socket_datagram_packet_clear(CGDatagramPacket* dgmPkt)
{
  if (!dgmPkt)
    return false;

  if (dgmPkt->data) {
    free(dgmPkt->data);
    dgmPkt->data = NULL;
  }
  dgmPkt->dataLen = 0;

  return true;
}

/****************************************
 * cg_socket_datagram_packet_copy
 ****************************************/

bool cg_socket_datagram_packet_copy(CGDatagramPacket* dstDgmPkt, CGDatagramPacket* srcDgmPkt)
{
  if (!dstDgmPkt || !srcDgmPkt)
    return false;

  cg_socket_datagram_packet_setdata(dstDgmPkt, cg_socket_datagram_packet_getdata(srcDgmPkt), cg_socket_datagram_packet_getlength(srcDgmPkt));
  cg_socket_datagram_packet_setlocalAddr(dstDgmPkt, cg_socket_datagram_packet_getlocalAddr(srcDgmPkt));
  cg_socket_datagram_packet_setlocalport(dstDgmPkt, cg_socket_datagram_packet_getlocalport(srcDgmPkt));
  cg_socket_datagram_packet_setremoteAddr(dstDgmPkt, cg_socket_datagram_packet_getremoteAddr(srcDgmPkt));
  cg_socket_datagram_packet_setremoteport(dstDgmPkt, cg_socket_datagram_packet_getremoteport(srcDgmPkt));

  return true;
}
