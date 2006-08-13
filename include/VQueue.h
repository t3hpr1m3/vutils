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
/**
 *  @class      CGnuQueue
 *  @brief      Gnutella packet queue
 *  @author     Josh Williams (volzman19@yahoo.com)
 *  @version    0.1
 *  @remarks    FIFO queue for holding inbound gnutella packets.
 */
/*============================================================================*
 *  This source file is part of the GnuCore (Gnutella) library.               *
 *                                                                            *
 *  Copyright (C) 2003  Josh Williams <volzman19@yahoo.com>                   *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation; either version 2 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the Free Software               *
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA *
 *                                                                            *
 *============================================================================*
 *                                  CHANGELOG                                 *
 *   Date       Description                                    Author         *
 * -----------  ---------------------------------------------  -------------- *
 *                                                                            *
 *============================================================================*/
#if !defined(__GNUQUEUE_H_INCLUDED__)
#define __GNUQUEUE_H_INCLUDED__

#include "GnuPackets.h"
#include <queue>
using std::queue;

template <class T>
class CGnuQueue : public queue<T>, public CGnuMutex
{
public:
    CGnuQueue();
    virtual ~CGnuQueue();

    T       PopFront();
    void    PushBack(const T& x);
};

template <class T>
CGnuQueue<T>::CGnuQueue()
{

}

template <class T>
CGnuQueue<T>::~CGnuQueue()
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
T CGnuQueue<T>::PopFront()
{
    Lock();
    T ret = front();
    pop();
    Unlock();
    return ret;
}

template <class T>
void CGnuQueue<T>::PushBack(const T& x)
{
    Lock();
    push(x);
    Unlock();
};

#endif // __GNUQUEUE_H_INCLUDED__
