dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(homerunner, 1.0)
AC_PROG_CC

AC_CANONICAL_SYSTEM

dnl Initialize automake
AM_INIT_AUTOMAKE

dnl this allows us specify individual liking flags for each target
AM_PROG_CC_C_O 

#
GLIB_REQ=2.32.0
TOTEM_REQ=3.10.0
LIBPEAS_REQ=0.7.3
PKG_CHECK_MODULES(GLIB, glib-2.0 >= $GLIB_REQ)
PKG_CHECK_MODULES(TOTEM, totem >= $TOTEM_REQ)
PKG_CHECK_MODULES(LIBPEAS, libpeas-1.0 >= $LIBPEAS_REQ)

dnl Initialize Libtool
LT_INIT

dnl Check if Libtool is present
dnl Libtool is used for building share libraries 
AC_PROG_LIBTOOL

AC_CONFIG_FILES(Makefile
                homerunner/Makefile
                include/Makefile)
AC_OUTPUT