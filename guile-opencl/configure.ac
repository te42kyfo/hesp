AC_PREREQ(2.61)
AC_INIT(guile-opencl, 0.1, marco.heisig@fau.de)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_LANG_C

LT_PREREQ([2.4.2])
LT_INIT

GUILE_PROGS
GUILE_FLAGS
AC_PATH_PROG([guile_snarf], [guile-snarf], [not-found])
if test "x$guile_snarf" = "xnot-found"; then
   AC_MSG_ERROR([`guile-snarf' not found.  Please install Guile 1.8.x or later.])
fi
GUILE_SITE_DIR

AX_HAVE_OPENCL
if test "x$ax_cv_have_opencl" != xyes ; then
   AC_MSG_ERROR([`OpenCL' not found. Please install an OpenCL implementation.])
fi

pkgdatadir="$prefix/share/guile/site/$GUILE_EFFECTIVE_VERSION"
if test "x$guilemoduledir" = "x"; then
  guilemoduledir="$pkgdatadir"
fi
AC_SUBST([guilemoduledir])

AC_CONFIG_HEADERS(src/config.h)
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 modules/Makefile
 testsuite/Makefile
])

AC_OUTPUT
