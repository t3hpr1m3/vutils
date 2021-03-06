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
#if !defined(__VSEMAPHORE_H_INCLUDED__)
#define __VSEMAPHORE_H_INCLUDED__

//
// TODO: PORT THIS MOFO
//

#include <vutils/VPlatform.h>

/* System Headers */
#if VPLATFORM == PLATFORM_WINDOWS
#include <windows.h>
typedef HANDLE  sem_handle;
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
#include <semaphore.h>
#include <sys/time.h>
typedef sem_t   sem_handle;
#endif

/* Local Headers */
#include <vutils/VException.h>
#include <vutils/VTypes.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace VUtils {

class VSemaphore {

public:
    VSemaphore(const char *pName = NULL) {
#if VPLATFORM == PLATFORM_WINDOWS
		if (pName == NULL)
			EXCPT(VException, "Must supply a semaphore name");
		strncpy(mSemaName, pName, 255);
		if ((mHandle = CreateSemaphore(NULL, 0, 1, mSemaName)) == NULL) {
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
		if (sem_init(&mHandle, 0, 0) != 0) {
#endif
			EXCPT(VException, "Unable to initialize Semaphore");
		}
    }

    bool Wait(VUINT pTimeout = 0) {
#if VPLATFORM == PLATFORM_WINDOWS
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
		if (pTimeout == 0) {
			sem_wait(&mHandle);
			return true;
		} else {
			struct timespec vTimeout;
			struct timeval	vCurrent;
			gettimeofday(&vCurrent, NULL);
			vTimeout.tv_sec = vCurrent.tv_sec + (pTimeout / 1000);
			vTimeout.tv_nsec = (vCurrent.tv_usec * 1000) + ((pTimeout % 1000) * 1000000);
			if (sem_timedwait(&mHandle, &vTimeout) == 0)
				return true;
			else
				//
				// TODO: Do some error checking here
				//
				return false;
		}

#endif
    }

    void Fire(void) {
#if VPLATFORM == PLATFORM_WINDOWS
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
		sem_post(&mHandle);
#endif
    }

private:
    sem_handle          mHandle;
	char                mSemaName[256];
};

} // End Namespace

#endif // __VSEMAPHORE_H_INCLUDED__

/* vi: set ts=4: */
