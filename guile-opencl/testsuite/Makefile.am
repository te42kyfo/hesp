TESTS                = \
kernel.scm predicates.scm info.scm

TEST_EXTENSIONS      = .scm
SCM_LOG_COMPILER     = @GUILE@
AM_SCM_LOG_COMPILER  = -s
AM_TESTS_ENVIRONMENT = LD_LIBRARY_PATH='$LD_LIBRARY_PATH:../src/.libs' \
                       GUILE_LOAD_PATH='$GUILE_LOAD_PATH:../modules'

CLEANFILES = *.log
