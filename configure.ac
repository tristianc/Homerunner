dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(homerunner, 1.0)
AC_PROG_CC([gcc clang])
AC_CONFIG_HEADER([config.h])

AC_CANONICAL_SYSTEM

dnl Initialize automake
AM_INIT_AUTOMAKE

dnl this allows us specify individual liking flags for each target
AM_PROG_CC_C_O 


dnl AS_COMPILER_FLAG([-fdiagnostics-color=auto], [AM_CFLAGS="$AM_CFLAGS -fdiagnostics-color=auto"])

GLIB_REQ=2.32.0
TOTEM_REQ=3.10.0
LIBPEAS_REQ=0.7.3
LIBXML2_REQ=2.9.0
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= $GLIB_REQ])
PKG_CHECK_MODULES([TOTEM], [totem >= $TOTEM_REQ])
PKG_CHECK_MODULES([LIBPEAS], [libpeas-1.0 >= $LIBPEAS_REQ])
PKG_CHECK_MODULES([LIBXML2], [libxml-2.0 >= $LIBXML2_REQ])

# Get HDHomerun library and include locations
# Learned from http://www.openismus.com/documents/linux/using_libraries/using_libraries.shtml
AC_ARG_WITH([hdhomerun-include-path],
	[AS_HELP_STRING([--with-hdhomerun-include-path], [location of the HDHomerun headers, defaults to /usr/include/hdhomerun])],
	[HDHOMERUN_CFLAGS="-I$withval"],
	[HDHOMERUN_CFLAGS='-I/usr/include/hdhomerun'])
AC_SUBST([HDHOMERUN_CFLAGS])
dnl AC_CHECK_HEADERS([hdhomerun/hdhomerun.h hdhomerun.h], [break], AC_MSG_ERROR([HDHomerun headers not found.]))

AC_ARG_WITH([hdhomerun-lib-path],
	[AS_HELP_STRING([--with-hdhomerun-lib-path], [location of the HDHomerun library])],
	[HDHOMERUN_LIBS="-L$withval -lhdhomerun"],
	[HDHOMERUN_LIBS='-lhdhomerun'])
AC_SUBST([HDHOMERUN_LIBS])

# Find glib-compile-resource command
AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources, no)
if test x$GLIB_COMPILE_RESOURCES = xno; then
	AC_MSG_ERROR(Could not find glib-compile-resources in your PATH)
fi

# Find glib-genmarshal command
AC_PATH_PROG(GLIB_GENMARSHAL, glib-genmarshal, no)
if test x$GLIB_GENMARSHAL = xno; then
	AC_MSG_ERROR(Could not find glib-genmarshal in your PATH)
fi

dnl Initialize Libtool
LT_INIT

dnl Check if Libtool is present
dnl Libtool is used for building share libraries 
AC_PROG_LIBTOOL

AC_CONFIG_FILES(Makefile
                homerunner/Makefile)
AC_OUTPUT
