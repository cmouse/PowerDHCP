AC_PREREQ([2.61])

AC_INIT([powerdhcp], [git])

AC_CONFIG_SRCDIR([dhcpd/dhcpd-main.cc])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([includes/config.h])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign tar-ustar -Wno-portability subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_USE_SYSTEM_EXTENSIONS

AC_CANONICAL_HOST
AC_C_BIGENDIAN
: ${CFLAGS="-W -Wall -O2 -ggdb"}
: ${CXXFLAGS="-W -Wall -O2 -ggdb"}

AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_LANG([C++])

AC_SYS_LARGEFILE
AC_GNU_SOURCE

m4_ifndef([LT_INIT],[
AC_PROG_LIBTOOL
],[
LT_INIT
])

LT_CONFIG_LTDL_DIR([libltdl])
AC_SUBST([moduledir], [$libdir/powerdhcp])
LTDL_INIT

AC_CC_PIE
AC_LD_RELRO
AC_CC_D_FORTIFY_SOURCE
AC_CC_PARAM_SSP_BUFFER_SIZE([1])
AX_PTHREAD([], [AC_MSG_ERROR(["pthreads are required to build this software"])])

BOOST_REQUIRE([1.35])
BOOST_TEST

AX_WITH_LUA

AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STRFTIME

AC_CHECK_HEADERS([sys/epoll.h sys/event.h machine/endian.h endian.h])
AC_CHECK_FUNCS([epoll_ctl poll select getprotobyname_r kevent])

AC_ARG_WITH([mysql],
  [AS_HELP_STRING([--with-mysql],[enable MySQL support])], 
  [MYSQL_CLIENT])

AC_CONFIG_FILES([
  Makefile
  common/Makefile
  dhcpd/Makefile
  dhcpctl/Makefile
  modules/Makefile
  tests/Makefile
])

AC_OUTPUT
