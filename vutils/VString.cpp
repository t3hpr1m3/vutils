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
#include <vutils/VString.h>

/* System Headers */
#include <cstdio>
#include <cstdarg>

/* Local Headers */

namespace VUtils
{

/********************************************************************
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 ********************************************************************/
VString::VString()
{
	mData	= NewBuf(1);
	mLength	= 0;
	mSize	= 1;
}

VString::VString(const VString& pString)
{
	const char *vPc = pString.C_Str();
	int vLength = pString.Length();
	mData = NewBuf(vLength+1);
	if (vLength > 0)
		memcpy(mData, vPc, vLength);
	mLength	= vLength;
	mSize	= vLength+1;
}

VString::VString(const char *pString)
{
	int vLength;

	if (pString == NULL)
		vLength = 0;
	else
		vLength = strlen(pString);
	mData = NewBuf(vLength+1);

	if (vLength > 0)
		memcpy(mData, pString, vLength);
	mLength	= vLength;
	mSize	= vLength+1;
}
VString::VString(int pSize)
{
	mData	= NewBuf(pSize+1);
	mLength	= 0;
	mSize	= pSize+1;
}

VString::~VString()
{
	delete[] mData;
	mData			= NULL;
}	

/********************************************************************
 *                        A T T R I B U T E S                       *
 ********************************************************************/

/*------------------------------------------------------------------*
 *								SubStr()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Returns nCount number of characters from the beginning
 *				of this VString, up to the length of the VString.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@param		pOffset	Index at which to begin returning substring.
 *	@param		pCount	Number of characters to return
 *
 *	@returns	(const char*) Requested characters.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
const VString& VString::SubStr(int pOffset, int pCount/*=-1*/) const
{
	static VString	vTemp;
	int				vLastIndex;

	vTemp = "";
	if (pOffset < 0)
		pOffset = 0;

	if (pOffset >= Length())
		return vTemp;

	if (pCount == -1)
		vLastIndex = Length();
	else
		vLastIndex = pOffset + pCount - 1;

	if (vLastIndex >= Length())
		vLastIndex = Length() - 1;

	for (int i = pOffset; i <= vLastIndex; i++)
	{
		vTemp.Cat(mData[i]);
	}

	vTemp.Cat('\0');

	return vTemp;
}

/*------------------------------------------------------------------*
 *								Find()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Searches the string for the given character.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@param		pChar	Character to scan for
 *
 *	@returns	(int) Offset of the first instance of pChar, or -1.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
int VString::Find(const char pChar) const
{
	char *vPc = strchr(mData, pChar);

	if (vPc == NULL)
		return -1;
	else
		return (vPc - mData);
}

int VString::RFind(const char pChar) const
{
	char *vPc = strrchr(mData, pChar);

	if (vPc == NULL)
		return -1;
	else
		return (vPc - mData);
}

/********************************************************************
 *                        O P E R A T I O N S                       *
 ********************************************************************/

/*------------------------------------------------------------------*
 *								Resize()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Increases/decreases the total number of characters
 *				this VString object is capable of holding.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@param		pSize	New size for this VString object
 *
 *	@returns	void
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
void VString::Resize(int pSize)
{
	char *vTemp;

	mSize = pSize+1;

	if (mData)
	{
		/* data already exists.  save it off */
		vTemp = NewBuf(mLength);
		memcpy(vTemp, mData, mLength);

		/* resize the member buffer */
		delete[] mData;
		mData = NewBuf(pSize + 1);
		mSize = pSize + 1;

		/* copy in the data */
		if (pSize > mLength)
		{
			memcpy(mData, vTemp, mLength);
			mData[mLength] = '\0';
		}
		else
		{
			memcpy(mData, vTemp, pSize);
			mData[pSize] = '\0';
			mLength = pSize;
		}

		/* kill the temp var */
		delete[] vTemp;
	}
	else
	{
		/* no data exists.  Create empty buffer */
		mData = NewBuf(mSize);
	}

	mLength = strlen(mData);
}

/*------------------------------------------------------------------*
 *								 Cpy() 								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Performs strcpy-like manipulation of this VString.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@remarks	VString is resized to the length of the string supplied.
 *
 *	@param		pString	Char array with optional printf-style formatting
 *
 *	@returns	Self-reference.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VString& VString::Copy(const char *pString, ...)
{
	int vLength;
	va_list vArgs;
	char vBuffer[1024];
	
	va_start(vArgs, pString);
	vsprintf(vBuffer, pString, vArgs);
	
	vLength = strlen(vBuffer);
	if (mData)
	{
		delete[] mData;
		mData = NULL;
	}
	mData = NewBuf(vLength+1);
	mLength = vLength;
	
	strncpy(mData, vBuffer, vLength);
	mSize = vLength+1;

	return *this;
}

/*------------------------------------------------------------------*
 *								  Cat()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Performs strcat-like manipulation of this VString.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@remarks	VString is resized accordingly.
 *
 *	@param		pString	Char array with optional printf-style formatting
 *
 *	@returns	Self reference.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VString& VString::Cat(const char *pString, ...)
{
	int		vLength;
	char	*vTemp;
	char	vBuffer[1024];
	va_list vArgs;

	va_start(vArgs, pString);
	vsprintf(vBuffer, pString, vArgs);

	vLength = strlen(vBuffer);
	if (vLength == 0)
		return *this;
	else
	{
		vTemp = NewBuf(vLength + strlen(mData) + 1);
		if (mData)
		{
			strncpy(vTemp, mData, vLength + strlen(mData));
			delete[] mData;
			mData = NULL;
		}
		strcat(vTemp, vBuffer);
		mData = vTemp;
		mLength += vLength;
	}
	mSize = mLength+1;
	return *this;
}

VString& VString::Cat(const char pChar)
{
	char *vTemp;

	vTemp = NewBuf(mSize+1);
	memcpy(vTemp, mData, mLength);
	vTemp[mLength] = pChar;
	delete[] mData;
	mData = vTemp;
	mLength++;
	mSize++;
	return *this;
}

/*------------------------------------------------------------------*
 *								Replace()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Replaces one substring of this String with another
 *				string.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@param		pSearch	String to be replaced
 *	@param		pString	String to be inserted
 *
 *	@returns	(int) Final length of the string.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
int VString::Replace(const char *pSearch, const char *pString)
{
	char	vBuffer[8192];
	char	vHold[8192];
	char	*vPcPos;
	int		vPos;
	int		vLength;

	vLength = (int)strlen(pSearch);
	memset(vBuffer, 0, sizeof(vBuffer));
	memset(vHold, 0, sizeof(vHold));

	strncpy(vHold, mData, sizeof(vHold));

	vPcPos = strstr(vHold, pSearch);
	while (vPcPos != NULL)
	{
		vPos = (int)(vPcPos - vHold);
		strncat(vBuffer, vHold, vPos);
		strcat(vBuffer, pString);
		strncpy(vHold, &vHold[vPos+vLength], sizeof(vHold));
		vPcPos = strstr(vHold, pSearch);
	}

	strcat(vBuffer, vHold);

	if (mData)
		delete[] mData;
	mLength = (int)strlen(vBuffer);
	mData = NewBuf(mLength+1);
	strncpy(mData, vBuffer, mLength);
	mSize = mLength+1;
	return mLength;
}

/*------------------------------------------------------------------*
 *								 Trim()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Trims spaces from the front and back of the VString.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@returns	void
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
const VString VString::Trim() const
{
	char	*vBuffer = NewBuf(mLength+1);
	char	*vPc;
	int		vOffset = 0;

	if (mLength == 0)
		return VString("");

	if (mData[0] == ' ')
	{
		vOffset = 0;
		vPc = mData;

		while ((*vPc == ' ') && (vOffset < mSize))
		{
			vOffset++;
			vPc++;
		}
		if (vOffset < mSize)
			strncpy(vBuffer, vPc, mLength);
		else
			strncpy(vBuffer, mData, mLength);
	}
	else
		strncpy(vBuffer, mData, mLength);

	vPc = &vBuffer[strlen(vBuffer)-1];

	while (*vPc == ' ')
	{
		*vPc = '\0';
		vPc--;
	}

	VString vRetString(vBuffer);
	
	delete[] vBuffer;

	return vRetString;
}

/*------------------------------------------------------------------*
 *								Split()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Splits the string into parts based on the separator
 *				provided.  Can also be given a count.
 *	@author		Josh Williams
 *	@date		08-Apr-2007
 *
 *
 *	@param		pSep	Separator
 *	@param		pCount	Number of times to split.
 *
 *	@returns	(VList) List of chunks
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 * 22-Jul-2007	Add empty string to the end of the	Josh Williams	*
 * 				list if the separator was the last					*
 * 				character.											*
 *																	*
 *------------------------------------------------------------------*/
const VStringList VString::Split(const char pSep, const int pCount /*=0*/)
{
	VStringList vStrings;
	int			vCount = 0;
	int			vLength = 0;
	char		*vBuffer = NewBuf(mSize);
	const char	*vPc1, *vPc2;

	vPc1 = C_Str();
	vPc2 = strchr(vPc1, pSep);
	while ((vPc2 != NULL))
	{
		if (pCount != 0 && vCount >= pCount)
			break;

		vLength = vPc2 - vPc1;
		memcpy(vBuffer, vPc1, vLength);
		vBuffer[vLength] = '\0';
		vStrings.push_back(vBuffer);

		vPc1 = vPc2;
		while (*vPc1 == pSep)
			vPc1++;

		vCount++;
		vPc2 = strchr(vPc1, pSep);
	}

	if (strlen(vPc1) > 0)
		vStrings.push_back(vPc1);
	else
		vStrings.push_back("");

	delete[] vBuffer;
		
	return vStrings;
}

const VStringList VString::Split(const char *pSep, const int pCount /*=0*/)
{
	VStringList vStrings;
	int			vCount = 0;
	int			vLength = 0;
	char		*vBuffer = NewBuf(mSize);
	const char	*vPc1, *vPc2;

	vPc1 = C_Str();
	vPc2 = strstr(vPc1, pSep);
	while ((vPc2 != NULL))
	{
		if (pCount != 0 && vCount >= pCount)
			break;

		vLength = vPc2 - vPc1;
		memcpy(vBuffer, vPc1, vLength);
		vBuffer[vLength] = '\0';
		vStrings.push_back(vBuffer);

		vCount++;
		vPc1 = vPc2 + strlen(pSep);
		vPc2 = strstr(vPc1, pSep);
	}

	if (strlen(vPc1) > 0)
		vStrings.push_back(vPc1);
	else
		vStrings.push_back("");

	delete[] vBuffer;
		
	return vStrings;
}

/********************************************************************
 *                          O P E R A T O R S                       *
 ********************************************************************/

/*------------------------------------------------------------------*
 *								operator[]							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Allows access to a specific character in the
 *				VString for modification.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@remarks	Returns a reference so the character can be
 *				replaced with another char.
 *
 *	@param		pOffset	Index of the element to be returned
 *
 *	@returns	(char&) Character requested.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
char& VString::operator[](int pOffset)
{
	if (pOffset > mLength)
		return mData[mLength-1];
	else
		return mData[pOffset];
}

/*------------------------------------------------------------------*
 *							    operator[]							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Returns the character in the VString represented
 *				by the given index.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@param		pOffset	Index of the element to be returned
 *
 *	@returns	(char) Character requested.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
char VString::operator[](int pOffset) const
{
	if (pOffset > mLength)
		return mData[mLength-1];
	else
		return mData[pOffset];
}

/*------------------------------------------------------------------*
 *							operator+=()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Performs VString concatenation.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@param		pStrCat	VString to be copied to the end of this VString
 *
 *	@returns	(String) Result of concatenation.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
const VString& VString::operator+=(const VString& pStrCat)
{
	Cat(pStrCat.C_Str());
	return *this;
}

/**
 *	@overload
 */
const VString& VString::operator+=(const char *pCat)
{
	Cat(pCat);
	return *this;
}

/*------------------------------------------------------------------*
 *							operator+()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Performs VString concatenation for assignment.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@param		pStrCat	VString to be added to the end of this VString
 *
 *	@returns	(String&) Result of concatenation.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
const VString VString::operator+(const VString& pStrCat) const
{
	VString vTemp(*this);
	vTemp.Cat(pStrCat.C_Str());
	return vTemp;
}

/**
 *	@overload
 */
const VString VString::operator+(const char *pCat) const
{
	VString vTemp(*this);
	vTemp.Cat(pCat);
	return vTemp;
}

/*------------------------------------------------------------------*
 *							operator+()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Allows concatenation when the VString object isn't
 *				first.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@param		pFront	Char pointer to be the beginning of the
 *				resulting VString
 *	@param		pBack	VString object to be the end of the resulting
 *				VString.
 *
 *	@returns	(CStr) Result of concatenation.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
const VString operator+(const char *pFront, const VString& pBack)
{
	VString vTemp(pFront);
	vTemp.Cat(pBack.C_Str());
	return vTemp;
}

/*------------------------------------------------------------------*
 *							  operator<<()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Allows this VString object to be used directly
 *				in stream operations.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@param		pOs		Stream object to output to
 *	@param		pStrOut	VString object to be output
 *
 *	@returns	(ostream&) Output object.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
ostream& operator<<(ostream& pOs, const VString& pStrOut)
{
	pOs << pStrOut.C_Str();
	return pOs;
}

/********************************************************************
 *                         C A L L B A C K S                        *
 ********************************************************************/

/********************************************************************
 *                         I N T E R N A L S                        *
 ********************************************************************/

} // End Namespace

/* vi: set ts=4: */
