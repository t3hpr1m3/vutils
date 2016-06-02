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
#if !defined(__VSOCKET_H_INCLUDED__)
#define __VSOCKET_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */

/* Local Headers */
#include <vutils/VObject.h>
#include <vutils/VSocketCommon.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Macros */

namespace VUtils {

class VSocket;

typedef VMap<SOCKET, VSocket*> VSocketMap;
typedef VVector<VPollSock> VPollVector;

class VSocket : public VObject {

public:
	/*==================================*
	 *     CONSTRUCTION/DESTRUCTION     *
	 *==================================*/
	VSocket();
	virtual ~VSocket();

	/*==================================*
	 *          INITIALIZATION          *
	 *==================================*/

	/*==================================*
	 *             ATTRIBUTES           *
	 *==================================*/
	int                 GetLastError() const;
	bool                GetPeerName(char *pAddress, int pAddrLen,
	                        VUSHORT *pPort = NULL);
	bool                GetPeerName(sockaddr *pSockAddr,
	                        socklen_t *pSockAddrLen);
	bool                GetSockName(char *pAddress, int pAddrLen,
	                        VUSHORT *pPort = NULL);
	bool                GetSockName(sockaddr *pSockAddr,
	                        socklen_t *pSockAddrLen);
	bool                GetSockOpt(int pOptionName, void *pOptionValue,
	                        int *pOptionLen, int pLevel = SOL_SOCKET);
	bool                SetSockOpt(int pOptionName, const void *pOptionValue,
	                        int nOptionLen, int pLevel = SOL_SOCKET);
	SS_STATE            GetStatus() const;
	SOCKET              GetHandle() const;

	/*==================================*
	 *           OPERATIONS             *
	 *==================================*/
	bool                Create(VUSHORT pPort = 0,
	                        int pSocketType = SOCK_STREAM,
	                        const char* pSockAddr = NULL);
	bool                Attach(SOCKET pHandle);
	SOCKET              Detach();
	bool                Accept(VSocket* pNewSocket, sockaddr* pSockAddr = NULL,
	                        socklen_t* pSockAddrLen = NULL);
	bool                Bind(VUSHORT pPort, const char *pAddress = NULL);
	void                Close();
	bool                Connect(const char *pAddress, VUSHORT pPort);
	bool                Listen(int pConnBacklog = 5);
	int                 Receive(char *pBuffer, int pBufLen, int pFlags = 0);
	int                 Send(const char *pBuffer, int pBufLen);
	int                 Shutdown(int pHow = SHUT_WR);
	VRESULT             Select(long *pFlags, VUSHORT pTimeout);

public:
	/*==================================*
	 *             OPERATORS            *
	 *==================================*/

private:
	/*==================================*
	 *             CALLBACKS            *
	 *==================================*/

protected:
	/*==================================*
	 *             INTERNALS            *
	 *==================================*/
	bool                GetIPbyName(const char *pHost, char *pAddr, int pSize);
	void                SetStatus(SS_STATE pStatus);

protected:
	SOCKET              mHandle;            /**< Actual Socket ID */
	SS_STATE            mStatus;            /**< Current connection status */
	VUSHORT             mPort;              /**< Local port for this socket */
	short               mTimeout;           /**< Number of seconds for connect,
	                                             select, etc */
	VSockAddr           mLocal;             /**< Connection info for the local
	                                             socket */
	VSockAddr           mEndPoint;          /**< Connection info for the remote
	                                             socket */
	int                 mError;             /**< Buffer to hold the last error
	                                             encountered. */
};


/********************************************************************
 *                                                                  *
 *                           I N L I N E S                          *
 *                                                                  *
 ********************************************************************/
inline
int VSocket::GetLastError() const {
	return mError;
}

inline
SS_STATE VSocket::GetStatus() const {
	return mStatus;
}

inline
SOCKET VSocket::GetHandle() const {
	return mHandle;
}

inline
void VSocket::SetStatus(SS_STATE pStatus) {
	mStatus = pStatus;
}

} // End Namespace

#endif // __VSOCKET_H_INCLUDED__

/* vi: set ts=4: */
