#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([Space Simulator], [0.0.1], [c.mitch.carroll@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_SUBDIRS([vtx])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lGL':
AC_CHECK_LIB([GL], [glBegin])
# FIXME: Replace `main' with a function in `-lGLU':
AC_CHECK_LIB([GLU], [gluPerspective])
# FIXME: Replace `main' with a function in `-lglut':
AC_CHECK_LIB([glut], [glutInit])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([sqrt])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 vtx/Makefile])
AC_OUTPUT
