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
#include <vutils/VErrorBlock.h>

/* System Headers */
#include <cstdarg>

/* Local Headers */
#include <vutils/VLog.h>

/* Macros */

namespace VUtils
{

/* Static Variables */

/********************************************************************
 *																	*
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 *																	*
 ********************************************************************/
VErrorBlock::VErrorBlock(const char *pFile, const char *pFunc, const int pLine)
	: mFile(pFile), mFunc(pFunc), mLine(pLine)
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
void VErrorBlock::operator() (const char *pFormat, ...) const
{
#ifdef TRACE_ENABLE
	char	vBuffer[1024];
	char	vText[1024];
	va_list	vArgs;
	/* Construct the trace message */
	memset(vBuffer, '\0', sizeof(vBuffer));
	va_start(vArgs, pFormat);
	vsnprintf(vBuffer, sizeof(vBuffer)-1, pFormat, vArgs);
	va_end(vArgs);
	VLog::GetLog().Write("\n");
	VLog::GetLog().Write("===================================================\n");
	VLog::GetLog().Write("=         An internal error has occurred          =\n");
	VLog::GetLog().Write("===================================================\n");
	sprintf(vText, "File: %s\n", mFile);
	VLog::GetLog().Write(vText);
	sprintf(vText, "Func: %s\n", mFunc);
	VLog::GetLog().Write(vText);
	sprintf(vText, "Line: %d\n", mLine);
	VLog::GetLog().Write(vText);
	VLog::GetLog().Write("*Error text*\n");
	VLog::GetLog().WriteRaw(vBuffer); // Send it to the parser
	VLog::GetLog().Write("\n");
	VLog::GetLog().Write("===================================================\n\n");
#endif
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
