#include "guile_opencl.h"

/* ==== predicates ====*/

inline static SCM
type_p(SCM smob, scm_t_bits target_tag) {
    if(!SCM_SMOB_PREDICATE(guile_opencl_tag, smob)) return SCM_BOOL_F;
    scm_t_bits tag = SCM_SMOB_FLAGS(smob);
    if(tag == target_tag) return SCM_BOOL_T;
    else return SCM_BOOL_F;
}

SCM      scm_cl_platform_p (SCM smob) { return type_p(smob, cl_platform_tag); }
SCM        scm_cl_device_p (SCM smob) { return type_p(smob, cl_device_tag); }
SCM       scm_cl_context_p (SCM smob) { return type_p(smob, cl_context_tag); }
SCM scm_cl_command_queue_p (SCM smob) { return type_p(smob, cl_command_queue_tag); }
SCM           scm_cl_mem_p (SCM smob) { return type_p(smob, cl_mem_tag); }
SCM       scm_cl_program_p (SCM smob) { return type_p(smob, cl_program_tag); }
SCM        scm_cl_kernel_p (SCM smob) { return type_p(smob, cl_kernel_tag); }
SCM         scm_cl_event_p (SCM smob) { return type_p(smob, cl_event_tag); }
SCM       scm_cl_sampler_p (SCM smob) { return type_p(smob, cl_sampler_tag); }


/* ==== C to SCM conversion routines ==== */

#define SCM_FROM_CL_BODY(handle, tag)                               \
    SCM smob = scm_new_smob(guile_opencl_tag, (scm_t_bits)handle);  \
    SCM_SET_SMOB_FLAGS(smob, tag);                                  \
    return smob

SCM scm_from_cl_platform_id (cl_platform_id     handle) {
    SCM_FROM_CL_BODY(handle, cl_platform_tag);
}
SCM scm_from_cl_device        (cl_device_id     handle) {
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
        scm_wrong_type_arg_msg(__func__, 1, obj, "cl-platform");
    return (cl_platform_id)SCM_SMOB_DATA(obj);
}
cl_device_id scm_to_cl_device_id (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_device_p(obj)))
        scm_wrong_type_arg_msg(__func__, 1, obj, "cl-device");
    return (cl_device_id)SCM_SMOB_DATA(obj);
}
cl_context scm_to_cl_context (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_context_p(obj)))
        scm_wrong_type_arg_msg(__func__, 1, obj, "cl-context");
    return (cl_context)SCM_SMOB_DATA(obj);
}
cl_command_queue scm_to_cl_command_queue (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_command_queue_p(obj)))
        scm_wrong_type_arg_msg(__func__, 1, obj, "cl-command-queue");
    return (cl_command_queue)SCM_SMOB_DATA(obj);
}
cl_mem scm_to_cl_mem (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_mem_p(obj)))
        scm_wrong_type_arg_msg(__func__, 1, obj, "cl-mem");
    return (cl_mem)SCM_SMOB_DATA(obj);
}
cl_program scm_to_cl_program (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_program_p(obj)))
        scm_wrong_type_arg_msg(__func__, 1, obj, "cl-program");
    return (cl_program)SCM_SMOB_DATA(obj);
}
cl_kernel scm_to_cl_kernel (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_kernel_p(obj)))
        scm_wrong_type_arg_msg(__func__, 1, obj, "cl-kernel");
    return (cl_kernel)SCM_SMOB_DATA(obj);
}
cl_event scm_to_cl_event (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_event_p(obj)))
        scm_wrong_type_arg_msg(__func__, 1, obj, "cl-event");
    return (cl_event)SCM_SMOB_DATA(obj);
}
cl_sampler scm_to_cl_sampler (SCM obj, const char *subr) {
    if(!scm_to_bool(scm_cl_sampler_p(obj)))
        scm_wrong_type_arg_msg(__func__, 1, obj, "cl-sampler");
    return (cl_sampler)SCM_SMOB_DATA(obj);
}

int print_guile_opencl(SCM smob, SCM port, scm_print_state *pstate) {
    scm_t_bits tag = SCM_SMOB_FLAGS(smob);

    if(tag == cl_platform_tag) {
        cl_platform_id platform = (cl_platform_id) SCM_SMOB_DATA(smob);
        SCM key = scm_from_uint32(CL_PLATFORM_NAME);
        scm_puts("#<", port);
        scm_display(scm_get_cl_platform_info(smob, key), port);
        scm_puts(">", port);
        return 1;
    }
    if(tag == cl_device_tag) {
        cl_device_id device = (cl_device_id) SCM_SMOB_DATA(smob);
        SCM key = scm_from_uint32(CL_DEVICE_NAME);
        scm_puts("#<", port);
        scm_display(scm_get_cl_device_info(smob, key), port);
        scm_puts(">", port);
        return 1;
    }
    if(tag == cl_context_tag) {
        cl_context context = (cl_context) SCM_SMOB_DATA(smob);
        scm_puts("#<cl-context>", port); // TODO
        return 1;
    }
    if(tag == cl_command_queue_tag) {
        cl_command_queue command_queue = (cl_command_queue) SCM_SMOB_DATA(smob);
        scm_puts("#<cl-command-queue>", port); // TODO
        return 1;
    }
    if(tag == cl_mem_tag) {
        cl_mem mem = (cl_mem) SCM_SMOB_DATA(smob);
        scm_puts("#<cl-mem>", port); // TODO
        return 1;
    }
    if(tag == cl_program_tag) {
        cl_program program = (cl_program) SCM_SMOB_DATA(smob);
        scm_puts("#<cl-program>", port); // TODO
        return 1;
    }
    if(tag == cl_kernel_tag) {
        cl_kernel kernel = (cl_kernel) SCM_SMOB_DATA(smob);
        scm_puts("#<cl-kernel>", port); // TODO
        return 1;
    }
    if(tag == cl_event_tag) {
        cl_event event = (cl_event) SCM_SMOB_DATA(smob);
        scm_puts("#<cl-event>", port); // TODO
        return 1;
    }
    if(tag == cl_sampler_tag) {
        cl_sampler sampler = (cl_sampler) SCM_SMOB_DATA(smob);
        scm_puts("#<cl-sampler>", port); // TODO
        return 1;
    }
    scm_misc_error(__func__, "smob of invalid type", SCM_EOL);
    return 1;
}
