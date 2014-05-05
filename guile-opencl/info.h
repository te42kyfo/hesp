/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

/* Query information from the OpenCL object. The param name must be a Guile
   string matching the name in the OpenCL specification,
   e.g. "CL_PLATFORM_VENDOR". */
SCM scm_get_cl_platform_info          (SCM platform, SCM param_name);
SCM scm_get_cl_device_info            (SCM device,   SCM param_name);
SCM scm_get_cl_mem_info               (SCM memobj,   SCM param_name);
SCM scm_get_cl_context_info           (SCM context,  SCM param_name);
SCM scm_get_cl_command_queue_info     (SCM queue,    SCM param_name);
SCM scm_get_cl_program_info           (SCM program,  SCM param_name);
SCM scm_get_cl_kernel_info            (SCM kernel,   SCM param_name);
SCM scm_get_cl_event_info             (SCM event,    SCM param_name);
SCM scm_get_cl_sampler_info           (SCM sampler,  SCM param_name);
SCM scm_get_cl_program_build_info     (SCM program,  SCM device, SCM param_name);
SCM scm_get_cl_work_group_info        (SCM kernel,   SCM device, SCM param_name);
SCM scm_get_cl_event_profiling_info   (SCM event,    SCM param_name);

/* Dispatch to specific scm_get_cl_OBJECT_info command depending on type. If
   param name is omitted, return an alist of all avilable information. */
SCM scm_get_cl_info (SCM any,      SCM param_name);

void guile_opencl_init_info();
