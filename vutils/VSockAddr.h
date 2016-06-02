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
#if !defined(__VSOCKADDR_H_INCLUDED__)
#define __VSOCKADDR_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */

/* Local Headers */
#include <vutils/VTypes.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Macros */

namespace VUtils {

/**
 *  @class      VSockAddr
 *  @brief      Wrapper around the sockaddr_in structure.
 *  @author     Josh Williams
 *  @version    0.1.0
 *  @remarks    Allows for easy access to the members of the sockaddr_in
 *              structure.  Handles flipping port numbers and converting
 *              IP addresses to text form.
 */
class VSockAddr : public sockaddr_in {
public:
	/*==================================*
	 *     CONSTRUCTION/DESTRUCTION     *
	 *==================================*/
	VSockAddr();
	VSockAddr(const sockaddr& sa);
	VSockAddr(const sockaddr_in& sin);
	VSockAddr(VULONG pAddrIP, VUSHORT pPort = 0);
	VSockAddr(const char* pcAddr, VUSHORT pPort = 0);

public:
	/*==================================*
	 *             ATTRIBUTES           *
	 *==================================*/
	const char*         GetAddr() const;
	VULONG              GetAddrIP() const;
	VUSHORT             GetPort() const;

public:
	/*==================================*
	 *           OPERATIONS             *
	 *==================================*/
	void                SetAddr(const char *pAddr);
	void                SetAddrIP(VULONG pAddrIP);
	void                SetPort(VUSHORT pPort);

public:
	/*==================================*
	 *             OPERATORS            *
	 *==================================*/
	const VSockAddr&    operator=(const sockaddr& sa);
	const VSockAddr&    operator=(const sockaddr_in& sin);

protected:
	/*==================================*
	 *             CALLBACKS            *
	 *==================================*/

private:
	/*==================================*
	 *             INTERNALS            *
	 *==================================*/

private:
	/*==================================*
	 *             VARIABLES            *
	 *==================================*/
};



/********************************************************************
 *                                                                  *
 *                           I N L I N E S                          *
 *                                                                  *
 ********************************************************************/

inline
VSockAddr::VSockAddr() {
	sin_family = AF_INET;
	sin_port = 0;
	sin_addr.s_addr = 0;
}

inline
VSockAddr::VSockAddr(const sockaddr& sa) {
	memcpy(this, &sa, sizeof(sockaddr));
}

inline
VSockAddr::VSockAddr(const sockaddr_in& sin) {
	memcpy(this, &sin, sizeof(sockaddr_in));
}

/**
 *  @param      pAddr
 *                  Dotted decimal IP address.
 *  @param      pPort
 *                  Port number to assign.
 */
inline
VSockAddr::VSockAddr(const char* pAddr, const VUSHORT pPort /*=0*/) {
	sin_family = AF_INET;
	sin_port = htons(pPort);
	sin_addr.s_addr = inet_addr(pAddr);
}

/**
 *  @param      pAddrIP
 *                  Network byte ordered address.
 *  @param      pPort
 *                  Port number to assign.
 */
inline
VSockAddr::VSockAddr(VULONG pAddrIP, VUSHORT pPort /*=0*/) {
	sin_family = AF_INET;
	sin_port = htons(pPort);
	sin_addr.s_addr = htonl(pAddrIP);
}

/**
 *  @brief Retrieves the address for this structure in dotted decimal form
 */
inline
const char* VSockAddr::GetAddr() const {
	return inet_ntoa(sin_addr);
}

/**
 *  @brief Retrieves the address for this structure in network byte order.
 */
inline
unsigned long VSockAddr::GetAddrIP() const {
	return ntohl(sin_addr.s_addr);
}

inline
unsigned short VSockAddr::GetPort() const {
	return ntohs(sin_port);
}

inline
void VSockAddr::SetAddr(const char *pAddr) {
	sin_addr.s_addr = inet_addr(pAddr);
}

inline
void VSockAddr::SetAddrIP(VULONG pAddrIP) {
	sin_addr.s_addr = pAddrIP;
}

inline
void VSockAddr::SetPort(VUSHORT pPort) {
	sin_port = htons(pPort);
}

inline
const VSockAddr& VSockAddr::operator=(const sockaddr& sa) {
	memcpy (this, &sa, sizeof(sockaddr));
	return *this;
}

inline
const VSockAddr& VSockAddr::operator=(const sockaddr_in& sin) {
	memcpy (this, &sin, sizeof(sockaddr_in));
	return *this;
}

} // End Namespace

#endif // __VSOCKADDR_H_INCLUDED__

/* vi: set ts=4: */
