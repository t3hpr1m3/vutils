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
#if !defined(__VERRORBLOCK_H_INCLUDED__)
#define __VERRORBLOCK_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */

/* Local Headers */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Macros */

namespace VUtils
{

class VLog;

/**
 *  @class      VErrorBlock
 *
 *  @brief      Wraps a single line of error text.
 *  @author     Josh Williams
 *  @version    0.1.0
 *  @date       18-Apr-2005
 */
class VErrorBlock {
public:
	/*==================================*
	 *     CONSTRUCTION/DESTRUCTION     *
	 *==================================*/
	VErrorBlock(const char *pFile, const char *pFunc, const int pLine);

public:
	/*==================================*
	 *             ATTRIBUTES           *
	 *==================================*/

public:

	/*==================================*
	 *             OPERATIONS           *
	 *==================================*/

public:
	/*==================================*
	 *             OPERATORS            *
	 *==================================*/
	void                operator() (const char *pFormat, ...) const;

protected:
	/*==================================*
	 *             CALLBACKS            *
	 *==================================*/

private:
	/*==================================*
	 *             INTERNALS            *
	 *==================================*/

private:
	/*==================================*
	 *             VARIABLES            *
	 *==================================*/
	const char          *mFile;
	const char          *mFunc;
	int                 mLine;
};


/********************************************************************
 *                                                                  *
 *                           I N L I N E S                          *
 *                                                                  *
 ********************************************************************/

} // End Namespace

#endif // __VERRORBLOCK_H_INCLUDED__

/* vi: set ts=4: */
