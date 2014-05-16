/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

/* There can only be 256 different Smob types in the system. Guile-OpenCL
   requires only a single guile type and implements its own type system with
   the 16 extra bits in a Smob. */
scm_t_bits guile_opencl_tag;    // set in init_guile_opencl()
static const scm_t_bits cl_platform_tag      =  0x0001;
static const scm_t_bits cl_device_tag        =  0x0002;
static const scm_t_bits cl_context_tag       =  0x0003;
static const scm_t_bits cl_command_queue_tag =  0x0004;
static const scm_t_bits cl_buffer_tag        =  0x0005;
static const scm_t_bits cl_image_tag         =  0x0006;
static const scm_t_bits cl_program_tag       =  0x0007;
static const scm_t_bits cl_kernel_tag        =  0x0008;
static const scm_t_bits cl_event_tag         =  0x0009;
static const scm_t_bits cl_sampler_tag       =  0x0010;

SCM SCM_CL_VERSION_1_0;
SCM SCM_CL_VERSION_1_1;
SCM SCM_CL_FALSE;
SCM SCM_CL_TRUE;
SCM SCM_CL_PLATFORM_PROFILE;
SCM SCM_CL_PLATFORM_VERSION;
SCM SCM_CL_PLATFORM_NAME;
SCM SCM_CL_PLATFORM_VENDOR;
SCM SCM_CL_PLATFORM_EXTENSIONS;
SCM SCM_CL_DEVICE_TYPE_DEFAULT;
SCM SCM_CL_DEVICE_TYPE_CPU;
SCM SCM_CL_DEVICE_TYPE_GPU;
SCM SCM_CL_DEVICE_TYPE_ACCELERATOR;
SCM SCM_CL_DEVICE_TYPE_ALL;
SCM SCM_CL_DEVICE_TYPE;
SCM SCM_CL_DEVICE_VENDOR_ID;
SCM SCM_CL_DEVICE_MAX_COMPUTE_UNITS;
SCM SCM_CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS;
SCM SCM_CL_DEVICE_MAX_WORK_GROUP_SIZE;
SCM SCM_CL_DEVICE_MAX_WORK_ITEM_SIZES;
SCM SCM_CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR;
SCM SCM_CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT;
SCM SCM_CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT;
SCM SCM_CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG;
SCM SCM_CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT;
SCM SCM_CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE;
SCM SCM_CL_DEVICE_MAX_CLOCK_FREQUENCY;
SCM SCM_CL_DEVICE_ADDRESS_BITS;
SCM SCM_CL_DEVICE_MAX_READ_IMAGE_ARGS;
SCM SCM_CL_DEVICE_MAX_WRITE_IMAGE_ARGS;
SCM SCM_CL_DEVICE_MAX_MEM_ALLOC_SIZE;
SCM SCM_CL_DEVICE_IMAGE2D_MAX_WIDTH;
SCM SCM_CL_DEVICE_IMAGE2D_MAX_HEIGHT;
SCM SCM_CL_DEVICE_IMAGE3D_MAX_WIDTH;
SCM SCM_CL_DEVICE_IMAGE3D_MAX_HEIGHT;
SCM SCM_CL_DEVICE_IMAGE3D_MAX_DEPTH;
SCM SCM_CL_DEVICE_IMAGE_SUPPORT;
SCM SCM_CL_DEVICE_MAX_PARAMETER_SIZE;
SCM SCM_CL_DEVICE_MAX_SAMPLERS;
SCM SCM_CL_DEVICE_MEM_BASE_ADDR_ALIGN;
SCM SCM_CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE;
SCM SCM_CL_DEVICE_SINGLE_FP_CONFIG;
SCM SCM_CL_DEVICE_GLOBAL_MEM_CACHE_TYPE;
SCM SCM_CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE;
SCM SCM_CL_DEVICE_GLOBAL_MEM_CACHE_SIZE;
SCM SCM_CL_DEVICE_GLOBAL_MEM_SIZE;
SCM SCM_CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE;
SCM SCM_CL_DEVICE_MAX_CONSTANT_ARGS;
SCM SCM_CL_DEVICE_LOCAL_MEM_TYPE;
SCM SCM_CL_DEVICE_LOCAL_MEM_SIZE;
SCM SCM_CL_DEVICE_ERROR_CORRECTION_SUPPORT;
SCM SCM_CL_DEVICE_PROFILING_TIMER_RESOLUTION;
SCM SCM_CL_DEVICE_ENDIAN_LITTLE;
SCM SCM_CL_DEVICE_AVAILABLE;
SCM SCM_CL_DEVICE_COMPILER_AVAILABLE;
SCM SCM_CL_DEVICE_EXECUTION_CAPABILITIES;
SCM SCM_CL_DEVICE_QUEUE_PROPERTIES;
SCM SCM_CL_DEVICE_NAME;
SCM SCM_CL_DEVICE_VENDOR;
SCM SCM_CL_DRIVER_VERSION;
SCM SCM_CL_DEVICE_PROFILE;
SCM SCM_CL_DEVICE_VERSION;
SCM SCM_CL_DEVICE_EXTENSIONS;
SCM SCM_CL_DEVICE_PLATFORM;
SCM SCM_CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF;
SCM SCM_CL_DEVICE_HOST_UNIFIED_MEMORY;
SCM SCM_CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR;
SCM SCM_CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT;
SCM SCM_CL_DEVICE_NATIVE_VECTOR_WIDTH_INT;
SCM SCM_CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG;
SCM SCM_CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT;
SCM SCM_CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE;
SCM SCM_CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF;
SCM SCM_CL_DEVICE_OPENCL_C_VERSION;
SCM SCM_CL_FP_DENORM;
SCM SCM_CL_FP_INF_NAN;
SCM SCM_CL_FP_ROUND_TO_NEAREST;
SCM SCM_CL_FP_ROUND_TO_ZERO;
SCM SCM_CL_FP_ROUND_TO_INF;
SCM SCM_CL_FP_FMA;
SCM SCM_CL_FP_SOFT_FLOAT;
SCM SCM_CL_NONE;
SCM SCM_CL_READ_ONLY_CACHE;
SCM SCM_CL_READ_WRITE_CACHE;
SCM SCM_CL_LOCAL;
SCM SCM_CL_GLOBAL;
SCM SCM_CL_EXEC_KERNEL;
SCM SCM_CL_EXEC_NATIVE_KERNEL;
SCM SCM_CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
SCM SCM_CL_QUEUE_PROFILING_ENABLE;
SCM SCM_CL_CONTEXT_REFERENCE_COUNT;
SCM SCM_CL_CONTEXT_DEVICES;
SCM SCM_CL_CONTEXT_PROPERTIES;
SCM SCM_CL_CONTEXT_NUM_DEVICES;
SCM SCM_CL_CONTEXT_PLATFORM;
SCM SCM_CL_QUEUE_CONTEXT;
SCM SCM_CL_QUEUE_DEVICE;
SCM SCM_CL_QUEUE_REFERENCE_COUNT;
SCM SCM_CL_QUEUE_PROPERTIES;
SCM SCM_CL_MEM_READ_WRITE;
SCM SCM_CL_MEM_WRITE_ONLY;
SCM SCM_CL_MEM_READ_ONLY;
SCM SCM_CL_MEM_USE_HOST_PTR;
SCM SCM_CL_MEM_ALLOC_HOST_PTR;
SCM SCM_CL_MEM_COPY_HOST_PTR;
SCM SCM_CL_R;
SCM SCM_CL_A;
SCM SCM_CL_RG;
SCM SCM_CL_RA;
SCM SCM_CL_RGB;
SCM SCM_CL_RGBA;
SCM SCM_CL_BGRA;
SCM SCM_CL_ARGB;
SCM SCM_CL_INTENSITY;
SCM SCM_CL_LUMINANCE;
SCM SCM_CL_Rx;
SCM SCM_CL_RGx;
SCM SCM_CL_RGBx;
SCM SCM_CL_SNORM_INT8;
SCM SCM_CL_SNORM_INT16;
SCM SCM_CL_UNORM_INT8;
SCM SCM_CL_UNORM_INT16;
SCM SCM_CL_UNORM_SHORT_565;
SCM SCM_CL_UNORM_SHORT_555;
SCM SCM_CL_UNORM_INT_101010;
SCM SCM_CL_SIGNED_INT8;
SCM SCM_CL_SIGNED_INT16;
SCM SCM_CL_SIGNED_INT32;
SCM SCM_CL_UNSIGNED_INT8;
SCM SCM_CL_UNSIGNED_INT16;
SCM SCM_CL_UNSIGNED_INT32;
SCM SCM_CL_HALF_FLOAT;
SCM SCM_CL_FLOAT;
SCM SCM_CL_MEM_OBJECT_BUFFER;
SCM SCM_CL_MEM_OBJECT_IMAGE2D;
SCM SCM_CL_MEM_OBJECT_IMAGE3D;
SCM SCM_CL_MEM_TYPE;
SCM SCM_CL_MEM_FLAGS;
SCM SCM_CL_MEM_SIZE;
SCM SCM_CL_MEM_HOST_PTR;
SCM SCM_CL_MEM_MAP_COUNT;
SCM SCM_CL_MEM_REFERENCE_COUNT;
SCM SCM_CL_MEM_CONTEXT;
SCM SCM_CL_MEM_ASSOCIATED_MEMOBJECT;
SCM SCM_CL_MEM_OFFSET;
SCM SCM_CL_IMAGE_FORMAT;
SCM SCM_CL_IMAGE_ELEMENT_SIZE;
SCM SCM_CL_IMAGE_ROW_PITCH;
SCM SCM_CL_IMAGE_SLICE_PITCH;
SCM SCM_CL_IMAGE_WIDTH;
SCM SCM_CL_IMAGE_HEIGHT;
SCM SCM_CL_IMAGE_DEPTH;
SCM SCM_CL_ADDRESS_NONE;
SCM SCM_CL_ADDRESS_CLAMP_TO_EDGE;
SCM SCM_CL_ADDRESS_CLAMP;
SCM SCM_CL_ADDRESS_REPEAT;
SCM SCM_CL_ADDRESS_MIRRORED_REPEAT;
SCM SCM_CL_FILTER_NEAREST;
SCM SCM_CL_FILTER_LINEAR;
SCM SCM_CL_SAMPLER_REFERENCE_COUNT;
SCM SCM_CL_SAMPLER_CONTEXT;
SCM SCM_CL_SAMPLER_NORMALIZED_COORDS;
SCM SCM_CL_SAMPLER_ADDRESSING_MODE;
SCM SCM_CL_SAMPLER_FILTER_MODE;
SCM SCM_CL_MAP_READ;
SCM SCM_CL_MAP_WRITE;
SCM SCM_CL_PROGRAM_REFERENCE_COUNT;
SCM SCM_CL_PROGRAM_CONTEXT;
SCM SCM_CL_PROGRAM_NUM_DEVICES;
SCM SCM_CL_PROGRAM_DEVICES;
SCM SCM_CL_PROGRAM_SOURCE;
SCM SCM_CL_PROGRAM_BINARY_SIZES;
SCM SCM_CL_PROGRAM_BINARIES;
SCM SCM_CL_PROGRAM_BUILD_STATUS;
SCM SCM_CL_PROGRAM_BUILD_OPTIONS;
SCM SCM_CL_PROGRAM_BUILD_LOG;
SCM SCM_CL_BUILD_SUCCESS;
SCM SCM_CL_BUILD_NONE;
SCM SCM_CL_BUILD_ERROR;
SCM SCM_CL_BUILD_IN_PROGRESS;
SCM SCM_CL_KERNEL_FUNCTION_NAME;
SCM SCM_CL_KERNEL_NUM_ARGS;
SCM SCM_CL_KERNEL_REFERENCE_COUNT;
SCM SCM_CL_KERNEL_CONTEXT;
SCM SCM_CL_KERNEL_PROGRAM;
SCM SCM_CL_KERNEL_WORK_GROUP_SIZE;
SCM SCM_CL_KERNEL_COMPILE_WORK_GROUP_SIZE;
SCM SCM_CL_KERNEL_LOCAL_MEM_SIZE;
SCM SCM_CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE;
SCM SCM_CL_KERNEL_PRIVATE_MEM_SIZE;
SCM SCM_CL_EVENT_COMMAND_QUEUE;
SCM SCM_CL_EVENT_COMMAND_TYPE;
SCM SCM_CL_EVENT_REFERENCE_COUNT;
SCM SCM_CL_EVENT_COMMAND_EXECUTION_STATUS;
SCM SCM_CL_EVENT_CONTEXT;
SCM SCM_CL_COMMAND_NDRANGE_KERNEL;
SCM SCM_CL_COMMAND_TASK;
SCM SCM_CL_COMMAND_NATIVE_KERNEL;
SCM SCM_CL_COMMAND_READ_BUFFER;
SCM SCM_CL_COMMAND_WRITE_BUFFER;
SCM SCM_CL_COMMAND_COPY_BUFFER;
SCM SCM_CL_COMMAND_READ_IMAGE;
SCM SCM_CL_COMMAND_WRITE_IMAGE;
SCM SCM_CL_COMMAND_COPY_IMAGE;
SCM SCM_CL_COMMAND_COPY_IMAGE_TO_BUFFER;
SCM SCM_CL_COMMAND_COPY_BUFFER_TO_IMAGE;
SCM SCM_CL_COMMAND_MAP_BUFFER;
SCM SCM_CL_COMMAND_MAP_IMAGE;
SCM SCM_CL_COMMAND_UNMAP_MEM_OBJECT;
SCM SCM_CL_COMMAND_MARKER;
SCM SCM_CL_COMMAND_ACQUIRE_GL_OBJECTS;
SCM SCM_CL_COMMAND_RELEASE_GL_OBJECTS;
SCM SCM_CL_COMMAND_READ_BUFFER_RECT;
SCM SCM_CL_COMMAND_WRITE_BUFFER_RECT;
SCM SCM_CL_COMMAND_COPY_BUFFER_RECT;
SCM SCM_CL_COMMAND_USER;
SCM SCM_CL_COMPLETE;
SCM SCM_CL_RUNNING;
SCM SCM_CL_SUBMITTED;
SCM SCM_CL_QUEUED;
SCM SCM_CL_BUFFER_CREATE_TYPE_REGION;
SCM SCM_CL_PROFILING_COMMAND_QUEUED;
SCM SCM_CL_PROFILING_COMMAND_SUBMIT;
SCM SCM_CL_PROFILING_COMMAND_START;
SCM SCM_CL_PROFILING_COMMAND_END;

void guile_opencl_init_constants();
