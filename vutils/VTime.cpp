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
#include <vutils/VTime.h>

/* System Headers */

/* Local Headers */

namespace VUtils {

VTime::VTime(const time_t* pTime/*=NULL*/) {
	Set(pTime);
}

VTime::~VTime() {}

void VTime::Set(const time_t *pTime/*=NULL*/) {
	if (pTime == NULL) {
		time_t vNow = time(NULL);
		mTm = localtime(&vNow);
	} else {
		mTm = localtime(pTime);
	}
}

VString VTime::Format(const char *pFormat) const {
	static char vBuffer[256];

	if (pFormat == NULL)
		return "";

	strftime(vBuffer, sizeof(vBuffer), pFormat, mTm);

	return VString(vBuffer);
}

} // End Namespace

/* vi: set ts=4: */
