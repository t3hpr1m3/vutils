/*======================================================================*
 *																		*
 *					* * N O   S T E A L I N G * *						*
 *																		*
 *  Copyright (C) 2004 V-Man   All Rights Reserved						*
 *																		*
 *	AUTHOR																*
 *		V-Man <V-Man@udpviper.com>										*
 *																		*
 *	Dis is mah stuff.  If'n you use it, I get dah credit.  k?			*
 *																		*
 *																		*
 *======================================================================*/
#if !defined(__VSOCKET_H_INCLUDED__)
#define __VSOCKET_H_INCLUDED__

#include "VPlatform.h"

/* System Headers */

/* Local Headers */
#include "VObject.h"
#include "VSocketCommon.h"

/* Macros */
#define LOG_SOCKET(message, ...) \
	LogSocket(__FILE__, __FUNCTION__, __LINE__, message, ## __VA_ARGS__)

namespace VUtils
{

class VSocket;

typedef VMap<SOCKET, VSocket*> VSocketMap;
typedef VVector<VPollSock> VPollVector;

class VSocket : public VObject
{

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
	int					GetLastError() const;
	bool				GetPeerName(char *pAddress, int pAddrLen,
							VUSHORT *pPort = NULL);
	bool				GetPeerName(sockaddr *pSockAddr,
							socklen_t *pSockAddrLen);
	bool				GetSockName(char *pAddress, int pAddrLen,
							VUSHORT *pPort = NULL);
	bool				GetSockName(sockaddr *pSockAddr,
							socklen_t *pSockAddrLen);
	bool				GetSockOpt(int pOptionName, void *pOptionValue,
							int *pOptionLen, int pLevel = SOL_SOCKET);
	bool				SetSockOpt(int pOptionName, const void *pOptionValue,
							int nOptionLen, int pLevel = SOL_SOCKET);
	SS_STATE			GetStatus() const;
	SOCKET				GetHandle() const;	
	long				GetEvents() const;

	/*==================================*
	 *           OPERATIONS             *
	 *==================================*/
	bool				Create(VUSHORT pPort = 0,
							int pSocketType = SOCK_STREAM,
							const char* pSockAddr = NULL);
	bool				Attach(SOCKET pHandle);
	SOCKET				Detach();
	bool				InitLog(const char *pLogName);
	bool				Accept(VSocket* pNewSocket, sockaddr* pSockAddr = NULL,
							socklen_t* pSockAddrLen = NULL);
	bool				Bind(VUSHORT pPort, const char *pAddress = NULL);
	void				Close();
	bool				Connect(const char *pAddress, VUSHORT pPort, VUSHORT pTimeout);
	bool				Listen(int pConnBacklog = 5);
	int					Receive(char *pBuffer, int pBufLen, int pFlags = 0);
	int					Send(const char *pBuffer, int pBufLen);
	int					Shutdown(int pHow = SHUT_WR);
	VRESULT				Select(long *pFlags, VUSHORT pTimeout);

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
	void				LogSocket(const char *pFile, const char *pFunc,
							const int pLine, char *pMessage, ...);
	bool				GetIPbyName(const char *pHost, char *pAddr,	int pSize);
	void				SocketReport();
	void				SetStatus(SS_STATE pStatus);

protected:
	SOCKET				mHandle;			/**< Actual Socket ID */
    SS_STATE			mStatus;			/**< Current connection status */
    VUSHORT			  	mPort;				/**< Local port for this socket */
    short           	mTimeout;			/**< Number of seconds for connect,
											  	 select, etc */
    VSockAddr       	mLocal;				/**< Connection info for the local
											  	 socket */
    VSockAddr       	mEndPoint;			/**< Connection info for the remote
											  	 socket */
	int					mError;				/**< Buffer to hold the last error
											  	 encountered. */
	static FILE*		mLogFile;			/**< Name of the file to use for
											  	 logging.  If NULL, no logging
												 will be performed. */
	static VMutex		mLogMutex;			/**< Used to lock the log file for
											  	 dedicated logging */
	static bool			mLogEnabled;		/**< Whether or not logging
											  	 was enabled at runtime. */
};


/********************************************************************
 *                                                                  *
 *                           I N L I N E S                          *
 *                                                                  *
 ********************************************************************/
inline
int VSocket::GetLastError() const
{
	return mError;
}

inline
SS_STATE VSocket::GetStatus() const
{
	return mStatus;
}

inline
SOCKET VSocket::GetHandle() const
{
	return mHandle;
}

inline
void VSocket::SetStatus(SS_STATE pStatus)
{
	mStatus = pStatus;
}

} // End Namespace

#endif // __VSOCKET_H_INCLUDED__

/* vi: set ts=4: */
