/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "conversion.h"
#include "error.h"
#include "constants.h"
#include "predicates.h"
#include "kernel.h"

/* ensure compatibility across guile versions */
#ifndef SCM_NEWSMOB
#define SCM_NEWSMOB(smob, tag, data) \
    smob = scm_new_smob(tag, data);
#endif

#define RETURN_SCM_FROM_CL(data, tag) do {                              \
        SCM x;                                                   \
        SCM_NEWSMOB(x, guile_opencl_tag, (scm_t_bits)data);      \
        SCM_SET_SMOB_FLAGS(x, tag);                              \
        return x;                                                \
    } while (0)

SCM scm_from_cl_platform_id   (cl_platform_id   data) {
    RETURN_SCM_FROM_CL(data, cl_platform_tag);
}
SCM scm_from_cl_device_id     (cl_device_id     data) {
    RETURN_SCM_FROM_CL(data, cl_device_tag);
}
SCM scm_from_cl_context       (cl_context       data) {
    RETURN_SCM_FROM_CL(data, cl_context_tag);
}
SCM scm_from_cl_command_queue (cl_command_queue data) {
    RETURN_SCM_FROM_CL(data, cl_command_queue_tag);
}
SCM scm_from_cl_buffer        (cl_mem           data) {
    RETURN_SCM_FROM_CL(data, cl_buffer_tag);
}
SCM scm_from_cl_image         (cl_mem           data) {
    RETURN_SCM_FROM_CL(data, cl_image_tag);
}
SCM scm_from_cl_program       (cl_program       data) {
    RETURN_SCM_FROM_CL(data, cl_program_tag);
}
SCM scm_from_cl_kernel        (cl_kernel        data, SCM types) {
    SCM x;
    size_t typec = scm_to_size_t(scm_length(types));
    scm_cl_arg_type *c_types;
    typed_cl_kernel *tkernel;
    c_types = (scm_cl_arg_type *)scm_gc_malloc(sizeof(scm_cl_arg_type) * typec,
                                               "scm_cl_arg_type");
    size_t index = 0;
    SCM    rest  = types;
    while(!scm_to_bool(scm_null_p(rest))) {
        c_types[index] = scm_to_int(scm_car(rest));
        ++index;
        rest = scm_cdr(rest);
    }
    tkernel = (typed_cl_kernel *)scm_gc_malloc(sizeof(typed_cl_kernel),
                                               "typed_cl_kernel");
    tkernel->kernel = data;
    tkernel->typec = typec;
    tkernel->types = c_types;
    RETURN_SCM_FROM_CL(tkernel, cl_kernel_tag);
}
SCM scm_from_cl_event         (cl_event         data) {
    RETURN_SCM_FROM_CL(data, cl_event_tag);
}
SCM scm_from_cl_sampler       (cl_sampler       data) {
    RETURN_SCM_FROM_CL(data, cl_sampler_tag);
}

#undef RETURN_SCM_FROM_CL

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
cl_mem scm_to_cl_buffer (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_buffer_p(obj)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-buffer");
    return (cl_mem)SCM_SMOB_DATA(obj);
}
cl_mem scm_to_cl_image (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_image_p(obj)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-image");
    return (cl_mem)SCM_SMOB_DATA(obj);
}
cl_mem scm_to_cl_mem (SCM obj, const char *subr) {
    if(! (   scm_to_bool(scm_cl_image_p  (obj))
          || scm_to_bool(scm_cl_buffer_p (obj))))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, obj, "cl-image");
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
    typed_cl_kernel *tk = (typed_cl_kernel *)SCM_SMOB_DATA(obj);
    cl_kernel        k  = tk->kernel;
    scm_remember_upto_here_1(obj);
    return k;
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
