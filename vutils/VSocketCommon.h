/*======================================================================*
 *                                                                      *
 *  Copyright (C) 2004-2016 Josh Williams (vmizzle@gmail.com)           *
 *                                                                      *
 * Permission is hereby granted, free of charge, to any person          *
 * obtaining a copy of this software and associated documentation files *
 * (the "Software"), to deal in the Software without restriction,       *
 * including without limitation the rights to use, copy, modify, merge, *
 * publish, distribute, sublicense, and/or sell copies of the Software, *
 * and to permit persons to whom the Software is furnished to do so,    *
 * subject to the following conditions:                                 *
 *                                                                      *
 * The above copyright notice and this permission notice shall be       *
 * included in all copies or substantial portions of the Software.      *
 *                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      *
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                *
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  *
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   *
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    *
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     *
 * SOFTWARE.                                                            *
 *                                                                      *
 *======================================================================*/
#if !defined(__VSOCKETCOMMON_H_INCLUDED__)
#define __VSOCKETCOMMON_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */
#if VPLATFORM == PLATFORM_WINDOWS
#include <process.h>
#include <winsock2.h>
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#endif

/* Local Headers */
#include <vutils/VSockAddr.h>
#include <vutils/VMutex.h>
#include <vutils/VMap.h>
#include <vutils/VVector.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace VUtils
{

/*----------------------------------*
 *      Constant definitions        *
 *----------------------------------*/
#define IP_ADDR_LEN         30
#define CONNECT_TIMEOUT     15
#define INVALID_SOCKET      -1

#if VPLATFORM == PLATFORM_WINDOWS
#define SHUT_RD             0x00
#define SHUT_WR             0x01
#define SHUT_RDWR           0x02
#define SETERRNO()          errno = WSAGetLastError()
typedef int                 socklen_t;
#define ioctl               ioctlsocket
#define EWOULDBLOCK         WSAEWOULDBLOCK
#define ENOTSOCK            WSAENOTSOCK
#define EISCONN             WSAEISCONN
#define ENOTCONN            WSAENOTCONN
#define EINPROGRESS         WSAEINPROGRESS
typedef unsigned int        thread_handle;
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
#define FD_READ_BIT         0
#define FD_READ             (1 << FD_READ_BIT)

#define FD_WRITE_BIT        1
#define FD_WRITE            (1 << FD_WRITE_BIT)

#define FD_OOB_BIT          2
#define FD_OOB              (1 << FD_OOB_BIT)

#define FD_ACCEPT_BIT       3
#define FD_ACCEPT           (1 << FD_ACCEPT_BIT)

#define FD_CONNECT_BIT      4
#define FD_CONNECT          (1 << FD_CONNECT_BIT)

#define FD_CLOSE_BIT        5
#define FD_CLOSE            (1 << FD_CLOSE_BIT)
#define MAX_PATH            260
#define closesocket         close
#define SETERRNO()          (void)0
typedef int                 SOCKET;
typedef pthread_t           thread_handle;
#endif

/** List of available socket states. */
enum SS_STATE {
	SS_UNALLOCATED,
	SS_ALLOCATED,
	SS_BOUND,
	SS_LISTENING,
	SS_CONNECTING,
	SS_CONNECTED
};

class VPollSock {
public:
	SOCKET      mHandle;
	long        mEvents;
	bool        mRead;
	bool        mWrite;
	SS_STATE    mStatus;
};

} // End Namespace

#endif // __VSOCKETCOMMON_H_INCLUDED__

/* vi: set ts=4: */
