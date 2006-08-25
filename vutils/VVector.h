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
#if !defined(__VVECTOR_H_INCLUDED__)
#define __VVECTOR_H_INCLUDED__

#include "VPlatform.h"

/* System Headers */
#include <vector>

/* Local Headers */
#include "VMutex.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace VUtils
{

template <class T>
class VVector : public std::vector<T>, public VMutex
{
public:
    VVector();
    virtual ~VVector();
};

template <class T>
VVector<T>::VVector()
{

}

template <class T>
VVector<T>::~VVector()
{

}

} // End Namespace

#endif // __VVECTOR_H_INCLUDED__

/* vi: set ts=4: */
