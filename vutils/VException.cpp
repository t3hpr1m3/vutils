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
#include <vutils/VException.h>

/* System Headers */

/* Local Headers */

/* Macros */

namespace VUtils
{

/* Static Variables */


/********************************************************************
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 ********************************************************************/
VException::VException(const VString& pErr /*=NULL*/,
		const VString& pFile /*=""*/, const VString& pFunc /*=""*/,
		int pLine /*=0*/)
	: mError(pErr), mFile(pFile), mFunc(pFunc), mLine(pLine) {}

/********************************************************************
 *                        A T T R I B U T E S                       *
 ********************************************************************/

/********************************************************************
 *                        O P E R A T I O N S                       *
 ********************************************************************/
VString VException::Dump() const {
	VString	vTemp;
	vTemp  = "An exception has been thrown!\n\n";
	vTemp += "--------------------------------\n";
	vTemp += "            DETAILS\n";
	vTemp += "--------------------------------\n";
	vTemp += "Function:    " + Func() + "()\n";
	if (Error().Length() != 0)
		vTemp += "Description: " + Error() + "\n";
	if (File().Length() != 0) {
		vTemp += "File:        " + File() + "\n";
		if (Line() != 0) {
			vTemp += "Line:        ";
			vTemp.Cat("%d\n", Line());
		}
	}
	vTemp += "\n";

	return vTemp;
}

/********************************************************************
 *                         O P E R A T O R S                        *
 ********************************************************************/
ostream& operator<<(std::ostream& pOs, const VException& pExc) {
	pOs << pExc.Dump();
	return pOs;
}

/********************************************************************
 *                         C A L L B A C K S                        *
 ********************************************************************/

/********************************************************************
 *                          I N T E R N A L S                       *
 ********************************************************************/

} // End Namespace

/* vi: set ts=4: */
