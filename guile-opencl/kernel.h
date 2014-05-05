/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

/* kernel related functions */

/* Example usage:
 * cl-make-kernel(program "convolution"
 *                (list
 *                 cl_image
 *                 cl_image
 *                 cl_int
 *                 cl_int
 *                 cl_buffer
 *                 cl_int
 *                 cl_sampler))
 *
 * Valid types for the arglist are all built-in scalar OpenCL types, together
 * with cl_event.
 */
//SCM scm_make_cl_kernel (SCM program, SCM name, SCM arglist);

SCM scm_enqueue_kernel (SCM command_queue,
                        SCM offset_list, SCM global_list, SCM local_list,
                        SCM event_wait_list); // -> event

SCM scm_set_kernel_arg (SCM kernel, SCM argnum, SCM value);

void guile_opencl_init_kernel();
