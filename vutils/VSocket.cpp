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
#include <vutils/VSocket.h>

/* System Headers */
#include <time.h>
#include <errno.h>
#include <stdarg.h>

/* Local Headers */
#include <vutils/VLog.h>

DECLARE_CLASS( "VSocket" );

namespace VUtils
{

/********************************************************************
 *																	*
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 *																	*
 ********************************************************************/
VSocket::VSocket()
{
	mStatus		= SS_UNALLOCATED;
	mHandle		= -1;
	mPort		= 0;
	mTimeout	= 1;
	mError		= 0;
}

VSocket::~VSocket()
{
	if (mStatus != SS_UNALLOCATED)
		Close();
}

/********************************************************************
 *																	*
 *                        A T T R I B U T E S                       *
 *																	*
 ********************************************************************/

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
bool VSocket::GetPeerName(char *pAddress, int pAddrLen,
							VUSHORT *pPort /*=NULL*/)
{
	bool		vRetval = true;
	VSockAddr	vAddr;
	socklen_t	vSAlen;

	BEG_FUNC("GetPeerName")("%p(%s), %d, %d", pAddress, pAddress,
								pAddrLen, pPort);

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
bool VSocket::GetPeerName(sockaddr *pSockAddr, socklen_t *pSockAddrLen)
{
	bool	vRetval = true;

	BEG_FUNC("GetPeerName")("%p, %p", pSockAddr, pSockAddrLen);

	*pSockAddrLen = sizeof(*pSockAddr);

	/* Obtail remote connection info */
	if (getpeername(mHandle, pSockAddr, pSockAddrLen) == 0)
	{
		SETERRNO();
		mError = errno;
	}

	return END_FUNC(vRetval);
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
bool VSocket::GetSockName(char *pAddress, int pAddrLen,
							VUSHORT *pPort /*=NULL*/)
{
	bool		vRetval = true;
	VSockAddr	vAddr;
	socklen_t	vSAlen = sizeof(vAddr);

	BEG_FUNC("GetSockName")("%p, %d, %d", pAddress, pAddrLen, pPort);

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
		vRetval = false;
	}

	return END_FUNC(vRetval);
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
bool VSocket::GetSockName(sockaddr *pSockAddr, socklen_t *pSockAddrLen)
{
	bool vRetval = true;

	BEG_FUNC("GetSockName")("%p, %p", pSockAddr, pSockAddrLen);

	*pSockAddrLen = sizeof(*pSockAddr);

	/* Obtail local connection info */
	if (getpeername(mHandle, pSockAddr, pSockAddrLen) == 0)
	{
		SETERRNO();
		mError = errno;
		vRetval = false;
	}

	return END_FUNC(vRetval);
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
bool VSocket::GetSockOpt(int pOptName, void *pOptValue,
						int *pOptLen, int pLevel /*=SOL_SOCKET*/)
{
	bool	vRetval = true;

	BEG_FUNC("GetSockOpt")("%d, %p, %p, %d", pOptName, pOptValue,
							pOptLen, pLevel);

	if (getsockopt(mHandle, pLevel, pOptName, (char*)pOptValue,
					(socklen_t*)pOptLen) != 0)
	{
		SETERRNO();
		mError = errno;
		vRetval = false;
	}

	return END_FUNC(vRetval);
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
bool VSocket::SetSockOpt(int pOptName, const void *pOptValue,
						int pOptLen, int pLevel /*=SOL_SOCKET*/)
{
	bool	vRetval = true;

	BEG_FUNC("SetSockOpt")("%d, %p, %d, %d", pOptName, pOptValue,
								pOptLen, pLevel);

	if (setsockopt(mHandle, pLevel, pOptName, (const char*)pOptValue,
					pOptLen) != 0)
	{
		SETERRNO();
		mError = errno;
		vRetval = false;
	}

	return END_FUNC(vRetval);
}


/********************************************************************
 *																	*
 *                        O P E R A T I O N S                       *
 *																	*
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
 *  @param		pSockAddr
 *  				Optional address to bind to.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VSocket::Create(VUSHORT pPort /*=0*/,
		int pSocketType /*=SOCK_STREAM*/, const char* pSockAddr/*=NULL*/)
{
	SOCKET	vSocketID = -1; // Default to an invalid state
	bool	vResult = false;
	int		vRetval = 0;
	long	vLongBlock = 0;		

	BEG_FUNC("Create")("%d, %d, %p", pPort, pSocketType, pSockAddr);

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
		/*
		 * If a port/address were supplied, try and bind to them.
		 */
		if (pPort != 0)
			vResult = Bind(pPort, pSockAddr);
		else
			vResult = true;
	}

	if (vResult)
		VTRACE("Successfully created socket.\n");
	else
		VTRACE("Unable to create socket.\n");

	return END_FUNC(vResult);
}

/*------------------------------------------------------------------*
 *                              Attach()                            *
 *------------------------------------------------------------------*/
/**
 *	@brief		Attach this VSocket to a pre-existing socket handle.
 *
 *  @remarks	Attaches this object to an existing socket handle and
 *  			enters the processing loop.
 *
 *  @param		pHandle
 *  				Handle to the socket to attach to.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
bool VSocket::Attach(SOCKET pHandle)
{
	char	vAddress[51];
	VUSHORT vPort = 0;

	BEG_FUNC("Attach")("%d", pHandle);

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
	mHandle = pHandle;
	GetPeerName(vAddress, 50, &vPort);
	mEndPoint.SetPort(vPort);
	mEndPoint.SetAddr(vAddress);

	memset(vAddress, 0, 50);
	GetSockName(vAddress, 50, &vPort);
	mLocal.SetPort(vPort);
	mLocal.SetAddr(vAddress);

	mStatus = SS_CONNECTED;

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
SOCKET VSocket::Detach()
{
	SOCKET vHandle = mHandle;

	BEG_FUNC("Detach")(NULL);

	if (mHandle != -1)
	{
		mStatus = SS_UNALLOCATED;
		mHandle = -1;
	}
	else
		VERROR("Invalid socket handle\n");

	return END_FUNC(vHandle);
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
bool VSocket::Accept(VSocket* pNewSocket, sockaddr* pSockAddr/*=NULL*/,
					socklen_t* pSockAddrLen/*=NULL*/)
{
	SOCKET	vTempHandle = -1; /* temp Socket ID */
	SOCKET	vNewHandle = -1; /* new Socket ID  */
	int		vPeerSize;
	bool	vRetval = false;
	bool	vOk = true;

	BEG_FUNC("Accept")("%p, %p, %p", pNewSocket, pSockAddr, pSockAddrLen);

	if (mStatus != SS_LISTENING) /* Socket not in listening mode! */
	{
		VTRACE("Socket not in listening mode. Cannot accept "
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
		vRetval = true;
	}
	else
	{
		SETERRNO();
		mError = errno;
		VTRACE("accept failed with error code %d\n", errno);
	}

	return END_FUNC(vRetval);
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
bool VSocket::Bind(VUSHORT pPort, const char *pAddress/*=NULL*/)
{
	char	vDestAddr[IP_ADDR_LEN];
	bool	vRetval = false;
	int		vReturnCode = 0;
	VULONG vAddrIP;

	BEG_FUNC("Bind")("%d, %p", pPort, pAddress);

	/*
	 * Socket initialized?
	 */
	if (mStatus != SS_ALLOCATED)
	{
		VTRACE("Error-Socket not initialized\n");
		mError = ENOTSOCK;
		return END_FUNC(false);
	}

	/*
	 * Are we already bound?
	 */
	if (mStatus == SS_BOUND)
	{
		VTRACE("Error-Socket already bound to %s:%d\n",
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
		return END_FUNC(true);
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
		VTRACE("bind failed.  Errno code %d\n", mError);
		return END_FUNC(false);
	}
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
void VSocket::Close()
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
 *                                                                  *
 *------------------------------------------------------------------*/
bool VSocket::Connect(const char *pAddress, VUSHORT pPort)
{
	int		vResult = 0;
	bool	vRetval = true;
	char	vDestAddr[IP_ADDR_LEN];
	VULONG	vAddrIP;

	BEG_FUNC("Connect")("%p[%s], %d, %d", pAddress, pAddress, pPort);

	/*
	 * Already connected?
	 */
	if (mStatus == SS_CONNECTED)
	{
		VERROR("Socket already connected %s:%d\n", mEndPoint.GetAddr(),
				mEndPoint.GetPort());
		mError = EISCONN;
		return END_FUNC(VERR_ALREADY_IN_USE);
	}

	/*
	 * Socket initialized?
	 */
	if (mStatus != SS_ALLOCATED && mStatus != SS_BOUND)
	{
		VERROR("Error-Socket not initialized\n");
		mError = ENOTSOCK;
		return END_FUNC(VERR_INVALID_STATUS);
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
		VERROR("Unable to resolve address: %s\n", vDestAddr);
		mError = EINVAL;
		return END_FUNC(VERR_RESOLVE_ERROR);
	}
	else
	{
		mEndPoint.SetPort(pPort);
		mEndPoint.SetAddrIP(vAddrIP);
		VTRACE("Address %s resolved to %s\n", vDestAddr, mEndPoint.GetAddr());

		mStatus = SS_CONNECTING;

		/*
		 * Try and establish the connection.
		 */
		vResult = connect(mHandle, (struct sockaddr*)&mEndPoint,
							sizeof(sockaddr_in));
		if (vResult != 0)
		{
			SETERRNO();
			mError = errno;
			VERROR("connect returned error-%d:%s\n", errno,
						strerror(errno));
			return END_FUNC(VERR_CONNECT);
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
bool VSocket::Listen(int pConnBacklog)
{
	bool vRetval = true;

	BEG_FUNC("Listen")("%d", pConnBacklog);

	/*
	 * Already connected?
	 */
	if (mStatus == SS_CONNECTED)
	{
		VTRACE("Error-Socket already connected to %s\n",
					mEndPoint.GetAddr());
		mError = EISCONN;
		vRetval = false;
	}

	/*
	 * Socket initialized and bound?
	 */
	if (vRetval && mStatus != SS_ALLOCATED && mStatus != SS_BOUND)
	{
		VTRACE("Error-Socket not initialized or bound\n");
		mError = ENOTCONN;
		vRetval = false;
	}

	if (vRetval)
	{
		/*
		 * Try to put the socket into listening mode.
		 */
		if (listen(mHandle, pConnBacklog) == 0)
			VTRACE("Socket successfully placed in listen mode.\n");
		else
		{
			SETERRNO();
			mError = errno;
			vRetval = false;
			VTRACE("Error entering listening mode.  Error-%d:%s\n",
						mError, strerror(mError));
		}

		if (vRetval)
		{
			/*
			 * With the socket in listening mode, start the processing
			 * thread so we can be notified of incoming connections.
			 */
			mStatus = SS_LISTENING;
		}
	}

	return END_FUNC(vRetval);
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
int VSocket::Receive(char *pBuffer, int pBufLen, int pFlags)
{
	bool	vContinue = true;
	int		vRetval = -1;

	BEG_FUNC("Receive")("%p, %d, %d", pBuffer, pBufLen, pFlags);

	/*
	 * Are we connected?
	 */
	if (mStatus != SS_CONNECTED)
	{
		VTRACE("Error-not connected\n");
		mError = ENOTCONN;
		vContinue = false;
	}

	if (pBuffer == NULL)
	{
		VTRACE("Error-null buffer passed\n");
		mError = EINVAL;
		vContinue = false;
	}
	else
	{
		if (pBufLen <= 0)
		{
			VTRACE("Error-invalid buffer length specified\n");
			mError = EINVAL;
		}
		else
		{
			vRetval = recv(mHandle, pBuffer, pBufLen, pFlags);
			if (vRetval == -1)
			{
				SETERRNO();
				mError = errno;
			}
			else if (vRetval > 0)
			{
				pBuffer[vRetval] = 0;
				VTRACE("Received-%s\n", pBuffer);
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
int VSocket::Send(const char *pBuffer, int pBufLen)
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
int VSocket::Shutdown(int pHow /*=SHUT_WR*/)
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
			VTRACE("Error calling shutdown: %d:%s\n", errno, strerror(errno));
		}
	}

	return END_FUNC(vRetval);
}

/*------------------------------------------------------------------*
 *                              Select()                          	*
 *------------------------------------------------------------------*/
/**
 *	@brief		Polls the socket for read/writability.
 *
 *  @param		pFlags
 *					FD_READ, FD_WRITE, or both.
 *	@param		pTimeout
 *					Length of time to poll.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
VRESULT VSocket::Select(long *pFlags, VUSHORT pTimeout)
{
	SOCKET			vTempSock = INVALID_SOCKET;
	struct timeval	vSeltime;
	fd_set			vReadfds, vWritefds, vExcpfds;
	int				vRetval = -1;
	long			vTempFlags;

	BEG_FUNC("Select")("%p, %ld, %d", pFlags, *pFlags, pTimeout);
	vTempSock = mHandle;

	if (*pFlags == 0L)
	{
		VTRACE("Nothing to do.  0 passed for flags\n");
		return END_FUNC(VERR_SUCCESS);
	}

	vSeltime.tv_sec = pTimeout;
	vSeltime.tv_usec = 0;
	vTempFlags = *pFlags;

	while (1)
	{
		FD_ZERO(&vReadfds);
		FD_ZERO(&vWritefds);
		FD_ZERO(&vExcpfds);

		VTRACE(" * Flags passed *\n");
		if (vTempFlags & FD_READ)
		{
			VTRACE("READ\n");
			FD_SET(vTempSock, &vReadfds);
		}
		if (vTempFlags & FD_WRITE)
		{
			VTRACE("WRITE\n");
			FD_SET(vTempSock, &vWritefds);
		}
		FD_SET(vTempSock, &vExcpfds);
		
		VTRACE("Socket ID: %d\n", vTempSock);

		*pFlags = 0L;

		vRetval = select(vTempSock+1, &vReadfds, &vWritefds, &vExcpfds, &vSeltime);
		SETERRNO();
		mError = errno;

		if (vRetval < 0)
		{
			VERROR("Error calling select().\nError => [%d:%s]\n", errno, strerror(errno));
			return END_FUNC(VERR_NET);
		}
		else if (vRetval == 0)
		{
			VTRACE("select() returned 0\n");
			return END_FUNC(VERR_SUCCESS);
		}
		else
		{
            VTRACE("select returned %d\n", vRetval);
			if (FD_ISSET(vTempSock, &vReadfds))
			{
				VTRACE("Socket readable\n");
				*pFlags |= FD_READ;
			}
			if (FD_ISSET(vTempSock, &vWritefds))
			{
				VTRACE("Socket writable\n");
				*pFlags |= FD_WRITE;
			}

			if (FD_ISSET(vTempSock, &vExcpfds))
			{
				VTRACE("Socket has exception set\n");
				VTRACE("errno => %d:%s\n", errno, strerror(errno));
				*pFlags |= FD_READ; // so the client picks up on the error
			}
			break;
		}
	}

	return END_FUNC(VERR_SUCCESS);
}

/********************************************************************
 *																	*
 *                          O P E R A T O R S                       *
 *																	*
 ********************************************************************/

/********************************************************************
 *																	*
 *                          C A L L B A C K S                       *
 *																	*
 ********************************************************************/

/********************************************************************
 *																	*
 *                          I N T E R N A L S                       *
 *																	*
 ********************************************************************/

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
bool VSocket::GetIPbyName(const char *pHost, char *pIPaddr, int pSize)
{
	struct	hostent *vHostInfo;
	bool	vRetval = false;

	BEG_FUNC("GetIPbyName")("%p, %p, %d", pHost, pIPaddr, pSize);

	*pIPaddr = 0;
	vHostInfo = gethostbyname(pHost);

	if (vHostInfo)
	{
		struct in_addr *vAddr;
		vAddr = (struct in_addr*)vHostInfo->h_addr_list[0];
		strncpy(pIPaddr, inet_ntoa(*vAddr), pSize-1);
		VTRACE("Address (%s) resolved to (%s)\n", pHost, pIPaddr);
		vRetval = true;
	}
	else
	{
		SETERRNO();
		mError = errno;
	}

	return END_FUNC(vRetval);
}

} // End Namespace

/* vi: set ts=4: */
