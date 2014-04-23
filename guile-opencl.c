#include <CL/cl.h>
#include <libguile.h>

/* Ideas:
   - print tree of smob dependencies
   - grant smooth coexistence guile gc and OpenCL reference counting
*/

/* There can only be 256 different Smob types in the system. Guile-OpenCL
   requires only a single guile type and implements its own type system with
   the 16 extra bits in a Smob. */
static scm_t_bits guile_opencl_tag;    // set in init_guile_opencl()
static const scm_t_bits cl_platform_tag      =  0x0001;
static const scm_t_bits cl_device_tag        =  0x0002;
static const scm_t_bits cl_context_tag       =  0x0003;
static const scm_t_bits cl_command_queue_tag =  0x0004;
static const scm_t_bits cl_mem_tag           =  0x0005;
static const scm_t_bits cl_program_tag       =  0x0006;
static const scm_t_bits cl_kernel_tag        =  0x0007;
static const scm_t_bits cl_event_tag         =  0x0008;
static const scm_t_bits cl_sampler_tag       =  0x0009;
/* If the given value is CL_SUCCESS, do nothing, else throw an appropriate
   error message. */
#define HCE(value) handle_cl_errors(value, __func__);
static void handle_cl_errors(cl_int value, const char* subr) {
    switch(value) {
    case CL_SUCCESS: return;
#define GENERATE_CASE(name) case name: scm_misc_error(subr, #name, SCM_EOL); return
        GENERATE_CASE(CL_DEVICE_NOT_FOUND);
        GENERATE_CASE(CL_DEVICE_NOT_AVAILABLE);
        GENERATE_CASE(CL_COMPILER_NOT_AVAILABLE);
        GENERATE_CASE(CL_MEM_OBJECT_ALLOCATION_FAILURE);
        GENERATE_CASE(CL_OUT_OF_RESOURCES);
        GENERATE_CASE(CL_OUT_OF_HOST_MEMORY);
        GENERATE_CASE(CL_PROFILING_INFO_NOT_AVAILABLE);
        GENERATE_CASE(CL_MEM_COPY_OVERLAP);
        GENERATE_CASE(CL_IMAGE_FORMAT_MISMATCH);
        GENERATE_CASE(CL_IMAGE_FORMAT_NOT_SUPPORTED);
        GENERATE_CASE(CL_BUILD_PROGRAM_FAILURE);
        GENERATE_CASE(CL_MAP_FAILURE);
        GENERATE_CASE(CL_MISALIGNED_SUB_BUFFER_OFFSET);
        GENERATE_CASE(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST);
        GENERATE_CASE(CL_INVALID_VALUE);
        GENERATE_CASE(CL_INVALID_DEVICE_TYPE);
        GENERATE_CASE(CL_INVALID_PLATFORM);
        GENERATE_CASE(CL_INVALID_DEVICE);
        GENERATE_CASE(CL_INVALID_CONTEXT);
        GENERATE_CASE(CL_INVALID_QUEUE_PROPERTIES);
        GENERATE_CASE(CL_INVALID_COMMAND_QUEUE);
        GENERATE_CASE(CL_INVALID_HOST_PTR);
        GENERATE_CASE(CL_INVALID_MEM_OBJECT);
        GENERATE_CASE(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
        GENERATE_CASE(CL_INVALID_IMAGE_SIZE);
        GENERATE_CASE(CL_INVALID_SAMPLER);
        GENERATE_CASE(CL_INVALID_BINARY);
        GENERATE_CASE(CL_INVALID_BUILD_OPTIONS);
        GENERATE_CASE(CL_INVALID_PROGRAM);
        GENERATE_CASE(CL_INVALID_PROGRAM_EXECUTABLE);
        GENERATE_CASE(CL_INVALID_KERNEL_NAME);
        GENERATE_CASE(CL_INVALID_KERNEL_DEFINITION);
        GENERATE_CASE(CL_INVALID_KERNEL);
        GENERATE_CASE(CL_INVALID_ARG_INDEX);
        GENERATE_CASE(CL_INVALID_ARG_VALUE);
        GENERATE_CASE(CL_INVALID_ARG_SIZE);
        GENERATE_CASE(CL_INVALID_KERNEL_ARGS);
        GENERATE_CASE(CL_INVALID_WORK_DIMENSION);
        GENERATE_CASE(CL_INVALID_WORK_GROUP_SIZE);
        GENERATE_CASE(CL_INVALID_WORK_ITEM_SIZE);
        GENERATE_CASE(CL_INVALID_GLOBAL_OFFSET);
        GENERATE_CASE(CL_INVALID_EVENT_WAIT_LIST);
        GENERATE_CASE(CL_INVALID_EVENT);
        GENERATE_CASE(CL_INVALID_OPERATION);
        GENERATE_CASE(CL_INVALID_GL_OBJECT);
        GENERATE_CASE(CL_INVALID_BUFFER_SIZE);
        GENERATE_CASE(CL_INVALID_MIP_LEVEL);
        GENERATE_CASE(CL_INVALID_GLOBAL_WORK_SIZE);
        GENERATE_CASE(CL_INVALID_PROPERTY);
#undef GENERATE_CASE
    default:
        scm_misc_error(subr, "unknown error", SCM_EOL);
    }
}

SCM scm_from_platform_info(cl_platform_id platform,
                           cl_platform_info param_name) {
    size_t buf_len;
    HCE( clGetPlatformInfo(platform, param_name, 0, NULL, &buf_len) );
    char buffer[buf_len];
    HCE( clGetPlatformInfo(platform, param_name, buf_len, buffer, NULL) );
    return scm_from_locale_stringn(buffer, buf_len - 1);
}

SCM scm_from_device_info(cl_device_id device,
                         cl_device_info param_name) {
    size_t buf_len;
    HCE( clGetDeviceInfo(device, param_name, 0, NULL, &buf_len) );
    char buffer[buf_len];
    HCE( clGetDeviceInfo(device, param_name, buf_len, buffer, NULL) );
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
        SCM dim       = scm_from_device_info(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
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
        cl_platform_id platform = *(cl_platform_id*)buffer;
        SCM smob = scm_new_smob(guile_opencl_tag, (scm_t_bits)platform);
        SCM_SET_SMOB_FLAGS(smob, cl_platform_tag);
        return smob;
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

SCM scm_from_context_info(cl_context context,
                          cl_context_info param_name) {
    size_t buf_len;
    HCE( clGetContextInfo(context, param_name, 0, NULL, &buf_len) );
    char buffer[buf_len];
    HCE( clGetContextInfo(context, param_name, buf_len, buffer, NULL) );
    switch(param_name) {
    case CL_CONTEXT_NUM_DEVICES: {
        cl_uint value = *(cl_uint*)buffer;
        return scm_from_uint64((uint64_t) value);
    }
    case CL_CONTEXT_DEVICES: {
        SCM ret = SCM_EOL;
        cl_device_id *devices = (cl_device_id*)buffer;
        SCM scm_num_devices = scm_from_context_info(context, CL_CONTEXT_NUM_DEVICES);
        cl_uint num_devices = scm_to_uint(scm_num_devices);
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

SCM scm_from_command_queue_info(cl_command_queue queue,
                                cl_command_queue_info param_name) {
    // TODO
    return SCM_EOL;
}

SCM scm_from_mem_info(cl_mem memobj,
                      cl_mem_info param_name) {
    // TODO
    return SCM_EOL;
}

SCM scm_from_sampler_info(cl_sampler sampler,
                          cl_sampler_info param_name) {
    // TODO
    return SCM_EOL;
}

SCM scm_from_program_info(cl_program program,
                          cl_program_info param_name) {
    // TODO
    return SCM_EOL;
}

SCM scm_from_program_build_info(cl_program program,
                                cl_device_id device,
                                cl_program_build_info param_name) {
    // TODO
    return SCM_EOL;
}

SCM scm_from_kernel_info(cl_kernel kernel,
                         cl_kernel_info param_name) {
    // TODO
    return SCM_EOL;
}

SCM scm_from_kernel_work_group_info(cl_kernel kernel,
                                    cl_device_id device,
                                    cl_kernel_work_group_info param_name) {
    // TODO
    return SCM_EOL;
}

SCM scm_from_event_profiling_info(cl_event event,
                                  cl_profiling_info param_name) {
    // TODO
    return SCM_EOL;
}

int print_guile_opencl(SCM smob, SCM port, scm_print_state *pstate) {
    scm_t_bits tag = SCM_SMOB_FLAGS(smob);

    if(tag == cl_platform_tag) {
        cl_platform_id platform = (cl_platform_id) SCM_SMOB_DATA(smob);
        scm_puts("#<", port);
        scm_display(scm_from_platform_info(platform, CL_PLATFORM_NAME), port);
        scm_puts(">", port);
        return 1;
    }
    if(tag == cl_device_tag) {
        cl_device_id device = (cl_device_id) SCM_SMOB_DATA(smob);
        scm_puts("#<", port);
        scm_display(scm_from_device_info(device, CL_DEVICE_NAME), port);
        scm_puts(">", port);
        return 1;
    }
    if(tag == cl_context_tag) {
        cl_context context = (cl_context) SCM_SMOB_DATA(smob);
        scm_puts("#< context >", port); // TODO
        return 1;
    }
    if(tag == cl_command_queue_tag) {
        cl_command_queue command_queue = (cl_command_queue) SCM_SMOB_DATA(smob);
        scm_puts("#< command queue >", port); // TODO
        return 1;
    }
    if(tag == cl_mem_tag) {
        cl_mem mem = (cl_mem) SCM_SMOB_DATA(smob);
        scm_puts("#< mem >", port); // TODO
        return 1;
    }
    if(tag == cl_program_tag) {
        cl_program program = (cl_program) SCM_SMOB_DATA(smob);
        scm_puts("#< program >", port); // TODO
        return 1;
    }
    if(tag == cl_kernel_tag) {
        cl_kernel kernel = (cl_kernel) SCM_SMOB_DATA(smob);
        scm_puts("#< kernel >", port); // TODO
        return 1;
    }
    if(tag == cl_event_tag) {
        cl_event event = (cl_event) SCM_SMOB_DATA(smob);
        scm_puts("#< event >", port); // TODO
        return 1;
    }
    if(tag == cl_sampler_tag) {
        cl_sampler sampler = (cl_sampler) SCM_SMOB_DATA(smob);
        scm_puts("#< sampler >", port); // TODO
        return 1;
    }
    scm_misc_error(__func__, "smob of invalid type", SCM_EOL);
    return 1;
}

/* Given any OpenCL Smob, returns an alist of object attributes. */
SCM get_info(SCM smob) {
    scm_assert_smob_type(guile_opencl_tag, smob);
    scm_t_bits tag = SCM_SMOB_FLAGS(smob);
    SCM ret = SCM_EOL;
    if(tag == cl_platform_tag) {
        cl_platform_id platform = (cl_platform_id) SCM_SMOB_DATA(smob);
#define ACONS_PLATFORM_INFO(platform, param_name, list)                 \
        ret = scm_acons(scm_from_locale_string( #param_name ),          \
                        scm_from_platform_info(platform, param_name), list)
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
        ret = scm_acons(scm_from_locale_string( #param_name ),          \
                        scm_from_device_info(device, param_name), list)
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


/* return a list of platform Smobs. */
SCM get_platforms() {
    cl_uint         num_platforms;
    HCE( clGetPlatformIDs(0, NULL, &num_platforms) );
    if(0 == num_platforms) return SCM_EOL;

    cl_platform_id platforms[num_platforms];
    if(NULL == platforms) perror( __func__ );
    HCE( clGetPlatformIDs(num_platforms, platforms, NULL) );

    SCM  ret = SCM_EOL;
    for(cl_uint ui = 0; ui < num_platforms; ++ui) {
        SCM smob = scm_new_smob(guile_opencl_tag, (scm_t_bits)platforms[ui]);
        SCM_SET_SMOB_FLAGS(smob, cl_platform_tag);
        ret = scm_cons(smob, ret);
    }
    return ret;
}

/* Return a list of device Smobs */
SCM get_devices(SCM smob) {
    scm_assert_smob_type(guile_opencl_tag, smob);
    scm_t_bits tag = SCM_SMOB_FLAGS(smob);
    if(tag != cl_platform_tag) {
        scm_wrong_type_arg_msg(__func__, 1, smob, "cl_platform");
    }
    // TODO allow filtering for cl_device_type
    cl_platform_id platform = (cl_platform_id)SCM_SMOB_DATA(smob);
    cl_uint         num_devices;
    HCE( clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
                        0, NULL, &num_devices) );
    if(0 == num_devices) return SCM_EOL;

    cl_device_id devices[num_devices];
    if(NULL == devices) perror( __func__ );
    HCE( clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
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
SCM make_context(SCM platform_smob, SCM device_list) {
    scm_assert_smob_type(guile_opencl_tag, platform_smob);
    scm_t_bits tag = SCM_SMOB_FLAGS(platform_smob);
    if(tag != cl_platform_tag) {
        scm_wrong_type_arg_msg(__func__, 1, platform_smob, "cl_platform");
    }
    // TODO
    return SCM_EOL;
}

SCM make_queue(SCM context, SCM device) {
    // TODO
    return SCM_EOL;
}

inline static SCM
type_p(SCM smob, scm_t_bits target_tag) {
    if(!SCM_SMOB_PREDICATE(guile_opencl_tag, smob)) return SCM_BOOL_F;
    scm_t_bits tag = SCM_SMOB_FLAGS(smob);
    if(tag == target_tag) return SCM_BOOL_T;
    else return SCM_BOOL_F;
}

SCM      is_platform(SCM smob) { return type_p(smob, cl_platform_tag); }
SCM        is_device(SCM smob) { return type_p(smob, cl_device_tag); }
SCM       is_context(SCM smob) { return type_p(smob, cl_context_tag); }
SCM is_command_queue(SCM smob) { return type_p(smob, cl_command_queue_tag); }
SCM           is_mem(SCM smob) { return type_p(smob, cl_mem_tag); }
SCM       is_program(SCM smob) { return type_p(smob, cl_program_tag); }
SCM        is_kernel(SCM smob) { return type_p(smob, cl_kernel_tag); }
SCM         is_event(SCM smob) { return type_p(smob, cl_event_tag); }
SCM       is_sampler(SCM smob) { return type_p(smob, cl_sampler_tag); }

void init_guile_opencl() {
    /* this code relies on scm_t_bits being big enough to hold all OpenCL
       handles. TODO maybe compile-time assertion*/
    if(!(sizeof(cl_platform_id) <= sizeof(scm_t_bits) &&
         sizeof(cl_device_id) <= sizeof(scm_t_bits) &&
         sizeof(cl_context) <= sizeof(scm_t_bits) &&
         sizeof(cl_command_queue) <= sizeof(scm_t_bits) &&
         sizeof(cl_mem) <= sizeof(scm_t_bits) &&
         sizeof(cl_program) <= sizeof(scm_t_bits) &&
         sizeof(cl_kernel) <= sizeof(scm_t_bits) &&
         sizeof(cl_event) <= sizeof(scm_t_bits) &&
         sizeof(cl_sampler) <= sizeof(scm_t_bits))) {
        scm_misc_error(__func__, "opencl handles are bigger than supported.", SCM_EOL);
    }

    guile_opencl_tag = scm_make_smob_type("OpenCL", sizeof(void *));
    scm_set_smob_print(guile_opencl_tag, print_guile_opencl);
    scm_c_define_gsubr("info",           1, 0, 0, get_info);
    scm_c_define_gsubr("get-platforms",  0, 0, 0, get_platforms);
    scm_c_define_gsubr("get-devices",    1, 0, 0, get_devices);
    scm_c_define_gsubr("make-context",   1, 0, 0, make_context);
    scm_c_define_gsubr("make-queue",     2, 0, 0, make_queue);
    scm_c_define_gsubr("platform?",      1, 0, 0, is_platform);
    scm_c_define_gsubr("device?",        1, 0, 0, is_device);
    scm_c_define_gsubr("context?",       1, 0, 0, is_context);
    scm_c_define_gsubr("command-queue?", 1, 0, 0, is_command_queue);
    scm_c_define_gsubr("mem?",           1, 0, 0, is_mem);
    scm_c_define_gsubr("program?",       1, 0, 0, is_program);
    scm_c_define_gsubr("kernel?",        1, 0, 0, is_kernel);
    scm_c_define_gsubr("event?",         1, 0, 0, is_event);
    scm_c_define_gsubr("sampler?",       1, 0, 0, is_sampler);
}
