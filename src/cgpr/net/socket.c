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

#include <stdio.h>
#include <string.h>

#include <cgpr/net/interface.h>
#include <cgpr/net/socket.h>
#include <cgpr/util/logs.h>
#include <cgpr/util/time.h>

#if defined(WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#if defined(CG_USE_OPENSSL)
#pragma comment(lib, "libeay32MD.lib")
#pragma comment(lib, "ssleay32MD.lib")
#endif
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

/****************************************
 * static variable
 ****************************************/

static int _gSocketCnt = 0;

/****************************************
 * prototype
 ****************************************/

bool cg_socket_tosockaddrin(const char* addr, int port, struct sockaddr_in* sockaddr, bool isBindAddr);
bool cg_socket_tosockaddrinfo(int sockType, const char* addr, int port, struct addrinfo** addrInfo, bool isBindAddr);

#define cg_socket_getrawtype(socket) (((socket->type & CG_NET_SOCKET_STREAM) == CG_NET_SOCKET_STREAM) ? SOCK_STREAM : SOCK_DGRAM)

/****************************************
 *
 * Socket
 *
 ****************************************/

/****************************************
 * cg_socket_startup
 ****************************************/

void cg_socket_startup(void)
{
  if (_gSocketCnt == 0) {
#if defined(WIN32)
    WSADATA wsaData;
    int err;

    err = WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

#if !defined(WIN32)
    // Thanks for Brent Hills (10/26/04)
    signal(SIGPIPE, SIG_IGN);
#endif

#if defined(CG_USE_OPENSSL)
    SSL_library_init();
#endif
  }
  _gSocketCnt++;
}

/****************************************
 * cg_socket_cleanup
 ****************************************/

void cg_socket_cleanup(void)
{
  _gSocketCnt--;
  if (_gSocketCnt <= 0) {
#if defined(WIN32)
    WSACleanup();
#endif

#if !defined(WIN32)
    // Thanks for Brent Hills (10/26/04)
    signal(SIGPIPE, SIG_DFL);
#endif
  }
}

/****************************************
 * cg_socket_new
 ****************************************/

CGSocket* cg_socket_new(int type)
{
  CGSocket* sock;

  cg_socket_startup();

  sock = (CGSocket*)malloc(sizeof(CGSocket));
  if (!sock)
    return NULL;

#if defined(WIN32)
  sock->id = INVALID_SOCKET;
#else
  sock->id = -1;
#endif

  cg_socket_settype(sock, type);
  cg_socket_setdirection(sock, CG_NET_SOCKET_NONE);

  sock->ipaddr = cg_string_new();

  cg_socket_setaddress(sock, "");
  cg_socket_setport(sock, -1);

#if defined(CG_USE_OPENSSL)
  sock->ctx = NULL;
  sock->ssl = NULL;
#endif

  return sock;
}

/****************************************
 * cg_socket_delete
 ****************************************/

bool cg_socket_delete(CGSocket* sock)
{
  if (!sock)
    return true;

  cg_socket_close(sock);
  cg_string_delete(sock->ipaddr);
  free(sock);

  cg_socket_cleanup();

  return true;
}

/****************************************
 * cg_socket_isbound
 ****************************************/

bool cg_socket_isbound(CGSocket* sock)
{
  if (!sock)
    return false;

#if defined(WIN32)
  return (sock->id != INVALID_SOCKET) ? true : false;
#else
  return (0 < sock->id) ? true : false;
#endif
}

/****************************************
 * cg_socket_isboundaddress
 ****************************************/

bool cg_socket_isboundaddress(CGSocket* sock, const char* addr)
{
  if (!sock)
    return false;

  return cg_streq(cg_socket_getaddress(sock), addr);
}

/****************************************
 * cg_socket_setid
 ****************************************/

void cg_socket_setid(CGSocket* sock, SOCKET value)
{
#if defined(WIN32) || defined(HAVE_IP_PKTINFO) || (!defined(WIN32) && defined(HAVE_SO_NOSIGPIPE))
  int on = 1;
#endif

  if (!sock)
    return;

  sock->id = value;

#if defined(WIN32) || defined(HAVE_IP_PKTINFO)
  if (CG_NET_SOCKET_DGRAM == cg_socket_gettype(socket))
    setsockopt(sock->id, IPPROTO_IP, IP_PKTINFO, &on, sizeof(on));
#endif

#if !defined(WIN32) && defined(HAVE_SO_NOSIGPIPE)
  setsockopt(sock->id, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on));
#endif
}

/****************************************
 * cg_socket_close
 ****************************************/

bool cg_socket_close(CGSocket* sock)
{
  if (!sock)
    return true;

  if (cg_socket_isbound(sock) == false)
    return true;

#if defined(CG_USE_OPENSSL)
  if (cg_socket_isssl(sock) == true) {
    if (sock->ctx) {
      SSL_shutdown(sock->ssl);
      SSL_free(sock->ssl);
      sock->ssl = NULL;
    }
    if (sock->ctx) {
      SSL_CTX_free(sock->ctx);
      sock->ctx = NULL;
    }
  }
#endif

#if defined(WIN32)
  WSAAsyncSelect(sock->id, NULL, 0, FD_CLOSE);
  shutdown(sock->id, SD_BOTH);
  closesocket(sock->id);

  sock->id = INVALID_SOCKET;
#else
  int flag = fcntl(sock->id, F_GETFL, 0);
  if (0 <= flag)
    fcntl(sock->id, F_SETFL, flag | O_NONBLOCK);
  shutdown(sock->id, 2);
  close(sock->id);

  sock->id = -1;
#endif

  cg_socket_setaddress(sock, "");
  cg_socket_setport(sock, -1);

  return true;
}

/****************************************
 * cg_socket_listen
 ****************************************/

bool cg_socket_listen(CGSocket* sock)
{
  if (!sock)
    return false;

  int ret = listen(sock->id, SOMAXCONN);
  return (ret == 0) ? true : false;
}

/****************************************
 * cg_socket_bind
 ****************************************/

bool cg_socket_bind(CGSocket* sock, int bindPort, const char* bindAddr, CGSocketOption* opt)
{
  struct addrinfo* addrInfo;
  int ret;

  if (!sock)
    return false;

  if (bindPort <= 0 /* || bindAddr == NULL*/)
    return false;

  if (cg_socket_tosockaddrinfo(cg_socket_getrawtype(sock), bindAddr, bindPort, &addrInfo, cg_socket_option_isbindinterface(opt)) == false)
    return false;
  cg_socket_setid(sock, socket(addrInfo->ai_family, addrInfo->ai_socktype, 0));
  if (sock->id == -1) {
    cg_socket_close(sock);
    return false;
  }

  if (cg_socket_option_isreuseaddress(opt)) {
    if (cg_socket_setreuseaddress(sock, true) == false) {
      cg_socket_close(sock);
      return false;
    }
  }

  if (cg_socket_option_ismulticastloop(opt)) {
    if (cg_socket_setmulticastloop(sock, true) == false) {
      cg_socket_close(sock);
      return false;
    }
  }

  ret = bind(sock->id, addrInfo->ai_addr, addrInfo->ai_addrlen);
  freeaddrinfo(addrInfo);

  if (ret != 0)
    return false;

  cg_socket_setdirection(sock, CG_NET_SOCKET_SERVER);
  cg_socket_setaddress(sock, bindAddr);
  cg_socket_setport(sock, bindPort);

  return true;
}

/****************************************
 * cg_socket_accept
 ****************************************/

bool cg_socket_accept(CGSocket* serverSock, CGSocket* clientSock)
{
  struct sockaddr_in sockaddr;
  socklen_t socklen;
  char localAddr[CG_NET_SOCKET_MAXHOST];
  char localPort[CG_NET_SOCKET_MAXSERV];
  struct sockaddr_storage sockClientAddr;
  socklen_t nLength = sizeof(sockClientAddr);

  cg_socket_setid(clientSock, accept(serverSock->id, (struct sockaddr*)&sockClientAddr, &nLength));

#if defined(WIN32)
  if (clientSock->id == INVALID_SOCKET)
    return false;
#else
  if (clientSock->id < 0)
    return false;
#endif

  cg_socket_setaddress(clientSock, cg_socket_getaddress(serverSock));
  cg_socket_setport(clientSock, cg_socket_getport(serverSock));
  socklen = sizeof(struct sockaddr_in);

  if (getsockname(clientSock->id, (struct sockaddr*)&sockaddr, &socklen) == 0 && getnameinfo((struct sockaddr*)&sockaddr, socklen, localAddr, sizeof(localAddr), localPort, sizeof(localPort), NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
    /* Set address for the sockaddr to real addr */
    cg_socket_setaddress(clientSock, localAddr);
  }

  return true;
}

/****************************************
 * cg_socket_connect
 ****************************************/

bool cg_socket_connect(CGSocket* sock, const char* addr, int port)
{
  struct addrinfo* toaddrInfo;
  int ret;

  if (!sock)
    return false;

  if (cg_socket_tosockaddrinfo(cg_socket_getrawtype(sock), addr, port, &toaddrInfo, true) == false)
    return false;

  if (cg_socket_isbound(sock) == false) {
    cg_socket_setid(sock, socket(toaddrInfo->ai_family, toaddrInfo->ai_socktype, 0));
  }

  ret = connect(sock->id, toaddrInfo->ai_addr, toaddrInfo->ai_addrlen);
  freeaddrinfo(toaddrInfo);

  cg_socket_setdirection(sock, CG_NET_SOCKET_CLIENT);

#if defined(CG_USE_OPENSSL)
  if (cg_socket_isssl(sock) == true) {
    sock->ctx = SSL_CTX_new(SSLv23_client_method());
    sock->ssl = SSL_new(sock->ctx);
    if (SSL_set_fd(sock->ssl, cg_socket_getid(sock)) == 0) {
      cg_socket_close(sock);
      return false;
    }
    if (SSL_connect(sock->ssl) < 1) {
      cg_socket_close(sock);
      return false;
    }
  }
#endif

  return (ret == 0) ? true : false;
}

/****************************************
 * cg_socket_read
 ****************************************/

ssize_t cg_socket_read(CGSocket* sock, char* buffer, size_t bufferLen)
{
  ssize_t recvLen;

  if (!sock)
    return -1;

#if defined(CG_USE_OPENSSL)
  if (cg_socket_isssl(sock) == false) {
#endif

    recvLen = recv(sock->id, buffer, bufferLen, 0);

#if defined(CG_USE_OPENSSL)
  }
  else {
    recvLen = SSL_read(sock->ssl, buffer, bufferLen);
  }
#endif

  return recvLen;
}

/****************************************
 * cg_socket_write
 ****************************************/

#define CG_NET_SOCKET_SEND_RETRY_CNT 10
#define CG_NET_SOCKET_SEND_RETRY_WAIT_MSEC 20

size_t cg_socket_write(CGSocket* sock, const char* cmd, size_t cmdLen)
{
  ssize_t nSent;
  size_t nTotalSent = 0;
  size_t cmdPos = 0;
  int retryCnt = 0;

  if (!sock)
    return 0;

  if (cmdLen <= 0)
    return 0;

  do {
#if defined(CG_USE_OPENSSL)
    if (cg_socket_isssl(sock) == false) {
#endif

      nSent = send(sock->id, cmd + cmdPos, cmdLen, 0);

#if defined(CG_USE_OPENSSL)
    }
    else {
      nSent = SSL_write(sock->ssl, cmd + cmdPos, cmdLen);
    }
#endif

    if (nSent <= 0) {
      retryCnt++;
      if (CG_NET_SOCKET_SEND_RETRY_CNT < retryCnt) {
        nTotalSent = 0;
        break;
      }

      cg_wait(CG_NET_SOCKET_SEND_RETRY_WAIT_MSEC);
    }
    else {
      nTotalSent += nSent;
      cmdPos += nSent;
      cmdLen -= nSent;
      retryCnt = 0;
    }

  } while (0 < cmdLen);

  return nTotalSent;
}
/****************************************
 * cg_socket_readline
 ****************************************/

ssize_t cg_socket_readline(CGSocket* sock, char* buffer, size_t bufferLen)
{
  ssize_t readCnt;
  ssize_t readLen;
  char c;

  if (!sock)
    return -1;

  readCnt = 0;
  while (readCnt < (bufferLen - 1)) {
    readLen = cg_socket_read(sock, &buffer[readCnt], sizeof(char));
    if (readLen <= 0)
      return -1;
    readCnt++;
    if (buffer[readCnt - 1] == CG_SOCKET_LF)
      break;
  }
  buffer[readCnt] = '\0';
  if (buffer[readCnt - 1] != CG_SOCKET_LF) {
    do {
      readLen = cg_socket_read(sock, &c, sizeof(char));
      if (readLen <= 0)
        break;
    } while (c != CG_SOCKET_LF);
  }

  return readCnt;
}

/****************************************
 * cg_socket_skip
 ****************************************/

size_t cg_socket_skip(CGSocket* sock, size_t skipLen)
{
  ssize_t readCnt;
  ssize_t readLen;
  char c;

  if (!sock)
    return 0;

  readCnt = 0;
  while (readCnt < skipLen) {
    readLen = cg_socket_read(sock, &c, sizeof(char));
    if (readLen <= 0)
      break;
    readCnt++;
  }

  return readCnt;
}

/****************************************
 * cg_socket_sendto
 ****************************************/

size_t cg_socket_sendto(CGSocket* sock, const char* addr, int port, const byte* data, size_t dataLen)
{
  struct addrinfo* addrInfo;
  ssize_t sentLen;
  bool isBoundFlag;

  if (!sock)
    return 0;

  if (!data && (dataLen <= 0))
    return 0;

  isBoundFlag = cg_socket_isbound(sock);
  sentLen = -1;

  if (cg_socket_tosockaddrinfo(cg_socket_getrawtype(sock), addr, port, &addrInfo, true) == false)
    return -1;
  if (isBoundFlag == false)
    cg_socket_setid(sock, socket(addrInfo->ai_family, addrInfo->ai_socktype, 0));

  /* Setting multicast time to live in any case to default */
  cg_socket_setmulticastttl(sock, CG_NET_SOCKET_MULTICAST_DEFAULT_TTL);

  if (0 <= sock->id)
    sentLen = sendto(sock->id, data, dataLen, 0, addrInfo->ai_addr, addrInfo->ai_addrlen);

  cg_net_socket_debug(CG_LOG_NET_PREFIX_SEND, cg_socket_getaddress(sock), addr, data, dataLen);

  freeaddrinfo(addrInfo);

  if (isBoundFlag == false)
    cg_socket_close(sock);

  return sentLen;
}

/****************************************
 * cg_socket_recv
 ****************************************/

ssize_t cg_socket_recv(CGSocket* sock, CGDatagramPacket* dgmPkt)
{
  ssize_t recvLen = 0;
  byte recvBuf[CG_NET_SOCKET_DGRAM_RECV_BUFSIZE + 1];
  char remoteAddr[CG_NET_SOCKET_MAXHOST];
  char remotePort[CG_NET_SOCKET_MAXSERV];
  char* localAddr;
  struct sockaddr_storage from;
  socklen_t fromLen;

  if (!sock)
    return -1;

  fromLen = sizeof(from);
  recvLen = recvfrom(sock->id, recvBuf, sizeof(recvBuf) - 1, 0, (struct sockaddr*)&from, &fromLen);

  if (recvLen <= 0)
    return recvLen;

  cg_socket_datagram_packet_setdata(dgmPkt, recvBuf, recvLen);

  cg_socket_datagram_packet_setlocalport(dgmPkt, cg_socket_getport(sock));
  cg_socket_datagram_packet_setremoteAddr(dgmPkt, "");
  cg_socket_datagram_packet_setremoteport(dgmPkt, 0);

  if (getnameinfo((struct sockaddr*)&from, fromLen, remoteAddr, sizeof(remoteAddr), remotePort, sizeof(remotePort), NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
    cg_socket_datagram_packet_setremoteAddr(dgmPkt, remoteAddr);
    cg_socket_datagram_packet_setremoteport(dgmPkt, cg_str2int(remotePort));
  }

  localAddr = cg_net_selectaddr((struct sockaddr*)&from);
  cg_socket_datagram_packet_setlocalAddr(dgmPkt, localAddr);

  cg_net_socket_debug(CG_LOG_NET_PREFIX_RECV, remoteAddr, localAddr, recvBuf, recvLen);

  free(localAddr);

  return recvLen;
}

/****************************************
 * cg_socket_setreuseaddress
 ****************************************/

bool cg_socket_setreuseaddress(CGSocket* sock, bool flag)
{
  int sockOptRet;
#if defined(WIN32)
  bool optval;
#else
  int optval;
#endif

  if (!sock)
    return false;

#if defined(WIN32)
  optval = (flag == true) ? 1 : 0;
  sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
#else
  optval = (flag == true) ? 1 : 0;
  sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
#if defined(USE_SO_REUSEPORT) || defined(TARGET_OS_MAC) || defined(TARGET_OS_IPHONE)
  if (sock_opt_ret == 0) {
    sock_opt_ret = setsockopt(sock->id, SOL_SOCKET, SO_REUSEPORT, (const char*)&optval, sizeof(optval));
  }
#endif
#endif

  return (sockOptRet == 0) ? true : false;
}

/****************************************
 * cg_socket_setmulticastloop
 ****************************************/

bool cg_socket_setmulticastloop(CGSocket* sock, bool flag)
{
  int sockOptRet;
#if defined(WIN32)
  bool optval;
#else
  int optval;
#endif

  if (!sock)
    return false;

#if defined(WIN32)
  optval = (flag == true) ? 1 : 0;
  sockOptRet = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&optval, sizeof(optval));
#else
  optval = (flag == true) ? 1 : 0;
  sockOptRet = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&optval, sizeof(optval));
#endif

  return (sockOptRet == 0) ? true : false;
}

/****************************************
 * cg_socket_setmulticastttl
 ****************************************/

bool cg_socket_setmulticastttl(CGSocket* sock, int val)
{
  int sockOptRet;
  int ttl;
  unsigned int len = 0;

  if (!sock)
    return false;

  ttl = val;
#if defined(WIN32)
  sockOptRet = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&ttl, sizeof(ttl));
#else
  sockOptRet = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, (const unsigned char*)&ttl, sizeof(ttl));
  if (sockOptRet == 0) {
    len = sizeof(ttl);
    getsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, (socklen_t*)&len);
  }
#endif

  return (sockOptRet == 0) ? true : false;
}

/****************************************
 * cg_socket_settimeout
 ****************************************/

bool cg_socket_settimeout(CGSocket* sock, int sec)
{
  int sockOptRet;

#if defined(WIN32)
  struct timeval timeout;
  timeout.tv_sec = sec;
  timeout.tv_usec = 0;

  if (!sock)
    return false;

  sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
  if (sockOptRet == 0)
    sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#else
  struct timeval timeout;
  timeout.tv_sec = (clock_t)sec;
  timeout.tv_usec = 0;

  if (!sock)
    return false;

  sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
  if (sockOptRet == 0)
    sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#endif

  return (sockOptRet == 0) ? true : false;
}

/****************************************
 * cg_socket_joingroup
 ****************************************/

bool cg_socket_joingroup(CGSocket* sock, const char* mcastAddr, const char* ifAddr)
{
  struct addrinfo hints;
  struct addrinfo *mcastAddrInfo, *ifAddrInfo;

  /**** for IPv6 ****/
  struct ipv6_mreq ipv6mr;
  struct sockaddr_in6 toaddr6, ifaddr6;
  int scopeId;

  /**** for IPv4 ****/
  struct ip_mreq ipmr;
  struct sockaddr_in toaddr, ifaddr;

  bool joinSuccess;
  int sockOptRetCode;

  if (!sock)
    return false;

  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;

  if (getaddrinfo(mcastAddr, NULL, &hints, &mcastAddrInfo) != 0)
    return false;

  if (getaddrinfo(ifAddr, NULL, &hints, &ifAddrInfo) != 0) {
    freeaddrinfo(mcastAddrInfo);
    return false;
  }

  joinSuccess = true;

  if (cg_net_isipv6address(mcastAddr) == true) {
    memcpy(&toaddr6, mcastAddrInfo->ai_addr, sizeof(struct sockaddr_in6));
    memcpy(&ifaddr6, ifAddrInfo->ai_addr, sizeof(struct sockaddr_in6));
    ipv6mr.ipv6mr_multiaddr = toaddr6.sin6_addr;
    scopeId = cg_net_getipv6scopeid(ifAddr);
    ipv6mr.ipv6mr_interface = scopeId /*if_nametoindex*/;

    sockOptRetCode = setsockopt(sock->id, IPPROTO_IPV6, IPV6_MULTICAST_IF, (char*)&scopeId, sizeof(scopeId));

    if (sockOptRetCode != 0)
      joinSuccess = false;

    sockOptRetCode = setsockopt(sock->id, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char*)&ipv6mr, sizeof(ipv6mr));

    if (sockOptRetCode != 0)
      joinSuccess = false;
  }
  else {
    memcpy(&toaddr, mcastAddrInfo->ai_addr, sizeof(struct sockaddr_in));
    memcpy(&ifaddr, ifAddrInfo->ai_addr, sizeof(struct sockaddr_in));
    memcpy(&ipmr.imr_multiaddr.s_addr, &toaddr.sin_addr, sizeof(struct in_addr));
    memcpy(&ipmr.imr_interface.s_addr, &ifaddr.sin_addr, sizeof(struct in_addr));
    sockOptRetCode = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_IF, (char*)&ipmr.imr_interface.s_addr, sizeof(struct in_addr));
    if (sockOptRetCode != 0)
      joinSuccess = false;
    sockOptRetCode = setsockopt(sock->id, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&ipmr, sizeof(ipmr));

    if (sockOptRetCode != 0)
      joinSuccess = false;
  }

  freeaddrinfo(mcastAddrInfo);
  freeaddrinfo(ifAddrInfo);

  return joinSuccess;
}

/****************************************
 * cg_socket_tosockaddrin
 ****************************************/

bool cg_socket_tosockaddrin(const char* addr, int port, struct sockaddr_in* sockaddr, bool isBindAddr)
{
  cg_socket_startup();

  memset(sockaddr, 0, sizeof(struct sockaddr_in));

  sockaddr->sin_family = AF_INET;
  sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
  sockaddr->sin_port = htons((unsigned short)port);

  if (isBindAddr == true) {
    sockaddr->sin_addr.s_addr = inet_addr(addr);
    if (sockaddr->sin_addr.s_addr == INADDR_NONE) {
      struct hostent* hent = gethostbyname(addr);
      if (hent == NULL)
        return false;
      memcpy(&(sockaddr->sin_addr), hent->h_addr, hent->h_length);
    }
  }

  return true;
}

/****************************************
 * cg_socket_tosockaddrinfo
 ****************************************/

bool cg_socket_tosockaddrinfo(int sockType, const char* addr, int port, struct addrinfo** addrInfo, bool isBindAddr)
{
  struct addrinfo hints;
  char portStr[32];
  int errorn;

  cg_socket_startup();

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = sockType;
  hints.ai_flags = /*AI_NUMERICHOST | */ AI_PASSIVE;
  sprintf(portStr, "%d", port);
  if ((errorn = getaddrinfo(addr, portStr, &hints, addrInfo)) != 0)
    return false;
  if (isBindAddr == true)
    return true;
  hints.ai_family = (*addrInfo)->ai_family;
  freeaddrinfo(*addrInfo);
  if ((errorn = getaddrinfo(NULL, portStr, &hints, addrInfo)) != 0)
    return false;
  return true;
}
