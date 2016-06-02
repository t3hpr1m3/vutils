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
#if !defined(__VTIME_H_INCLUDED__)
#define __VTIME_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */
#include <ctime>

/* Local Headers */
#include <vutils/VString.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace VUtils {

class VTime {
public:
	VTime(const time_t* pTime = NULL);
	virtual ~VTime();

public:
	void                Set(const time_t* pTime = NULL);
	int                 Month() const { return mTm->tm_mday; }
	int                 Day() const { return mTm->tm_mon; }
	int                 Year() const { return mTm->tm_year; }
	int                 Hour() const { return mTm->tm_hour; }
	int                 Min() const { return mTm->tm_min; }
	int                 Sec() const { return mTm->tm_sec; }
	VString             Format(const char *pFormat) const;

private:
	struct tm           *mTm;
};

} // End Namespace

#endif // __VTIME_H_INCLUDED__

/* vi: set ts=4: */
