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
#if !defined(__VLIST_H_INCLUDED__)
#define __VLIST_H_INCLUDED__

/* System Headers */
#include <list>

/* Local Headers */
#include <vutils/VMutex.h>

namespace VUtils
{

template <class T>
class VList : public std::list<T>, public VMutex
{
public:
    VList();
    virtual ~VList();
};

template <class T>
VList<T>::VList()
{

}

template <class T>
VList<T>::~VList()
{

}

} // End Namespace

#endif // __VLIST_H_INCLUDED__
