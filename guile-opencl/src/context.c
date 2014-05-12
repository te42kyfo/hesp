/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "context.h"
#include "conversion.h"
#include "error.h"
#include "info.h"

SCM_DEFINE (scm_make_cl_context, "cl-make-context", 0, 0, 1,
            (SCM devices),
            "Create a new OpenCL context containing all OpenCL devices\n"
            "given in @var{devices}") {
    // assert we get a list of devices that is not empty
    if(!scm_to_bool(scm_list_p(devices)))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, devices, "list");
    if(scm_is_null(devices))
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, devices, "non-empty list");

    size_t len = scm_to_size_t(scm_length(devices));
    cl_device_id d[len];

    SCM rest = devices;
    for(size_t i = 0; i < len; ++i) {
        SCM e    = SCM_CAR(rest);
        rest = SCM_CDR(rest);
        d[i] = scm_to_cl_device_id_here(e);
    }

    /* A platform is needed to create a context. I simply take the platform of
       the first device and rely on the OpenCL implementation to detect if
       some devices are from different platforms that the first. */
    SCM scm_platform = scm_get_cl_device_info(SCM_CAR(devices),
                                              scm_from_cl_uint(CL_DEVICE_PLATFORM));
    cl_platform_id platform = scm_to_cl_platform_id_here(scm_platform);
    cl_context_properties cps[3];
    cps[0] = (cl_context_properties)CL_CONTEXT_PLATFORM;
    cps[1] = (cl_context_properties)platform;
    cps[2] = 0;

    cl_int err;
    cl_context context = clCreateContext(cps, len, d,
                                         cl_callback, (void *)(__func__), &err);
    CL_CHECK( err );
    return scm_from_cl_context(context);
}

void guile_opencl_init_context() {
#ifndef SCM_MAGIC_SNARFER
#include "context.x"
#endif
}
