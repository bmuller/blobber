#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([glblobber],[0.0],[jayscribbs@gmail.com])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE()

# Checks for programs.
AC_PROG_CXX
AC_PROG_CXXCPP
AC_LANG_CPLUSPLUS
AC_CANONICAL_HOST
AC_PROG_INSTALL

# Checks for libraries.
AM_PROG_LIBTOOL
AM_PROG_AS

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
PKG_CHECK_MODULES([GLUT],[glut >= 2.4.0],,[
 AC_MSG_ERROR([No GLUT library found (version 2.4.0 or higher). You can get one from http://freeglut.sourcefourge.net])
])

AC_CONFIG_FILES([Makefile glblobber/Makefile])

AC_OUTPUT
