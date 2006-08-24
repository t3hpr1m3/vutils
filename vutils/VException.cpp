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
#include <vutils/VException.h>

/* System Headers */

/* Local Headers */

/* Macros */

namespace VUtils
{

/* Static Variables */

/********************************************************************
 *																	*
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 *																	*
 ********************************************************************/
VException::VException(const VString& pErr /*=NULL*/,
		const VString& pFile /*=""*/, const VString& pFunc /*=""*/,
		int pLine /*=0*/)
	: mError(pErr), mFile(pFile), mFunc(pFunc), mLine(pLine)
{

}

/********************************************************************
 *																	*
 *                        A T T R I B U T E S                       *
 *																	*
 ********************************************************************/

/********************************************************************
 *																	*
 *                        O P E R A T I O N S                       *
 *																	*
 ********************************************************************/
VString VException::Dump() const
{
	VString	vTemp;
	vTemp  = "An exception has been thrown!\n\n";
	vTemp += "--------------------------------\n";
	vTemp += "            DETAILS\n";
	vTemp += "--------------------------------\n";
	vTemp += "Function:    " + Func() + "()\n";
	if (Error().Length() != 0)
		vTemp += "Description: " + Error() + "\n";
	if (File().Length() != 0)
	{
		vTemp += "File:        " + File() + "\n";
		if (Line() != 0)
		{
			vTemp += "Line:        ";
			vTemp.Cat("%d\n", Line());
		}
	}
	vTemp += "\n";

	return vTemp;
}

/********************************************************************
 *																	*
 *                          O P E R A T O R S                       *
 *																	*
 ********************************************************************/
ostream& operator<<(std::ostream& pOs, const VException& pExc)
{
	pOs << pExc.Dump();
	return pOs;
}

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
