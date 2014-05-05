/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "conversion.h"
#include "error.h"
#include "constants.h"
#include "predicates.h"

#define SCM_FROM_CL_BODY(handle, tag)                            \
    SCM x = scm_new_smob(guile_opencl_tag, (scm_t_bits)handle);  \
    SCM_SET_SMOB_FLAGS(x, tag);                                  \
    return x

SCM scm_from_cl_platform_id   (cl_platform_id     handle) {
    SCM_FROM_CL_BODY(handle, cl_platform_tag);
}
SCM scm_from_cl_device_id     (cl_device_id     handle) {
    SCM_FROM_CL_BODY(handle, cl_device_tag);
}
SCM scm_from_cl_context       (cl_context       handle) {
    SCM_FROM_CL_BODY(handle, cl_context_tag);
}
SCM scm_from_cl_command_queue (cl_command_queue handle) {
    SCM_FROM_CL_BODY(handle, cl_command_queue_tag);
}
SCM scm_from_cl_mem           (cl_mem           handle) {
    SCM_FROM_CL_BODY(handle, cl_mem_tag);
}
SCM scm_from_cl_program       (cl_program       handle) {
    SCM_FROM_CL_BODY(handle, cl_program_tag);
}
SCM scm_from_cl_kernel        (cl_kernel        handle) {
    SCM_FROM_CL_BODY(handle, cl_kernel_tag);
}
SCM scm_from_cl_event         (cl_event         handle) {
    SCM_FROM_CL_BODY(handle, cl_event_tag);
}
SCM scm_from_cl_sampler       (cl_sampler       handle) {
    SCM_FROM_CL_BODY(handle, cl_sampler_tag);
}

/* ==== SCM to C conversion routines ==== */

cl_platform_id scm_to_cl_platform_id (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_platform_p(obj)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-platform");
    return (cl_platform_id)SCM_SMOB_DATA(obj);
}
cl_device_id scm_to_cl_device_id (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_device_p(obj)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-device");
    return (cl_device_id)SCM_SMOB_DATA(obj);
}
cl_context scm_to_cl_context (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_context_p(obj)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-context");
    return (cl_context)SCM_SMOB_DATA(obj);
}
cl_command_queue scm_to_cl_command_queue (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_command_queue_p(obj)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-command-queue");
    return (cl_command_queue)SCM_SMOB_DATA(obj);
}
cl_mem scm_to_cl_mem (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_mem_p(obj)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-mem");
    return (cl_mem)SCM_SMOB_DATA(obj);
}
cl_program scm_to_cl_program (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_program_p(obj)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-program");
    return (cl_program)SCM_SMOB_DATA(obj);
}
cl_kernel scm_to_cl_kernel (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_kernel_p(obj)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-kernel");
    return (cl_kernel)SCM_SMOB_DATA(obj);
}
cl_event scm_to_cl_event (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_event_p(obj)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-event");
    return (cl_event)SCM_SMOB_DATA(obj);
}
cl_sampler scm_to_cl_sampler (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_sampler_p(obj)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-sampler");
    return (cl_sampler)SCM_SMOB_DATA(obj);
}

void guile_opencl_init_conversion() {
#ifndef SCM_MAGIC_SNARFER
#include "conversion.x"
#endif
}
