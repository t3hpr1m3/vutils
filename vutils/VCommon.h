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
#if !defined(__VCOMMON_H_INCLUDED__)
#define __VCOMMON_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */

/* Local Headers */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

template <class T>
T MIN(T a, T b)
{
    return (a < b ? a : b);
}

template <class T>
T MAX(T a, T b)
{
    return (a > b ? a : b);
}

#define VUNUSED(x) (void)x;

#endif // __VCOMMON_H_INCLUDED__
