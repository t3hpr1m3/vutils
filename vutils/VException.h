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
#if !defined(__VEXCEPTION_H_INCLUDED__)
#define __VEXCEPTION_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */
#include <iostream>

/* Local Headers */
#include <vutils/VString.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Macros */
#define EXCPT(a, b) throw (a ( b , __FILE__ , __FUNCTION__ , __LINE__ ))

using std::ostream;

namespace VUtils
{

/**
 *	@class		VException
 *
 *	@brief		General purpose Exception class.
 *	@author		Josh Williams
 *	@version	0.1.0
 *	@date		12-Oct-2004
 */
class VException
{
public:
	/*==================================*
	 *	   CONSTRUCTION/DESTRUCTION		*
	 *==================================*/
	VException(const VString& pErr = NULL, const VString& pFile = "", 
		const VString& pFunc = "", int pLine = 0);
	virtual ~VException() {}

public:
	/*==================================*
	 *			  ATTRIBUTES			*
	 *==================================*/
	const VString&		Error() const;
	const VString&		File() const;
	const VString&		Func() const;
	int					Line() const;

public:
	/*==================================*
	 *			  OPERATIONS			*
	 *==================================*/
	virtual VString		Dump() const;

public:
	/*==================================*
	 *			   OPERATORS			*
	 *==================================*/
	friend ostream&		operator<<(ostream& pOs, const VException &pEx);

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
	VString				mError;	/**< Error string for this exception */
	VString				mFile;	/**< Source file where the exception occurred */
	VString				mFunc;	/**< Function where the exception occurred */
	int					mLine;	/**< Line # where the exception was thrown */
};



/********************************************************************
 *																	*
 *							I N L I N E S							*
 *																	*
 ********************************************************************/
inline
const VString& VException::Error() const
{
	return mError;
}

inline
const VString& VException::File() const
{
	return mFile;
}

inline
const VString& VException::Func() const
{
	return mFunc;
}

inline
int VException::Line() const
{
	return mLine;
}

} // End Namespace

#endif // __VEXCEPTION_H_INCLUDED__

/* vi: set ts=4: */
