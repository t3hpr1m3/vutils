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
#if !defined(__VPLATFORM_H_INCLUDED__)
#define __VPLATFORM_H_INCLUDED__

/* System Headers */

/* Local Headers */

/* Macros */

#define PLATFORM_WINDOWS    1
#define PLATFORM_MAC        2
#define PLATFORM_LINUX      3

//==========================================//
//     SIMPLE PLATFORM-SPECIFIC STUFF       //
//==========================================//
#if defined(__WIN32__) || defined(_WIN32)

#define VPLATFORM   PLATFORM_WINDOWS
#define vsnprintf   _vsnprintf

#elif defined(__APPLE_CC__)

#define VPLATFORM   PLATFORM_MAC

#else

#define VPLATFORM   PLATFORM_LINUX

#endif


#endif // __VPLATFORM_H_INCLUDED__
