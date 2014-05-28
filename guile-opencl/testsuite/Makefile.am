SCM_TESTS = \
functions/cl-info.scm \
functions/get-cl-platforms.scm \
functions/make-cl-buffer.scm \
functions/make-cl-context.scm \
usage/kernel-invocation.scm

TESTS = ${SCM_TESTS}

TEST_EXTENSIONS = .scm

AM_TESTS_ENVIRONMENT = \
    GUILE_LOAD_PATH=$(top_srcdir)/modules:$(top_srcdir)/testsuite; \
    export GUILE_LOAD_PATH;

SCM_LOG_COMPILER = \
    $(LIBTOOL) \
    -dlopen ${top_builddir}/src/libguile-opencl.la \
    --mode=execute $(GUILE)
AM_SCM_LOG_FLAGS = -s

CLEANFILES = *.log

EXTRA_DIST = \
    ${TESTS} srfi/srfi-64.scm srfi/srfi-64/testing.scm

