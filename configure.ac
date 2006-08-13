AC_INIT([VUtils], [0.1.0], [volzman19@yahoo.com], [vutils])
AC_CONFIG_SRCDIR([src/VObject.cpp])
AC_CONFIG_HEADER([include/config.h])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.

AC_ARG_ENABLE(tracing,
	AC_HELP_STRING([--enable-tracing], [Enable trace output]),
	[trace_enable=$enableval], [trace_enable=no])

if test $trace_enable = yes; then
	AC_DEFINE(TRACE_ENABLE, 1, [Define to enable trace output])
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
                 include/Makefile
                 src/Makefile])
AC_OUTPUT
