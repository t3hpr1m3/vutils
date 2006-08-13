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
#if !defined(__VMUTEX_H_INCLUDED__)
#define __VMUTEX_H_INCLUDED__

#include "VPlatform.h"

/* System Headers */
#if VPLATFORM == PLATFORM_WINDOWS
#include <windows.h>
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
#include <pthread.h>
#endif

/* Local Headers */

namespace VUtils
{

class VMutex
{

public:
    VMutex(void) {
#if VPLATFORM == PLATFORM_WINDOWS
        InitializeCriticalSection(&m_lock);
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
		pthread_mutex_init(&m_lock, NULL);
#endif
    }

    void Lock(void) {
#if VPLATFORM == PLATFORM_WINDOWS
        EnterCriticalSection(&m_lock);
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
		pthread_mutex_lock(&m_lock);
#endif
    }

    void Unlock(void) {
#if VPLATFORM == PLATFORM_WINDOWS
        LeaveCriticalSection(&m_lock);
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
		pthread_mutex_unlock(&m_lock);
#endif
    }

private:
#if VPLATFORM == PLATFORM_WINDOWS
    CRITICAL_SECTION m_lock;
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
    pthread_mutex_t m_lock;
#endif                                      /* End platform specific */

};

} // End Namespace

#endif // __VMUTEX_H_INCLUDED__

/* vi: set ts=4: */
