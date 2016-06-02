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
#if !defined(__VEXCEPTION_H_INCLUDED__)
#define __VEXCEPTION_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */
#include <iostream>

/* Local Headers */
#include <vutils/VString.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Macros */
#define EXCPT(a, b) throw (a ( b , __FILE__ , __FUNCTION__ , __LINE__ ))

using std::ostream;

namespace VUtils {

/**
 *  @class      VException
 *
 *  @brief      General purpose Exception class.
 *  @author     Josh Williams
 *  @version    0.1.0
 *  @date       12-Oct-2004
 */
class VException {
public:
	/*==================================*
	 *     CONSTRUCTION/DESTRUCTION     *
	 *==================================*/
	VException(const VString& pErr = "", const VString& pFile = "",
			const VString& pFunc = "", int pLine = 0);
	virtual ~VException() {}

public:
	/*==================================*
	 *             ATTRIBUTES           *
	 *==================================*/
	const VString&      Error() const;
	const VString&      File() const;
	const VString&      Func() const;
	int                 Line() const;

public:
	/*==================================*
	 *             OPERATIONS           *
	 *==================================*/
	virtual VString     Dump() const;

public:
	/*==================================*
	 *             OPERATORS            *
	 *==================================*/
	friend ostream&     operator<<(ostream& pOs, const VException &pEx);

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
	VString             mError; /**< Error string for this exception */
	VString             mFile;  /**< Source file where the exception occurred */
	VString             mFunc;  /**< Function where the exception occurred */
	int                 mLine;  /**< Line # where the exception was thrown */
};



/********************************************************************
 *                                                                  *
 *                           I N L I N E S                          *
 *                                                                  *
 ********************************************************************/
inline
const VString& VException::Error() const {
	return mError;
}

inline
const VString& VException::File() const {
	return mFile;
}

inline
const VString& VException::Func() const {
	return mFunc;
}

inline
int VException::Line() const {
	return mLine;
}

} // End Namespace

#endif // __VEXCEPTION_H_INCLUDED__

/* vi: set ts=4: */
