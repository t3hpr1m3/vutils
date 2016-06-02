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
#if !defined(__VMD5HASHER_H_INCLUDED__)
#define __VMD5HASHER_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */

/* Local Headers */
#include <vutils/VObject.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Macros */

namespace VUtils {

class VMD5Hasher : public VObject {
public:
	/*==================================*
	 *     CONSTRUCTION/DESTRUCTION     *
	 *==================================*/
	VMD5Hasher();
	virtual ~VMD5Hasher();

public:
	/*==================================*
	 *             ATTRIBUTES           *
	 *==================================*/
	VRESULT             GetDigest(VBYTE *pBuf, VULONG pLen);
	VRESULT             GetDigestString(char *pBuf, VULONG pLen);

public:
	/*==================================*
	 *           OPERATIONS             *
	 *==================================*/
	void                Init();
	void                Update(VBYTE *pBuf, VULONG pLen);
	void                Transform(VULONG *pBuf, VULONG *pIn);
	void                Final();
	static VRESULT      HashString(char *pInBuf, VULONG pInLen,
	                        char *pOutBuf, VULONG pOutLen);

public:
	/*==================================*
	 *             OPERATORS            *
	 *==================================*/

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
	bool                mHashComplete; /**< Has hashing been done? */
	VULONG              mBitCounts[2]; /**< Number of bits handled mod2^64 */
	VULONG              mBuffer[4];    /**< Scratch buffer */
	VBYTE               mInBuf[64];    /**< Input buffer */
	VBYTE               mDigest[16];   /**< Actual digest after MD5Final
	                                        call */
	char                mDigestString[33]; /**< Digest stored as a string */
	static VBYTE        MD5_PADDING[64];
};



/********************************************************************
 *                                                                  *
 *                           I N L I N E S                          *
 *                                                                  *
 ********************************************************************/

} // End Namespace

#endif // __VMD5HASHERFILE_H_INCLUDED__

/* vi: set ts=4: */
