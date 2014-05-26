SCM_TESTS = \
functions/cl-info.scm \
functions/make-cl-buffer.scm \
functions/get-cl-platforms.scm \
usage/kernel-invocation.scm

TESTS = ${SCM_TESTS}

TEST_EXTENSIONS      = .scm

AM_TESTS_ENVIRONMENT = \
    LD_LIBRARY_PATH=$(abs_builddir)/../src/.libs:$$LD_LIBRARY_PATH; \
    export LD_LIBRARY_PATH; \
    GUILE_LOAD_PATH=$(abs_builddir)/../modules:$(abs_builddir)/; \
    export GUILE_LOAD_PATH;

SCM_LOG_COMPILER = $(GUILE)
AM_SCM_LOG_FLAGS = -s

CLEANFILES = *.log

EXTRA_DIST = \
    ${TESTS}
