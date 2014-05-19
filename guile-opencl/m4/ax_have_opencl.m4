# Author: Marco Heisig <marco.heisig@fau.de>

AC_DEFUN([AX_HAVE_OPENCL], [dnl
  AC_CHECK_HEADER([CL/cl.h], [dnl
    AC_SEARCH_LIBS([clGetPlatformIDs], [OpenCL], [dnl
      AC_CACHE_CHECK([for OpenCL], [ax_cv_have_opencl], [dnl
    AC_LINK_IFELSE([dnl
      AC_LANG_PROGRAM(
        [#include <CL/cl.h>],
        [cl_int err; err = clGetPlatformIDs(0, NULL, NULL);])],
      [ax_cv_have_opencl=yes],
      [ax_cv_have_opencl=no])])])])
if test "$ax_cv_have_opencl" = yes; then
  AC_DEFINE(HAVE_OPENCL,,[define if the platform has OpenCL])
fi
])dnl
