#include "guile_opencl.h"

SCM scm_get_platform_info(SCM scm_platform, SCM scm_param_name) {
    cl_platform_id   platform   = scm_to_cl_platform_id_here(scm_platform);
    cl_platform_info param_name = scm_to_uint32(scm_param_name);
    size_t buf_len;
    CL_CHECK( clGetPlatformInfo(platform, param_name, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetPlatformInfo(platform, param_name, buf_len, buffer, NULL) );
    return scm_from_locale_stringn(buffer, buf_len - 1);
}

SCM scm_get_device_info(SCM scm_device, SCM scm_param_name) {
    cl_device_id   device     = scm_to_cl_device_id_here(scm_device);
    cl_device_info param_name = scm_to_uint32(scm_param_name);
    size_t buf_len;
    CL_CHECK( clGetDeviceInfo(device, param_name, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetDeviceInfo(device, param_name, buf_len, buffer, NULL) );
    switch(param_name) {
    case CL_DEVICE_TYPE: {
        cl_device_type type = *(cl_device_type*)buffer;
        switch(type) {
        case CL_DEVICE_TYPE_CPU:
            return scm_from_locale_string("CL_DEVICE_TYPE_CPU");
        case CL_DEVICE_TYPE_GPU:
            return scm_from_locale_string("CL_DEVICE_TYPE_GPU");
        case CL_DEVICE_TYPE_ACCELERATOR:
            return scm_from_locale_string("CL_DEVICE_TYPE_ACCELERATOR");
        case CL_DEVICE_TYPE_DEFAULT:
            return scm_from_locale_string("CL_DEVICE_TYPE_DEFAULT");
        case CL_DEVICE_TYPE_ALL:
            return scm_from_locale_string("CL_DEVICE_TYPE_ALL");
        }
        scm_misc_error(__func__, "invalid cl_device_type", SCM_EOL);
    }
    case CL_DEVICE_VENDOR_ID:
    case CL_DEVICE_MAX_COMPUTE_UNITS:
    case CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:
    case CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR:
    case CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT:
    case CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT:
    case CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG:
    case CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT:
    case CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE:
    case CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF:
    case CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR:
    case CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT:
    case CL_DEVICE_NATIVE_VECTOR_WIDTH_INT:
    case CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG:
    case CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT:
    case CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE:
    case CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF:
    case CL_DEVICE_MAX_CLOCK_FREQUENCY:
    case CL_DEVICE_ADDRESS_BITS:
    case CL_DEVICE_MAX_READ_IMAGE_ARGS:
    case CL_DEVICE_MAX_WRITE_IMAGE_ARGS:
    case CL_DEVICE_MAX_SAMPLERS:
    case CL_DEVICE_MEM_BASE_ADDR_ALIGN:
    case CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE:
    case CL_DEVICE_MAX_CONSTANT_ARGS: {
        cl_uint value = *(cl_uint*)buffer;
        return scm_from_uint64((uint64_t) value);
    }
    case CL_DEVICE_MAX_MEM_ALLOC_SIZE:
    case CL_DEVICE_GLOBAL_MEM_CACHE_SIZE:
    case CL_DEVICE_GLOBAL_MEM_SIZE:
    case CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:
    case CL_DEVICE_LOCAL_MEM_SIZE: {
        cl_ulong value = *(cl_ulong*)buffer;
        return scm_from_uint64((uint64_t) value);
    }
    case CL_DEVICE_MAX_WORK_GROUP_SIZE:
    case CL_DEVICE_IMAGE2D_MAX_WIDTH:
    case CL_DEVICE_IMAGE2D_MAX_HEIGHT:
    case CL_DEVICE_IMAGE3D_MAX_WIDTH:
    case CL_DEVICE_IMAGE3D_MAX_HEIGHT:
    case CL_DEVICE_IMAGE3D_MAX_DEPTH:
    case CL_DEVICE_MAX_PARAMETER_SIZE:
    case CL_DEVICE_PROFILING_TIMER_RESOLUTION: {
        size_t value = *(size_t*)buffer;
        return scm_from_uint64((uint64_t) value);
    }
    case CL_DEVICE_MAX_WORK_ITEM_SIZES: {
        SCM dim_key   = scm_from_uint32(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
        SCM dim       = scm_get_device_info(scm_device, dim_key);
        cl_uint imax  = scm_to_uint(dim);
        SCM ret       = SCM_EOL;
        size_t *sizes = (size_t*)buffer;
        for(int i = 0; i < imax; ++i) {
            ret = scm_cons(scm_from_size_t(sizes[i]), ret);
        }
        return scm_reverse(ret);
    }
    case CL_DEVICE_SINGLE_FP_CONFIG:{
        cl_device_fp_config value = *(cl_device_fp_config*)buffer;
        SCM ret = SCM_EOL;
#define ACONS_FP_CONFIG(param_name, value, list)        \
        scm_acons(scm_from_locale_string(#param_name),  \
                  scm_from_bool(value && param_name),   \
                  list);
        ret = ACONS_FP_CONFIG(CL_FP_DENORM, value, ret);
        ret = ACONS_FP_CONFIG(CL_FP_INF_NAN, value, ret);
        ret = ACONS_FP_CONFIG(CL_FP_ROUND_TO_NEAREST, value, ret);
        ret = ACONS_FP_CONFIG(CL_FP_ROUND_TO_ZERO, value, ret);
        ret = ACONS_FP_CONFIG(CL_FP_ROUND_TO_INF, value, ret);
        ret = ACONS_FP_CONFIG(CL_FP_FMA, value, ret);
        ret = ACONS_FP_CONFIG(CL_FP_SOFT_FLOAT, value, ret);
#undef ACONS_FP_CONFIG
        return ret;
    }
    case CL_DEVICE_GLOBAL_MEM_CACHE_TYPE: {
        cl_device_mem_cache_type value = *(cl_device_mem_cache_type*)buffer;
        if(CL_NONE == value)
            return scm_from_locale_string("CL_NONE");
        if(CL_READ_ONLY_CACHE == value)
            return scm_from_locale_string("CL_READ_ONLY_CACHE");
        if(CL_READ_WRITE_CACHE == value)
            return scm_from_locale_string("CL_READ_WRITE_CACHE");
        scm_misc_error(__func__, "inalid cl_device_mem_cache_type", SCM_EOL);
    }
    case CL_DEVICE_LOCAL_MEM_TYPE: {
        cl_device_local_mem_type value = *(cl_device_local_mem_type*)buffer;
        if(CL_NONE == value) return scm_from_locale_string("CL_NONE");
        if(CL_LOCAL == value) return scm_from_locale_string("CL_LOCAL");
        if(CL_GLOBAL == value) return scm_from_locale_string("CL_GLOBAL");
        scm_misc_error(__func__, "inalid cl_device_local_mem_type", SCM_EOL);
    }
    case CL_DEVICE_EXECUTION_CAPABILITIES: {
        cl_device_exec_capabilities value = *(cl_device_exec_capabilities*)buffer;
        SCM ret = SCM_EOL;
#define ACONS_EXEC_CAPABILITIES(param_name, value, list)    \
        scm_acons(scm_from_locale_string(#param_name),      \
                  scm_from_bool(value && param_name),       \
                  list);
        ret = ACONS_EXEC_CAPABILITIES(CL_EXEC_KERNEL, value, ret);
        ret = ACONS_EXEC_CAPABILITIES(CL_EXEC_NATIVE_KERNEL, value, ret);
#undef ACONS_EXEC_CAPABILITIES
        return ret;
    }
    case CL_DEVICE_NAME:
    case CL_DEVICE_VENDOR:
    case CL_DRIVER_VERSION:
    case CL_DEVICE_PROFILE:
    case CL_DEVICE_VERSION:
    case CL_DEVICE_OPENCL_C_VERSION:
    case CL_DEVICE_EXTENSIONS: {
        return scm_from_locale_stringn(buffer, buf_len - 1);
    }
    case CL_DEVICE_PLATFORM: {
        return scm_from_cl_platform_id( *(cl_platform_id*)buffer );
    }
    case CL_DEVICE_IMAGE_SUPPORT:
    case CL_DEVICE_ERROR_CORRECTION_SUPPORT:
    case CL_DEVICE_ENDIAN_LITTLE:
    case CL_DEVICE_AVAILABLE:
    case CL_DEVICE_COMPILER_AVAILABLE: {
        cl_bool value = *(cl_bool*)buffer;
        if(value) return SCM_BOOL_T;
        else      return SCM_BOOL_F;
    }
    }
    scm_misc_error(__func__, "invalid param_name or not implemented", SCM_EOL);
}
SCM scm_get_context_info(SCM scm_context, SCM scm_param_name) {
    cl_context      context    = scm_to_cl_context_here(scm_context);
    cl_context_info param_name = scm_to_uint32(scm_param_name);

    size_t buf_len;
    CL_CHECK( clGetContextInfo(context, param_name, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetContextInfo(context, param_name, buf_len, buffer, NULL) );
    switch(param_name) {
    case CL_CONTEXT_NUM_DEVICES: {
        cl_uint value = *(cl_uint*)buffer;
        return scm_from_uint64((uint64_t) value);
    }
    case CL_CONTEXT_DEVICES: {
        SCM ret = SCM_EOL;
        cl_device_id *devices = (cl_device_id*)buffer;
        SCM num_devices_key = scm_from_uint32(CL_CONTEXT_NUM_DEVICES);
        SCM scm_num_devices = scm_get_context_info(scm_context, num_devices_key);
        cl_uint num_devices = scm_to_uint32(scm_num_devices);
        for(cl_uint ui = 0; ui < num_devices; ++ui) {
            SCM smob = scm_new_smob(guile_opencl_tag, (scm_t_bits)devices[ui]);
            SCM_SET_SMOB_FLAGS(smob, cl_device_tag);
            ret = scm_cons(smob, ret);
        }
        return ret;
    }
    }
    scm_misc_error(__func__, "invalid param_name or not implemented", SCM_EOL);
}

SCM scm_get_command_queue_info(SCM scm_command_queue, SCM scm_param_name) {
    cl_command_queue      queue      = scm_to_cl_command_queue_here(scm_command_queue);
    cl_command_queue_info param_name = scm_to_uint32(scm_param_name);

    size_t buf_len;
    CL_CHECK( clGetCommandQueueInfo(queue, param_name, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetCommandQueueInfo(queue, param_name, buf_len, buffer, NULL) );

    switch(param_name) {
    case CL_QUEUE_CONTEXT: {
        return scm_from_cl_context( *(cl_context*)buffer );
    }
    case CL_QUEUE_DEVICE: {
        return scm_from_cl_device_id( *(cl_device_id*)buffer );
    }
    case CL_QUEUE_PROPERTIES: {
        // TODO
        return SCM_EOL;
    }
    }
    scm_misc_error(__func__, "invalid param_name or not implemented", SCM_EOL);
}

SCM scm_get_mem_info(SCM scm_memobj, SCM scm_param_name) {
    cl_mem      memobj     = scm_to_cl_mem_here(scm_memobj);
    cl_mem_info param_name = scm_to_uint32(scm_param_name);
    // TODO
    return SCM_EOL;
}

SCM scm_get_program_info(SCM scm_program, SCM scm_param_name) {
    cl_program      program    = scm_to_cl_program_here(scm_program);
    cl_program_info param_name = scm_to_uint32(scm_param_name);
    // TODO
    return SCM_EOL;
}

SCM scm_get_kernel_info(SCM scm_kernel,
                        SCM scm_param_name) {
    // TODO
    return SCM_EOL;
}

SCM scm_get_sampler_info(SCM scm_sampler, SCM scm_param_name) {
    cl_sampler      sampler    = scm_to_cl_sampler_here(scm_sampler);
    cl_sampler_info param_name = scm_to_uint32(scm_param_name);
    // TODO
    return SCM_EOL;
}

SCM scm_get_program_build_info(SCM scm_program,
                               SCM scm_device,
                               SCM scm_param_name) {
    cl_program      program    = scm_to_cl_program_here(scm_program);
    cl_device_id       device     = scm_to_cl_device_id_here(scm_device);
    cl_sampler_info param_name = scm_to_uint32(scm_param_name);
    // TODO
    return SCM_EOL;
}

SCM scm_get_work_group_info(SCM scm_kernel,
                            SCM scm_device,
                            SCM scm_param_name) {
    // TODO
    return SCM_EOL;
}

SCM scm_get_event_profiling_info(SCM scm_event,
                                 SCM scm_param_name) {
    // TODO
    return SCM_EOL;
}



/* Given any OpenCL Smob, returns an alist of object attributes. */
SCM scm_get_info(SCM smob, SCM param_name) {
    scm_assert_smob_type(guile_opencl_tag, smob);
    scm_t_bits tag = SCM_SMOB_FLAGS(smob);
    SCM ret = SCM_EOL;
    SCM key; // the string under which to store the information
    SCM val; // the CL constant to retrieve the information
    if(tag == cl_platform_tag) {
        cl_platform_id platform = (cl_platform_id) SCM_SMOB_DATA(smob);
#define ACONS_PLATFORM_INFO(platform, param_name, list)                 \
        key = scm_from_locale_string( #param_name );                    \
        val = scm_from_uint32( param_name );                            \
        ret = scm_acons(key, scm_get_platform_info(smob, val), list);

        ACONS_PLATFORM_INFO(platform, CL_PLATFORM_EXTENSIONS, ret);
        ACONS_PLATFORM_INFO(platform, CL_PLATFORM_VENDOR, ret);
        ACONS_PLATFORM_INFO(platform, CL_PLATFORM_NAME, ret);
        ACONS_PLATFORM_INFO(platform, CL_PLATFORM_VERSION, ret);
#undef ACONS_PLATFORM_INFO
        return ret;
    }
    if(tag == cl_device_tag) {
        cl_device_id device = (cl_device_id) SCM_SMOB_DATA(smob);
#define ACONS_DEVICE_INFO(device, param_name, list)                     \
        key = scm_from_locale_string( #param_name );                    \
        val = scm_from_uint32( param_name );                            \
        ret = scm_acons(key, scm_get_device_info(smob, val), list);

        ACONS_DEVICE_INFO(device, CL_DRIVER_VERSION, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_VERSION, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_VENDOR, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_NAME, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_MAX_PARAMETER_SIZE, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_MAX_CONSTANT_ARGS, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_MAX_COMPUTE_UNITS, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_LOCAL_MEM_SIZE, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_GLOBAL_MEM_SIZE, ret);
        ACONS_DEVICE_INFO(device, CL_DEVICE_EXTENSIONS, ret);
        return ret;
    }
    if(tag == cl_context_tag) {
        cl_context context = (cl_context) SCM_SMOB_DATA(smob);
        // TODO
        return SCM_EOL;
    }
    if(tag == cl_command_queue_tag) {
        cl_command_queue command_queue = (cl_command_queue) SCM_SMOB_DATA(smob);
        // TODO
        return SCM_EOL;
    }
    if(tag == cl_mem_tag) {
        cl_mem mem = (cl_mem) SCM_SMOB_DATA(smob);
        // TODO
        return SCM_EOL;
    }
    if(tag == cl_program_tag) {
        cl_program program = (cl_program) SCM_SMOB_DATA(smob);
        // TODO
        return SCM_EOL;
    }
    if(tag == cl_kernel_tag) {
        cl_kernel kernel = (cl_kernel) SCM_SMOB_DATA(smob);
        // TODO
        return SCM_EOL;
    }
    if(tag == cl_event_tag) {
        cl_event event = (cl_event) SCM_SMOB_DATA(smob);
        // TODO
        return SCM_EOL;
    }
    if(tag == cl_sampler_tag) {
        cl_sampler sampler = (cl_sampler) SCM_SMOB_DATA(smob);
        // TODO
        return SCM_EOL;
    }
    scm_misc_error(__func__, "smob of invalid type", SCM_EOL);
    return SCM_EOL;
}
