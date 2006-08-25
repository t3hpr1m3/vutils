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
#include <vutils/VFile.h>

/* System Headers */
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#if VPLATFORM == PLATFORM_WINDOWS
#include <windows.h>
#include <dirent.h>
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
#include <unistd.h>
#endif

/* Local Headers */
#include <vutils/VMD5Hasher.h>
#include <vutils/VLog.h>

#if VPLATFORM == PLATFORM_WINDOWS
#define stat	_stat
#define getcwd  _getcwd
#endif

using std::ifstream;

DECLARE_CLASS( "VFile" );

namespace VUtils
{

#if VPLATFORM == PLATFORM_WINDOWS
char    VFile::FILE_SEP = '\\';
#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX
int		VFile::MAX_PATH = 256;
char    VFile::FILE_SEP = '/';
#endif
int VFile::MAX_READ = 2048;

/********************************************************************
 *																	*
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 *																	*
 ********************************************************************/
VFile::VFile()
{
	Reset();
}

VFile::VFile(const VString& pFile, long pOps /*=FIL_READ*/)
{
	VRESULT vRetval = Open(pFile, pOps);
	if (vRetval != VERR_SUCCESS)
		EXCPT(VFileException, FormatError(vRetval));
}

VFile::~VFile()
{
	Close();
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

/*------------------------------------------------------------------*
 *							 	 Open()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Attempts to open the specified file for reading/writing.
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@param		pFile
 *					Path, relative or absolute, of the file
 *					to be opened.
 *	@param		pOps
 *					FileOp flags indicating how the file is to be processed.
 *
 *	@return		Success/failure
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VFile::Open(const VString& pFile, long pOps /*=FIL_READ*/)
{
	VFileInfo	vInfo;
	VRESULT		vResult;

	BEG_FUNC("Open")("%p(%s), %d", &pFile, pFile.C_Str(), pOps);

	/* check to see if we already have a file opened */
	if (IsOpen())
		return VERR_FILE_ALREADY_OPEN;

	/* clean up variables */
	Reset();

	mOp = pOps;

	/* Try and stat the file */
	vResult = Stat(pFile, vInfo);

	if (vResult == VERR_FILE_NOT_FOUND)
	{
		if (!(pOps & FIL_CREATE))
		{
			VTRACE("File not found and create flag not passed\n");
			return END_FUNC(VERR_FILE_NOT_FOUND);
		}
	}
	else if (vResult != VERR_SUCCESS)
		return END_FUNC(vResult);


	mFileInfo = vInfo;

	VTRACE("File exists and was stat'd\n");

	return END_FUNC(OpenFile(vInfo.Full(), pOps));
}

/*------------------------------------------------------------------*
 *							 	 Read()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Reads the specified number of bytes from the file.
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@param		pBuffer
 *					Buffer to hold the data read from file.
 *	@param		pBytes
 *					Size of pBuffer.
 *
 *	@exception	VFileException	File is at invalid status, or an error
 *								is encountered during reading.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VFile::Read(char *pBuffer, int &pBytes)
{
    int vBytesToRead;

	BEG_FUNC("Read")("%p, %d", pBuffer, pBytes);

	vBytesToRead = pBytes;

	if (mHandle.is_open() == false)
	{
		VTRACE("called with file not open\n");
		return END_FUNC(VERR_FILE_NOT_OPEN);
	}

	if (!(mOp & FIL_READ))
		return END_FUNC(VERR_FILE_NOT_OPEN_R);

	if (vBytesToRead > MAX_READ)
		vBytesToRead = MAX_READ;

	/* if we're already at eof, return 0 */
	if (mHandle.eof())
	{
		pBytes = 0;
		VTRACE("Already at EOF\n");
		return END_FUNC(VERR_SUCCESS);
  	}		

	/* read the data */
	mHandle.read(pBuffer, vBytesToRead);
	
	/* did we hit eof? */
	if (mHandle.eof())
		VTRACE("EOF hit\n");

	if (mHandle.bad())
		return END_FUNC(VERR_BAD_FILE_HANDLE);
	else if (mHandle.fail() && !mHandle.eof())
		return END_FUNC(VERR_READ_FILE);

	pBytes = mHandle.gcount();
	VTRACE("%d bytes read\n", pBytes);

	return END_FUNC(VERR_SUCCESS);
}

/*------------------------------------------------------------------*
 *							 ReadLine()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Reads the specified number of bytes from the file, up to
 *				the first newline or eof.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@param		pBuffer	VString object to hold the data read
 *	@param		pBytes	Maximum number of bytes to read from file
 *
 *	@return		Number of bytes read.  0 is returned if already at eof.
 *
 *	@exception	VFileException	File is at invalid status, or an error
 *								is encountered during reading.
 *
 *	@remarks	The newline is not included in the returned string.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VFile::ReadLine(VString& pBuffer, int &pBytes)
{
	char	*vBuffer;

	BEG_FUNC("ReadLine")("%p, %d", &pBuffer, pBytes);

	if (mHandle.is_open() == false)
		return END_FUNC(VERR_FILE_NOT_OPEN);
	if (!(mOp & FIL_READ))
		return END_FUNC(VERR_FILE_NOT_OPEN_R);

	if (pBytes > MAX_READ)
		pBytes = MAX_READ;

	/* if we're already at eof, return 0 */
	if (mHandle.eof())
	{
		pBytes = 0;
		return END_FUNC(VERR_SUCCESS);
	}

	/* create the read buffer */
	vBuffer = new char[pBytes];
	memset(vBuffer, '\0', pBytes);

	/* read the data */
	mHandle.getline(vBuffer, pBytes);

	if (mHandle.bad())
	{
		delete[] pBuffer;
		return END_FUNC(VERR_BAD_FILE_HANDLE);
	}
	else if (mHandle.fail() && !mHandle.eof())
	{
		delete[] vBuffer;
		return END_FUNC(VERR_READ_FILE);
	}
	
	pBuffer = vBuffer;

	delete[] vBuffer;

	pBytes = mHandle.gcount();
	return END_FUNC(VERR_SUCCESS);
}

/*------------------------------------------------------------------*
 *							 	Write()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Writes the data in the supplied buffer to the current
 *				position within the file.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@param		pBuffer		Data to be written
 *	@param		pBytes		Number of bytes contained in pcBuffer to
 *							be written.
 *
 *	@return		Success/failure
 *
 *	@exception	VFileException	File is at invalid status, or an error
 *								is encountered during writing.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VFile::Write(const char *pBuffer, int pBytes)
{
	BEG_FUNC("Write")("%p, %d", pBuffer, pBytes);

	if (mHandle.is_open() == false)
		return END_FUNC(VERR_FILE_NOT_OPEN);

	if (!(mOp & FIL_WRITE))
		return END_FUNC(VERR_FILE_NOT_OPEN_W);

	mHandle.write(pBuffer, pBytes);

	return END_FUNC(VERR_SUCCESS);
}

/*------------------------------------------------------------------*
 *							 	Seek()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Positions the read/write pointer to the specified offset
 *				within the file.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@param		pOffset		Position, in bytes, to orient the pointer.
 *
 *	@return		Success/failure
 *
 *	@exception	VFileException	File is at invalid status, or an error
 *								is encountered during reading.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VFile::Seek(long pOffset)
{
	BEG_FUNC("Seek")("%d", pOffset);

	if (mHandle.is_open() == false)
		return END_FUNC(VERR_FILE_NOT_OPEN);

	if (mHandle.eof())
		mHandle.clear();

	mHandle.seekg(pOffset, fstream::beg);

	if (mHandle.bad())
		return END_FUNC(VERR_BAD_FILE_HANDLE);
	if (mHandle.fail())
		return END_FUNC(VERR_INVALID_STATUS);

	return END_FUNC(VERR_SUCCESS);
}

/*------------------------------------------------------------------*
 *							 	Close()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Closes the specified file, returning all internal
 *				variables to a clean state.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@return		None
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
void VFile::Close()
{
	BEG_FUNC("Close")(NULL);

	if (IsOpen())
		mHandle.close();

	Reset();

	END_FUNCV();
}

/*------------------------------------------------------------------*
 *							 Truncate()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Wipes the currently opened file clean, leaving it with
 *				a size of 0 bytes.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@return		Success/failure
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VFile::Truncate()
{
	BEG_FUNC("Truncate")(NULL);

	/* store the name of the currently open file */
	VString vFull = mFileInfo.Full();

	/* close the file */
	Close();

	/* reopen the file, passing the truncate flag */
	return END_FUNC(OpenFile(mFileInfo.Full(), mOp | FIL_TRUNCATE));
}	

/*------------------------------------------------------------------*
 *								Stat()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Requests statistical information about the specified
 *				file.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@param		pFile		Path, relative or absolute, of the file
 *							to stat.
 *	@param		pInfo		Object to hold the obtained information.
 *
 *	@retval		VERR_INVALID_PATH	Path is invalid
 *	@retval		VERR_FILE_NOT_FOUND	Specified file does not exist
 *	@retval		VERR_SUCCESS		Everything is ok, my son
 *
 *	@exception	VFileException	Path specified is actually a directory.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VFile::Stat(const VString& pFile, VFileInfo& pInfo)
{
	struct stat	vFileStat;
	VRESULT		vRetval;

	BEG_FUNC("Stat")("%p, %p", &pFile, &pInfo);

	if ((vRetval = ResolvePath(pFile, pInfo.mPath, pInfo.mName,
						pInfo.mExt)) != VERR_SUCCESS)
		return END_FUNC(vRetval);

	if (stat(pInfo.Path() + pInfo.Name(), &vFileStat) == -1)
		return END_FUNC(VERR_FILE_NOT_FOUND);

	if (S_ISDIR(vFileStat.st_mode))
		EXCPT(VFileException, VString("Path is a directory => [" + pFile + "]"));

	pInfo.mSize = vFileStat.st_size;
	pInfo.mModify.Set(&vFileStat.st_mtime);

	return END_FUNC(VERR_SUCCESS);
}

/*------------------------------------------------------------------*
 *							   ResolvePath()						*
 *------------------------------------------------------------------*/
/**
 *	@brief		Evaluates a filename supplied, negotiating special characters
 *				such as '../' and '~'.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@param		pFile		Path of the file to be resolved
 *	@param		pPath		Evaluated absolute path of the file
 *	@param		pName		Simple name of the file (no path)
 *	@param		pExt		File extension
 *
 *	@return		Success/failure
 *
 *	@exception	VInvalidPathException	Empty filename was supplied, or
 *										something went wrong during parsing.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VFile::ResolvePath(const VString& pFile, VString& pPath,
						VString& pName, VString& pExt)
{
	VString		vFull;
	char		vWork[MAX_PATH];
	char		vChunkL[MAX_PATH];
	char		vChunkR[MAX_PATH];

	BEG_FUNC("ResolvePath")("%s, %p, %p, %p", pFile.C_Str(), &pPath,
								&pName, &pExt);

	/* make sure we weren't passed an empty file name */
	if (pFile.Length() == 0)
		return END_FUNC(VERR_INVALID_PATH);

	/* on Win32, there is already a (*sigh*) helper function for this */
#if VPLATFORM == PLATFORM_WINDOWS
	char *vFile;
	if (GetFullPathName(pFile, MAX_PATH, vWork, &vFile) == 0)
	{
		VTRACE("GetFullPathName() failed with error %d\n", GetLastError());
		return END_FUNC(VERR_INVALID_PATH);
	}

	vFull = vWork;

#elif VPLATFORM == PLATFORM_MAC || VPLATFORM == PLATFORM_LINUX

	if (pFile[0] == '/')
	{
		/* absolute path supplied */
		vFull = pFile;
	}
	else if (pFile[0] == '~')
	{
		/* home directory */
		char *vHome = getenv("HOME");
		if (vHome == NULL)
		{
			VString vErr("Error getting environment variable HOME");
			vErr.Cat("\nOS Error => [%s]", strerror(errno));
			EXCPT(VException, vErr);
		}
		vFull = vHome + pFile.SubStr(1);
	}
	else
	{
		/* relative path */
		getcwd(vWork, sizeof(vWork));
		vFull = vWork;
		vFull.Cat("/%s", pFile.C_Str());
	}

	/* replace any ../'s we find */
	strncpy(vWork, vFull.C_Str(), sizeof(vWork));
	char *vPc = strstr(vWork, "../");
	while (vPc != NULL)
	{
		strncpy(vChunkR, vPc+3, sizeof(vChunkR));
		*vPc = '\0';
		strncpy(vChunkL, vWork, sizeof(vChunkL));
		vChunkL[strlen(vChunkL)-1] = '\0';
		vPc = strrchr(vChunkL, '/');
		if (vPc != NULL)
		{
			*vPc = '\0';
			sprintf(vWork, "%s/%s", vChunkL, vChunkR);
		}
		else
			EXCPT(VInvalidPathException, vFull);

		vPc = strstr(vWork, "../");
	}

	/* now, replace any ./'s we find */
	vPc = strstr(vWork, "./");
	while (vPc != NULL)
	{
		strncpy(vChunkR, vPc+2, sizeof(vChunkR));
		*vPc = '\0';
		strncpy(vChunkL, vWork, sizeof(vChunkL));
		vChunkL[strlen(vChunkL)-1] = '\0';
		sprintf(vWork, "%s/%s", vChunkL, vChunkR);

		vPc = strstr(vWork, "./");
	}

	vFull = vWork;

#endif                                      /* End platform specific */

	/* phew.  now, strFull contains an absolute file name */
	int vOffset = vFull.RFind(FILE_SEP);
	if (vOffset == -1)
		EXCPT(VInvalidPathException, vFull);
	pPath = vFull.SubStr(0, vOffset+1);
	pName = vFull.SubStr(vOffset+1);

	/* let's try and find an extension */
	vOffset = pName.RFind('.');
	if (vOffset != -1)
		pExt = pName.SubStr(vOffset+1);

	return END_FUNC(VERR_SUCCESS);
}

/*------------------------------------------------------------------*
 *							 CheckSum()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Generates an MD5 sum of the currently opened file.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@param		pSum		Buffer to hold the 32 byte sum
 *
 *	@return		Success/failure
 *
 *	@remarks	If the sum has already been computed for this file,
 *				the existing value is returned.
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VFile::CheckSum(VString& pSum)
{
	static ifstream		vFile;
	static char			vBuffer[1024];
	static char			vDigest[33];
	static int			vBytes = 0;
	static VMD5Hasher	vHasher;
	VRESULT				vRetval = VERR_SUCCESS;

	BEG_FUNC("CheckSum")("%p", &pSum);

	if (mFileHashed == false)
	{
		vFile.open(this->Full(), ifstream::binary);
		if (vFile.bad())
		{
			return END_FUNC(VERR_OPEN_FILE);
		}

		vHasher.Init();

		vFile.read(vBuffer, 1024);
		vBytes = vFile.gcount();

		while (vBytes > 0)
		{
			vHasher.Update((VBYTE *)vBuffer, vBytes);
			vFile.read(vBuffer, 1024);
			vBytes = vFile.gcount();
		}

		vFile.close();

		vHasher.Final();

		vRetval = vHasher.GetDigestString(vDigest, sizeof(vDigest));
		if (vRetval != VERR_SUCCESS)
		{
			return END_FUNC(vRetval);
		}
			
		vDigest[32] = '\0';
		mFileHashed = true;
	}

	pSum = vDigest;

	return END_FUNC(VERR_SUCCESS);
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

/*------------------------------------------------------------------*
 *								Reset()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Resets all internal variables to an initial state.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@return		None
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
void VFile::Reset()
{
	BEG_FUNC("Reset")(NULL);

	mFileInfo.Reset();
	mOp = FIL_NONE;
	mFileHashed = false;

	END_FUNCV();
}

/*------------------------------------------------------------------*
 *							 OpenFile()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Allocates a standard library handle for the specified
 *				file.  This function is used internally to perform the
 *				low-level file opening.
 *
 *	@author		Josh Williams
 *	@date		02-Dec-2004
 *
 *	@param		strFile		Full path to the file to be opened.
 *	@param		ops			Flags to use on file open.
 *
 *	@return		Success/failure
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
VRESULT VFile::OpenFile(const VString& pFile, long pOps)
{
	fstream::openmode	vMode = static_cast<fstream::openmode>(0);

	BEG_FUNC("OpenFile")("%p(%s), %d", &pFile, pFile.C_Str(), pOps);

	/* set the flags */
	if (pOps & FIL_READ)
		vMode |= std::ios::in;
	if (pOps & FIL_WRITE)
		vMode |= fstream::out;
	if (pOps & FIL_APPEND)
		vMode |= fstream::app;
	if (pOps & FIL_TRUNCATE)
		vMode |= fstream::trunc;
	if (pOps & FIL_BINARY)
		vMode |= fstream::binary;

	/* magic time.  try and open the stream */
	mHandle.open(pFile, vMode);

	if (mHandle.bad())
		return END_FUNC(VERR_OPEN_FILE);

	return END_FUNC(VERR_SUCCESS);
}

bool VFile::IsDirectory(const char *pPath)
{
	BEG_FUNC("IsDirectory")("%p(%s)", pPath, pPath);

	struct stat vSt;
	if (stat(pPath, &vSt) == -1)
	    return END_FUNC(false);

	return END_FUNC(S_ISDIR(vSt.st_mode));
}

} // End Namespace

/* vi: set ts=4: */
