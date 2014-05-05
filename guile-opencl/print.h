/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <libguile.h>

/* display Guile OpenCL objects */
int print_guile_opencl(SCM smob, SCM port, scm_print_state *pstate);

void guile_opencl_init_print();
