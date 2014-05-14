/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "device.h"
#include "conversion.h"
#include "error.h"
#include "constants.h"

SCM_DEFINE (scm_get_cl_devices, "get-cl-devices", 1, 1, 0,
            (SCM platform, SCM flags),
            "Return a list of all avilable OpenCL devices that are\n"
            "avilable on platform \var{platform}.") {
    cl_platform_id p = scm_to_cl_platform_id_here(platform);
    cl_uint        n;
    cl_device_type t;
    if(SCM_UNBNDP(flags)) {
        t = CL_DEVICE_TYPE_ALL;
    } else {
        t = scm_to_cl_ulong(flags);
    }
    CL_CHECK( clGetDeviceIDs(p, t, 0, NULL, &n) );
    if(0 == n) return SCM_EOL;

    cl_device_id devices[n];
    if(NULL == devices) perror( __func__ );
    CL_CHECK( clGetDeviceIDs(p, t, n, devices, NULL) );

    SCM  ret = SCM_EOL;
    for(cl_uint ui = 0; ui < n; ++ui) {
        ret = scm_cons(scm_from_cl_device_id(devices[ui]), ret);
    }
    return ret;
}

void guile_opencl_init_device() {
#ifndef SCM_MAGIC_SNARFER
#include "device.x"
#endif
}
