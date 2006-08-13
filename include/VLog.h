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
#if !defined(__VLOG_H_INCLUDED__)
#define __VLOG_H_INCLUDED__

#include "VPlatform.h"

/* System Headers */
#include <string>
#include <cstdarg>
#include <iostream>
#include <sstream>

/* Local Headers */
#include "VMutex.h"
#include "VTypes.h"
#include "VObject.h"
#include "VErrorBlock.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Macros */
#define VTRACE VLog::GetLog().Trace
#define VERROR (VErrorBlock(__FILE__, __FUNCTION__, __LINE__))

#define DECLARE_CLASS( a ) \
		static const char __CLASS__[] = a

#define Z_TO_STRING( a ) VObject::RetString( a )

#ifdef TRACE_ENABLE

#if VPLATFORM == PLATFORM_WINDOWS
#define BEG_FUNC(func) \
	static char __FUNCTION__[] = func; \
	static std::string __CFUNC__; \
	MakeFName(__CLASS__, __FUNCTION__, __CFUNC__); \
	VLog::CUDPFuncWrapper x(__CFUNC__.c_str()); \
	x 
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
#define BEG_FUNC(func) \
	static std::string __CFUNC__; \
	MakeFName(__CLASS__, __FUNCTION__, __CFUNC__); \
	VLog::CUDPFuncWrapper x(__CFUNC__.c_str()); \
	x
#else
#error Unsupported platform
#endif


#define END_FUNC(x) \
		EndFunc( __CFUNC__.c_str(), x)

#define END_FUNCV() \
		EndFunc( __CFUNC__.c_str() )

#else // No Trace

#if VPLATFORM == PLATFORM_WINDOWS
#define BEG_FUNC( func ) \
	static char __FUNCTION__[] = func; \
	VLog::CUDPFuncWrapper x( func ); \
	x
#elif VPLATFORM == PLATFORM_LINUX || VPLATFORM == PLATFORM_MAC
#define BEG_FUNC( func ) \
	VLog::CUDPFuncWrapper x( func ); \
	x
#else
#error Unsupported platform
#endif

#define END_FUNC( x ) \
	x
#define END_FUNCV()
#endif // TRACE_ENABLE

namespace VUtils
{
	
void BegFunc(const char *pFunc, const char *pMsg, ...);
void EndFunc(const std::string& pFunc);
bool EndFunc(const std::string& pFunc, bool pResult);
VRESULT EndFunc(const std::string& pFunc, VRESULT vResult);
template <class T>
T EndFunc(const std::string& pFunc, const T& pResult);
int	MakeFName(const char *pClass, const char *pFunc, std::string& pCFname);



class VLog : public VMutex
{
public:
	class CUDPFuncWrapper
	{
	public:
		CUDPFuncWrapper(const char *pFunc)
			: mFunc(pFunc) {}
		void operator() (const char *pFormat, ...) const
		{
			va_list		vArgs;
			static char	vBuffer[1024];
			if (pFormat == NULL)
				BegFunc(mFunc, NULL);
			else
			{
				va_start(vArgs, pFormat);
				vsnprintf(vBuffer, 1024-1, pFormat, vArgs);
				BegFunc(mFunc, vBuffer);
			}
		}
	private:
		const char	*mFunc;
	};

public:
	/*==================================*
	 *	   CONSTRUCTION/DESTRUCTION		*
	 *==================================*/
	VLog();
	VLog(const VLog& pRhs);
	virtual ~VLog();

public:
	/*==================================*
	 *			  ATTRIBUTES			*
	 *==================================*/
	static VLog&		GetLog();
	int					GetIndent();

public:
	/*==================================*
	 *			  OPERATIONS			*
	 *==================================*/
	static void			EnableTrace(const char *pPgmName);
	void				Init(int pPid);
	void				Timestamps(bool pTimestamps = true);
	void				SetIndent(int pIndent);
#ifdef TRACE_ENABLE
	void				Trace(const char *pMsg, ...);
#else
	void				Trace(const char *pMsg, ...) {if (pMsg) (void)0;}
#endif

public:
	/*==================================*
	 *			   OPERATORS			*
	 *==================================*/

protected:
	/*==================================*
	 *             CALLBACKS			*
	 *==================================*/

private:
	/*==================================*
	 *			   INTERNALS			*
	 *==================================*/
	void				WriteRaw(const char *pText) const;
	void 				Write(const char *pText) const;

private:
	/*==================================*
	 *             VARIABLES            *
	 *==================================*/
	static bool			mTraceOn;		/**< whether or not tracing has been
										  	 enabled at run-time */
	static std::string	mLogName;		/**< name of the trace file used for
										  	 output */
	FILE*				mLogHandle;		/**< file-system handle to the trace
										  	 file */
	int					mIndentLvl;		/**< level of indentation to perform
										  	 when writing trace info	*/
	bool				mLogOpen;		/**< whether or not the log file is
										  	 open */
	bool				mTimestamps;	/**< whether or not to add timestamps to
											 each line of trace output */
	static int			mMainPid;		/**< Main pid (central log file) */
};



/********************************************************************
 *																	*
 *							I N L I N E S							*
 *																	*
 ********************************************************************/

/**
 *	Returns the level of indention currently set for the trace log.
 */
inline
int VLog::GetIndent()
{
	return mIndentLvl;
}

/**
 *	Sets whether or not timestamps should be inserted for each line
 *	of trace output.
 */
inline
void VLog::Timestamps(bool pTimestamps /*=true*/)
{
	mTimestamps = pTimestamps;
}
/**
 *	Sets the level of indentation to use for tracing.  Never call this function
 *	directly.  It is handled by the ZFuncWrapper class.
 */
inline
void VLog::SetIndent(int pIndent)
{
	mIndentLvl += pIndent;
}

/**
 *	Logs function entry and increases log indentation.
 */
inline
void BegFunc(const char *pFunc, const char *pMsg, ...)
{
	static char 	vBuffer[1024];
	static va_list	vArgs;

	if (pMsg == NULL)
		VTRACE("%s() -> ENTRY\n", pFunc);
	else
	{
		va_start(vArgs, pMsg);
		vsnprintf(vBuffer, sizeof(vBuffer)-1, pMsg, vArgs);
		va_end(vArgs);
		VTRACE("%s(%s) -> ENTRY\n", pFunc, vBuffer);
	}
	VLog::GetLog().SetIndent(1);
}

/**
 *	Logs function exit and decreases log indentation.
 */
inline
void EndFunc(const std::string& pFunc)
{
	VLog::GetLog().SetIndent(-1);
	VTRACE("%s() <- EXIT\n", pFunc.c_str());
	return;
}

/**
 *	@overload
 */
inline
bool EndFunc(const std::string& pFunc, bool pResult)
{
	VLog::GetLog().SetIndent(-1);
	VTRACE("%s() <- EXIT with %s\n", pFunc.c_str(), pResult ? "true" : "false");
	return pResult;
}

/**
 *	@overload
 */
inline
VRESULT EndFunc(const std::string& pFunc, VRESULT pResult)
{
	VLog::GetLog().SetIndent(-1);
	VTRACE("%s() <- EXIT with %s\n", pFunc.c_str(), Z_TO_STRING(pResult));
	return pResult;
}

/**
 *	@overload
 */
template <class T>
T EndFunc(const std::string& pFunc, const T& pResult)
{
	std::ostringstream	output_stream;
	output_stream << pFunc << "() <- EXIT with " << pResult << "\n";
	VLog::GetLog().SetIndent(-1);
	VTRACE(output_stream.str().c_str());
	return pResult;
}

inline
int MakeFName(const char *pClass, const char *pFunc, std::string& pCFname)
{
	pCFname = pClass;
	pCFname += "::";
	pCFname += pFunc;
	return pCFname.length();
}

} // End Namespace

#endif // __VLOG_H_INCLUDED__

/* vi: set ts=4: */
