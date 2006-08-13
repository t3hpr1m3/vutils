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
#if !defined(__VASYNCSOCKET_H_INCLUDED__)
#define __VASYNCSOCKET_H_INCLUDED__

#include "VPlatform.h"

/* System Headers */

/* System Headers */
#include <VObject.h>
#include <VSocketCommon.h>

namespace VUtils
{

class VAsyncSocket;

typedef VMap<SOCKET, VAsyncSocket*> VSocketMap;
typedef VVector<VPollSock> VPollVector;

/**
 *  @class      VAsyncSocket
 *  @brief      Asynchronous socket class
 *  @author     Josh Williams
 *  @version    0.5.0
 *  @date       2002-2003
 *  @remarks    Allows for asynchronous handling of socket communication using
 *              callback functions.  Sockets are set to non-blocking, so none
 *              of the function calls will hang.  Keep in mind, you are still
 *              responsible for initializing the WinSock library under Windows.
 */
class VAsyncSocket : public VObject
{

public:
	/*==================================*
	 *     CONSTRUCTION/DESTRUCTION     *
	 *==================================*/
	VAsyncSocket();
	virtual ~VAsyncSocket();

	/*==================================*
	 *          INITIALIZATION          *
	 *==================================*/
	bool				Create(unsigned short pPort = 0,
							int pSocketType = SOCK_STREAM,
							long pEvents = FD_READ | FD_WRITE | FD_OOB |
								FD_ACCEPT |	FD_CONNECT | FD_CLOSE,
							const char* pSockAddr = NULL);

	/*==================================*
	 *             ATTRIBUTES           *
	 *==================================*/
	bool				Attach(SOCKET pHandle,
							long pEvents = FD_READ | FD_WRITE | FD_OOB | 
								FD_ACCEPT | FD_CONNECT | FD_CLOSE);
	SOCKET				Detach();
    static VAsyncSocket*		FromHandle(SOCKET pHandle);
	int					GetLastError();
	bool				GetPeerName(char *pAddress, int pAddrLen,
							unsigned short *pPort = NULL);
	bool				GetPeerName(sockaddr *pSockAddr,
							socklen_t *pSockAddrLen);
	bool				GetSockName(char *pAddress, int pAddrLen,
							unsigned short *pPort = NULL);
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
	bool				InitLog(const char *pLogName);
	bool				Accept(VAsyncSocket* pNewSocket, sockaddr* pSockAddr = NULL,
							socklen_t* pSockAddrLen = NULL);
	void				AsyncSelect(long pEvents = FD_READ | FD_WRITE | 
							FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);
	bool				Bind(unsigned short pPort, const char *pAddress = NULL);
	void				Close();
	bool				Connect(const char *pAddress, unsigned short pPort);
	bool				Listen(int pConnBacklog = 5);
	int					Receive(char *pBuffer, int pBufLen, int pFlags = 0);
	int					Send(const char *pBuffer, int pBufLen);
	int					Shutdown(int pHow = SHUT_WR);

private:
	/*==================================*
	 *             CALLBACKS            *
	 *==================================*/
	virtual void		OnReceive(int pErrorCode = 0);
	virtual void		OnSend(int pErrorCode = 0);
	virtual void		OnAccept(int pErrorCode = 0);
	virtual void		OnConnect(int pErrorCode = 0);
	virtual void		OnClose(int pErrorCode = 0);

private:

	/*==================================*
	 *             INTERNALS            *
	 *==================================*/
	void				LogSocket(const char *pFile, const char *pFunc,
							const int pLine, char *pMessage, ...);
	void				SetDirty();
	bool				GetIPbyName(const char *pHost, char *pAddr,
							int pSize);
	void				SocketReport();
	bool				BeginProcess();
	void				SetStatus(SS_STATE pStatus);

#if VPLATFORM == PLATFORM_WINDOWS
	static void			_Process(void* pParam);
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
	static void*		_Process(void* pParam);
#endif
	void				Process();

private:
	SOCKET				mHandle;			/**< Actual Socket ID */
    SS_STATE			mStatus;			/**< Current connection status */
	long				mEvents;			/**< Bitmask of flags we are
											  	 currently monitoring for. */
    unsigned short  	mPort;				/**< Local port for this socket */
    short           	mTimeout;			/**< Number of seconds for connect,
											  	 select, etc */
    VSockAddr       	mLocal;				/**< Connection info for the local
											  	 socket */
    VSockAddr       	mEndPoint;			/**< Connection info for the remote
											  	 socket */
	int					mError;				/**< Buffer to hold the last error
											  	 encountered. */
	static bool			mProcessing;		/**< Indicates whether or not the
											  	 Process thread is currently
												 active. */
	static bool			mDirty;				/**< Used to notify the Process
											  	 thread that the list of
												 monitored sockets has changed. */
	static FILE*		mLogFile;			/**< Name of the file to use for
											  	 logging.  If NULL, no logging
												 will be performed. */
	static int			mNumPollFDs;		/**< Number of socket id's being
											  	 polled. */
	static VSocketMap	mSocketMap;			/**< List of currently active
											  	 sockets. */
	static VPollVector	mPollFDs;			/**< Vector of socket id's being
											  	 polled. */

	static VMutex		mLogMutex;			/**< Used to lock the log file for
											  	 dedicated logging */
	static VMutex		mProcessLock;		/**< Main loop lock	*/
	static thread_handle mProcessThread;	/**< Handle to the processing
											  	 thread */
	static bool			mLogEnabled;		/**< Whether or not logging
											  	 was enabled at runtime. */
};

/********************************************************************
 *                                                                  *
 *                           I N L I N E S                          *
 *                                                                  *
 ********************************************************************/
inline
SS_STATE VAsyncSocket::GetStatus() const
{
	return mStatus;
}

inline
SOCKET VAsyncSocket::GetHandle() const
{
	return mHandle;
}

inline
long VAsyncSocket::GetEvents() const
{
	return mEvents;
}

inline
void VAsyncSocket::SetStatus(SS_STATE pStatus)
{
	mStatus = pStatus;
}

} // End Namespace

#endif // __VASYNCSOCKET_H_INCLUDED__

/* vi: set ts=4: */
