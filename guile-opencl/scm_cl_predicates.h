#pragma once
#include "guile-opencl.h"

/* Return SCM_BOOL_T if the argument is of the specified type and SCM_BOOL_F
   otherwise. */
SCM scm_cl_platform_p      (SCM x);
SCM scm_cl_device_p        (SCM x);
SCM scm_cl_context_p       (SCM x);
SCM scm_cl_command_queue_p (SCM x);
SCM scm_cl_mem_p           (SCM x);
SCM scm_cl_program_p       (SCM x);
SCM scm_cl_kernel_p        (SCM x);
SCM scm_cl_event_p         (SCM x);
SCM scm_cl_sampler_p       (SCM x);
