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
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 ********************************************************************/
VErrorBlock::VErrorBlock(const char *pFile, const char *pFunc, const int pLine)
	: mFile(pFile), mFunc(pFunc), mLine(pLine) {}

/********************************************************************
 *                        A T T R I B U T E S                       *
 ********************************************************************/

/********************************************************************
 *                        O P E R A T I O N S                       *
 ********************************************************************/

/********************************************************************
 *                         O P E R A T O R S                        *
 ********************************************************************/

void VErrorBlock::operator() (const char *pFormat, ...) const {
#ifdef TRACE_ENABLE
	char    vBuffer[1024];
	char    vText[1024];
	va_list vArgs;
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
 *                         C A L L B A C K S                        *
 ********************************************************************/

/********************************************************************
 *                          I N T E R N A L S                       *
 ********************************************************************/

} // End Namespace

/* vi: set ts=4: */
