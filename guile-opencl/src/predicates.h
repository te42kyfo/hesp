/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

/* Return SCM_BOOL_T if the argument is of the specified type and SCM_BOOL_F
   otherwise. */
SCM scm_cl_platform_p      (SCM x);
SCM scm_cl_device_p        (SCM x);
SCM scm_cl_context_p       (SCM x);
SCM scm_cl_command_queue_p (SCM x);
SCM scm_cl_buffer_p        (SCM x);
SCM scm_cl_image_p         (SCM x);
SCM scm_cl_program_p       (SCM x);
SCM scm_cl_kernel_p        (SCM x);
SCM scm_cl_event_p         (SCM x);
SCM scm_cl_sampler_p       (SCM x);

void guile_opencl_init_predicates ();
