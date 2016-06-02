/*======================================================================*
 *                                                                      *
 *  Copyright (C) 2004-2016 Josh Williams (vmizzle@gmail.com)           *
 *                                                                      *
 * Permission is hereby granted, free of charge, to any person          *
 * obtaining a copy of this software and associated documentation files *
 * (the "Software"), to deal in the Software without restriction,       *
 * including without limitation the rights to use, copy, modify, merge, *
 * publish, distribute, sublicense, and/or sell copies of the Software, *
 * and to permit persons to whom the Software is furnished to do so,    *
 * subject to the following conditions:                                 *
 *                                                                      *
 * The above copyright notice and this permission notice shall be       *
 * included in all copies or substantial portions of the Software.      *
 *                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      *
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                *
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  *
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   *
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    *
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     *
 * SOFTWARE.                                                            *
 *                                                                      *
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

namespace VUtils {

template <class T>
class VQueue : public queue<T>, public VMutex {
public:
    VQueue();
    virtual ~VQueue();

    T                   PopFront();
    void                PushBack(const T& x);
};

template <class T>
VQueue<T>::VQueue() {}

template <class T>
VQueue<T>::~VQueue() {
    T x;
    while(!empty()) {
        x = front();
        pop();
        delete x;
    }
}

template <class T>
T VQueue<T>::PopFront() {
    Lock();
    T ret = front();
    pop();
    Unlock();
    return ret;
}

template <class T>
void VQueue<T>::PushBack(const T& x) {
    Lock();
    push(x);
    Unlock();
};

} // End Namespace

#endif // __VQUEUE_H_INCLUDED__
