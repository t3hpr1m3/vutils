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
#include <vutils/VAsyncSocket.h>

/* System Headers */
#include <time.h>
#include <errno.h>
#include <stdarg.h>

/* Local Headers */
#include <vutils/VLog.h>

namespace VUtils
{

/*------------------------------*
 * Initialize static members    *
 *------------------------------*/
VSocketMap		VAsyncSocket::mSocketMap;
VPollVector		VAsyncSocket::mPollFDs;
bool    		VAsyncSocket::mProcessing = false;
bool			VAsyncSocket::mDirty = false;
bool			VAsyncSocket::mLogEnabled = false;
int				VAsyncSocket::mNumPollFDs = 0;
FILE*			VAsyncSocket::mLogFile = NULL;

VMutex			VAsyncSocket::mLogMutex;
VMutex			VAsyncSocket::mProcessLock;
thread_handle	VAsyncSocket::mProcessThread;

DECLARE_CLASS( "VAsyncSocket" );

/********************************************************************
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 ********************************************************************/
VAsyncSocket::VAsyncSocket()
{
	mStatus		= SS_UNALLOCATED;
	mHandle		= -1;
	mPort		= 0;
	mTimeout	= 1;
	mError		= 0;
	memset(&mLocal,		0, sizeof(VSockAddr));
	memset(&mEndPoint,	0, sizeof(VSockAddr));
}

VAsyncSocket::~VAsyncSocket()
{
	if (mStatus != SS_UNALLOCATED)
		Close();
	if (mSocketMap.size() == 0)
	{
		mProcessLock.Lock();
		mProcessLock.Unlock();
	}
	if (mLogFile != NULL)
		fclose(mLogFile);
}

/********************************************************************
 *                      I N I T I L I Z A T I O N                   *
 ********************************************************************/

/*------------------------------------------------------------------*
 *                              Create()                            *
 *------------------------------------------------------------------*/
/**
 *	@brief		Initialize this socket and prepare for use.
 *
 *  @remarks	Creates the underlying socket handle and sets the socket
 *      		to non-blocking mode.  If an address/port are specified, the
 *      		socket is bound automatically.
 *
 *  @param		pPort
 *  				Optional port number to bind to.
 *  @param		pSocketType
 *  				Type of socket.
 *  @param		pEvents
 *  				Bitmask of flags specifying what events the
 *          		application is interested in.
 *  @param		pSockAddr
 *  				Optional address to bind to.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::Create(unsigned short pPort /*=0*/,
		int pSocketType /*=SOCK_STREAM*/,
		long pEvents
		/*=FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE*/,
		const char* pSockAddr/*=NULL*/)
{
	SOCKET	vSocketID = -1; // Default to an invalid state
	bool	vResult = false;
	int		vRetval = 0;
	long	vLongBlock = 0;
		

	BEG_FUNC("Create")("%d, %d, %d, %p", pPort, pSocketType,
								pEvents, pSockAddr);

	/*
	 * Make sure we aren't already created or bound.
	 */
	if (mStatus != SS_UNALLOCATED)
	{
		VTRACE("Socket already initialized\n");
		mError = EISCONN;
		return END_FUNC(false);
	}

	/*
	 * Create the actual socket.
	 */
	vSocketID = socket(AF_INET, pSocketType, 0);
	if (vSocketID != -1)
	{
		VTRACE("Socket %d created\n", vSocketID);
		mStatus = SS_ALLOCATED;
		mHandle = vSocketID;
		mEvents = pEvents;

		/*==============================*
		 * Try and switch the socket to *
		 * non-blocking mode.           *
		 *==============================*/
#if defined(WIN32) || defined(_WINDOWS)     /* Windows platforms */
		vLongBlock = 1;
		vRetval = ioctl(mHandle, FIONBIO, &vLongBlock);
#else                                       /* Unix/linux */
		vLongBlock = fcntl(mHandle, F_GETFL);
		if (vLongBlock == -1L)
			vRetval = -1;
		else
		{
			vLongBlock |= O_NONBLOCK;
			vRetval = fcntl(mHandle, F_SETFL, vLongBlock);
		}
#endif
		if (vRetval == -1)
		{
			SETERRNO();
			mError = errno;
			VTRACE("Unable to set socket to non-blocking.  Error-%s\n",
							strerror(errno));
		}
		else
		{
			/*
			 * If a port/address were supplied, try and bind to them.
			 */
			if (pPort != 0)
				vResult = Bind(pPort, pSockAddr);
			else
				vResult = true;
		}
	}

	if (vResult)
		VTRACE("Successfully created socket.\n");
	else
		VTRACE("Unable to create socket.\n");

	return END_FUNC(vResult);
}

/********************************************************************
 *                        A T T R I B U T E S                       *
 ********************************************************************/

/*------------------------------------------------------------------*
 *                              Attach()                            *
 *------------------------------------------------------------------*/
/**
 *	@brief		Attach this VAsyncSocket to a pre-existing socket handle.
 *
 *  @remarks	Attaches this object to an existing socket handle and
 *  			enters the processing loop.
 *
 *  @param		pHandle
 *  				Handle to the socket to attach to.
 *  @param		pEvents
 *  				Bitmask of flags specifying what events the
 *          		application is interested in.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::Attach(int pHandle,
		long pEvents
		/*=FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE*/)
{
	bool	vRetval = false, vOk = true;
	char	vAddress[51];
	unsigned short vPort = 0;

	BEG_FUNC("Attach")("%d, %d", pHandle, pEvents);

	if (mStatus == SS_CONNECTED)
	{
		VTRACE("Error-socket already connected to %s:%d\n",
					mEndPoint.GetAddr(), mEndPoint.GetPort());
		mError = EISCONN;
		return END_FUNC(false);
	}

	if (mStatus == SS_BOUND)
	{
		VTRACE("Error-socket already bound to %s:%d\n", 
					mLocal.GetAddr(), mLocal.GetPort());
		mError = EISCONN;
		return END_FUNC(false);
	}

	mStatus = SS_ALLOCATED;
	mEvents = pEvents;
	mHandle = pHandle;
	GetPeerName(vAddress, 50, &vPort);
	mEndPoint.sin_family = AF_INET;
	mEndPoint.SetPort(vPort);
	mEndPoint.SetAddr(vAddress);

	memset(vAddress, 0, 50);
	GetSockName(vAddress, 50, &vPort);
	mLocal.SetPort(vPort);
	mLocal.SetAddr(vAddress);

	mStatus = SS_CONNECTED;

	vRetval = BeginProcess();

	return END_FUNC(true);
}

/*------------------------------------------------------------------*
 *                              Detach()                            *
 *------------------------------------------------------------------*/
/**
 *	@brief		Remove the socket from the processing queue and
 *				return the handle.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
int VAsyncSocket::Detach()
{
	SOCKET vHandle = mHandle;

	BEG_FUNC("Detack")(NULL);

	if (mHandle != -1)
	{
		mSocketMap.Lock();
		mSocketMap.erase(mHandle);
		mSocketMap.Unlock();
		SetDirty();
		mStatus = SS_UNALLOCATED;
		mHandle = -1;
	}
	else
		VTRACE("Invalid socket handle\n");

	return END_FUNC(vHandle);
}

/*------------------------------------------------------------------*
 *                            FromHandle()                          *
 *------------------------------------------------------------------*/
/**
 *	@brief		Retrieves an VAsyncSocket object from the socket map
 *				by file descriptor.
 *
 *  @param		pHandle
 *  				File descriptor of the socket in question.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
VAsyncSocket* VAsyncSocket::FromHandle(SOCKET pHandle)
{
	VAsyncSocket* vSocket = mSocketMap[pHandle];
	return vSocket;
}

/*------------------------------------------------------------------*
 *                          GetLastError()                          *
 *------------------------------------------------------------------*/
/**
 *	@brief		Returns the last error encountered by this socket.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
int VAsyncSocket::GetLastError()
{
	return mError;
}

/*------------------------------------------------------------------*
 *                          GetPeerName()                           *
 *------------------------------------------------------------------*/
/**
 *	@brief		Retrieves address/port info for the remote socket.
 *
 *  @param		pAddress
 *					Buffer to hold the address.
 *  @param		pAddrLen
 *					Maximum size of pcAddress.
 *  @param		pPort
 *  				Port number opened remotely.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::GetPeerName(char *pAddress, int pAddrLen,
							unsigned short *pPort /*=NULL*/)
{
	bool		vRetval = true;
	VSockAddr	vAddr;
	socklen_t	vSAlen;

	BEG_FUNC("GetPeerName")("%p, %d, %p", pAddress, pAddrLen, pPort);

	vSAlen = sizeof(vAddr);

	/* Obtail remote connection info */
	if (getpeername(mHandle, (struct sockaddr*)&vAddr, &vSAlen) != -1)
	{
		if (pPort != NULL)
			*pPort = vAddr.GetPort();

		if (pAddress != NULL)
		{
			memset(pAddress, 0, pAddrLen);
			if (vAddr.GetAddr() != NULL)
				strncpy(pAddress, vAddr.GetAddr(), pAddrLen);
			else
				strncpy(pAddress, "No address", pAddrLen);
		}
	}
	else
		vRetval = false;

	return END_FUNC(vRetval);
}

/*------------------------------------------------------------------*
 *                          GetPeerName()                           *
 *------------------------------------------------------------------*/
/**
 *	@brief		Retrieves address/port info for the remote socket.
 *
 *  @param		pSockAddr
 *  				Structure pointer to hold the socket info.
 *  @param		pSockAddrLen
 *  				Pointer to the length pSockAddr.  On return,
 *          		pSockAddrLen will contain the actual size of
 *          		pSockAddr.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::GetPeerName(sockaddr *pSockAddr, socklen_t *pSockAddrLen)
{
	BEG_FUNC("GetPeerName")("%p, %p", pSockAddr, pSockAddrLen);

	*pSockAddrLen = sizeof(*pSockAddr);

	/* Obtail remote connection info */
	if (getpeername(mHandle, pSockAddr, pSockAddrLen) == 0)
	{
		SETERRNO();
		mError = errno;
		return END_FUNC(false);
	}

	return END_FUNC(true);
}

/*------------------------------------------------------------------*
 *                          GetSockName()                           *
 *------------------------------------------------------------------*/
/**
 *	@brief		Retrieves address/port info for the local socket.
 *
 *  @param		pAddress
 *  				Buffer to hold the address.
 *  @param		pAddrLen
 *  				Maximum size of pAddress.
 *  @param		pPort
 *  				Port number opened locally.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::GetSockName(char *pAddress, int pAddrLen,
							unsigned short *pPort /*=NULL*/)
{
	VSockAddr	vAddr;
	socklen_t	vSAlen = sizeof(vAddr);

	BEG_FUNC("GetSockName")("%p, %d, %p", pAddress, pAddrLen, pPort);

	/* Obtail local connection info */
	if (getsockname(mHandle, (struct sockaddr*)&vAddr, &vSAlen) != -1)
	{
		if (pPort != NULL)
			*pPort = vAddr.GetPort();
		if (pAddress != NULL)
		{
			memset(pAddress, 0, pAddrLen);
			if (vAddr.GetAddr() != NULL)
				strncpy(pAddress, vAddr.GetAddr(), pAddrLen);
			else
				strncpy(pAddress, "No address", pAddrLen-1);
		}
	}
	else
	{
		SETERRNO();
		mError = errno;
		return END_FUNC(false);
	}

	return END_FUNC(true);
}

/*------------------------------------------------------------------*
 *                          GetSockName()                           *
 *------------------------------------------------------------------*/
/**
 *	@brief		Retrieves address/port info for the remote socket.
 *
 *  @param		pSockAddr
 *  				Structure pointer to hold the socket info.
 *  @param		pSockAddrLen
 *  				Pointer to the length pSockAddr.  On return,
 *          		pSockAddrLen will contain the actual size of
 *          		pSockAddr.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::GetSockName(sockaddr *pSockAddr, socklen_t *pSockAddrLen)
{
	BEG_FUNC("GetSockName")("%p, %p", pSockAddr, pSockAddrLen);

	*pSockAddrLen = sizeof(*pSockAddr);

	/* Obtail local connection info */
	if (getpeername(mHandle, pSockAddr, pSockAddrLen) == 0)
	{
		SETERRNO();
		mError = errno;
		return END_FUNC(false);
	}

	return END_FUNC(true);
}


/*------------------------------------------------------------------*
 *                          GetSockOpt()                            *
 *------------------------------------------------------------------*/
/**
 *	@brief		Retrieves an option associated with this socket.
 *
 *  @param		pOptName
 *  				Option whose value is to be retrieved.
 *  @param		pOptValue
 *  				Buffer to hold the retrieved value.
 *  @param		pOptLen
 *  				Pointer to the size of the pOptValue buffer.
 *  @param		pLevel
 *  				Level at which the option is defined.  The only
 *          		supported levels are SOL_SOCKET (default)
 *          		and IPPROTO_TCP.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::GetSockOpt(int pOptName, void *pOptValue,
						int *pOptLen, int pLevel /*=SOL_SOCKET*/)
{
	BEG_FUNC("GetSockOpt")("%d, %p, %p, %d", pOptName, pOptValue,
							pOptLen, pLevel);

	if (getsockopt(mHandle, pLevel, pOptName, (char*)pOptValue,
					(socklen_t*)pOptLen) != 0)
	{
		SETERRNO();
		mError = errno;
		return END_FUNC(false);
	}

	return END_FUNC(true);
}

/*------------------------------------------------------------------*
 *                          SetSockOpt()                            *
 *------------------------------------------------------------------*/
/**
 *	@brief		Modifies one of the options associated with this socket.
 *
 *  @param		pOptName
 *  				Option whose value is to be modified.
 *  @param		pOptValue
 *  				Buffer that holds the new value.
 *  @param		pOptLen
 *  				Size of the pOptValue buffer.
 *  @param		pLevel
 *  				Level at which the option is defined.  The only
 *          		supported levels are SOL_SOCKET (default)
 *          		and IPPROTO_TCP.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::SetSockOpt(int pOptName, const void *pOptValue,
						int pOptLen, int pLevel /*=SOL_SOCKET*/)
{
	BEG_FUNC("SetSockOpt")("%d, %p, %d, %d", pOptName, pOptValue, 
							pOptLen, pLevel);

	if (setsockopt(mHandle, pLevel, pOptName, (const char*)pOptValue,
					pOptLen) != 0)
	{
		SETERRNO();
		mError = errno;
		return END_FUNC(false);
	}

	return END_FUNC(true);
}


/********************************************************************
 *                        O P E R A T I O N S                       *
 ********************************************************************/

/*------------------------------------------------------------------*
 *                            InitLog()                             *
 *------------------------------------------------------------------*/
/**
 *	@brief		Initializes logging within the socket library.
 *
 *  @param		pLogName
 *  				Name to be used for the log file.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::InitLog(const char *pLogName)
{
	if (!mLogEnabled)
	{
		mLogFile = fopen(pLogName, "a+");
		if (mLogFile != NULL)
			mLogEnabled = true;
	}
	return mLogEnabled;
}

/*------------------------------------------------------------------*
 *                              Accept()                            *
 *------------------------------------------------------------------*/
/**
 *	@brief		Accept a pending connection from the incoming queue.
 *
 *  @param		pNewSocket
 *  				Pointer to hold the newly accepted socket.
 *  @param		pSockAddr
 *  				Pointer to hold the address info for the remote
 *          		socket.
 *  @param		pSockAddrLen
 *  				Size of the address pointer.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::Accept(VAsyncSocket* pNewSocket, sockaddr* pSockAddr/*=NULL*/,
					socklen_t* pSockAddrLen/*=NULL*/)
{
	SOCKET	vTempHandle = -1; /* temp Socket ID */
	SOCKET	vNewHandle = -1; /* new Socket ID  */
	int		vPeerSize;
	bool	vRetval = false;

	BEG_FUNC("Accept")("%p, %p, %p", pNewSocket, pSockAddr, pSockAddrLen);

	if (mStatus != SS_LISTENING) /* Socket not in listening mode! */
	{
		VERROR("Socket not in listening mode. Cannot accept "
					"incoming connection\n");
		mError = EINVAL;
		return END_FUNC(false);
	}

	/* We are in a proper state to accept connections.  Try it! */
	vPeerSize = sizeof(mEndPoint);
	vTempHandle = mHandle;

	/* Attempt to accept the connection */
	vNewHandle = accept(vTempHandle, (struct sockaddr*)&mEndPoint, (socklen_t*)&vPeerSize);
	if (vNewHandle >= 0)
	{
		/* Accepted an incoming connection */
		VTRACE("Connection accepted\n");
		pNewSocket->Attach(vNewHandle);
		if (pSockAddr != NULL)
		{
			memcpy(pSockAddr, &mEndPoint, vPeerSize);
		}
		if (pSockAddrLen != NULL)
			*pSockAddrLen = vPeerSize;
	}
	else
	{
		SETERRNO();
		mError = errno;
		VERROR("accept failed with error code %d\n", errno);
		return END_FUNC(false);
	}

	return END_FUNC(true);
}

/*------------------------------------------------------------------*
 *                           AsyncSelect()                          *
 *------------------------------------------------------------------*/
/**
 *	@brief		Modify the flags this socket is interested in
 *				notifications for.
 *
 *  @param		pEvents
 *  				Bitmask of flags specifying what events the
 *					application is interested in.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
void VAsyncSocket::AsyncSelect(long pEvents
		/*=FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE*/)
{
	BEG_FUNC("AsyncSelect")("%d", pEvents);

	mEvents = pEvents;

	SetDirty();

	END_FUNCV();
}

/*------------------------------------------------------------------*
 *                              Bind()                              *
 *------------------------------------------------------------------*/
/**
 *	@brief		Binds this socket object to a specific port and
 *				(optional) address.
 *
 *  @param		pPort
 *  				Port number to bind to
 *  @param		pAddress
 *  				(Optional) Address to bind to
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::Bind(unsigned short pPort, const char *pAddress/*=NULL*/)
{
	char	vDestAddr[IP_ADDR_LEN];
	bool	vRetval = false, vContinue = true;
	int		vReturnCode = 0;
	unsigned long vAddrIP;

	BEG_FUNC("Bind")("%d, %p", pPort, pAddress);

	/*
	 * Socket initialized?
	 */
	if (mStatus != SS_ALLOCATED)
	{
		VTRACE("Socket not initialized\n");
		mError = ENOTSOCK;
		return END_FUNC(false);
	}

	/*
	 * Are we already bound?
	 */
	if (mStatus == SS_BOUND)
	{
		VERROR("Socket already bound to %s:%d\n",
					mLocal.GetAddr(), mLocal.GetPort());
		mError = EINVAL;
		return END_FUNC(false);
	}

	if (pAddress == NULL)
	{
		mPort = pPort;
		mLocal.sin_family = AF_INET;
		mLocal.SetPort(pPort);
		mLocal.SetAddrIP(INADDR_ANY);
	}
	else
	{
		// Resolve the destination: Name or Address?
		if (!GetIPbyName(pAddress, vDestAddr, sizeof(vDestAddr)))
			strncpy(vDestAddr, pAddress, IP_ADDR_LEN); // Copy in the address passed

		// Convert the address to network byte order
		vAddrIP = inet_addr(vDestAddr);
		if (vAddrIP != INADDR_NONE) // Valid address format
		{
			mPort = pPort;
			mLocal.sin_family = AF_INET;
			mLocal.SetPort(pPort);
			mLocal.SetAddrIP(vAddrIP);
			VTRACE("Address resolved to %s\n", vDestAddr);
		}
		else
		{
			VTRACE("Unable to resolve address\n");
			mError = EINVAL;
			return END_FUNC(false);
		}
	}

	/*
	 * Try to bind to the specified port/address.
	 */
	vReturnCode = bind(mHandle, (struct sockaddr*)&mLocal, sizeof(sockaddr_in));
	if (vReturnCode == 0)
	{
		// Socket successfully bound.
		mStatus = SS_BOUND;
		VTRACE("Bound to port %d\n", pPort);
	}
	else
	{
		//--------------------------------------//
		// Determine the cause of the failure.  //
		// Normally a failed call to bind       //
		// indicats the local port is in use.   //
		//--------------------------------------//
		SETERRNO();
		mError = errno;
		VERROR("bind failed.  Errno code %d\n", mError);
		return END_FUNC(false);
	}

	return END_FUNC(true);
}

/*------------------------------------------------------------------*
 *                              Close()                             *
 *------------------------------------------------------------------*/
/**
 *	@brief		Clean up this socket object and reset it to an
 *				unused state.
 *
 *  @remarks	Called when all processing on this socket has finished.
 *				Once Close() is called, this socket cannot be used again
 *				until Create() is called.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 * 25-May-2003  Added code to remove the socket     Josh Williams   *
 *              from the processing map on Close()                  *
 *                                                                  *
 *------------------------------------------------------------------*/
void VAsyncSocket::Close()
{
	BEG_FUNC("Close")(NULL);

	if (mHandle != -1)
	{
		SOCKET vHandle = Detach();
		closesocket(vHandle);
	}
	else
		VTRACE("Socket already closed\n");

	END_FUNCV();
}

/*------------------------------------------------------------------*
 *                              Connect()                           *
 *------------------------------------------------------------------*/
/**
 *	@brief		Establish a connection with a remote machine.
 *
 *  @param		pAddress
 *  				Address to connect to.
 *  @param		pPort
 *  				Port number to connect to.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 * 1-Jun-2003	Added EINPROGRESS to the list of	Josh Williams	*
 *				errors to be "ignored" on connect.					*
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::Connect(const char *pAddress, unsigned short pPort)
{
	int		vResult = 0;
	bool	vRetval = true;
	char	vDestAddr[IP_ADDR_LEN];
	unsigned long vAddrIP;

	BEG_FUNC("Connect")("%p, %d", pAddress, pPort);

	/*
	 * Already connected?
	 */
	if (mStatus == SS_CONNECTED)
	{
		VERROR("Socket already connected to %s\n",
					mEndPoint.GetAddr());
		mError = EISCONN;
		return END_FUNC(false);
	}

	/*
	 * Socket initialized?
	 */
	if (mStatus != SS_ALLOCATED && mStatus != SS_BOUND)
	{
		VERROR("Socket not initialized\n");
		mError = ENOTSOCK;
		return END_FUNC(false);
	}

	/*
	 * Resolve the destination: Name or Address?
	 */
	if (!GetIPbyName(pAddress, vDestAddr, sizeof(vDestAddr)))
		strncpy(vDestAddr, pAddress, IP_ADDR_LEN-1);// Copy in the address passed and try it

	/*
	 * Convert the address to network byte order.
	 */
	vAddrIP = inet_addr(vDestAddr);
	if (vAddrIP == INADDR_NONE) // Invalid address format
	{
		VTRACE("Unable to resolve address\n");
		mError = EINVAL;
		return END_FUNC(false);
	}
	else
	{
		mEndPoint.sin_family = AF_INET;
		mEndPoint.SetPort(pPort);
		mEndPoint.SetAddrIP(vAddrIP);
		VTRACE("Address resolved to %s\n", vDestAddr);

		/*
		 * Try and establish the connection.
		 */
		vResult = connect(mHandle, (struct sockaddr*)&mEndPoint,
							sizeof(sockaddr_in));
		if (vResult != 0)
		{
			SETERRNO();
			if (errno != EWOULDBLOCK && errno != EINPROGRESS)
			{
				VERROR("Connect returned error-%s\n",
							strerror(errno));
				return END_FUNC(false);
			}
			else if (errno == EINPROGRESS)
			{
				VTRACE("Connect returned EINPROGRESS\n", 
							strerror(errno));
				mStatus = SS_CONNECTING;
				vRetval = true;
				vResult = 0;
			}
			else
			{
				mStatus = SS_CONNECTING;
				vRetval = true;
				vResult = 0;// We're in non-blocking mode,
							// so EWOULDBLOCK is ok.
			}
			mError = errno;
		}
		else
			mStatus = SS_CONNECTED; // Connection established.  EUREKA!

		if (vRetval != -1)
		{
			memset(vDestAddr, 0, IP_ADDR_LEN);
			GetSockName(vDestAddr, IP_ADDR_LEN, &pPort);
			mLocal.sin_family = AF_INET;
			mLocal.SetPort(pPort);
			mLocal.SetAddr(vDestAddr);
			/*
			 * With the connection either established or pending,
			 * start the processing thread.  We will either begin
			 * receiving data or receive notification of connection
			 * success/failure.
			 */
			if (!BeginProcess())
				VTRACE("Unable to enter the processing loop.\n");
		}
	}

	return END_FUNC(vRetval);
}

/*------------------------------------------------------------------*
 *                              Listen()                            *
 *------------------------------------------------------------------*/
/**
 *	@brief		Begin listening for incoming client connections.
 *
 *  @param		pConnBacklog Number of connections to allow in the
 *  				queue before rejecting.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::Listen(int pConnBacklog)
{
	BEG_FUNC("Listen")("%d", pConnBacklog);

	/*
	 * Already connected?
	 */
	if (mStatus == SS_CONNECTED)
	{
		VERROR("Socket already connected to %s\n",
					mEndPoint.GetAddr());
		mError = EISCONN;
		return END_FUNC(false);
	}

	/*
	 * Socket initialized and bound?
	 */
	if (mStatus != SS_ALLOCATED && mStatus != SS_BOUND)
	{
		VERROR("Socket not initialized or bound\n");
		mError = ENOTCONN;
		return END_FUNC(false);
	}

	/*
	 * Try to put the socket into listening mode.
	 */
	if (listen(mHandle, pConnBacklog) == 0)
		VTRACE("Socket successfully placed in listen mode.\n");
	else
	{
		SETERRNO();
		mError = errno;
		VERROR("Error entering listening mode.  Error-%d:%s\n",
					mError, strerror(mError));
		return END_FUNC(false);
	}

	/*
	 * With the socket in listening mode, start the processing
	 * thread so we can be notified of incoming connections.
	 */
	mStatus = SS_LISTENING;
	if (!BeginProcess())
		return END_FUNC(false);

	return END_FUNC(true);
}

/*------------------------------------------------------------------*
 *                              Receive()                           *
 *------------------------------------------------------------------*/
/**
 *	@brief		Retrieve data pending on this socket.
 *
 *	@remarks	Should be called by OnReceive(), when data is known
 *				to be available.
 *
 *  @param		pBuffer
 *  				Buffer to hold the retrieved data.
 *  @param		pBufLen
 *  				Maximum length of pcBuffer.
 *  @param		pFlags
 *  				Options to be passed to recv().
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
int VAsyncSocket::Receive(char *pBuffer, int pBufLen, int pFlags)
{
	bool	vContinue = true;
	int		vRetval = -1;

	BEG_FUNC("Receive")("%p, %d, %d", pBuffer, pBufLen, pFlags);

	/*
	 * Are we connected?
	 */
	if (mStatus != SS_CONNECTED)
	{
		VERROR("Not connected\n");
		mError = ENOTCONN;
		return END_FUNC(false);
	}

	if (pBuffer == NULL)
	{
		VERROR("Null buffer passed\n");
		mError = EINVAL;
		return END_FUNC(false);
	}
	else
	{
		if (pBufLen <= 0)
		{
			VERROR("Invalid buffer length specified\n");
			mError = EINVAL;
			return END_FUNC(false);
		}
		else
		{
			vRetval = recv(mHandle, pBuffer, pBufLen, pFlags);
			if (vRetval == -1)
			{
				SETERRNO();
				mError = errno;
				return END_FUNC(false);
			}
			else if (vRetval > 0)
			{
				pBuffer[vRetval] = 0;
				VTRACE("Received-%d bytes\n", vRetval);
			}
		}
	}

	return END_FUNC(vRetval);
}

/*------------------------------------------------------------------*
 *                              Send()                              *
 *------------------------------------------------------------------*/
/**
 *	@brief		Transmit data on this socket.
 *
 *  @remarks	Since we're in non-blocking mode, the number of bytes
 *				transmitted can be less than the number desired.
 *
 *  @param		pBuffer
 *  				Buffer containing the data to be sent.
 *  @param		pBufLen
 *  				Number of bytes to be transmitted.
 *
 *  @return		Returns the number of bytes actually sent, or -1 in
 *  			case of an error.  Call GetLastError() to get the
 *  			actual error code.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
int VAsyncSocket::Send(const char *pBuffer, int pBufLen)
{
	int vRetval = 0;

	BEG_FUNC("Send")("%p, %d", pBuffer, pBufLen);

	vRetval = send(mHandle, pBuffer, pBufLen, MSG_NOSIGNAL);

	if (vRetval > 0)
		VTRACE("%d bytes transmitted\n", vRetval);
	else if (vRetval < 0)
	{
		SETERRNO();
		mError = errno;
	}

	return END_FUNC(vRetval);
}

/*------------------------------------------------------------------*
 *                              Shutdown()                          *
 *------------------------------------------------------------------*/
/**
 *	@brief		Shutdown communications on this socket.
 *
 *	@remarks	Called when the local side wishes to disconnect
 *				communications.  Optional parameter specifies which
 *				communications should be closed (send, recv, or both).
 *
 *  @param		pHow
 *					Designates whether the socket should allow sends or
 *  				receives to finish.
 *
 *  @return		If the call is successful, returns 0.  Otherwise, -1 is
 *				returned, and a specific error code can be retrieved by
 *				calling GetLastError().
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
int VAsyncSocket::Shutdown(int pHow /*=SHUT_WR*/)
{
	int vRetval = 0;

	BEG_FUNC("Shutdown")("%d", pHow);

	if (mHandle != -1)
	{
		vRetval = shutdown(mHandle, pHow);
		if (vRetval != 0)
		{
			SETERRNO();
			mError = errno;
			VERROR("Error calling shutdown: %d:%s\n", errno, strerror(errno));
		}
	}

	return END_FUNC(vRetval);
}

/********************************************************************
 *                          C A L L B A C K S                       *
 ********************************************************************/

/*------------------------------------------------------------------*
 *                              OnConnect()                         *
 *------------------------------------------------------------------*/
/**
 *	@brief		Callback for establishing an outgoing connection.
 *
 *	@param		pErrorCode
 *					Error encountered(if any).
 *
 *	@remarks	Should be overridden by the program.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
void VAsyncSocket::OnConnect(int pErrorCode)
{
	/*
	 * If we're here, the application asked to be notified of FD_CONNECT
	 * events, but didn't override OnConnect().  Turn that flag off.
	 */
	mEvents &= ~FD_CONNECT;
}

/*------------------------------------------------------------------*
 *                              OnSend()                            *
 *------------------------------------------------------------------*/
/**
 *	@brief		Notification that the socket is now writable.
 *
 *	@param		pErrorCode
 *					Error encountered(if any).
 *
 *  @remarks	Should be overridden by the program, if interested
 *  			in socket writability.  Does not gurantee that a
 *  			call to Send() would necessarily transmit all data.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
void VAsyncSocket::OnSend(int pErrorCode)
{
	/*
	 * If we're here, the application asked to be notified of FD_WRITE
	 * events, but didn't override OnSend().  Turn that flag off.
	 */
	mEvents &= ~FD_WRITE;
}

/*------------------------------------------------------------------*
 *                              OnReceive()                         *
 *------------------------------------------------------------------*/
/**
 *	@brief		Fired when data is found on the socket, or the
 *				remote host closed the socket without shutdown.
 *
 *	@param		pErrorCode
 *					Error encountered(if any).
 *
 *  @remarks	Should be overridden by the program.  Should call
 *  			Receive() to check for data.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
void VAsyncSocket::OnReceive(int pErrorCode)
{
	/*
	 * If we're here, the application asked to be notified of FD_READ
	 * events, but didn't override OnReceive().  Turn that flag off.
	 */
	mEvents &= ~FD_READ;
}

/*------------------------------------------------------------------*
 *                              OnAccept()                          *
 *------------------------------------------------------------------*/
/**
 *	@brief		Callback for accepting an incoming connection.
 *
 *	@param		pErrorCode
 *					Error encountered(if any).
 *
 *  @remarks	Should be overridden by the program.  Needs to handle
 *      		accepting the incoming connection so it's slot in the
 *      		queue can be freed.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
void VAsyncSocket::OnAccept(int pErrorCode)
{
	/*
	 * If we're here, the application asked to be notified of FD_ACCEPT
	 * events, but didn't override OnAccept().  Turn that flag off.
	 */
	mEvents &= ~FD_ACCEPT;
}

/*------------------------------------------------------------------*
 *                              OnClose()                           *
 *------------------------------------------------------------------*/
/**
 *	@brief		Fired when the remote host gracefully closes the
 *				connection.
 *
 *	@param		pErrorCode
 *					Error encountered(if any).
 *
 *  @remarks	Should be overridden by the program.  OnClose() should
 *				call Shutdown().
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
void VAsyncSocket::OnClose(int pErrorCode)
{
	/*
	 * If we're here, the application asked to be notified of FD_CLOSE
	 * events, but didn't override OnClose().  Turn that flag off.
	 */
	mEvents &= ~FD_CLOSE;
}

/********************************************************************
 *                          I N T E R N A L S                       *
 ********************************************************************/

/*------------------------------------------------------------------*
 *								SetDirty()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Notifies the Process thread that the vector of polled
 *				sockets needs to be updated.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
void VAsyncSocket::SetDirty()
{
	mPollFDs.Lock();
	mDirty = true;
	mPollFDs.Unlock();
}

/*------------------------------------------------------------------*
 *                          GetIPbyName()                           *
 *------------------------------------------------------------------*/
/**
 *	@brief		DNS lookup function.  Converts host to IP address.
 *
 *  @param		pHost
 *  				Host name to be converted.
 *  @param		pIPaddr
 *					Buffer to hold the converted addr.
 *  @param		pSize
 *  				Size of pcIPaddr.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::GetIPbyName(const char *pHost, char *pIPaddr, int pSize)
{
	struct	hostent *vHostInfo;
	bool	vRetval = false;

	BEG_FUNC("GetIPbyName")("%p(%s), %p, %d", pHost, pHost, pIPaddr, pSize);

	*pIPaddr = 0;
	vHostInfo = gethostbyname(pHost);

	if (vHostInfo)
	{
		struct in_addr *vAddr;
		vAddr = (struct in_addr*)vHostInfo->h_addr_list[0];
		strncpy(pIPaddr, inet_ntoa(*vAddr), pSize-1);
		VTRACE("Address (%s) resolved to (%s)\n", pHost, pIPaddr);
		return END_FUNC(true);
	}
	else
	{
		SETERRNO();
		mError = errno;
		return END_FUNC(false);
	}
}


/*------------------------------------------------------------------*
 *                           BeginProcess()                         *
 *------------------------------------------------------------------*/
/**
 *	@brief		Puts this socket into the processing queue
 *
 *	@remarks	Places this socket object into the map of sockets
 *				to be processed for events.  If the processing
 *				thread is not active, this function will start it.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VAsyncSocket::BeginProcess()
{
	bool vRetval = false;
	mSocketMap.Lock();

	mSocketMap[mHandle] = this;
	SetDirty();
	if (!mProcessing)
	{
		mProcessLock.Lock();
#if defined(WIN32) || defined(_WINDOWS)     /* Windows platforms */
		mProcessThread = _beginthread(_Process, 0, this);
		if (mProcessThread != -1L)
#else                                       /* Unix/linux */
		int vResult = pthread_create(&mProcessThread, NULL, _Process, this);
		if (vResult == 0)
#endif                                      /* End platform specific */
		{
			vRetval = true;
			mProcessing = true;
		}
		else
		{
			SETERRNO();
			mError = errno;
		}
	}
	else
		vRetval = true;
	mSocketMap.Unlock();

	return vRetval;
}

/*------------------------------------------------------------------*
 *                              Process()                           *
 *------------------------------------------------------------------*/
/**
 *	@brief		Handle activity on this socket.
 *
 *	@remarks	The first instance of this function is static, to
 *				allow it to be passed to _beginthread/pthread_create.
 *
 *  @param		pParam
 *  				Pointer to this VAsyncSocket object.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 * 22-May-2003  Added calls to SendErrorCode()      Josh Williams   *
 * 25-May-2003  Modified function to allow it to    Josh Williams   *
 *              handle events for ALL INSTANCES                     *
 *              of class VAsyncSocket.                                   *
 *                                                                  *
 *------------------------------------------------------------------*/
#if defined(WIN32) || defined(_WINDOWS)     /* Windows platforms */
void VAsyncSocket::_Process(void *pParam)
{
	((VAsyncSocket*)pParam)->Process();
	return;
}
#else
void* VAsyncSocket::_Process(void *pParam)
{
	((VAsyncSocket*)pParam)->Process();
	return NULL;
}
#endif

#define CALL_WITH_NULL_CHECK(_object, _function)\
{\
	if (_object != NULL)\
		_object->_function;\
}

/** @brief See _Process(void* pParam) */
void VAsyncSocket::Process()
{
	SOCKET					vTempHandle = -1;
	struct timeval			vSelTime;
	fd_set					vReadFDs, vWriteFDs, vExcpFDs;
	int	  					vRetval = -1;
	SOCKET					vHandle;
	int						vMaxSocket = -1;
	char					vBuffer[1];
	bool					vRead, vWrite;
	int						vError, vErrorSize;
	VSocketMap::iterator	vIt;
	VPollVector::iterator	vIt2;
	VAsyncSocket			*vSocket;

	BEG_FUNC("Process")(NULL);

	vTempHandle = mHandle;
	vErrorSize = sizeof(vError);

	for (;;)
	{
		vSelTime.tv_sec		= 1;
		vSelTime.tv_usec	= 0;
		FD_ZERO(&vReadFDs);
		FD_ZERO(&vWriteFDs);
		FD_ZERO(&vExcpFDs);

		/*
		 * Lock the socket map and load up the fd_set's
		 */
		mSocketMap.Lock();
		if (mSocketMap.size() == 0)
		{
			mProcessing = false;
			mSocketMap.Unlock();
			break;
		}
		mPollFDs.Lock();
		if (mDirty || mNumPollFDs == 0)
		{
			mPollFDs.clear();
			mPollFDs.reserve(mSocketMap.size());
			mNumPollFDs = 0;
			VPollSock* vBack;
			for (vIt = mSocketMap.begin(); vIt != mSocketMap.end(); vIt++)
			{
				if (vIt->first != -1 && vIt->second->GetEvents())
				{
					vRead = false;
					vWrite = false;
					vHandle = vIt->first;
					vSocket = vIt->second;
					long vEvents = vSocket->GetEvents();
					if ((vEvents & (FD_READ) && vSocket->GetStatus() == SS_CONNECTED) ||
						(vEvents & (FD_ACCEPT) && vSocket->GetStatus() == SS_LISTENING) ||
						(vEvents & (FD_CLOSE) && vSocket->GetStatus() == SS_CONNECTED))
						vRead = true;
					if ((vEvents & (FD_WRITE) && vSocket->GetStatus() == SS_CONNECTED) ||
						(vEvents & (FD_CONNECT) && vSocket->GetStatus() == SS_CONNECTING))
						vWrite = true;
					if (vRead || vWrite)
					{
						if (vHandle > vMaxSocket)
							vMaxSocket = vHandle;
						mPollFDs.resize(mPollFDs.size()+1);
						vBack = &mPollFDs.back();
						vBack->mHandle = vHandle;
						vBack->mEvents = vSocket->GetEvents();
						vBack->mRead = vRead;
						vBack->mWrite = vWrite;
						vBack->mStatus = vSocket->GetStatus();
						mNumPollFDs++;
					}
				}
			}
			mDirty = false;
		}
		VTRACE("Sockets in poll vector: %d\n", mPollFDs.size());

		mSocketMap.Unlock();
		mPollFDs.Unlock();

		FD_ZERO(&vReadFDs);
		FD_ZERO(&vWriteFDs);
		FD_ZERO(&vExcpFDs);
		for (vIt2 = mPollFDs.begin(); vIt2 != mPollFDs.end(); vIt2++)
		{
			if (vIt2->mRead)
				FD_SET(vIt2->mHandle, &vReadFDs);
			if (vIt2->mWrite)
				FD_SET(vIt2->mHandle, &vWriteFDs);
			FD_SET(vIt2->mHandle, &vExcpFDs);
		}

		vRetval = select(vMaxSocket+1, &vReadFDs, &vWriteFDs, &vExcpFDs, &vSelTime);
		SETERRNO();
		mError = errno;

		if (vRetval < 0)
			VERROR("Error calling select.  Error-%d:%s\n", errno, strerror(errno));
		else if (vRetval == 0)
			VTRACE("select returned 0\n");
		else
		{
			for (vIt2 = mPollFDs.begin(); vIt2 != mPollFDs.end(); vIt2++)
			{
				vSocket = FromHandle(vIt2->mHandle);
				if (vSocket)
				{
					CALL_WITH_NULL_CHECK(vSocket, GetSockOpt(SO_ERROR, &vError, &vErrorSize, SOL_SOCKET));
					if (FD_ISSET(vIt2->mHandle, &vExcpFDs))
					{
						VTRACE("Socket %d has exception set\n", vIt2->mHandle);
						switch (vIt2->mStatus)
						{
						case SS_CONNECTING:
							if (vIt2->mEvents & (FD_CONNECT))
								CALL_WITH_NULL_CHECK(vSocket, OnConnect(vError));
							break;
						case SS_CONNECTED:
							// Add code to handle OOB data
							break;
						default:
							// we should not be here
							break;
						}
					}
					if (FD_ISSET(vIt2->mHandle, &vWriteFDs))
					{
						VTRACE("Socket %d writable\n", vIt2->mHandle);
						switch (vIt2->mStatus)
						{
						case SS_UNALLOCATED:
							VTRACE("socket %d is SS_UNALLOCATED\n", vIt2->mHandle);
							break;
						case SS_CONNECTING:
							if (vIt2->mEvents & (FD_CONNECT))
							{
								CALL_WITH_NULL_CHECK(vSocket, SetStatus(SS_CONNECTED));
								SetDirty();
								CALL_WITH_NULL_CHECK(vSocket, OnConnect(vError));
							}
							break;
						case SS_CONNECTED:
							if (vIt2->mEvents & (FD_WRITE))
							{
								CALL_WITH_NULL_CHECK(vSocket, SetStatus(SS_CONNECTED));
								SetDirty();
								CALL_WITH_NULL_CHECK(vSocket, OnSend(vError));
							}
							break;
						default:
							// we should definitely not be here
							break;
						}
					}
					if (FD_ISSET(vIt2->mHandle, &vReadFDs))
					{
						VTRACE("Socket %d readable\n", vIt2->mHandle);
						switch (vIt2->mStatus)
						{
						case SS_UNALLOCATED:
							VTRACE("socket %d is SS_UNALLOCATED\n", vIt2->mHandle);
							break;
						case SS_LISTENING:
							if (vIt2->mEvents & (FD_ACCEPT))
							{
								VTRACE("Calling OnAccept() for socket %d\n", vIt2->mHandle);
								CALL_WITH_NULL_CHECK(vSocket, OnAccept(vError));
							}
							else
							{
								/*
								 * If we're in listening mode and they're not watching
								 * FD_ACCEPT, we're wasting our time.
								 */
								VTRACE("Socket %d in Listening mode, but not watching FD_ACCEPT\n", vIt2->mHandle);
								CALL_WITH_NULL_CHECK(vSocket, Shutdown(vError));
							}
							break;
						case SS_CONNECTED:
							if (vSocket != NULL)
							{
								vRetval = vSocket->Receive(vBuffer, 1, MSG_PEEK);
								if (vRetval == 0)
								{
									VTRACE("socket shutdown by peer.\n");
									if (vIt2->mEvents & (FD_CLOSE))
									{
										CALL_WITH_NULL_CHECK(vSocket, OnClose(vError));
									}
									else if (vIt2->mEvents & (FD_READ))
									{
										CALL_WITH_NULL_CHECK(vSocket, OnReceive(vError));
									}
								}
								else if (vRetval == -1)
								{
									if (vIt2->mEvents & (FD_CLOSE))
									{
										CALL_WITH_NULL_CHECK(vSocket, OnClose(vError));
									}
									else if (vIt2->mEvents & (FD_READ))
									{
										CALL_WITH_NULL_CHECK(vSocket, OnReceive(vError));
									}
								}
								else
									CALL_WITH_NULL_CHECK(vSocket, OnReceive(vError));
							}
							break;
						default:
							// should not be here
							break;
						}
					}
				}
				else
					VTRACE("Socket %d disappeared while in select\n", vIt2->mHandle);
			} // Vector iteration

		} // nRetval
	}

	mProcessLock.Unlock();
	END_FUNCV();
}

} // End Namespace

/* vi: set ts=4: */
