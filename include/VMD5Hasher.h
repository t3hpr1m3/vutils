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
#if !defined(__VMD5HASHER_H_INCLUDED__)
#define __VMD5HASHER_H_INCLUDED__

#include "VPlatform.h"

/* System Headers */

/* Local Headers */
#include "VObject.h"

/* Macros */

namespace VUtils
{

class VMD5Hasher : public VObject
{
public:
	/*==================================*
	 *	   CONSTRUCTION/DESTRUCTION		*
	 *==================================*/
	VMD5Hasher();
	virtual ~VMD5Hasher();

public:
	/*==================================*
	 *			  ATTRIBUTES			*
	 *==================================*/
	VRESULT			GetDigest(VBYTE *pBuf, VUINT pLen);
	VRESULT			GetDigestString(char *pBuf, VUINT pLen);

public:
	/*==================================*
	 *			  OPERATIONS			*
	 *==================================*/
	void			Init();
	void			Update(VBYTE *pBuf, VUINT pLen);
	void			Transform(VULONG *pBuf, VULONG *pIn);
	void			Final();
	static VRESULT	HashString(char *pInBuf, VUINT pInLen,
								char *pOutBuf, VUINT pOutLen);

public:
	/*==================================*
	 *			   OPERATORS			*
	 *==================================*/

protected:
	/*==================================*
	 *			   CALLBACKS			*
	 *==================================*/

private:
	/*==================================*
	 *			   INTERNALS			*
	 *==================================*/

private:
	/*==================================*
	 *             VARIABLES            *
	 *==================================*/
	bool			mHashComplete;		/**< Has hashing been done?	*/
	VULONG			mBitCounts[2];		/**< Number of bits handled mod2^64 */
	VULONG			mBuffer[4];			/**< Scratch buffer	*/
	VBYTE			mInBuf[64];			/**< Input buffer */
	VBYTE			mDigest[16];		/**< Actual digest after MD5Final
										  	 call */
	char			mDigestString[33];	/**< Digest stored as a string */
	static VBYTE	MD5_PADDING[64];
};



/********************************************************************
 *																	*
 *							I N L I N E S							*
 *																	*
 ********************************************************************/

} // End Namespace

#endif // __VMD5HASHER_H_INCLUDED__

/* vi: set ts=4: */
