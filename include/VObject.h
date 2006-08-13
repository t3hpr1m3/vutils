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
#if !defined(__VOBJECT_H_INCLUDED__)
#define __VOBJECT_H_INCLUDED__

/* System Headers */

/* Local Headers */
#include "VTypes.h"
#include "VString.h"
#include "VException.h"

namespace VUtils
{

/**
 *	@class		VObject
 *
 *	@brief		Base class for all VUtils objects.
 *	@author		Josh Williams
 *	@version	0.1.0
 *	@date		12-Oct-2004
 */
class VObject
{
public:
	/*==================================*
	 *	   CONSTRUCTION/DESTRUCTION		*
	 *==================================*/
	VObject();
	virtual ~VObject();

public:
	/*==================================*
	 *			  ATTRIBUTES			*
	 *==================================*/

public:
	/*==================================*
	 *			  OPERATIONS			*
	 *==================================*/
	static const char*	FormatError(VRESULT vErr);
	static const char*	RetString(VRESULT pResult);

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
};



/********************************************************************
 *																	*
 *							I N L I N E S							*
 *																	*
 ********************************************************************/

} // End Namespace

#endif // __VOBJECT_H_INCLUDED__

/* vi: set ts=4: */
