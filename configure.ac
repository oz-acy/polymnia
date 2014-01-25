dnl ====================================================================
dnl   configure.ac for POLYMNIA
dnl ====================================================================

AC_PREREQ([2.69])
AC_INIT(polymnia, 0.5.0, [acy@hiemalis.org])
AC_CONFIG_SRCDIR([colcnv.cpp])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE


dnl =======================================
dnl   Checks for programs.
dnl =======================================
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

dnl ===================================
dnl   Checks for libraries.
dnl ===================================


dnl ===================================
dnl   Checks for header files.
dnl ===================================


dnl ==================================================================
dnl   Checks for typedefs, structures, and compiler characteristics.
dnl ==================================================================
dnl AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE


dnl ==================================================================
dnl   Checks for library functions.
dnl ==================================================================
AC_HEADER_STDC
AC_CHECK_FUNCS([memset])




dnl ==================================================================
dnl   Without cygwin
dnl   option : --without-cygwin
dnl ==================================================================
AC_ARG_WITH(cygwin,
[  --without-cygwin        add option -mno-cygwin])
if test "$with_cygwin" = "no"; then
	CPPFLAGS="-mno-cygwin $CPPFLAGS"
	CFLAGS="-mno-cygwin $CFLAGS"
	CXXFLAGS="-mno-cygwin $CXXFLAGS"
	LDFLAGS="-mno-cygwin $LDFLAGS"
fi




dnl =====================================
dnl   Output
dnl =====================================
AC_CONFIG_FILES([Makefile])
AC_OUTPUT


dnl EOF
