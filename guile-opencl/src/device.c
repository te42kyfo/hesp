/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "device.h"
#include "conversion.h"
#include "error.h"
#include "constants.h"

SCM_DEFINE (scm_get_cl_devices, "get-cl-devices", 1, 1, 0,
            (SCM platform, SCM flags),
            "Return a list of all avilable OpenCL devices that are\n"
            "avilable on platform \var{platform}.")
#define FUNC_NAME s_scm_get_cl_devices
{
    cl_platform_id platform_id = scm_to_cl_platform_id_here(platform);
    cl_uint        num_devices;
    cl_device_type device_type;
    if(SCM_UNBNDP(flags)) {
        device_type = CL_DEVICE_TYPE_ALL;
    } else {
        device_type = scm_to_cl_ulong(flags);
    }
    CL_CHECK( clGetDeviceIDs(platform_id,
                             device_type, 0, NULL, &num_devices) );
    if(0 == num_devices) return SCM_EOL;

    cl_device_id devices[num_devices];
    CL_CHECK( clGetDeviceIDs(platform_id,
                             device_type, num_devices, devices, NULL) );

    SCM  ret = SCM_EOL;
    for(cl_uint index = 0; index < num_devices; ++index) {
        ret = scm_cons(scm_from_cl_device_id(devices[index]), ret);
    }
    return ret;
}
#undef FUNC_NAME

void guile_opencl_init_device() {
#ifndef SCM_MAGIC_SNARFER
#include "device.x"
#endif
}
