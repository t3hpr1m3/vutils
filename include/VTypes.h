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
#if !defined(__VTYPES_H_INCLUDED__)
#define __VTYPES_H_INCLUDED__

typedef unsigned short  VUSHORT;
typedef unsigned int    VUINT;
typedef unsigned long   VULONG;
typedef unsigned char   VBYTE;

#if defined(WIN32) || defined(_WINDOWS)
#define THREAD_FUNC		void
#define THREAD_RETURN	return
#else
#define THREAD_FUNC		void*
#define THREAD_RETURN	return NULL
#endif

enum VErrorCodes
{
	VERR_SUCCESS = 0,
	VERR_UNKNOWN = 100,		// 100
	VERR_INVALID_PATH,		// 101
	VERR_FILE_NOT_FOUND,	// 102
	VERR_FILE_ALREADY_OPEN,	// 103
	VERR_INVALID_ARGUMENT,	// 104
	VERR_OPEN_FILE,			// 105
	VERR_INVALID_STATUS,	// 106
	VERR_FILE_NOT_OPEN,		// 107
	VERR_FILE_NOT_OPEN_R,	// 108
	VERR_FILE_NOT_OPEN_W,	// 109
	VERR_FILE_NOT_OPEN_RW,	// 110
	VERR_READ_FILE,			// 111
	VERR_BAD_FILE_HANDLE,	// 112
	VERR_CREATE_SOCKET,     // 113
	VERR_CONNECT,           // 114
	VERR_COMM,              // 115
	VERR_NULL_POINTER,		// 116
	VERR_INVALID_VALUE,		// 117
	VERR_TIMEOUT,			// 118
	VERR_RESOLVE_ERROR,		// 119
	VERR_MAX				// 120
};

typedef VErrorCodes VRESULT;

#endif // __VTYPES_H_INCLUDED__
