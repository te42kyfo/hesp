/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include "predicates.h"
#include "constants.h"

/* Return SCM_BOOL_T iff x is a valid smob and the flags match target_tag and
   SCM_BOOL_F otherwise. */
inline static SCM
type_p(SCM x, scm_t_bits target_tag) {
    if(!SCM_SMOB_PREDICATE(guile_opencl_tag, x)) return SCM_BOOL_F;
    scm_t_bits tag = SCM_SMOB_FLAGS(x);
    if(tag == target_tag) return SCM_BOOL_T;
    else return SCM_BOOL_F;
}

SCM_DEFINE (scm_cl_platform_p, "cl-platform?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL platform,\n"
            "else @code{#f}.")
{
    return type_p(x, cl_platform_tag);
}

SCM_DEFINE (scm_cl_device_p, "cl-device?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL device,\n"
            "else @code{#f}.")
{
    return type_p(x, cl_device_tag);
}

SCM_DEFINE (scm_cl_context_p, "cl-context?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL context,\n"
            "else @code{#f}.")
{
    return type_p(x, cl_context_tag);
}

SCM_DEFINE (scm_cl_command_queue_p, "cl-command-queue?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL command queue,\n"
            "else @code{#f}.")
{
    return type_p(x, cl_command_queue_tag);
}

SCM_DEFINE (scm_cl_buffer_p, "cl-buffer?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL buffer,\n"
            "else @code{#f}.")
{
    return type_p(x, cl_buffer_tag);
}

SCM_DEFINE (scm_cl_image_p, "cl-image?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL image,\n"
            "else @code{#f}.")
{
    return type_p(x, cl_image_tag);
}

SCM_DEFINE (scm_cl_program_p, "cl-program?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL program,\n"
            " else @code{#f}.")
{
    return type_p(x, cl_program_tag);
}

SCM_DEFINE (scm_cl_kernel_p, "cl-kernel?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL kernel,\n"
            "else @code{#f}.")
{
    return type_p(x, cl_kernel_tag);
}

SCM_DEFINE (scm_cl_event_p, "cl-event?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL event,\n"
            "else @code{#f}.")
{
    return type_p(x, cl_event_tag);
}

SCM_DEFINE (scm_cl_sampler_p, "cl-sampler?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL sampler,\n"
            "else @code{#f}.")
{
    return type_p(x, cl_sampler_tag);
}

void guile_opencl_init_predicates () {
#ifndef SCM_MAGIC_SNARFER
#include "predicates.x"
#endif
}
