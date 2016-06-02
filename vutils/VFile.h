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
#if !defined(__VFILE_H_INCLUDED__)
#define __VFILE_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */
#include <fstream>

/* Local Headers */
#include <vutils/VObject.h>
#include <vutils/VFileInfo.h>
#include <vutils/VTime.h>
#include <vutils/VString.h>
#include <vutils/VException.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using std::fstream;

/* Macros */

namespace VUtils {

class VFileException : public VException {
public:
	VFileException(const VString& strErr = "", const VString& strFunc = "",
			const VString& strFile = "", int nLine = 0)
		: VException(strErr, strFunc, strFile, nLine) {}
};

class VInvalidPathException : public VFileException {
public:
	VInvalidPathException(const VString& strPath, const VString& strFunc = "",
			const VString& strFile = "", int nLine = 0)
		: VFileException(VString("Invalid path => [") + strPath + "]", strFunc,
				strFile, nLine) {}
};

class VFileNotFoundException : public VFileException {
public:
	VFileNotFoundException(const VString& strPath, const VString& strFunc = "",
			const VString& strFile = "", int nLine = 0)
		: VFileException(VString("File not found => [") + strPath + "]", strFunc,
				strFile, nLine) {}
};

/**
 *  @class      VFile
 *
 *  @brief      General purpose file wrapper class.
 *  @author     Josh Williams
 *  @version    0.1.0
 *  @date       12-Oct-2004
 */
class VFile : public VObject {
public:
	/**
	 * Flags used for opening a file
	 */
	enum FileOp {
		FIL_NONE = 0,
		FIL_READ = 1,
		FIL_WRITE = 2,
		FIL_READWRITE = 3,
		FIL_APPEND = 4,
		FIL_CREATE = 8,
		FIL_TRUNCATE = 16,
		FIL_BINARY = 32
	};

	enum FileSeek {
		FIL_BEG = -2,
		FIL_END = -1
	};

public:
	/*==================================*
	 *     CONSTRUCTION/DESTRUCTION     *
	 *==================================*/
	VFile();
	VFile(const VString& pFile, long pOps = FIL_READ | FIL_BINARY);
	virtual ~VFile();

public:
	/*==================================*
	 *             ATTRIBUTES           *
	 *==================================*/
	const VString&      Path() const;
	const VString&      Name() const;
	const VString&      Ext() const;
	const VString       Full() const;
	const VULONG        Size() const;
	const VTime         Modified() const;
	bool                IsOpen();
	const VULONG        Offset();

public:
	/*==================================*
	 *           OPERATIONS             *
	 *==================================*/
	VRESULT             Open(const VString& pStrFile, long pOps = FIL_READ | FIL_BINARY);
	VRESULT             Read(char *pBuffer, int& pBytes);
	VRESULT             ReadLine(VString& pBuffer, int& pBytes);
	VRESULT             Write(const char *pBuffer, int pBytes);
	VRESULT             Seek(long pOffset);
	void                Close();
	VRESULT             Truncate();
	static VRESULT      Stat(const VString& pStrFile, VFileInfo& pInfo);
	static VRESULT      ResolvePath(const VString& pStrFile, VString& pStrPath,
	                        VString& pStrName, VString& pStrExt);
	VRESULT             CheckSum(VString& pStrSum);
	static VUINT        MaxPath() { return MAX_PATH; }
	static bool         IsDirectory(const char *pPath);

public:
	/*==================================*
	 *             OPERATORS            *
	 *==================================*/

protected:
	/*==================================*
	 *             CALLBACKS            *
	 *==================================*/

private:
	/*==================================*
	 *             INTERNALS            *
	 *==================================*/
	void                Reset();
	VRESULT             OpenFile(const VString& pStrFile, long pOps);

private:
	/*==================================*
	 *             VARIABLES            *
	 *==================================*/
#if defined(WIN32) || defined(_WINDOWS) /* Windows platforms */
#else                                   /* Unix/linux */
#ifndef MAX_PATH
	static int          MAX_PATH;       /**< Maximum length for path strings */
#endif
#endif                                  /* End platform specific */
	static int          MAX_READ;       /**< Maximum number of bytes to read
	                                         on each call to Read() or
	                                         ReadLine() */
	static char         FILE_SEP;
	VFileInfo           mFileInfo;      /**< Information about the currently
	                                         open file */
	long                mOp;            /**< Flags specified on Open() */
	fstream             mHandle;        /**< Actual system file opened for
	                                         reading/writing */
	bool                mFileHashed;    /**< Whether or not the checksum has
	                                         been computed for this file */
};

/********************************************************************
 *                                                                  *
 *                           I N L I N E S                          *
 *                                                                  *
 ********************************************************************/
inline
const VString& VFile::Path() const {
	return mFileInfo.Path();
}

inline
const VString& VFile::Name() const {
	return mFileInfo.Name();
}

inline
const VString& VFile::Ext() const {
	return mFileInfo.Ext();
}

inline
const VString VFile::Full() const {
	return mFileInfo.Full();
}

inline
const VULONG VFile::Size() const {
	return mFileInfo.Size();
}

inline
const VTime VFile::Modified() const {
	return mFileInfo.Modified();
}

inline
bool VFile::IsOpen() {
	return mHandle.is_open();
}

inline
const VULONG VFile::Offset() {
	return mHandle.tellg();
}

} // End Namespace

#endif // __VFILE_H_INCLUDED__

/* vi: set ts=4: */
