/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "context.h"
#include "conversion.h"
#include "error.h"
#include "info.h"

SCM_DEFINE (scm_make_cl_context, "make-cl-context", 0, 0, 1,
            (SCM devices),
            "Create a new OpenCL context containing all OpenCL devices\n"
            "given in @var{devices}")
#define FUNC_NAME s_scm_make_cl_context
{
    size_t len;
    SCM_VALIDATE_NONEMPTYLIST_COPYLEN(SCM_ARG1, devices, len);
    cl_device_id d[len];

    SCM    rest = devices;
    size_t index = 0;
    while (!scm_is_null(rest)) {
        if(index >= len)
            scm_misc_error(FUNC_NAME,
                           "argument device list was concurrently modified",
                           scm_list_1(devices));
        SCM e    = SCM_CAR(rest);
        rest = SCM_CDR(rest);
        d[index] = scm_to_cl_device_id_here(e);
        ++index;
    }
    if(index != len)
        scm_misc_error(FUNC_NAME,
                       "argument device list was concurrently modified",
                       scm_list_1(devices));

    /* A platform is needed to create a context. I simply take the platform of
       the first device and rely on the OpenCL implementation to detect if
       some devices are from different platforms than the first. */
    SCM scm_platform = scm_cl_device_info(SCM_CAR(rest),
                                          scm_from_cl_uint(CL_DEVICE_PLATFORM));
    cl_platform_id platform = scm_to_cl_platform_id_here(scm_platform);
    cl_context_properties cps[3];
    cps[0] = (cl_context_properties)CL_CONTEXT_PLATFORM;
    cps[1] = (cl_context_properties)platform;
    cps[2] = 0;

    cl_int err;
    cl_context context = clCreateContext(cps, len, d,
                                         NULL, NULL, &err); // TODO callback
    CL_CHECK( err );
    return scm_from_cl_context(context);
}
#undef FUNC_NAME

void guile_opencl_init_context() {
#ifndef SCM_MAGIC_SNARFER
#include "context.x"
#endif
}
