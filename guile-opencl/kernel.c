/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include "CL/cl.h"
#include "kernel.h"
#include "conversion.h"
#include "error.h"

// TODO
SCM scm_make_cl_kernel(SCM program, SCM name) {
    cl_program p = scm_to_cl_program_here(program);
    char      *c_name;
    cl_int     err;
    cl_kernel  kernel;
    scm_dynwind_begin(0);
    c_name = scm_to_utf8_string(name);
    kernel = clCreateKernel(p, c_name, &err);
    scm_dynwind_free(name);
    scm_dynwind_end();
    CL_CHECK( err );
    return scm_from_cl_kernel(kernel);
}


void guile_opencl_init_kernel() {
#ifndef SCM_MAGIC_SNARFER
#include "kernel.x"
#endif
}
