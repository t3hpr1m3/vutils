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
#if !defined(__VCONFIG_H_INCLUDED__)
#define __VCONFIG_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */
#include <utility>
#include <map>
#include <stdlib.h>

/* Local Headers */
#include <vutils/VString.h>
#include <vutils/VFile.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace VUtils
{

class VConfig : public VFile
{
private:
	typedef std::map<const VString, VString> _Settings;
	class VSetting
	{

	public:
		VSetting(_Settings::iterator i) : mIter(i) {}

	public:
		operator const char*() const { return mIter->second.C_Str(); }
		operator int() const { return atoi(mIter->second.C_Str()); }
		operator double() const { return atof(mIter->second.C_Str()); }
		const char* ToString() const { return mIter->second.C_Str(); }
		void operator=(const char *pValue) { mIter->second = pValue; }
		void operator=(const int pValue) { mIter->second.Copy("%d", pValue); }
		void operator=(const double pValue) { mIter->second.Copy("%f", pValue); }

	private:
		_Settings::iterator		mIter;
	};

	class VSection
	{
	public:
		VSection() {}

	public:
		VSetting	operator[](const char *pName)
		{
			_Settings::iterator vIt = mSettings.find(pName);
			if (vIt == mSettings.end())
				return VSetting(mSettings.insert(vIt, 
							std::pair<VString, VString> (pName, "") ));
			else
				return VSetting(vIt);
		}


	private:
		_Settings	mSettings;
	};

typedef std::map<const VString, VSection> _Sections;

public:
	VConfig();
	virtual ~VConfig();

public:
	bool		Load(const char *pFile);
	VSection&	operator[](const char *pSection);

private:
	void		WriteConfig();

private:
	VFile		mFile;
	_Sections	mSections;
};

} // End Namespace

#endif // __VCONFIG_H_INCLUDED__
