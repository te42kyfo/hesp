#pragma once
#include "guile-opencl.h"

/* These commands wrap a given OpenCL handle in a Guile Smob. They do
   always succeed. */
SCM scm_from_cl_platform_id   (cl_platform_id   platform);
SCM scm_from_cl_device_id     (cl_device_id     device);
SCM scm_from_cl_context       (cl_context       context);
SCM scm_from_cl_command_queue (cl_command_queue command_queue);
SCM scm_from_cl_mem           (cl_mem           mem);
SCM scm_from_cl_program       (cl_program       program);
SCM scm_from_cl_kernel        (cl_kernel        kernel);
SCM scm_from_cl_event         (cl_event         event);
SCM scm_from_cl_sampler       (cl_sampler       sampler);

/* These commands convert a given Guile Smob back to the underlying C
   handle. An error is thrown if the smob is of the wrong type */
#define scm_to_cl_platform_id_here(obj)   scm_to_cl_platform_id   (obj, __func__);
#define scm_to_cl_device_id_here(obj)     scm_to_cl_device_id     (obj, __func__);
#define scm_to_cl_context_here(obj)       scm_to_cl_context       (obj, __func__);
#define scm_to_cl_command_queue_here(obj) scm_to_cl_command_queue (obj, __func__);
#define scm_to_cl_mem_here(obj)           scm_to_cl_mem           (obj, __func__);
#define scm_to_cl_program_here(obj)       scm_to_cl_program       (obj, __func__);
#define scm_to_cl_kernel_here(obj)        scm_to_cl_kernel        (obj, __func__);
#define scm_to_cl_event_here(obj)         scm_to_cl_event         (obj, __func__);
#define scm_to_cl_sampler_here(obj)       scm_to_cl_sampler       (obj, __func__);

cl_platform_id   scm_to_cl_platform_id   (SCM platform,      const char *subr);
cl_device_id     scm_to_cl_device_id     (SCM device,        const char *subr);
cl_context       scm_to_cl_context       (SCM context,       const char *subr);
cl_command_queue scm_to_cl_command_queue (SCM command_queue, const char *subr);
cl_mem           scm_to_cl_mem           (SCM mem,           const char *subr);
cl_program       scm_to_cl_program       (SCM program,       const char *subr);
cl_kernel        scm_to_cl_kernel        (SCM kernel,        const char *subr);
cl_event         scm_to_cl_event         (SCM event,         const char *subr);
cl_sampler       scm_to_cl_sampler       (SCM sampler,       const char *subr);
