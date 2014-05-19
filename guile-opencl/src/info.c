/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "info.h"
#include "constants.h"
#include "conversion.h"
#include "error.h"

SCM_DEFINE (scm_cl_platform_info, "cl-platform-info", 2, 0, 0,
            (SCM platform, SCM param_name),
            "Return information of the OpenCl platform @var{platform}\n"
            "specified by the OpenCL constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_platform_info
{
    cl_platform_id   platform_id   = scm_to_cl_platform_id_here(platform);
    cl_platform_info par           = scm_to_cl_uint(param_name);
    size_t buf_len;
    CL_CHECK( clGetPlatformInfo(platform_id, par, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetPlatformInfo(platform_id, par, buf_len, buffer, NULL) );
    return scm_from_utf8_stringn(buffer, buf_len - 1);
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_device_info, "cl-device-info", 2, 0, 0,
            (SCM device, SCM param_name),
            "Return information of the OpenCl device @var{device}\n"
            "specified by the OpenCL constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_device_info
{
    cl_device_id   dev = scm_to_cl_device_id_here(device);
    cl_device_info par = scm_to_cl_uint(param_name);
    size_t buf_len;
    CL_CHECK( clGetDeviceInfo(dev, par, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetDeviceInfo(dev, par, buf_len, buffer, NULL) );
    switch(par) {
    case CL_DEVICE_TYPE: {
        cl_device_type type = *(cl_device_type*)buffer;
        switch(type) {
        case CL_DEVICE_TYPE_CPU:
            return scm_from_utf8_string("CL_DEVICE_TYPE_CPU");
        case CL_DEVICE_TYPE_GPU:
            return scm_from_utf8_string("CL_DEVICE_TYPE_GPU");
        case CL_DEVICE_TYPE_ACCELERATOR:
            return scm_from_utf8_string("CL_DEVICE_TYPE_ACCELERATOR");
        case CL_DEVICE_TYPE_DEFAULT:
            return scm_from_utf8_string("CL_DEVICE_TYPE_DEFAULT");
        case CL_DEVICE_TYPE_ALL:
            return scm_from_utf8_string("CL_DEVICE_TYPE_ALL");
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
        return scm_from_cl_uint((uint64_t) value);
    }
    case CL_DEVICE_MAX_MEM_ALLOC_SIZE:
    case CL_DEVICE_GLOBAL_MEM_CACHE_SIZE:
    case CL_DEVICE_GLOBAL_MEM_SIZE:
    case CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:
    case CL_DEVICE_LOCAL_MEM_SIZE: {
        cl_ulong value = *(cl_ulong*)buffer;
        return scm_from_cl_ulong((uint64_t) value);
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
        return scm_from_size_t((uint64_t) value);
    }
    case CL_DEVICE_MAX_WORK_ITEM_SIZES: {
        SCM dim_key   = scm_from_cl_uint(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
        SCM dim       = scm_cl_device_info(device, dim_key);
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
        scm_acons(scm_from_utf8_string(#param_name),  \
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
            return scm_from_utf8_string("CL_NONE");
        if(CL_READ_ONLY_CACHE == value)
            return scm_from_utf8_string("CL_READ_ONLY_CACHE");
        if(CL_READ_WRITE_CACHE == value)
            return scm_from_utf8_string("CL_READ_WRITE_CACHE");
        scm_misc_error(__func__, "inalid cl_device_mem_cache_type", SCM_EOL);
    }
    case CL_DEVICE_LOCAL_MEM_TYPE: {
        cl_device_local_mem_type value = *(cl_device_local_mem_type*)buffer;
        if(CL_NONE == value) return scm_from_utf8_string("CL_NONE");
        if(CL_LOCAL == value) return scm_from_utf8_string("CL_LOCAL");
        if(CL_GLOBAL == value) return scm_from_utf8_string("CL_GLOBAL");
        scm_misc_error(__func__, "inalid cl_device_local_mem_type", SCM_EOL);
    }
    case CL_DEVICE_EXECUTION_CAPABILITIES: {
        cl_device_exec_capabilities value = *(cl_device_exec_capabilities*)buffer;
        SCM ret = SCM_EOL;
#define ACONS_EXEC_CAPABILITIES(param_name, value, list)    \
        scm_acons(scm_from_utf8_string(#param_name),      \
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
        return scm_from_utf8_stringn(buffer, buf_len - 1);
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
    scm_wrong_type_arg(FUNC_NAME, SCM_ARG2, param_name);
    scm_wrong_type_arg(FUNC_NAME, SCM_ARG2, param_name);
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_buffer_info, "cl-buffer-info", 2, 0, 0,
            (SCM buffer, SCM param_name),
            "Return information of the OpenCl buffer object @var{buffer}\n"
            "specified by the OpenCL constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_buffer_info
{
    cl_mem      buf = scm_to_cl_buffer_here(buffer);
    cl_mem_info par = scm_to_cl_uint(param_name);

    size_t buf_len;
    CL_CHECK( clGetMemObjectInfo(buf, par, 0, NULL, &buf_len) );
    char data[buf_len];
    CL_CHECK( clGetMemObjectInfo(buf, par, buf_len, data, NULL) );

    switch(par) {
    case CL_MEM_SIZE: {
        size_t size = *(size_t*)data;
        return scm_from_size_t(size);
    }
        // TODO other cases
    }
    scm_wrong_type_arg(FUNC_NAME, SCM_ARG2, param_name);
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_image_info, "cl-image-info", 2, 0, 0,
            (SCM image, SCM param_name),
            "Return information of the OpenCl image object @var{image}\n"
            "specified by the OpenCL constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_image_info
{
    cl_mem      img = scm_to_cl_image_here(image);
    cl_mem_info par = scm_to_cl_uint(param_name);

    size_t buf_len;
    CL_CHECK( clGetMemObjectInfo(img, par, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetMemObjectInfo(img, par, buf_len, buffer, NULL) );

    switch(par) {
    case CL_MEM_SIZE: {
        size_t size = *(size_t*)buffer;
        return scm_from_size_t(size);
    }
        // TODO other cases
    }
    scm_wrong_type_arg(FUNC_NAME, SCM_ARG2, param_name);
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_context_info, "cl-context-info", 2, 0, 0,
            (SCM context, SCM param_name),
            "Return information of the OpenCl context @var{context}"
            "specified by the OpenCL constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_context_info
{
    cl_context      cnt = scm_to_cl_context_here(context);
    cl_context_info par = scm_to_cl_uint(param_name);

    size_t buf_len;
    CL_CHECK( clGetContextInfo(cnt, par, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetContextInfo(cnt, par, buf_len, buffer, NULL) );
    switch(par) {
    case CL_CONTEXT_NUM_DEVICES: {
        cl_uint value = *(cl_uint*)buffer;
        return scm_from_cl_uint(value);
    }
    case CL_CONTEXT_DEVICES: {
        SCM ret = SCM_EOL;
        cl_device_id *devices = (cl_device_id*)buffer;
        SCM num_devices_key = scm_from_cl_uint(CL_CONTEXT_NUM_DEVICES);
        SCM scm_num_devices = scm_cl_context_info(context, num_devices_key);
        cl_uint num_devices = scm_to_cl_uint(scm_num_devices);
        for(cl_uint ui = 0; ui < num_devices; ++ui) {
            SCM smob = scm_from_cl_device_id(devices[ui]);
            ret = scm_cons(smob, ret);
        }
        return ret;
    }
    }
    scm_wrong_type_arg(FUNC_NAME, SCM_ARG2, param_name);
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_command_queue_info, "cl-command-queue-info", 2, 0, 0,
            (SCM command_queue, SCM param_name),
            "Return information of the OpenCl command-queue @var{command_queue}\n"
            "specified by the OpenCL constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_command_queue_info
{
    cl_command_queue      cq = scm_to_cl_command_queue_here(command_queue);
    cl_command_queue_info par = scm_to_cl_uint(param_name);

    size_t buf_len;
    CL_CHECK( clGetCommandQueueInfo(cq, par, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetCommandQueueInfo(cq, par, buf_len, buffer, NULL) );

    switch(par) {
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
    scm_wrong_type_arg(FUNC_NAME, SCM_ARG2, param_name);
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_program_info, "cl-program-info", 2, 0, 0,
            (SCM program, SCM param_name),
            "Return information of the OpenCl program @var{program}\n"
            "specified by the OpenCL constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_program_info
{
    cl_program      prog = scm_to_cl_program_here(program);
    cl_program_info par  = scm_to_cl_uint(param_name);

    size_t buf_len;
    CL_CHECK( clGetProgramInfo(prog, par, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetProgramInfo(prog, par, buf_len, buffer, NULL) );

    switch(par) {
    case CL_PROGRAM_SOURCE: {
        return scm_from_utf8_stringn((char *)buffer, buf_len);
    }
        // TODO other cases
    }
    scm_wrong_type_arg(FUNC_NAME, SCM_ARG2, param_name);
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_kernel_info, "cl-kernel-info", 2, 0, 0,
            (SCM kernel, SCM param_name),
            "Return information of the OpenCl kernel @var{kernel}\n"
            "specified by the OpenCL constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_kernel_info
{
    cl_kernel      ker = scm_to_cl_kernel_here(kernel);
    cl_kernel_info par = scm_to_cl_uint(param_name);

    size_t buf_len;
    CL_CHECK( clGetKernelInfo(ker, par, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetKernelInfo(ker, par, buf_len, buffer, NULL) );

    switch(par) {
    case CL_KERNEL_FUNCTION_NAME: {
        return scm_from_utf8_string((char *)buffer);
    }
    case CL_KERNEL_CONTEXT: {
        return scm_from_cl_context(*(cl_context *)buffer);
    }
    case CL_KERNEL_PROGRAM: {
        return scm_from_cl_program(*(cl_program *)buffer);
    }
    case CL_KERNEL_NUM_ARGS: {
        cl_uint num_args = *(cl_uint *)buffer;
        return scm_from_cl_uint(num_args);
    }
    }

    scm_wrong_type_arg(FUNC_NAME, SCM_ARG2, param_name);
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_event_info, "cl-event-info", 2, 0, 0,
            (SCM event, SCM param_name),
            "Return information of the OpenCl event @var{event}\n"
            "specified by the OpenCL constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_event_info
{
    cl_event        e  = scm_to_cl_event_here(event);
    cl_sampler_info par = scm_to_cl_uint(param_name);

    size_t buf_len;
    CL_CHECK( clGetEventInfo(e, par, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetEventInfo(e, par, buf_len, buffer, NULL) );

    switch(par) {
    case CL_EVENT_COMMAND_QUEUE: {
        return scm_from_cl_command_queue(*(cl_command_queue *)buffer);
    }
        // TODO remaining cases
    }
    scm_wrong_type_arg(FUNC_NAME, SCM_ARG2, param_name);
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_sampler_info, "cl-sampler-info", 2, 0, 0,
            (SCM sampler, SCM param_name),
            "Return information of the OpenCl sampler @var{sampler}\n"
            "specified by the OpenCL constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_sampler_info
{
    cl_sampler      s  = scm_to_cl_sampler_here(sampler);
    cl_sampler_info par = scm_to_cl_uint(param_name);

    size_t buf_len;
    CL_CHECK( clGetSamplerInfo(s, par, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetSamplerInfo(s, par, buf_len, buffer, NULL) );

    switch(par) {
    case CL_SAMPLER_CONTEXT: {
        return scm_from_cl_context(*(cl_context *)buffer);
    }
    case CL_SAMPLER_NORMALIZED_COORDS: {
        cl_bool b = *(cl_bool *)buffer;
        if(b) return SCM_BOOL_T;
        else  return SCM_BOOL_F;
    }
        // TODO rest
    }
    scm_wrong_type_arg(FUNC_NAME, SCM_ARG2, param_name);
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_program_build_info, "cl-program-build-info", 3, 0, 0,
            (SCM program, SCM device, SCM param_name),
            "Return information of the OpenCl program @var{program}\n"
            "built for device @var{device} specified by the OpenCL constant\n"
            "@var{param_name}.")
#define FUNC_NAME s_scm_cl_program_build_info
{
    cl_program      p  = scm_to_cl_program_here(program);
    cl_device_id    d  = scm_to_cl_device_id_here(device);
    cl_sampler_info par = scm_to_cl_uint(param_name);

    size_t buf_len;
    CL_CHECK( clGetProgramBuildInfo(p, d, par, 0, NULL, &buf_len) );
    char buffer[buf_len];
    CL_CHECK( clGetProgramBuildInfo(p, d, par, buf_len, buffer, NULL) );

    switch (par) {
    case CL_PROGRAM_BUILD_STATUS: {
        // TODO
    }
    case CL_PROGRAM_BUILD_LOG: {
        return scm_from_utf8_stringn(buffer, buf_len);
    }
    }
    // TODO
    scm_wrong_type_arg(FUNC_NAME, SCM_ARG2, param_name);
    return SCM_EOL;
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_work_group_info, "cl-work-group-info", 3, 0, 0,
            (SCM kernel, SCM device, SCM param_name),
            "Return information of the OpenCl kernel @var{kernel}\n"
            "running on device @var{device} specified by the OpenCL\n"
            "constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_work_group_info
{
    // TODO
    return SCM_EOL;
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_event_profiling_info, "cl-event-profiling-info", 3, 0, 0,
            (SCM event, SCM param_name),
            "Return information of the OpenCl event @var{event}\n"
            "specified by the OpenCL constant @var{param_name}.")
#define FUNC_NAME s_scm_cl_event_profiling_info
{
    // TODO
    return SCM_EOL;
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_info, "cl-info", 1, 1, 0,
            (SCM smob, SCM param_name),
            "@var{smob} can by any Guile OpenCL smob. If @var{param_name}\n"
            "is omitted, return an alist of all attributes and values as\n"
            "specified in the OpenCL standard, otherwise dispatch the call\n"
            "to the corresponding get-NAME-info command.")
#define FUNC_NAME s_scm_cl_info
{
    scm_assert_smob_type(guile_opencl_tag, smob);
    scm_t_bits tag = SCM_SMOB_FLAGS(smob);

    if(!SCM_UNBNDP(param_name)) {
        if(tag == cl_platform_tag     ) return scm_cl_platform_info(smob, param_name);
        if(tag == cl_device_tag       ) return scm_cl_device_info(smob, param_name);
        if(tag == cl_context_tag      ) return scm_cl_context_info(smob, param_name);
        if(tag == cl_command_queue_tag) return scm_cl_command_queue_info(smob, param_name);
        if(tag == cl_buffer_tag       ) return scm_cl_buffer_info(smob, param_name);
        if(tag == cl_image_tag        ) return scm_cl_image_info(smob, param_name);
        if(tag == cl_program_tag      ) return scm_cl_program_info(smob, param_name);
        if(tag == cl_kernel_tag       ) return scm_cl_kernel_info(smob, param_name);
        if(tag == cl_event_tag        ) return scm_cl_event_info(smob, param_name);
        if(tag == cl_sampler_tag      ) return scm_cl_sampler_info(smob, param_name);
        scm_wrong_type_arg_msg(__func__, SCM_ARG1, smob, "opencl smob");
        return SCM_EOL;
    }

    SCM ret = SCM_EOL;
#define ACONS_CL_INFO(key, smob, alist)                                 \
    alist = scm_acons( scm_from_utf8_string( #key ),                    \
                       scm_cl_info(smob, scm_from_cl_uint(key)),        \
                       alist)

    if(tag == cl_platform_tag) {
        ACONS_CL_INFO(CL_PLATFORM_EXTENSIONS, smob, ret);
        ACONS_CL_INFO(CL_PLATFORM_VENDOR, smob, ret);
        ACONS_CL_INFO(CL_PLATFORM_NAME, smob, ret);
        ACONS_CL_INFO(CL_PLATFORM_VERSION, smob, ret);
        return ret;
    }
    if(tag == cl_device_tag) {
        ACONS_CL_INFO(CL_DRIVER_VERSION, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_VERSION, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_VENDOR, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_NAME, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_MAX_WORK_ITEM_SIZES, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_MAX_WORK_GROUP_SIZE, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_MAX_PARAMETER_SIZE, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_MAX_MEM_ALLOC_SIZE, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_MAX_CONSTANT_ARGS, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_MAX_COMPUTE_UNITS, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_MAX_CLOCK_FREQUENCY, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_LOCAL_MEM_SIZE, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_GLOBAL_MEM_SIZE, smob, ret);
        ACONS_CL_INFO(CL_DEVICE_EXTENSIONS, smob, ret);
        return ret;
    }
    if(tag == cl_context_tag) {
        ACONS_CL_INFO(CL_CONTEXT_NUM_DEVICES, smob, ret);
        ACONS_CL_INFO(CL_CONTEXT_DEVICES, smob, ret);
        return ret;
    }
    if(tag == cl_command_queue_tag) {
        ACONS_CL_INFO(CL_QUEUE_CONTEXT, smob, ret);
        ACONS_CL_INFO(CL_QUEUE_DEVICE, smob, ret);
        return ret;
    }
    if(tag == cl_buffer_tag) {
        ACONS_CL_INFO(CL_MEM_SIZE, smob, ret);
        return ret;
    }
    if(tag == cl_image_tag) {
        ACONS_CL_INFO(CL_MEM_SIZE, smob, ret);
        return ret;
    }
    if(tag == cl_program_tag) {
        ACONS_CL_INFO(CL_PROGRAM_SOURCE, smob, ret);
        return ret;
    }
    if(tag == cl_kernel_tag) {
        ACONS_CL_INFO(CL_KERNEL_FUNCTION_NAME, smob, ret);
        ACONS_CL_INFO(CL_KERNEL_CONTEXT, smob, ret);
        ACONS_CL_INFO(CL_KERNEL_PROGRAM, smob, ret);
        ACONS_CL_INFO(CL_KERNEL_NUM_ARGS, smob, ret);
        return ret;
    }
    if(tag == cl_event_tag) {
        ACONS_CL_INFO(CL_EVENT_COMMAND_QUEUE, smob, ret);
        return ret;
    }
    if(tag == cl_sampler_tag) {
        ACONS_CL_INFO(CL_SAMPLER_CONTEXT, smob, ret);
        return ret;
    }
    scm_wrong_type_arg_msg(__func__, SCM_ARG1, smob, "opencl smob");
    return ret;
}
#undef FUNC_NAME

void guile_opencl_init_info() {
#ifndef SCM_MAGIC_SNARFER
#include "info.x"
#endif
}
