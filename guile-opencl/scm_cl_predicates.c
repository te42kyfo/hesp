#include "guile-opencl.h"

/* ==== predicates ====*/

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
            "Return @code{#t} iff @var{x} is a OpenCL platform, else @code{#f}.")
{
    return type_p(x, cl_platform_tag);
}

SCM_DEFINE (scm_cl_device_p, "cl-device?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL device, else @code{#f}.")
{
    return type_p(x, cl_device_tag);
}

SCM_DEFINE (scm_cl_context_p, "cl-context?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL context, else @code{#f}.")
{
    return type_p(x, cl_context_tag);
}

SCM_DEFINE (scm_cl_command_queue_p, "cl-command-queue?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL command queue, else @code{#f}.")
{
    return type_p(x, cl_command_queue_tag);
}

SCM_DEFINE (scm_cl_mem_p, "cl-mem?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL mem, else @code{#f}.")
{
    return type_p(x, cl_mem_tag);
}

SCM_DEFINE (scm_cl_program_p, "cl-program?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL program, else @code{#f}.")
{
    return type_p(x, cl_program_tag);
}

SCM_DEFINE (scm_cl_kernel_p, "cl-kernel?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL kernel, else @code{#f}.")
{
    return type_p(x, cl_kernel_tag);
}

SCM_DEFINE (scm_cl_event_p, "cl-event?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL event, else @code{#f}.")
{
    return type_p(x, cl_event_tag);
}

SCM_DEFINE (scm_cl_sampler_p, "cl-sampler?", 1, 0, 0,
           (SCM x),
            "Return @code{#t} iff @var{x} is a OpenCL sampler, else @code{#f}.")
{
    return type_p(x, cl_sampler_tag);
}

void scm_init_cl_predicates () {
#ifndef SCM_MAGIC_SNARFER
#include "scm_cl_predicates.x"
#endif
}
