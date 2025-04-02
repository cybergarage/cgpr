##################################################################
#
# Cyber Garage Portable Runtime for C
#
##################################################################

AC_PREREQ([2.69])
AC_INIT([cgpr],[1.0.0])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_HEADER([include/cgpr/cgpr.h])
AC_CONFIG_SRCDIR([include/cgpr/src/util/bytes.c])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_MACRO_DIR([m4])

##############################
# Checks for programs.
##############################

AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_AWK
#AC_PROG_LIBTOOL
AC_PROG_RANLIB

if test x"$ac_cv_prog_cc_c99" = xno; then
   AC_MSG_ERROR([a C99 compiler is required to build cgpr])
fi

##############################
# Checks for header files.
##############################

AC_CHECK_HEADERS([stdbool.h])

##############################
# Checks for pthread
##############################

AC_CHECK_HEADERS([pthread.h],,[AC_MSG_ERROR(cgpr needs POSIX thread library)])
AC_CHECK_LIB([pthread],[main])

##############################
# Debug
##############################

AC_ARG_ENABLE(
 	[debug],
	AS_HELP_STRING([--enable-debug],[ enable debugging (default = no) ]),
	[case "${enableval}" in
    	yes | no ) enable_debug="${enableval}" ;;
	esac],
	[]
)

if [ test "$enable_debug" = yes ]; then
	AC_DEFINE([DEBUG],1,[Define to 1 if you want to debug])
fi

##############################
# Testing
##############################

AC_ARG_ENABLE(
 	[test],
	AS_HELP_STRING([--enable-test],[ build tests (default = no) ]),
	[case "${enableval}" in
    	yes | no ) build_test="${enableval}" ;;
	esac],
	[]
)

AM_CONDITIONAL(CG_ENABLE_TEST,test "$build_test" = yes)
if [ test "$build_test" = yes ]; then
	AC_CHECK_LIB([boost_system-mt],[main],,
		[AC_CHECK_LIB([boost_system],[main],,[AC_MSG_ERROR(cgpr needs boost::system)])])
	AC_CHECK_LIB([boost_unit_test_framework-mt],[main],,
		[AC_CHECK_LIB([boost_unit_test_framework],[main],,[AC_MSG_ERROR(cgpr needs boost::unit_test_framework)])])
fi

##############################
# Examples
##############################

AC_ARG_ENABLE(
	[examples],
	AS_HELP_STRING([--enable-examples],[ build examples (default = yes) ]),
	[case "${enableval}" in
	yes | no ) build_examples="${enableval}" ;;
	esac],
	[build_examples="yes"]
)
AM_CONDITIONAL(CG_ENABLE_EXAMPLES,test "$build_examples" = yes)

##############################
# Makefiles
##############################

AC_CONFIG_FILES([
Makefile
include/Makefile
lib/Makefile
lib/unix/Makefile
test/Makefile
test/unix/Makefile
])
AC_OUTPUT
