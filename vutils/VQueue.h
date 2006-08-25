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
#if !defined(__VQUEUE_H_INCLUDED__)
#define __VQUEUE_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */
#include <queue>

/* Local Headers */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using std::queue;

namespace VUtils
{

template <class T>
class VQueue : public queue<T>, public VMutex
{
public:
    VQueue();
    virtual ~VQueue();

    T       PopFront();
    void    PushBack(const T& x);
};

template <class T>
VQueue<T>::VQueue()
{

}

template <class T>
VQueue<T>::~VQueue()
{
    T x;
    while(!empty())
    {
        x = front();
        pop();
        delete x;
    }
}

template <class T>
T VQueue<T>::PopFront()
{
    Lock();
    T ret = front();
    pop();
    Unlock();
    return ret;
}

template <class T>
void VQueue<T>::PushBack(const T& x)
{
    Lock();
    push(x);
    Unlock();
};

} // End Namespace

#endif // __VQUEUE_H_INCLUDED__
