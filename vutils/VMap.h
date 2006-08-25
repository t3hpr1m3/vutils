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
#if !defined(__VMAP_H_INCLUDED__)
#define __VMAP_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */
#include <map>

/* Local Headers */
#include <vutils/VMutex.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace VUtils
{

template <class Key, class T, class Pred = std::less<Key> >
class VMap : public std::map<Key, T, Pred>, public VMutex
{
public:
    VMap();
    virtual ~VMap();
};

template <class Key, class T, class Pred>
VMap<Key, T, Pred>::VMap()
{

}

template <class Key, class T, class Pred>
VMap<Key, T, Pred>::~VMap()
{

}

} // End Namespace

#endif // __VMAP_H_INCLUDED__
