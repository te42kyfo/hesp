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
SCM scm_make_cl_kernel (SCM program, SCM name);

SCM scm_set_cl_kernel_arg (SCM kernel, SCM argnum, SCM bv);

SCM scm_enqueue_cl_kernel (SCM command_queue, SCM kernel,
                           SCM offset_list, SCM global_list, SCM local_list,
                           SCM event_wait_list); // -> event

void guile_opencl_init_kernel();
