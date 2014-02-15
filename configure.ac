AC_PREREQ([2.61])

dnl The following lines may be patched by set-version-auth.
AC_INIT([powerdhcp], [git])
dnl AC_SUBST([DIST_HOST], [TO_BE_PATCHED])
dnl End patch area.

AC_CONFIG_SRCDIR([dhcpd/dhcpd.cc])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([includes/config.h])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign tar-ustar -Wno-portability subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CANONICAL_HOST
AC_C_BIGENDIAN
: ${CXXFLAGS="-Wall -O2"}

AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_YACC
AM_PROG_LEX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_LANG([C++])

m4_ifndef([LT_INIT],[
AC_PROG_LIBTOOL
],[
LT_INIT
])

LT_CONFIG_LTDL_DIR([libltdl])
LTDL_INIT

BOOST_REQUIRE([1.53])
PKG_CHECK_MODULES([LUA], [lua5.1-c++]) 
MYSQL_CLIENT

AC_CONFIG_FILES([
  Makefile
  common/Makefile
  dhcpd/Makefile
  dhcpctl/Makefile
  modules/Makefile
  modules/gsql/Makefile
  tests/Makefile
])

AC_OUTPUT 
