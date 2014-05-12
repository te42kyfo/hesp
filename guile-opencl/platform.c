/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "platform.h"
#include "error.h"
#include "conversion.h"

SCM_DEFINE (scm_get_cl_platforms, "cl-get-platforms", 0, 0, 0,
            (),
            "Return a list of all avilable OpenCL platforms.") {
    cl_uint         num_platforms;
    CL_CHECK( clGetPlatformIDs(0, NULL, &num_platforms) );
    if(0 == num_platforms) return SCM_EOL;

    cl_platform_id platforms[num_platforms];
    if(NULL == platforms) perror( __func__ );
    CL_CHECK( clGetPlatformIDs(num_platforms, platforms, NULL) );

    SCM  ret = SCM_EOL;
    for(cl_uint ui = 0; ui < num_platforms; ++ui) {
        SCM smob = scm_from_cl_platform_id(platforms[ui]);
        ret = scm_cons(smob, ret);
    }
    return ret;
}


void guile_opencl_init_platform() {
#ifndef SCM_MAGIC_SNARFER
#include "platform.x"
#endif
}

