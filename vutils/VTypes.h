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
#if !defined(__VTYPES_H_INCLUDED__)
#define __VTYPES_H_INCLUDED__

#include <vutils/VPlatform.h>

typedef unsigned short  VUSHORT;
typedef unsigned int    VUINT;
typedef unsigned long   VULONG;
typedef unsigned char   VBYTE;

#if VPLATFORM == PLATFORM_WINDOWS
#define THREAD_FUNC     void
#define THREAD_RETURN   return
#else
#define THREAD_FUNC     void*
#define THREAD_RETURN   return NULL
#endif

enum VErrorCodes {
	VERR_SUCCESS = 0,
	VERR_UNKNOWN = 100,     // 100
	VERR_INVALID_PATH,      // 101
	VERR_FILE_NOT_FOUND,    // 102
	VERR_FILE_ALREADY_OPEN, // 103
	VERR_INVALID_ARGUMENT,  // 104
	VERR_OPEN_FILE,         // 105
	VERR_INVALID_STATUS,    // 106
	VERR_FILE_NOT_OPEN,     // 107
	VERR_FILE_NOT_OPEN_R,   // 108
	VERR_FILE_NOT_OPEN_W,   // 109
	VERR_FILE_NOT_OPEN_RW,  // 110
	VERR_READ_FILE,         // 111
	VERR_BAD_FILE_HANDLE,   // 112
	VERR_CREATE_SOCKET,     // 113
	VERR_CONNECT,           // 114
	VERR_COMM,              // 115
	VERR_NULL_POINTER,      // 116
	VERR_INVALID_VALUE,     // 117
	VERR_TIMEOUT,           // 118
	VERR_RESOLVE_ERROR,     // 119
	VERR_ALREADY_IN_USE,    // 120
	VERR_NET,               // 121
	VERR_MAX                // 122
};

typedef VErrorCodes VRESULT;

#endif // __VTYPES_H_INCLUDED__
