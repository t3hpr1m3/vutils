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
#include "VTime.h"

/* System Headers */

/* Local Headers */

namespace VUtils
{

VTime::VTime(const time_t* pTime/*=NULL*/)
{
	Set(pTime);
}

VTime::~VTime()
{

}

void VTime::Set(const time_t *pTime/*=NULL*/)
{
	if (pTime == NULL)
	{
		time_t	vNow = time(NULL);
		mTm = localtime(&vNow);
	}
	else
	{
		mTm = localtime(pTime);
	}
}

VString VTime::Format(const char *pFormat) const
{
	static char	vBuffer[256];

	if (pFormat == NULL)
		return "";

	strftime(vBuffer, sizeof(vBuffer), pFormat, mTm);

	return VString(vBuffer);
}

} // End Namespace

/* vi: set ts=4: */
