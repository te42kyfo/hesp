#include <stdbool.h>
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
    // assert we get a list of devices that is not empty
    if(!scm_to_bool(scm_list_p(device_list)))
        scm_wrong_type_arg_msg(__func__, 1, device_list, "list");
    if(scm_is_null(device_list))
        scm_wrong_type_arg_msg(__func__, 1, device_list, "non-empty list");

    size_t len = scm_to_size_t(scm_length(device_list));
    cl_device_id devices[len];

    SCM rest = device_list;
    for(size_t i = 0; i < len; ++i) {
        SCM e    = SCM_CAR(device_list);
        SCM rest = SCM_CDR(device_list);
        devices[i] = scm_to_cl_device_id_here(e);
    }

    /* A platform is needed to create a context. I simply take the platform of
       the first device and rely on the OpenCL implementation to detect if
       some devices are from different platforms that the first. */
    SCM scm_platform = scm_get_device_info(SCM_CAR(device_list),
                                           scm_from_uint32(CL_DEVICE_PLATFORM));
    cl_platform_id platform = scm_to_cl_platform_id_here(scm_platform);
    cl_context_properties cps[3];
    cps[0] = (cl_context_properties)CL_CONTEXT_PLATFORM;
    cps[1] = (cl_context_properties)platform;
    cps[2] = 0;

    cl_int err;
    cl_context context = clCreateContext(cps, len, devices,
                                         cl_callback, (void *)(__func__), &err);
    CL_CHECK( err );
    return scm_from_cl_context(context);
}

SCM scm_make_queue(SCM scm_context, SCM scm_device, SCM flags) {
    cl_context                  context = scm_to_cl_context_here(scm_context);
    cl_device_id                device  = scm_to_cl_device_id_here(scm_device);
    cl_command_queue_properties cqp     = 0;
    if(!SCM_UNBNDP(flags)) cqp = scm_to_uint32(flags);
    cl_int err;
    cl_command_queue queue = clCreateCommandQueue(context, device, cqp, &err);
    CL_CHECK( err );
    return scm_from_cl_command_queue(queue);
}

SCM scm_make_buffer(SCM scm_context, SCM scm_flags,
                    SCM scm_size, SCM scm_host_ptr) {
    cl_context   context   = scm_to_cl_context_here(scm_context);
    cl_mem_flags flags     = (cl_mem_flags)scm_to_uint32(scm_flags);
    size_t       size      = scm_to_size_t(scm_size);
    void        *host_ptr  = scm_to_pointer(scm_host_ptr);

    cl_int err;
    cl_mem buffer = clCreateBuffer(context, flags, size, host_ptr, &err);
    CL_CHECK( err );
    return scm_from_cl_mem(buffer);
}

SCM scm_read_buffer(SCM scm_command_queue,
                    SCM scm_buffer,
                    SCM scm_blocking_read,
                    SCM scm_offset,
                    SCM scm_size,
                    SCM scm_ptr,
                    SCM scm_event_wait_list) {
    cl_command_queue queue         = scm_to_cl_command_queue_here(scm_command_queue);
    cl_mem           buffer        = scm_to_cl_mem_here(scm_buffer);
    cl_bool          blocking_read = (cl_bool)scm_to_bool(scm_blocking_read);
    size_t           offset        = scm_to_size_t(scm_offset);
    size_t           size          = scm_to_size_t(scm_size);
    void            *ptr           = scm_to_pointer(scm_ptr);
    // TODO event wait list

    cl_event event;
    CL_CHECK( clEnqueueReadBuffer(queue, buffer, blocking_read,
                                  offset, size, ptr,
                                  0, NULL, &event) );
    return scm_from_cl_event(event);
}


SCM scm_write_buffer(SCM scm_command_queue,
                     SCM scm_buffer,
                     SCM scm_blocking_write,
                     SCM scm_offset,
                     SCM scm_size,
                     SCM scm_ptr,
                     SCM scm_event_wait_list) {
    cl_command_queue queue          = scm_to_cl_command_queue_here(scm_command_queue);
    cl_mem           buffer         = scm_to_cl_mem_here(scm_buffer);
    cl_bool          blocking_write = (cl_bool)scm_to_bool(scm_blocking_write);
    size_t           offset         = scm_to_size_t(scm_offset);
    size_t           size           = scm_to_size_t(scm_size);
    void      *ptr                  = scm_to_pointer(scm_ptr);
    // TODO event wait list
    cl_event event;
    CL_CHECK( clEnqueueWriteBuffer(queue, buffer, blocking_write,
                                   offset, size, ptr,
                                   0, NULL, &event) );
    return scm_from_cl_event(event);
}

SCM scm_map_buffer(SCM scm_command_queue, SCM scm_buffer,
                   SCM scm_blocking_map,
                   SCM scm_flags,
                   SCM scm_offset, SCM scm_size,
                   SCM scm_event_wait_list) {
    cl_command_queue queue          = scm_to_cl_command_queue_here(scm_command_queue);
    cl_mem           buffer         = scm_to_cl_mem_here(scm_buffer);
    cl_bool          blocking_map   = (cl_bool)scm_to_bool(scm_blocking_map);
    cl_map_flags     flags          = scm_to_uint32(scm_flags);
    size_t           offset         = scm_to_size_t(scm_offset);
    size_t           size           = scm_to_size_t(scm_size);
    // TODO event wait list
    cl_event event;
    cl_int   err;
    void *ptr = clEnqueueMapBuffer(queue, buffer, blocking_map,
                                     flags, offset, size,
                                     0, NULL, &event, &err);
    CL_CHECK( err );
    SCM scm_ptr   = scm_from_pointer(ptr, NULL);
    SCM bv        = scm_pointer_to_bytevector(scm_ptr, scm_size,
                                              SCM_UNDEFINED, SCM_UNDEFINED);
    SCM scm_event = scm_from_cl_event(event);
    return scm_values(scm_list_2(bv, scm_event));
}

SCM scm_copy_buffer(SCM scm_command_queue,
                    SCM scm_src_buffer, SCM scm_dst_buffer,
                    SCM scm_src_offset, SCM scm_dst_offset, SCM scm_size,
                    SCM scm_event_wait_list) {
    cl_command_queue queue          = scm_to_cl_command_queue_here(scm_command_queue);
    cl_mem           src            = scm_to_cl_mem_here(scm_src_buffer);
    cl_mem           dst            = scm_to_cl_mem_here(scm_dst_buffer);
    size_t           src_offset     = scm_to_size_t(scm_src_offset);
    size_t           dst_offset     = scm_to_size_t(scm_dst_offset);
    size_t           size           = scm_to_size_t(scm_size);

    cl_event event;
    // TODO event wait list
    CL_CHECK( clEnqueueCopyBuffer(queue, src, dst,
                                  src_offset, dst_offset, size,
                                  0, NULL, &event) );

}

SCM scm_unmap_mem(SCM scm_command_queue, SCM scm_mem,
                  SCM scm_ptr,
                  SCM scm_event_wait_list) {
    cl_command_queue queue          = scm_to_cl_command_queue_here(scm_command_queue);
    cl_mem           mem            = scm_to_cl_mem_here(scm_mem);
    void            *ptr            = scm_to_pointer(scm_ptr);
    // TODO event wait list
    cl_event event;
    CL_CHECK( clEnqueueUnmapMemObject(queue, mem, ptr, 0, NULL, &event) );
    return scm_from_cl_event(event);
}
