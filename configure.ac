dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(CppCommodityStuff, 1.0)

CXXFLAGS="$CXXFLAGS -std=c++17"
AC_PROG_CXX

AC_CANONICAL_SYSTEM

AC_CONFIG_MACRO_DIR([m4])

dnl Initialize automake
AM_INIT_AUTOMAKE

dnl this allows us to specify individual linking flags for each target
AM_PROG_CC_C_O 

dnl Initialize Libtool
LT_INIT

AC_CONFIG_FILES(Makefile
                include/Makefile
                lib/Makefile
                Hello/Makefile
                CSVFile/Makefile)
AC_OUTPUT
