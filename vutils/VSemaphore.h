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
#if !defined(__VSEMAPHORE_H_INCLUDED__)
#define __VSEMAPHORE_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */
#if VPLATFORM == PLATFORM_WINDOWS
#include <windows.h>
typedef HANDLE	sem_handle;
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
#include <semaphore.h>
typedef sem_t	sem_handle;
#endif

/* Local Headers */
#include <vutils/VException.h>

namespace VUtils
{

class VSemaphore
{

public:
    VSemaphore(const char *pName = NULL) {
#if VPLATFORM == PLATFORM_WINDOWS
		if (pName == NULL)
			EXCPT(VException, "Must supply a semaphore name");
		strncpy(mSemaName, pName, 255);
		if ((mHandle = CreateSemaphore(NULL, 0, 1, mSemaName)) == NULL)
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
		if (sem_init(&mHandle, 0, 0) != 0)
#endif
		{
			EXCPT(VException, "Unable to initialize Semaphore");
		}
    }

    void Wait(void) {
#if VPLATFORM == PLATFORM_WINDOWS
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
		sem_wait(&mHandle);
#endif
    }

    void Fire(void) {
#if VPLATFORM == PLATFORM_WINDOWS
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
		sem_post(&mHandle);
#endif
    }

private:
    sem_handle		mHandle;
	char			mSemaName[256];
};

} // End Namespace

#endif // __VSEMAPHORE_H_INCLUDED__

/* vi: set ts=4: */