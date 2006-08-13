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
#if !defined(__VFILEINFO_H_INCLUDED__)
#define __VFILEINFO_H_INCLUDED__

#include "VPlatform.h"

/* System Headers */

/* Local Headers */
#include "VTime.h"
#include "VString.h"

/* Macros */

namespace VUtils
{

/**
 *	@class		VFileInfo
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 */
class VFileInfo
{
	friend class VFile;

public:
	/*==================================*
	 *	   CONSTRUCTION/DESTRUCTION		*
	 *==================================*/
	VFileInfo() {}
	VFileInfo(const VFileInfo& pRhs);
	virtual ~VFileInfo() {}

public:
	/*==================================*
	 *			  ATTRIBUTES			*
	 *==================================*/
	const VString&		Path() const;
	const VString&		Name() const;
	const VString&		Ext() const;
	const VString		Full() const;
	const VULONG		Size() const;
	const VTime&		Modified() const;

public:
	/*==================================*
	 *			  OPERATIONS			*
	 *==================================*/
	void				Reset();

public:
	/*==================================*
	 *			   OPERATORS			*
	 *==================================*/

protected:
	/*==================================*
	 *			   CALLBACKS			*
	 *==================================*/

private:
	/*==================================*
	 *			   INTERNALS			*
	 *==================================*/

private:
	/*==================================*
	 *             VARIABLES            *
	 *==================================*/
	VString				mPath;
	VString				mName;
	VString				mExt;
	VULONG				mSize;
	VTime				mModify;
};



/********************************************************************
 *																	*
 *							I N L I N E S							*
 *																	*
 ********************************************************************/
inline
VFileInfo::VFileInfo(const VFileInfo& pRhs)
{
	mPath	= pRhs.Path();
	mName	= pRhs.Name();
	mExt	= pRhs.Ext();
	mSize	= pRhs.Size();
	mModify	= pRhs.Modified();
}

inline
const VString& VFileInfo::Path() const
{
	return mPath;
}

inline
const VString& VFileInfo::Name() const
{
	return mName;
}

inline
const VString& VFileInfo::Ext() const
{
	return mExt;
}

inline
const VString VFileInfo::Full() const
{
	return Path() + Name();
}

inline
const VULONG VFileInfo::Size() const
{
	return mSize;
}

inline
const VTime& VFileInfo::Modified() const
{
	return mModify;
}

inline
void VFileInfo::Reset()
{
	mPath	= "";
	mName	= "";
	mExt	= "";
	mSize	= 0L;
	mModify	= 0;
}

} // End Namespace

#endif // __VFILEINFO_H_INCLUDED__
