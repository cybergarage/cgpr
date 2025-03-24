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

#ifndef _CGPR_NET_CSOCKET_H_
#define _CGPR_NET_CSOCKET_H_

#if defined(CG_USE_OPENSSL)
#include <openssl/ssl.h>
#endif

#include <cgpr/net/socket_opt.h>
#include <cgpr/util/string.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#define CG_NET_SOCKET_NONE 0

#define CG_NET_SOCKET_STREAM 0x01
#define CG_NET_SOCKET_DGRAM 0x02

#define CG_NET_SOCKET_CLIENT 1
#define CG_NET_SOCKET_SERVER 2

#define CG_NET_SOCKET_MAXHOST 32
#define CG_NET_SOCKET_MAXSERV 32

#if !defined(WIN32)
typedef int SOCKET;
#endif

#define CG_SOCKET_LF '\n'

#define CG_NET_SOCKET_DGRAM_RECV_BUFSIZE 512
#define CG_NET_SOCKET_DGRAM_ANCILLARY_BUFSIZE 512
#define CG_NET_SOCKET_MULTICAST_DEFAULT_TTL 4
#define CG_NET_SOCKET_AUTO_IP_NET 0xa9fe0000
#define CG_NET_SOCKET_AUTO_IP_MASK 0xffff0000

/****************************************
 * Data Type
 ****************************************/

#include <cgpr/util/list.h>

typedef struct UEchoSocket {
  SOCKET id;
  int type;
  int direction;
  CGString* ipaddr;
  int port;
#if defined(CG_USE_OPENSSL)
  SSL_CTX* ctx;
  SSL* ssl;
#endif
} CGSocket;

typedef struct UEchoDatagramPacket {
  byte* data;
  size_t dataLen;

  CGString* localAddr;
  int localPort;

  CGString* remoteAddr;
  int remotePort;
} CGDatagramPacket;

/****************************************
 * Function (Socket)
 ****************************************/

void cg_socket_startup(void);
void cg_socket_cleanup(void);

CGSocket* cg_socket_new(int type);
#define cg_socket_stream_new() cg_socket_new(CG_NET_SOCKET_STREAM)
#define cg_socket_dgram_new() cg_socket_new(CG_NET_SOCKET_DGRAM)
bool cg_socket_delete(CGSocket* socket);

void cg_socket_setid(CGSocket* socket, SOCKET value);
#define cg_socket_getid(socket) (socket->id)

#define cg_socket_settype(socket, value) (socket->type = value)
#define cg_socket_gettype(socket) (socket->type)
#define cg_socket_issocketstream(socket) ((socket->type & CG_NET_SOCKET_STREAM) ? true : false)
#define cg_socket_isdatagramstream(socket) ((socket->type & CG_NET_SOCKET_DGRAM) ? true : false)

#define cg_socket_setdirection(socket, value) (socket->direction = value)
#define cg_socket_getdirection(socket) (socket->direction)
#define cg_socket_isclient(socket) ((socket->direction == CG_NET_SOCKET_CLIENT) ? true : false)
#define cg_socket_isserver(socket) ((socket->direction == CG_NET_SOCKET_SERVER) ? true : false)

#define cg_socket_setaddress(socket, value) cg_string_setvalue(socket->ipaddr, value)
#define cg_socket_getaddress(socket) cg_string_getvalue(socket->ipaddr)
bool cg_socket_isboundaddress(CGSocket* socket, const char* addr);

#define cg_socket_setport(socket, value) (socket->port = value)
#define cg_socket_getport(socket) (socket->port)

bool cg_socket_isbound(CGSocket* socket);
bool cg_socket_close(CGSocket* socket);

bool cg_socket_listen(CGSocket* socket);

bool cg_socket_bind(CGSocket* sock, int bindPort, const char* bindAddr, CGSocketOption* opt);
bool cg_socket_accept(CGSocket* sock, CGSocket* clientSock);
bool cg_socket_connect(CGSocket* sock, const char* addr, int port);
ssize_t cg_socket_read(CGSocket* sock, char* buffer, size_t bufferLen);
size_t cg_socket_write(CGSocket* sock, const char* buffer, size_t bufferLen);
ssize_t cg_socket_readline(CGSocket* sock, char* buffer, size_t bufferLen);
size_t cg_socket_skip(CGSocket* sock, size_t skipLen);

size_t cg_socket_sendto(CGSocket* sock, const char* addr, int port, const byte* data, size_t dataeLen);
ssize_t cg_socket_recv(CGSocket* sock, CGDatagramPacket* dgmPkt);

/****************************************
 * Function (Multicast)
 ****************************************/

bool cg_socket_joingroup(CGSocket* sock, const char* mcastAddr, const char* ifAddr);

/****************************************
 * Function (Option)
 ****************************************/

bool cg_socket_setreuseaddress(CGSocket* socket, bool flag);
bool cg_socket_setmulticastloop(CGSocket* sock, bool flag);
bool cg_socket_setmulticastttl(CGSocket* sock, int ttl);
bool cg_socket_settimeout(CGSocket* sock, int sec);

/****************************************
 * Function (DatagramPacket)
 ****************************************/

CGDatagramPacket* cg_socket_datagram_packet_new(void);
void cg_socket_datagram_packet_delete(CGDatagramPacket* dgmPkt);
bool cg_socket_datagram_packet_setdata(CGDatagramPacket* dgmPkt, const byte* data, size_t dataLen);
bool cg_socket_datagram_packet_clear(CGDatagramPacket* dgmPkt);

#define cg_socket_datagram_packet_getdata(dgmPkt) (dgmPkt->data)
#define cg_socket_datagram_packet_getlength(dgmPkt) (dgmPkt->dataLen)

#define cg_socket_datagram_packet_setlocalAddr(dgmPkt, addr) cg_string_setvalue(dgmPkt->localAddr, addr)
#define cg_socket_datagram_packet_getlocalAddr(dgmPkt) cg_string_getvalue(dgmPkt->localAddr)
#define cg_socket_datagram_packet_setlocalport(dgmPkt, port) (dgmPkt->localPort = port)
#define cg_socket_datagram_packet_getlocalport(dgmPkt) (dgmPkt->localPort)
#define cg_socket_datagram_packet_setremoteAddr(dgmPkt, addr) cg_string_setvalue(dgmPkt->remoteAddr, addr)
#define cg_socket_datagram_packet_getremoteAddr(dgmPkt) cg_string_getvalue(dgmPkt->remoteAddr)
#define cg_socket_datagram_packet_setremoteport(dgmPkt, port) (dgmPkt->remotePort = port)
#define cg_socket_datagram_packet_getremoteport(dgmPkt) (dgmPkt->remotePort)

bool cg_socket_datagram_packet_copy(CGDatagramPacket* dstDgmPkt, CGDatagramPacket* srcDgmPkt);

/****************************************
 * Function (SSLSocket)
 ****************************************/

#if defined(CG_USE_OPENSSL)
#define CG_NET_SOCKET_SSL 0x0100
#define cg_socket_ssl_new() cg_socket_new(CG_NET_SOCKET_STREAM | CG_NET_SOCKET_SSL)
#define cg_socket_isssl(socket) ((socket->type & CG_NET_SOCKET_SSL) ? true : false)
#endif

#ifdef __cplusplus
}
#endif

#endif
