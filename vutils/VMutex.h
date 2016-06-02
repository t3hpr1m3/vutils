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
#if !defined(__VTMUTEX_H_INCLUDED__)
#define __VTMUTEX_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */
#if defined(WIN32) || defined(_WINDOWS)     /* Windows platforms    */
#include <windows.h>
#else                                       /* Unix/linux */
#include <pthread.h>
#endif                                      /* End platform specific */

/* Local Headers */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace VUtils {

class VMutex {

public:
	VMutex(void) {
#if defined(WIN32) || defined(_WINDOWS)     /* Windows platforms */
		InitializeCriticalSection(&m_lock);
#else                                       /* Unix/linux */
		pthread_mutex_init(&m_lock, NULL);
#endif                                      /* End platform specific */
	}

	void Lock(void) {
#if defined(WIN32) || defined(_WINDOWS)     /* Windows platforms */
		EnterCriticalSection(&m_lock);
#else                                       /* Unix/linux */
		pthread_mutex_lock(&m_lock);
#endif                                      /* End platform specific */
	}

	void Unlock(void) {
#if defined(WIN32) || defined(_WINDOWS)     /* Windows platforms */
		LeaveCriticalSection(&m_lock);
#else                                       /* Unix/linux */
		pthread_mutex_unlock(&m_lock);
#endif                                      /* End platform specific */
	}

private:
#if defined(WIN32) || defined(_WINDOWS)     /* Windows platforms */
	CRITICAL_SECTION m_lock;
#else                                       /* Unix/linux */
	pthread_mutex_t m_lock;
#endif                                      /* End platform specific */

};

} // End Namespace

#endif // __VTMUTEX_H_INCLUDED__
