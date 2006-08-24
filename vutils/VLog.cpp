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
#include <vutils/VLog.h>

/* System Headers */
#include <cstdio>
#include <cstdarg>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

/* Local Headers */
#include <vutils/VMap.h>

/* Macros */

namespace VUtils
{

/* Static Variables */
bool		VLog::mTraceOn = false;
std::string VLog::mLogName = "";
int			VLog::mMainPid = 0;

typedef VMap<int, VLog> LogMap;

/********************************************************************
 *																	*
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 *																	*
 ********************************************************************/
VLog::VLog()
{
	mLogHandle	= NULL;
	mIndentLvl	= 0;
	mLogOpen	= false;
	mTimestamps = true;
}

VLog::VLog(const VLog& pRhs)
{
	mLogHandle	= pRhs.mLogHandle;
	mIndentLvl	= pRhs.mIndentLvl;
	mLogOpen	= pRhs.mLogOpen;
	mTimestamps	= pRhs.mTimestamps;
}

VLog::~VLog()
{
	char vBuffer[1024];
	if (mLogOpen)
	{
		sprintf(vBuffer, "*** Log Closed ***\n");
		fprintf(mLogHandle, vBuffer);
		fflush(mLogHandle);
		fclose(mLogHandle);
	}
	mLogHandle = NULL;
}

/********************************************************************
 *																	*
 *                        A T T R I B U T E S                       *
 *																	*
 ********************************************************************/

/*------------------------------------------------------------------*
 *								GetLog()							*
 *------------------------------------------------------------------*/
/** @brief Returns a reference to an active log file.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS:                                                   *
 *  Date        Description                         Author          *
 *============  ==================================  =============== *
 *                                                                  *
 *------------------------------------------------------------------*/
VLog& VLog::GetLog()
{
	static LogMap vLogMap;
	static int vPid;
	static LogMap::iterator vIt;

	vPid = getpid();

	vIt = vLogMap.find(vPid);
	if (vIt == vLogMap.end())
	{
		VLog vLog;
		vLogMap[vPid] = vLog;
		vLogMap[vPid].Init(vPid);
	}

	return vLogMap[vPid];
}

/********************************************************************
 *																	*
 *                        O P E R A T I O N S                       *
 *																	*
 ********************************************************************/

/*------------------------------------------------------------------*
 *							 InitTrace()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Initializes the tracing facilities, such as setting
 *				the name of the log file and opening the log.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@param		pPgmName
 *					Name of the program.  Log name will  be based
 *					off this name.
 *
 *	@remarks	If tracing has not been enabled at build time, this
 *				function does nothing.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
void VLog::EnableTrace(const char *pPgmName)
{
#ifdef TRACE_ENABLE
	char	vBuffer[256];
	char	*vPtr = NULL;

	strncpy(vBuffer, pPgmName, 256);
	if ((vPtr = strstr(vBuffer, ".exe")) != NULL)
	{	/* found the .exe extension */
		vPtr = '\0';
	}
	mMainPid = getpid();
	
	mLogName = vBuffer;
	mTraceOn = true;
#endif // TRACE_ENABLE
}

/*------------------------------------------------------------------*
 *							 Trace()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Constructs a suitable set of trace output based on
 *				the information provided.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@param		pMsg
 *					Actual message to be output
 *
 *	@remarks	If tracing has not been enabled at runtime, this
 *				function does nothing.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
#ifdef TRACE_ENABLE
void VLog::Trace(const char *pMsg, ...)
{
	static char		vBuffer[1024];
	static va_list	vArgs;

	/* If tracing wasn't initialized, do nothing */
	if (!mTraceOn)
		return;

	Lock();

	/* Construct the trace message */
	memset(vBuffer, '\0', sizeof(vBuffer));
	va_start(vArgs, pMsg);
	vsnprintf(vBuffer, sizeof(vBuffer)-1, pMsg, vArgs);
	va_end(vArgs);

	WriteRaw(vBuffer); // Send it to the parser

	Unlock();
}
#endif

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
void VLog::Init(int pPid)
{
	char		vBuffer[256];
	std::string vLogName;
	char		vPid[10];

	if (mTraceOn)
	{
		sprintf(vPid, "%d", pPid);
		if (getpid() == mMainPid)
			sprintf(vBuffer, "%s.log", mLogName.c_str());
		else
			sprintf(vBuffer, "%s_%s.log", mLogName.c_str(), vPid);
		vLogName = vBuffer;
		if (mLogOpen == false)
		{
			mLogHandle = fopen(vLogName.c_str(), "w");
			if (mLogHandle == NULL)
				return;
			else
			{
				mLogOpen = true;
				sprintf(vBuffer, "*** Log Opened - %d ***\n", pPid);
				fprintf(mLogHandle, vBuffer);
				fflush(mLogHandle);
			}
		}
	}
}

/*------------------------------------------------------------------*
 *							 WriteRaw()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Handles breaking a message up at line breaks and
 *				outputting each individual line.
 *
 *	@author		Josh Williams
 *	@date		09-Apr-2005
 *
 *	@param		pText
 *					Raw message to process
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 * 06-Jun-2005	Added runtime check for tracing.	Josh Williams	*
 *																	*
 *------------------------------------------------------------------*/
void VLog::WriteRaw(const char *pText) const
{
	static const char	*vBufBeg;
	static const char	*vBufEnd;
	static int			vBufLen;
	static char			vText[1024];

	/* If tracing wasn't initialized, do nothing */
	if (!mTraceOn)
		return;

	vBufBeg = &pText[0];
	vBufEnd = strchr(pText, '\n');
	while (vBufEnd != NULL)
	{
		vBufLen = vBufEnd - vBufBeg + 1;
		memset(vText, '\0', sizeof(vText));
		memcpy(vText, vBufBeg, vBufLen);
		Write(vText);
		vBufBeg = vBufEnd+1;
		vBufEnd = strchr(vBufBeg, '\n');
	}
	if (*vBufBeg != '\0')
		Write(vBufBeg);

}

/*------------------------------------------------------------------*
 *							 	Write()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Actually writes output to the trace file.
 *
 *	@author		Josh Williams
 *	@date		09-Apr-2005
 *
 *	@param		pText
 *					Text to be written.
 *
 *	@remarks	If timestamps are enabled, they will be written
 *				first.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 * 06-Jun-2005	Added runtime check for tracing.	Josh Williams	*
 *																	*
 *------------------------------------------------------------------*/
void VLog::Write(const char *pText) const
{
	time_t	vNow = time(NULL);
	char	vTime[50];
	
	/* If tracing wasn't initialized, do nothing */
	if (!mTraceOn)
		return;

	strftime(vTime, 50, "%F %H:%M:%S :: ", localtime(&vNow));
	fprintf(mLogHandle, vTime);
	for (int i = 0; i < mIndentLvl; i++)
		fprintf(mLogHandle, "   ");
	fprintf(mLogHandle, pText);
	fflush(mLogHandle);
}

} // End Namespace

/* vi: set ts=4: */
