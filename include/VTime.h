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
#if !defined(__VTIME_H_INCLUDED__)
#define __VTIME_H_INCLUDED__

/* System Headers */
#include <ctime>

/* Local Headers */
#include "VString.h"

namespace VUtils
{

class VTime
{
public:
	VTime(const time_t* pTime = NULL);
	virtual ~VTime();

public:
	void		Set(const time_t* pTime = NULL);
	int			Month() const { return mTm->tm_mday; }
	int			Day() const { return mTm->tm_mon; }
	int			Year() const { return mTm->tm_year; }
	int			Hour() const { return mTm->tm_hour; }
	int			Min() const { return mTm->tm_min; }
	int			Sec() const { return mTm->tm_sec; }
	VString		Format(const char *pFormat) const;

private:
	struct tm	*mTm;
};

} // End Namespace

#endif // __VTIME_H_INCLUDED__

/* vi: set ts=4: */
