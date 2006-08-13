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
#if !defined(__VPLATFORM_H_INCLUDED__)
#define __VPLATFORM_H_INCLUDED__

/* System Headers */

/* Local Headers */

/* Macros */

#define PLATFORM_WINDOWS	1
#define PLATFORM_MAC		2
#define PLATFORM_LINUX		3

//==========================================//
//     SIMPLE PLATFORM-SPECIFIC STUFF		//
//==========================================//
#if defined(__WIN32__) || defined(_WIN32)

#define VPLATFORM	PLATFORM_WINDOWS
#define vsnprintf	_vsnprintf

#elif defined(__APPLE_CC__)

#define VPLATFORM	PLATFORM_MAC

#else

#define VPLATFORM	PLATFORM_LINUX

#endif


#endif // __VPLATFORM_H_INCLUDED__
