###
# Makefile for vutils
###
AC_PREREQ([2.61])

AC_INIT([vutils], [0.6.2], [vmizzle@gmail.com])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_CONFIG_HEADER([vutils/config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AR
AC_PROG_INSTALL

LT_INIT

# Checks for libraries.
AC_MSG_CHECKING([whether to enable trace output])
AC_ARG_ENABLE(tracing,
	AC_HELP_STRING([--disable-tracing], [turn off trace output]))
if test x"$enable_tracing" = x"no"; then
	AC_MSG_RESULT(no)
else
	AC_MSG_RESULT(yes)
	AC_DEFINE(TRACE_ENABLE, 1, [Define to enable trace output])
fi

AC_MSG_CHECKING([whether to enable debug symbols])
AC_ARG_ENABLE(debug,
	AC_HELP_STRING([--disable-debug], [turn off debugging symbols]))

if test x"$disable_debug" = x"no"; then
	AC_MSG_RESULT(no)
	CFLAGS=${CFLAGS-"-g"}
	CXXFLAGS=${CXXFLAGS-"-g"}
	AC_DEFINE(DEBUG, 1, [Define to turn on debugging code])
else
	AC_MSG_RESULT([yes])
fi

# Checks for header files.
AC_HEADER_DIRENT
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h sys/ioctl.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_HEADER_STDC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([getcwd gethostbyname inet_ntoa memset select socket strchr strerror strrchr strstr])

AC_CONFIG_FILES([Makefile
                 vutils/Makefile])
AC_OUTPUT

echo "
Configuration:

CFLAGS:			${CFLAGS}
CXXFLAGS:		${CXXFLAGS}
CPPFLAGS:		${CPPFLAGS}
"
