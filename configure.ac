#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([pdfpreview], [0.1])
LT_INIT
AC_CONFIG_SRCDIR([pdfpreview.cpp])
AM_INIT_AUTOMAKE
AC_OUTPUT(Makefile)
#AC_CONFIG_HEADERS([config.h])

AC_ARG_ENABLE([static-bin],
[  --enable-static-bin    Build static binary],
[case "${enableval}" in
  yes) staticbin=true ;;
  no)  staticbin=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-static-bin]) ;;
esac],[staticbin=false])
AM_CONDITIONAL([BUILD_STATIC], [test x$staticbin = xtrue])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CPP

# Checks for libraries.
PKG_CHECK_MODULES(POPPLER, poppler >= 0.23)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset])

AC_OUTPUT
