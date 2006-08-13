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
#include "VObject.h"

/* System Headers */

/* Local Headers */

/* Macros */
#define CASE_VRESULT( result ) \
	case result: \
		return #result

namespace VUtils
{

/* Static Variables */
static const char* ErrStrings[] = {
	"Invalid path",							// 101
	"File not found",						// 102
	"File already open",					// 103
	"Invalid argument",						// 104
	"Open file",							// 105
	"Invalid status",						// 106
	"File not open",						// 107
	"File not open for reading",			// 108
	"File not open for writing",			// 119
	"File not open for read/write",			// 110
	"Error reading file",					// 111
	"Invalid file handle",					// 112
	"Error creating system socket",			// 113
	"Error connecting to remote host",		// 114
	"Error communication with host",		// 115
	"NULL pointer passed",					// 116
	"Invalid value supplied",				// 117
	"Operation timed out",					// 118
	"Name resolution error",				// 119
};

/********************************************************************
 *																	*
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 *																	*
 ********************************************************************/
VObject::VObject()
{

}

VObject::~VObject()
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
const char* VObject::FormatError(VRESULT pErr)
{
	static VString vError;

	if (pErr == 0)
		vError = "Success";
	else if (pErr == 100 || pErr >= VERR_MAX)
		vError = "Unknown";
	else 
		vError = ErrStrings[pErr-101];

	return vError.C_Str();
}

const char* VObject::RetString(VRESULT pResult)
{
	switch (pResult)
	{
	CASE_VRESULT(VERR_SUCCESS);
	CASE_VRESULT(VERR_UNKNOWN);
	CASE_VRESULT(VERR_INVALID_PATH);
	CASE_VRESULT(VERR_FILE_NOT_FOUND);
	CASE_VRESULT(VERR_FILE_ALREADY_OPEN);
	CASE_VRESULT(VERR_INVALID_ARGUMENT);
	CASE_VRESULT(VERR_OPEN_FILE);
	CASE_VRESULT(VERR_INVALID_STATUS);
	CASE_VRESULT(VERR_FILE_NOT_OPEN);
	CASE_VRESULT(VERR_FILE_NOT_OPEN_R);
	CASE_VRESULT(VERR_FILE_NOT_OPEN_W);
	CASE_VRESULT(VERR_FILE_NOT_OPEN_RW);
	CASE_VRESULT(VERR_READ_FILE);
	CASE_VRESULT(VERR_BAD_FILE_HANDLE);
	CASE_VRESULT(VERR_CREATE_SOCKET);
	CASE_VRESULT(VERR_CONNECT);
	CASE_VRESULT(VERR_COMM);
	CASE_VRESULT(VERR_NULL_POINTER);
	CASE_VRESULT(VERR_INVALID_VALUE);
	CASE_VRESULT(VERR_TIMEOUT);
	CASE_VRESULT(VERR_RESOLVE_ERROR);
	default:
		return "ZERR_UNKNOWN";
	}
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
