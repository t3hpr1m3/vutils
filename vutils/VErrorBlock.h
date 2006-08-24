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
#if !defined(__VERRORBLOCK_H_INCLUDED__)
#define __VERRORBLOCK_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */

/* Local Headers */

/* Macros */

namespace VUtils
{

class VLog;

/**
 *	@class		VErrorBlock
 *
 *	@brief		Wraps a single line of error text.
 *	@author		Josh Williams
 *	@version	0.1.0
 *	@date		18-Apr-2005
 */
class VErrorBlock
{
public:
	/*==================================*
	 *	   CONSTRUCTION/DESTRUCTION		*
	 *==================================*/
	VErrorBlock(const char *pFile, const char *pFunc, const int pLine);

public:
	/*==================================*
	 *			  ATTRIBUTES			*
	 *==================================*/

public:
	/*==================================*
	 *			  OPERATIONS			*
	 *==================================*/
	void				operator() (const char *pFormat, ...) const;

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
	const char			*mFile;
	const char			*mFunc;
	int					mLine;
};



/********************************************************************
 *																	*
 *							I N L I N E S							*
 *																	*
 ********************************************************************/

} // End Namespace

#endif // __VERRORBLOCK_H_INCLUDED__

/* vi: set ts=4: */
