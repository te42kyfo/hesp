#include "guile_opencl.h"

/* return a list of platform Smobs. */
SCM scm_get_platforms() {
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

/* Return a list of device Smobs */
SCM scm_get_devices(SCM smob) {
    scm_assert_smob_type(guile_opencl_tag, smob);
    scm_t_bits tag = SCM_SMOB_FLAGS(smob);
    if(tag != cl_platform_tag) {
        scm_wrong_type_arg_msg(__func__, 1, smob, "cl_platform");
    }
    // TODO allow filtering for cl_device_type
    cl_platform_id platform = (cl_platform_id)SCM_SMOB_DATA(smob);
    cl_uint         num_devices;
    CL_CHECK( clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
                        0, NULL, &num_devices) );
    if(0 == num_devices) return SCM_EOL;

    cl_device_id devices[num_devices];
    if(NULL == devices) perror( __func__ );
    CL_CHECK( clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
                        num_devices, devices, NULL) );

    SCM  ret = SCM_EOL;
    for(cl_uint ui = 0; ui < num_devices; ++ui) {
        SCM smob = scm_new_smob(guile_opencl_tag, (scm_t_bits)devices[ui]);
        SCM_SET_SMOB_FLAGS(smob, cl_device_tag);
        ret = scm_cons(smob, ret);
    }
    return ret;
}

/* Return a context Smob */
SCM scm_make_context(SCM device_list) {
    // TODO
    return SCM_EOL;
}

SCM scm_make_queue(SCM context, SCM device) {
    // TODO
    return SCM_EOL;
}

