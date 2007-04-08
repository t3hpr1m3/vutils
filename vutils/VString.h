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
#if !defined(__VSTRING_H_INCLUDED__)
#define __VSTRING_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */
#include <cstring>
#include <iostream>

/* Local Headers */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <vutils/VVector.h>

using std::ostream;

namespace VUtils
{

class VString;
typedef VVector<VString> VStringList;

class VString
{
public:
	/*==================================*
	 *	   CONSTRUCTION/DESTRUCTION		*
	 *==================================*/
	VString();
	VString(const char *pString);
	VString(int pSize);
	VString(const VString& pString);
	virtual ~VString();

	/*==================================*
	 *			  ATTRIBUTES			*
	 *==================================*/
	const char*		C_Str() const;
	int				Length() const;
	int				Size() const;
	const VString&	SubStr(int pOffset, int pCount = -1) const;
	int				Find(const char pChar) const;
	int				RFind(const char pChar) const;

	/*==================================*
	 *			  OPERATIONS			*
	 *==================================*/
	void			Resize(int pSize);
	void			Copy(const char *pString, ...);
	void			Cat(const char *pString, ...);
	void			Cat(const char pChar);
	int				Compare(const char *pString) const;
	int				Replace(const char *pSearch, const char *pString);
	const VString	Trim() const;
	const VStringList	Split(const char pSep, const int pCount);

	/*==================================*
	 *			   OPERATORS			*
	 *==================================*/
	char&			operator[](int pOffset);
	char			operator[](int pOffset) const;
	bool			operator<(const VString& pString) const;
	bool			operator==(const VString& pString) const;
	bool			operator==(const char* pString) const;
	VString&		operator=(const VString &pSource);
	VString&		operator=(const char *pSource);
	const VString&	operator+=(const VString& pStrCat);
	const VString&	operator+=(const char *pCat);
	const VString	operator+(const VString& pStrCat) const;
	const VString	operator+(const char *pCat) const;
	friend const VString	operator+(const char *pFront,
									const VString& pBack);
					operator const char*() const;
	friend ostream& operator<<(ostream& pOs, const VString& pStrOut);

protected:
	/*==================================*
	 *             CALLBACKS			*
	 *==================================*/

private:
	/*==================================*
	 *             INTERNALS            *
	 *==================================*/
	char*			NewBuf(int pSize) const;
	
private:
	/*==================================*
	 *             VARIABLES            *
	 *==================================*/
	char*			mData;
	int				mLength;
	int				mSize;
};


/*------------------------------------------------------------------*
 *								C_Str()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Returns the array of chars for this VString.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@remarks	Useful when this VString is used in printf variants
 *				where % formatting is used.
 *
 *	@returns	(char*) Array of chars.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
inline
const char* VString::C_Str() const
{
	return mData;
}

/*------------------------------------------------------------------*
 *								Length()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Returns the length of this VString object.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@returns	(int) Length of string.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
inline
int VString::Length() const
{
	return mLength;
}

/*------------------------------------------------------------------*
 *								 Size()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Returns the total size of the array currently
 *				allocated by this VString.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@remarks	Length returns the length of the string held by this
 *				object.  This function returns the total available
 *				space in the curently allocated char array.
 *
 *	@returns	(int) Total space allocated.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
inline
int VString::Size() const
{
	return mSize;
}

/*------------------------------------------------------------------*
 *								Compare()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Compares the supplied character array against this
 *				VString object using strcmp().
 *	@author		Josh Williams
 *	@date		30-Aug-2004
 *
 *	@param		pString	Char array to be compared against
 *
 *	@returns	0 if the string matches, <>0 otherwise
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
inline
int VString::Compare(const char *pString) const
{
	return strcmp(mData, pString);
}

/*------------------------------------------------------------------*
 *							operator=()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		VString assignment.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@param		pSource	VString whose value we should inherit.
 *
 *	@returns	(CStr&) This object after manipulation.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
inline
VString& VString::operator=(const VString& pSource)
{
	Copy(pSource.C_Str());
	return *this;
}

/**
 *	@overload
 */
inline
VString& VString::operator=(const char *pSource)
{
	Resize(strlen(pSource));
	memcpy(mData, pSource, strlen(pSource));
	mLength = strlen(pSource);
	return *this;
}

/*------------------------------------------------------------------*
 *						  operator const char*()					*
 *------------------------------------------------------------------*/
/**
 *	@brief		Allows this object to be treated as a normal
 *				character array.
 *	@author		Josh Williams
 *	@date		19-Sep-2003
 *
 *	@remarks	Useful when this object is used in standard library
 *				calls such as strcpy.
 *
 *	@returns	(char*) Pointer to the actual string held by this object
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
inline
VString::operator const char*() const
{
	return C_Str();
}

inline
bool VString::operator<(const VString& pString) const
{
	return (this->Compare(pString) < 0);
}

inline
bool VString::operator==(const VString& pString) const
{
	return (this->Compare(pString) == 0);
}

inline
bool VString::operator==(const char *pString) const
{
	return (this->Compare(pString) == 0);
}

inline char* VString::NewBuf(const int pSize) const
{
	char *pNewBuf = new char[pSize];
	memset(pNewBuf, '\0', pSize);
	return pNewBuf;
}

} // End Namespace

#endif // __VSTRING_H_INCLUDED__

/* vi: set ts=4: */
