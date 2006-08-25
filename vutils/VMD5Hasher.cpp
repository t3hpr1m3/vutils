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
#include <vutils/VMD5Hasher.h>

/* System Headers */
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fstream>

/* Local Headers */
#include <vutils/VFile.h>
#include <vutils/VCommon.h>
#include <vutils/VLog.h>

using std::ifstream;

DECLARE_CLASS( "VMD5Hasher" );

namespace VUtils
{

VBYTE VMD5Hasher::MD5_PADDING[64] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


/* MD5_F, MD5_G and MD5_H are basic MD5 functions: selection, majority, parity */
#define MD5_F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define MD5_G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits */
#ifndef ROTATE_LEFT
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#endif

/* MD5_FF, MD5_GG, MD5_HH, and MD5_II transformations for rounds 1, 2, 3, and 4 */
/* Rotation is separate from addition to prevent recomputation */
#define MD5_FF(a, b, c, d, x, s, ac) {(a) += MD5_F ((b), (c), (d)) + (x) + (VULONG)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define MD5_GG(a, b, c, d, x, s, ac) {(a) += MD5_G ((b), (c), (d)) + (x) + (VULONG)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define MD5_HH(a, b, c, d, x, s, ac) {(a) += MD5_H ((b), (c), (d)) + (x) + (VULONG)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define MD5_II(a, b, c, d, x, s, ac) {(a) += MD5_I ((b), (c), (d)) + (x) + (VULONG)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }

/* Constants for transformation */
#define MD5_S11 7  /* Round 1 */
#define MD5_S12 12
#define MD5_S13 17
#define MD5_S14 22
#define MD5_S21 5  /* Round 2 */
#define MD5_S22 9
#define MD5_S23 14
#define MD5_S24 20
#define MD5_S31 4  /* Round 3 */
#define MD5_S32 11
#define MD5_S33 16
#define MD5_S34 23
#define MD5_S41 6  /* Round 4 */
#define MD5_S42 10
#define MD5_S43 15
#define MD5_S44 21

/* Static Variables */

/********************************************************************
 *																	*
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 *																	*
 ********************************************************************/
VMD5Hasher::VMD5Hasher()
{
	mHashComplete = false;
}

VMD5Hasher::~VMD5Hasher()
{

}

/********************************************************************
 *																	*
 *                        A T T R I B U T E S                       *
 *																	*
 ********************************************************************/

/*------------------------------------------------------------------*
 *							  GetDigest()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Requests the computed digest in raw form.
 *	@author		Josh Williams
 *	@date		12-Sep-2004
 *
 *	@param		pBuf	buff to hold the generated digest.
 *	@param		pLen	Size of pBuf
 *
 *	@return
 *				- Z_OK		Digest returned
 *				- Z_FAIL	Hashing not yet performed
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VMD5Hasher::GetDigest(VBYTE *pBuf, VUINT pLen)
{
	BEG_FUNC("GetDigest")("%p, %d", pBuf, pLen);

	if (mHashComplete)
	{
		memcpy(pBuf, mDigest, MIN( sizeof(mDigest), pLen ));
		return END_FUNC(VERR_SUCCESS);
	}
	else
	{
		return END_FUNC(VERR_INVALID_STATUS);
	}
}

/*------------------------------------------------------------------*
 *							GetDigestString()						*
 *------------------------------------------------------------------*/
/**
 *	@brief		Requests the computed digest in formatted form.
 *	@author		Josh Williams
 *	@date		12-Sep-2004
 *
 *	@param		pBuf	buff to hold the generated digest.
 * 	@param		pLen	Size of pBuf
 *
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VMD5Hasher::GetDigestString(char *pBuf, VUINT pLen)
{
	BEG_FUNC("GetDigestString")("%p, %d", pBuf, pLen);
	if (mHashComplete)
	{
		memcpy(pBuf, mDigestString, MIN( sizeof(mDigestString), pLen ));
		return END_FUNC(VERR_SUCCESS);
	}
	else
	{
		return END_FUNC(VERR_INVALID_STATUS);
	}
}

/********************************************************************
 *																	*
 *                        O P E R A T I O N S                       *
 *																	*
 ********************************************************************/

/*------------------------------------------------------------------*
 *								Init()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Initializes all internal variables prior to generating
 *				a hash..
 *	@author		Josh Williams
 *	@date		12-Oct-2004
 *
 *	@returns	void
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
void VMD5Hasher::Init()
{
	BEG_FUNC("Init")(NULL);

	mHashComplete = false;

	mBitCounts[0] = mBitCounts[1] = 0;

	mBuffer[0] = (VULONG)0x67452301;
	mBuffer[1] = (VULONG)0xefcdab89;
	mBuffer[2] = (VULONG)0x98badcfe;
	mBuffer[3] = (VULONG)0x10325476;

	END_FUNCV();
}

/*------------------------------------------------------------------*
 *							    Update()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		MD5 block update operation.
 *	@author		Josh Williams
 *	@date		12-Oct-2004
 *
 *	@param		pBuf	Buffer containing data to be hashed
 *	@param		pLen	Number of VBYTEs contained in inBuf
 *
 *	@returns	void
 *
 *	@remarks	Continues an MD5 message-digest operation, processing
 *				another message block, and updating the context.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
void VMD5Hasher::Update(VBYTE *pBuf, VUINT pLen)
{
	VULONG	vIn[16];
	int		vMdi = 0;
	VUINT	i = 0;
	VUINT	ii = 0;

	// compute the number of VBYTEs mod 64
	vMdi = (VUINT)((mBitCounts[0] >> 3) & 0x3F);

	// update number of bits
	if ((mBitCounts[0] + ((VULONG)pLen << 3)) < mBitCounts[0])
		mBitCounts[1]++;
	mBitCounts[0] += ((VULONG)pLen << 3);
	mBitCounts[1] += ((VULONG)pLen >> 29);

	while (pLen--)
	{
		// add new character to buffer, increment mdi
		mInBuf[vMdi++] = *pBuf++;

		// transform if necessary
		if (vMdi == 0x40)
		{
			for (i = 0, ii = 0; i < 16; i++, ii += 4)
			{
				vIn[i] =(((VULONG)mInBuf[ii+3]) << 24) |
						(((VULONG)mInBuf[ii+2]) << 16) |
						(((VULONG)mInBuf[ii+1]) << 8) |
						((VULONG)mInBuf[ii]);
			}

			Transform(mBuffer, vIn);
			vMdi = 0;
		}
	}			
}

/*------------------------------------------------------------------*
 *							  Transform()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Basic MD5 step.  Transforms state based on block.
 *	@author		Josh Williams
 *	@date		12-Oct-2004
 *
 *	@param		pBuf	Output buffer
 *	@param		pIn		Input buffer
 *
 *	@returns	void
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
void VMD5Hasher::Transform(VULONG *pBuf, VULONG *pIn)
{
	VULONG vA = pBuf[0], vB = pBuf[1], vC = pBuf[2], vD = pBuf[3];

	// Round 1
	MD5_FF ( vA, vB, vC, vD, pIn[ 0], MD5_S11, (VULONG) 3614090360u); //  1
	MD5_FF ( vD, vA, vB, vC, pIn[ 1], MD5_S12, (VULONG) 3905402710u); //  2
	MD5_FF ( vC, vD, vA, vB, pIn[ 2], MD5_S13, (VULONG)  606105819u); //  3
	MD5_FF ( vB, vC, vD, vA, pIn[ 3], MD5_S14, (VULONG) 3250441966u); //  4
	MD5_FF ( vA, vB, vC, vD, pIn[ 4], MD5_S11, (VULONG) 4118548399u); //  5
	MD5_FF ( vD, vA, vB, vC, pIn[ 5], MD5_S12, (VULONG) 1200080426u); //  6
	MD5_FF ( vC, vD, vA, vB, pIn[ 6], MD5_S13, (VULONG) 2821735955u); //  7
	MD5_FF ( vB, vC, vD, vA, pIn[ 7], MD5_S14, (VULONG) 4249261313u); //  8
	MD5_FF ( vA, vB, vC, vD, pIn[ 8], MD5_S11, (VULONG) 1770035416u); //  9
	MD5_FF ( vD, vA, vB, vC, pIn[ 9], MD5_S12, (VULONG) 2336552879u); // 10
	MD5_FF ( vC, vD, vA, vB, pIn[10], MD5_S13, (VULONG) 4294925233u); // 11
	MD5_FF ( vB, vC, vD, vA, pIn[11], MD5_S14, (VULONG) 2304563134u); // 12
	MD5_FF ( vA, vB, vC, vD, pIn[12], MD5_S11, (VULONG) 1804603682u); // 13
	MD5_FF ( vD, vA, vB, vC, pIn[13], MD5_S12, (VULONG) 4254626195u); // 14
	MD5_FF ( vC, vD, vA, vB, pIn[14], MD5_S13, (VULONG) 2792965006u); // 15
	MD5_FF ( vB, vC, vD, vA, pIn[15], MD5_S14, (VULONG) 1236535329u); // 16

	// Round 2
	MD5_GG ( vA, vB, vC, vD, pIn[ 1], MD5_S21, (VULONG) 4129170786u); // 17
	MD5_GG ( vD, vA, vB, vC, pIn[ 6], MD5_S22, (VULONG) 3225465664u); // 18
	MD5_GG ( vC, vD, vA, vB, pIn[11], MD5_S23, (VULONG)  643717713u); // 19
	MD5_GG ( vB, vC, vD, vA, pIn[ 0], MD5_S24, (VULONG) 3921069994u); // 20
	MD5_GG ( vA, vB, vC, vD, pIn[ 5], MD5_S21, (VULONG) 3593408605u); // 21
	MD5_GG ( vD, vA, vB, vC, pIn[10], MD5_S22, (VULONG)   38016083u); // 22
	MD5_GG ( vC, vD, vA, vB, pIn[15], MD5_S23, (VULONG) 3634488961u); // 23
	MD5_GG ( vB, vC, vD, vA, pIn[ 4], MD5_S24, (VULONG) 3889429448u); // 24
	MD5_GG ( vA, vB, vC, vD, pIn[ 9], MD5_S21, (VULONG)  568446438u); // 25
	MD5_GG ( vD, vA, vB, vC, pIn[14], MD5_S22, (VULONG) 3275163606u); // 26
	MD5_GG ( vC, vD, vA, vB, pIn[ 3], MD5_S23, (VULONG) 4107603335u); // 27
	MD5_GG ( vB, vC, vD, vA, pIn[ 8], MD5_S24, (VULONG) 1163531501u); // 28
	MD5_GG ( vA, vB, vC, vD, pIn[13], MD5_S21, (VULONG) 2850285829u); // 29
	MD5_GG ( vD, vA, vB, vC, pIn[ 2], MD5_S22, (VULONG) 4243563512u); // 30
	MD5_GG ( vC, vD, vA, vB, pIn[ 7], MD5_S23, (VULONG) 1735328473u); // 31
	MD5_GG ( vB, vC, vD, vA, pIn[12], MD5_S24, (VULONG) 2368359562u); // 32

	// Round 3
	MD5_HH ( vA, vB, vC, vD, pIn[ 5], MD5_S31, (VULONG) 4294588738u); // 33
	MD5_HH ( vD, vA, vB, vC, pIn[ 8], MD5_S32, (VULONG) 2272392833u); // 34
	MD5_HH ( vC, vD, vA, vB, pIn[11], MD5_S33, (VULONG) 1839030562u); // 35
	MD5_HH ( vB, vC, vD, vA, pIn[14], MD5_S34, (VULONG) 4259657740u); // 36
	MD5_HH ( vA, vB, vC, vD, pIn[ 1], MD5_S31, (VULONG) 2763975236u); // 37
	MD5_HH ( vD, vA, vB, vC, pIn[ 4], MD5_S32, (VULONG) 1272893353u); // 38
	MD5_HH ( vC, vD, vA, vB, pIn[ 7], MD5_S33, (VULONG) 4139469664u); // 39
	MD5_HH ( vB, vC, vD, vA, pIn[10], MD5_S34, (VULONG) 3200236656u); // 40
	MD5_HH ( vA, vB, vC, vD, pIn[13], MD5_S31, (VULONG)  681279174u); // 41
	MD5_HH ( vD, vA, vB, vC, pIn[ 0], MD5_S32, (VULONG) 3936430074u); // 42
	MD5_HH ( vC, vD, vA, vB, pIn[ 3], MD5_S33, (VULONG) 3572445317u); // 43
	MD5_HH ( vB, vC, vD, vA, pIn[ 6], MD5_S34, (VULONG)   76029189u); // 44
	MD5_HH ( vA, vB, vC, vD, pIn[ 9], MD5_S31, (VULONG) 3654602809u); // 45
	MD5_HH ( vD, vA, vB, vC, pIn[12], MD5_S32, (VULONG) 3873151461u); // 46
	MD5_HH ( vC, vD, vA, vB, pIn[15], MD5_S33, (VULONG)  530742520u); // 47
	MD5_HH ( vB, vC, vD, vA, pIn[ 2], MD5_S34, (VULONG) 3299628645u); // 48

	// Round 4
	MD5_II ( vA, vB, vC, vD, pIn[ 0], MD5_S41, (VULONG) 4096336452u); // 49
	MD5_II ( vD, vA, vB, vC, pIn[ 7], MD5_S42, (VULONG) 1126891415u); // 50
	MD5_II ( vC, vD, vA, vB, pIn[14], MD5_S43, (VULONG) 2878612391u); // 51
	MD5_II ( vB, vC, vD, vA, pIn[ 5], MD5_S44, (VULONG) 4237533241u); // 52
	MD5_II ( vA, vB, vC, vD, pIn[12], MD5_S41, (VULONG) 1700485571u); // 53
	MD5_II ( vD, vA, vB, vC, pIn[ 3], MD5_S42, (VULONG) 2399980690u); // 54
	MD5_II ( vC, vD, vA, vB, pIn[10], MD5_S43, (VULONG) 4293915773u); // 55
	MD5_II ( vB, vC, vD, vA, pIn[ 1], MD5_S44, (VULONG) 2240044497u); // 56
	MD5_II ( vA, vB, vC, vD, pIn[ 8], MD5_S41, (VULONG) 1873313359u); // 57
	MD5_II ( vD, vA, vB, vC, pIn[15], MD5_S42, (VULONG) 4264355552u); // 58
	MD5_II ( vC, vD, vA, vB, pIn[ 6], MD5_S43, (VULONG) 2734768916u); // 59
	MD5_II ( vB, vC, vD, vA, pIn[13], MD5_S44, (VULONG) 1309151649u); // 60
	MD5_II ( vA, vB, vC, vD, pIn[ 4], MD5_S41, (VULONG) 4149444226u); // 61
	MD5_II ( vD, vA, vB, vC, pIn[11], MD5_S42, (VULONG) 3174756917u); // 62
	MD5_II ( vC, vD, vA, vB, pIn[ 2], MD5_S43, (VULONG)  718787259u); // 63
	MD5_II ( vB, vC, vD, vA, pIn[ 9], MD5_S44, (VULONG) 3951481745u); // 64

	pBuf[0] += vA;
	pBuf[1] += vB;
	pBuf[2] += vC;
	pBuf[3] += vD;
}

/*------------------------------------------------------------------*
 *								  Final()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		MD5 finalization.
 *	@author		Josh Williams
 *	@date		12-Oct-2004
 *
 *	@returns	void
 *
 *	@remarks	Ends an MD5 message-digest operation, writing the
 *				message digest and zeroizing the context.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
void VMD5Hasher::Final()
{
	VULONG	vIn[16];
	int		vMdi = 0;
	VUINT	i = 0;
	VUINT	ii = 0;
	VUINT	vPadLen = 0;

	BEG_FUNC("Final")(NULL);

	// save number of bits
	vIn[14] = mBitCounts[0];
	vIn[15] = mBitCounts[1];

	// compute number of VBYTEs mod 64
	vMdi = (int)((mBitCounts[0] >> 3) & 0x3F);

	// pad out to 56 mod 64
	vPadLen = (vMdi < 56) ? (56 - vMdi) : (120 - vMdi);
	Update(MD5_PADDING, vPadLen);

	// append length in bits and transform
	for (i = 0, ii = 0; i < 14; i++, ii += 4)
	{
		vIn[i] =(((VULONG)mInBuf[ii+3]) << 24) |
				(((VULONG)mInBuf[ii+2]) << 16) |
				(((VULONG)mInBuf[ii+1]) << 8) |
				((VULONG)mInBuf[ii]);
	}

	Transform(mBuffer, vIn);

	// store buffer in digest
	for (i = 0, ii = 0; i < 4; i++, ii += 4)
	{
		mDigest[ii]   = (VBYTE)( mBuffer[i]		  & 0xFF);
		mDigest[ii+1] = (VBYTE)((mBuffer[i] >>  8) & 0xFF);
		mDigest[ii+2] = (VBYTE)((mBuffer[i] >> 16) & 0xFF);
		mDigest[ii+3] = (VBYTE)((mBuffer[i] >> 24) & 0xFF);
	}

	for (int i = 0; i < 16; i++)
	{
		sprintf(&mDigestString[i*2], "%02x", mDigest[i]);
	}
	VTRACE("mDigestString => [%s]\n", mDigestString);

	mHashComplete = true;

	END_FUNCV();
}

VRESULT VMD5Hasher::HashString(char *pInBuf, VUINT pInLen, char *pOutBuf, VUINT pOutLen)
{
	static VMD5Hasher vHasher;

	BEG_FUNC("HashString")("%p, %d, %p, %d", pInBuf, pInLen, pOutBuf, pOutLen);

	vHasher.Init();
	vHasher.Update((VBYTE *)pInBuf, pInLen);
	vHasher.Final();

	vHasher.GetDigestString(pOutBuf, pOutLen);

	return END_FUNC(VERR_SUCCESS);
}

/********************************************************************
 *																	*
 *                          O P E R A T O R S                       *
 *																	*
 ********************************************************************/

/********************************************************************
 *																	*
 *                          C A L L B A C K S                       *
 *																	*
 ********************************************************************/

/********************************************************************
 *																	*
 *                          I N T E R N A L S                       *
 *																	*
 ********************************************************************/

} // End Namespace

/* vi: set ts=4: */
