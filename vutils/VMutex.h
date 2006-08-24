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
#if !defined(__VTMUTEX_H_INCLUDED__)
#define __VTMUTEX_H_INCLUDED__

/* System Headers */
#if defined(WIN32) || defined(_WINDOWS)     /* Windows platforms    */
#include <windows.h>
#else                                       /* Unix/linux */
#include <pthread.h>
#endif                                      /* End platform specific */

/* Local Headers */

namespace VUtils
{

class VMutex
{

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
