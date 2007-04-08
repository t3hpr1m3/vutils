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
#include <vutils/VConfig.h>

/* System Headers */
#include <fstream>

/* Local Headers */
#include <vutils/VLog.h>
#include <vutils/VTypes.h>
#include <vutils/VMD5Hasher.h>

DECLARE_CLASS( "VConfig" );

namespace VUtils
{

VConfig::VConfig()
{

}

VConfig::~VConfig()
{
	if (mFile.IsOpen())
		mFile.Close();
}

bool VConfig::Load(const char *pFile)
{
	VString vLine;		/* line read from config file				*/
	VString	vName;		/* name of the configuration element		*/
	VString	vValue;		/* value configuration element				*/
	VString	vSection;	/* section title							*/
	int		vPosEqual;	/* location of the equal sign (if present)	*/
	int		vMaxRead;
	int		vBytesRead;

	BEG_FUNC("Load")("%p(%s)", pFile, pFile);

	if (mFile.Open(pFile, FIL_READWRITE) != VERR_SUCCESS)
		return END_FUNC(false);
	//VStatus(mFile.GetError()->Code(), mFile.GetError()->Text());

	vMaxRead = 256;
	while (mFile.ReadLine(vLine, vMaxRead) == VERR_SUCCESS)
	{
		VTRACE("Readline returned %d bytes\n", vMaxRead);
		vBytesRead = vMaxRead;
		vMaxRead = 256;

		if (vBytesRead == -1)
			break;

		// check for a blank line
		if (vLine.Length() == 0)
			continue;

		// check for comments 
		if (vLine[0] == '#' || vLine[0] == ';')
			continue;

		// check for section heading
		if (vLine[0] == '[')
		{
			vSection = vLine.SubStr(1, vLine.Find(']')-1).Trim();
			VTRACE("Hit section %s\n", vSection.C_Str());
			VSection vSec;
			mSections[vSection] = vSec;
			continue;
		}

		vPosEqual = vLine.Find('=');

		if (vPosEqual != -1)
		{
			vName	= vLine.SubStr(0, vPosEqual).Trim();
			vValue	= vLine.SubStr(vPosEqual+1).Trim();

			VTRACE("Storing setting %s:%s\n", vName.C_Str(), vValue.C_Str());
			mSections[vSection][vName.C_Str()] = vValue.C_Str();
		}

	}
	mFile.Close();

	return END_FUNC(true);
}

VConfig::VSection& VConfig::operator[](const char *pSection)
{
	_Sections::iterator vIt;
	
	BEG_FUNC("operator[]")("%p(%s)", pSection, pSection);
	
	vIt = mSections.find(pSection);
	
	if (vIt == mSections.end())
	{
		VTRACE("Adding new section\n");
		VSection vSect;
		mSections[pSection] = vSect;
		END_FUNC(&(mSections[pSection]));
		return mSections[pSection];
	}
	else
	{
		VTRACE("Existing section found\n");
		END_FUNC(&(vIt->second));
		return vIt->second;
	}
}

void VConfig::WriteConfig()
{

}

} // End Namespace
