#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([Space Simulator], [0.0.1], [c.mitch.carroll@gmail.com])
AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SUBDIRS([vtx quat])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
PKG_CHECK_MODULES([GUILE],[guile-2.0])
# Checks for libraries.
AC_CHECK_LIB([GL], [glBegin])
AC_CHECK_LIB([GLU], [gluPerspective])
AC_CHECK_LIB([glut], [glutInit])
AC_CHECK_LIB([vtx], [Vtx])
AC_CHECK_LIB([quat], [Quat])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([sqrt])

AC_CONFIG_FILES([Makefile
		 src/Makefile])
AC_OUTPUT
