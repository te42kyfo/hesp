/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

/* Example usage:
 * cl-make-kernel(program1 "convolution"
 *                (list
 *                 cl_image
 *                 cl_image
 *                 cl_int
 *                 cl_int
 *                 cl_mem
 *                 cl_int
 *                 cl_sampler))
 *
 * Valid types for the arglist are all built-in scalar OpenCL types (except
 * cl_half), together with cl_event and cl_mem
 */

enum scm_cl_arg_type {
    SCM_TYPE_CL_BOOL,
    SCM_TYPE_CL_CHAR,
    SCM_TYPE_CL_UCHAR,
    SCM_TYPE_CL_SHORT,
    SCM_TYPE_CL_USHORT,
    SCM_TYPE_CL_INT,
    SCM_TYPE_CL_UINT,
    SCM_TYPE_CL_LONG,
    SCM_TYPE_CL_ULONG,
    SCM_TYPE_CL_FLOAT,
    SCM_TYPE_CL_DOUBLE,
    //SCM_TYPE_CL_HALF,
    SCM_TYPE_CL_BUFFER,
    SCM_TYPE_CL_IMAGE,
    SCM_TYPE_CL_SAMPLER,
    SCM_TYPE_LAST = SCM_TYPE_CL_SAMPLER
  };

typedef enum scm_cl_arg_type scm_cl_arg_type;

struct typed_cl_kernel {
    cl_kernel kernel;
    size_t    typec;
    scm_cl_arg_type* types;
};

typedef struct typed_cl_kernel typed_cl_kernel;

SCM scm_make_cl_kernel (SCM program, SCM name, SCM args);

SCM scm_set_cl_kernel_args (SCM kernel, SCM args);

SCM scm_enqueue_cl_kernel (SCM command_queue, SCM kernel,
                           SCM offset_list, SCM global_list, SCM local_list,
                           SCM event_wait_list); // -> event

void guile_opencl_init_kernel();
