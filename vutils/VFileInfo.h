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
#if !defined(__VFILEINFO_H_INCLUDED__)
#define __VFILEINFO_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */

/* Local Headers */
#include <vutils/VTime.h>
#include <vutils/VString.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Macros */

namespace VUtils
{

/**
 *  @class      VFileInfo
 *
 *  @author     Josh Williams
 *  @date       02-Dec-2004
 */
class VFileInfo {
	friend class VFile;

public:
	/*==================================*
	 *     CONSTRUCTION/DESTRUCTION     *
	 *==================================*/
	VFileInfo() {}
	VFileInfo(const VFileInfo& pRhs);
	virtual ~VFileInfo() {}

public:
	/*==================================*
	 *             ATTRIBUTES           *
	 *==================================*/
	const VString&      Path() const;
	const VString&      Name() const;
	const VString&      Ext() const;
	const VString       Full() const;
	const VULONG        Size() const;
	const VTime&        Modified() const;

public:
	/*==================================*
	 *           OPERATIONS             *
	 *==================================*/
	void                Reset();

public:
	/*==================================*
	 *           OPERATORS              *
	 *==================================*/

protected:
	/*==================================*
	 *             CALLBACKS            *
	 *==================================*/

private:
	/*==================================*
	 *             INTERNALS            *
	 *==================================*/

private:
	/*==================================*
	 *             VARIABLES            *
	 *==================================*/
	VString             mPath;
	VString             mName;
	VString             mExt;
	VULONG              mSize;
	VTime               mModify;
};

/********************************************************************
 *                                                                  *
 *                           I N L I N E S                          *
 *                                                                  *
 ********************************************************************/
inline
VFileInfo::VFileInfo(const VFileInfo& pRhs) {
	mPath   = pRhs.Path();
	mName   = pRhs.Name();
	mExt    = pRhs.Ext();
	mSize   = pRhs.Size();
	mModify = pRhs.Modified();
}

inline
const VString& VFileInfo::Path() const {
	return mPath;
}

inline
const VString& VFileInfo::Name() const {
	return mName;
}

inline
const VString& VFileInfo::Ext() const {
	return mExt;
}

inline
const VString VFileInfo::Full() const {
	return Path() + Name();
}

inline
const VULONG VFileInfo::Size() const {
	return mSize;
}

inline
const VTime& VFileInfo::Modified() const {
	return mModify;
}

inline
void VFileInfo::Reset() {
	mPath   = "";
	mName   = "";
	mExt    = "";
	mSize   = 0L;
	mModify = 0;
}

} // End Namespace

#endif // __VFILEINFO_H_INCLUDED__
