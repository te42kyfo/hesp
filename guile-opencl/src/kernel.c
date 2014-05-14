/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include "CL/cl.h"
#include "kernel.h"
#include "conversion.h"
#include "error.h"
#include "predicates.h"
#include "stdbool.h"

SCM_DEFINE (scm_make_cl_kernel, "make-cl-kernel", 2, 0, 0,
            (SCM program, SCM name),
            "Create an OpenCL kernel referring to the function named"
            "@var{name} in the OpenCL program @var{program}") {
    cl_program p = scm_to_cl_program_here(program);
    char      *c_name;
    cl_int     err;
    cl_kernel  kernel;
    SCM name_bv = scm_string_to_utf8(name);
    c_name = SCM_BYTEVECTOR_CONTENTS(name_bv);
    kernel = clCreateKernel(p, c_name, &err);
    scm_remember_upto_here_1(name_bv);
    CL_CHECK( err );
    return scm_from_cl_kernel(kernel);
}

SCM_DEFINE (scm_set_cl_kernel_arg, "set-cl-kernel-arg", 3, 0, 0,
            (SCM kernel, SCM argnum, SCM data),
            "Set the argument @var{argnum} of the OpenCL kernel @var{kernel}\n"
            "to the value stored inside the length-one bytevector or\n"
            "cl memory object @var{data}.") {
    bool cl_mem_p = scm_to_bool(scm_cl_mem_p(data));
    bool cl_bv_p  = scm_to_bool(scm_bytevector_p(data));
    SCM_ASSERT_TYPE(cl_mem_p || cl_bv_p,
                    data, SCM_ARG3, __func__, "bytevector or cl-mem");
    cl_kernel k = scm_to_cl_kernel_here(kernel);
    cl_uint   n = scm_to_cl_uint(argnum);
    size_t    s;
    size_t   *p;
    cl_mem mem;
    if(cl_bv_p) {
        s =           SCM_BYTEVECTOR_LENGTH(data);
        p = (size_t *)SCM_BYTEVECTOR_CONTENTS(data);
    } else {
        s = sizeof(cl_mem);
        mem = (cl_mem)SCM_SMOB_DATA(data);
        p = &mem;
    }

    CL_CHECK( clSetKernelArg(k, n, s, p) );
    scm_remember_upto_here_1(data);
    return kernel;
}

SCM_DEFINE (scm_enqueue_cl_kernel, "enqueue-cl-kernel", 5, 0, 1,
            (SCM queue, SCM kernel,
             SCM offsets, SCM global_dims, SCM local_dims,
             SCM events),
            "Enqueue the OpenCL kernel @var{kernel} in the OpenCL queue\n"
            "@var{queue}. @var{offsets}, @var{global_dims} and\n"
            "@var{local_dims} define the index offset, index size and\n"
            "local work group size of the operation respectively.\n"
            "@var{events} is a list of cl_events that have to be completed\n"
            "before the operation starts.\n") {
    cl_command_queue q = scm_to_cl_command_queue_here(queue);
    cl_kernel        k = scm_to_cl_kernel_here(kernel);
    SCM_ASSERT_TYPE(scm_to_bool(scm_list_p(offsets)),
                    offsets, SCM_ARG3, __func__, "list");
    SCM_ASSERT_TYPE(scm_to_bool(scm_list_p(global_dims)),
                    global_dims, SCM_ARG4, __func__, "list");
    SCM_ASSERT_TYPE(scm_to_bool(scm_list_p(local_dims)),
                    local_dims, SCM_ARG5, __func__, "list");
    SCM     obv;
    SCM     gbv;
    SCM     lbv;
    const size_t s = sizeof(size_t);
    switch(s) { // compiler can optimize this away
    case 1: {
        obv = scm_list_to_u8vector(offsets);
        gbv = scm_list_to_u8vector(global_dims);
        lbv = scm_list_to_u8vector(local_dims);
        break;
    }
    case 2: {
        obv = scm_list_to_u16vector(offsets);
        gbv = scm_list_to_u16vector(global_dims);
        lbv = scm_list_to_u16vector(local_dims);
        break;
    }
    case 4: {
        obv = scm_list_to_u32vector(offsets);
        gbv = scm_list_to_u32vector(global_dims);
        lbv = scm_list_to_u32vector(local_dims);
        break;
    }
    case 8: {
        obv = scm_list_to_u64vector(offsets);
        gbv = scm_list_to_u64vector(global_dims);
        lbv = scm_list_to_u64vector(local_dims);
        break;
    }
    default:
        scm_misc_error(__func__, "crazy sizeof(size_t)", scm_from_size_t(s));
    }
    size_t  ol =           SCM_BYTEVECTOR_LENGTH(   obv ) / s;
    size_t *o  = (size_t *)SCM_BYTEVECTOR_CONTENTS( obv )    ;
    size_t  gl =           SCM_BYTEVECTOR_LENGTH(   gbv ) / s;
    size_t *g  = (size_t *)SCM_BYTEVECTOR_CONTENTS( gbv )    ;
    size_t  ll =           SCM_BYTEVECTOR_LENGTH(   lbv ) / s;
    size_t *l  = (size_t *)SCM_BYTEVECTOR_CONTENTS( lbv )    ;

    if(! ( ol == gl && gl == ll ) )
        scm_misc_error(__func__,
                       "~a, ~a and ~a are not of the same length",
                       scm_list_3(offsets, global_dims, local_dims));
    size_t dim = ol;

    // TODO event wait list
    cl_event event;
    CL_CHECK( clEnqueueNDRangeKernel(q, k, dim, o, g, l, 0, NULL, &event) );
    scm_remember_upto_here_1(obv);
    scm_remember_upto_here_1(gbv);
    scm_remember_upto_here_1(lbv);
    return scm_from_cl_event(event);
}

void guile_opencl_init_kernel() {
#ifndef SCM_MAGIC_SNARFER
#include "kernel.x"
#endif
}

