;; Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later

(define-module (guile-opencl)
  #:export
  (make-cl-buffer                       ; buffer.c
   alias-bytevector
   enqueue-read-cl-buffer
   enqueue-write-cl-buffer
   enqueue-copy-cl-buffer
   enqueue-map-cl-buffer
   enqueue-unmap-cl-buffer
   CL_VERSION_1_0                       ; constants.c
   CL_VERSION_1_1
   CL_FALSE
   CL_TRUE
   CL_PLATFORM_PROFILE
   CL_PLATFORM_VERSION
   CL_PLATFORM_NAME
   CL_PLATFORM_VENDOR
   CL_PLATFORM_EXTENSIONS
   CL_DEVICE_TYPE_DEFAULT
   CL_DEVICE_TYPE_CPU
   CL_DEVICE_TYPE_GPU
   CL_DEVICE_TYPE_ACCELERATOR
   CL_DEVICE_TYPE_ALL
   CL_DEVICE_TYPE
   CL_DEVICE_VENDOR_ID
   CL_DEVICE_MAX_COMPUTE_UNITS
   CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
   CL_DEVICE_MAX_WORK_GROUP_SIZE
   CL_DEVICE_MAX_WORK_ITEM_SIZES
   CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR
   CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT
   CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT
   CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG
   CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT
   CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE
   CL_DEVICE_MAX_CLOCK_FREQUENCY
   CL_DEVICE_ADDRESS_BITS
   CL_DEVICE_MAX_READ_IMAGE_ARGS
   CL_DEVICE_MAX_WRITE_IMAGE_ARGS
   CL_DEVICE_MAX_MEM_ALLOC_SIZE
   CL_DEVICE_IMAGE2D_MAX_WIDTH
   CL_DEVICE_IMAGE2D_MAX_HEIGHT
   CL_DEVICE_IMAGE3D_MAX_WIDTH
   CL_DEVICE_IMAGE3D_MAX_HEIGHT
   CL_DEVICE_IMAGE3D_MAX_DEPTH
   CL_DEVICE_IMAGE_SUPPORT
   CL_DEVICE_MAX_PARAMETER_SIZE
   CL_DEVICE_MAX_SAMPLERS
   CL_DEVICE_MEM_BASE_ADDR_ALIGN
   CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE
   CL_DEVICE_SINGLE_FP_CONFIG
   CL_DEVICE_GLOBAL_MEM_CACHE_TYPE
   CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE
   CL_DEVICE_GLOBAL_MEM_CACHE_SIZE
   CL_DEVICE_GLOBAL_MEM_SIZE
   CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE
   CL_DEVICE_MAX_CONSTANT_ARGS
   CL_DEVICE_LOCAL_MEM_TYPE
   CL_DEVICE_LOCAL_MEM_SIZE
   CL_DEVICE_ERROR_CORRECTION_SUPPORT
   CL_DEVICE_PROFILING_TIMER_RESOLUTION
   CL_DEVICE_ENDIAN_LITTLE
   CL_DEVICE_AVAILABLE
   CL_DEVICE_COMPILER_AVAILABLE
   CL_DEVICE_EXECUTION_CAPABILITIES
   CL_DEVICE_QUEUE_PROPERTIES
   CL_DEVICE_NAME
   CL_DEVICE_VENDOR
   CL_DRIVER_VERSION
   CL_DEVICE_PROFILE
   CL_DEVICE_VERSION
   CL_DEVICE_EXTENSIONS
   CL_DEVICE_PLATFORM
   CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF
   CL_DEVICE_HOST_UNIFIED_MEMORY
   CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR
   CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT
   CL_DEVICE_NATIVE_VECTOR_WIDTH_INT
   CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG
   CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT
   CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE
   CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF
   CL_DEVICE_OPENCL_C_VERSION
   CL_FP_DENORM
   CL_FP_INF_NAN
   CL_FP_ROUND_TO_NEAREST
   CL_FP_ROUND_TO_ZERO
   CL_FP_ROUND_TO_INF
   CL_FP_FMA
   CL_FP_SOFT_FLOAT
   CL_NONE
   CL_READ_ONLY_CACHE
   CL_READ_WRITE_CACHE
   CL_LOCAL
   CL_GLOBAL
   CL_EXEC_KERNEL
   CL_EXEC_NATIVE_KERNEL
   CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE
   CL_QUEUE_PROFILING_ENABLE
   CL_CONTEXT_REFERENCE_COUNT
   CL_CONTEXT_DEVICES
   CL_CONTEXT_PROPERTIES
   CL_CONTEXT_NUM_DEVICES
   CL_CONTEXT_PLATFORM
   CL_QUEUE_CONTEXT
   CL_QUEUE_DEVICE
   CL_QUEUE_REFERENCE_COUNT
   CL_QUEUE_PROPERTIES
   CL_MEM_READ_WRITE
   CL_MEM_WRITE_ONLY
   CL_MEM_READ_ONLY
   CL_MEM_USE_HOST_PTR
   CL_MEM_ALLOC_HOST_PTR
   CL_MEM_COPY_HOST_PTR
   CL_R
   CL_A
   CL_RG
   CL_RA
   CL_RGB
   CL_RGBA
   CL_BGRA
   CL_ARGB
   CL_INTENSITY
   CL_LUMINANCE
   CL_Rx
   CL_RGx
   CL_RGBx
   CL_SNORM_INT8
   CL_SNORM_INT16
   CL_UNORM_INT8
   CL_UNORM_INT16
   CL_UNORM_SHORT_565
   CL_UNORM_SHORT_555
   CL_UNORM_INT_101010
   CL_SIGNED_INT8
   CL_SIGNED_INT16
   CL_SIGNED_INT32
   CL_UNSIGNED_INT8
   CL_UNSIGNED_INT16
   CL_UNSIGNED_INT32
   CL_HALF_FLOAT
   CL_FLOAT
   CL_MEM_OBJECT_BUFFER
   CL_MEM_OBJECT_IMAGE2D
   CL_MEM_OBJECT_IMAGE3D
   CL_MEM_TYPE
   CL_MEM_FLAGS
   CL_MEM_SIZE
   CL_MEM_HOST_PTR
   CL_MEM_MAP_COUNT
   CL_MEM_REFERENCE_COUNT
   CL_MEM_CONTEXT
   CL_MEM_ASSOCIATED_MEMOBJECT
   CL_MEM_OFFSET
   CL_IMAGE_FORMAT
   CL_IMAGE_ELEMENT_SIZE
   CL_IMAGE_ROW_PITCH
   CL_IMAGE_SLICE_PITCH
   CL_IMAGE_WIDTH
   CL_IMAGE_HEIGHT
   CL_IMAGE_DEPTH
   CL_ADDRESS_NONE
   CL_ADDRESS_CLAMP_TO_EDGE
   CL_ADDRESS_CLAMP
   CL_ADDRESS_REPEAT
   CL_ADDRESS_MIRRORED_REPEAT
   CL_FILTER_NEAREST
   CL_FILTER_LINEAR
   CL_SAMPLER_REFERENCE_COUNT
   CL_SAMPLER_CONTEXT
   CL_SAMPLER_NORMALIZED_COORDS
   CL_SAMPLER_ADDRESSING_MODE
   CL_SAMPLER_FILTER_MODE
   CL_MAP_READ
   CL_MAP_WRITE
   CL_PROGRAM_REFERENCE_COUNT
   CL_PROGRAM_CONTEXT
   CL_PROGRAM_NUM_DEVICES
   CL_PROGRAM_DEVICES
   CL_PROGRAM_SOURCE
   CL_PROGRAM_BINARY_SIZES
   CL_PROGRAM_BINARIES
   CL_PROGRAM_BUILD_STATUS
   CL_PROGRAM_BUILD_OPTIONS
   CL_PROGRAM_BUILD_LOG
   CL_BUILD_SUCCESS
   CL_BUILD_NONE
   CL_BUILD_ERROR
   CL_BUILD_IN_PROGRESS
   CL_KERNEL_FUNCTION_NAME
   CL_KERNEL_NUM_ARGS
   CL_KERNEL_REFERENCE_COUNT
   CL_KERNEL_CONTEXT
   CL_KERNEL_PROGRAM
   CL_KERNEL_WORK_GROUP_SIZE
   CL_KERNEL_COMPILE_WORK_GROUP_SIZE
   CL_KERNEL_LOCAL_MEM_SIZE
   CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE
   CL_KERNEL_PRIVATE_MEM_SIZE
   CL_EVENT_COMMAND_QUEUE
   CL_EVENT_COMMAND_TYPE
   CL_EVENT_REFERENCE_COUNT
   CL_EVENT_COMMAND_EXECUTION_STATUS
   CL_EVENT_CONTEXT
   CL_COMMAND_NDRANGE_KERNEL
   CL_COMMAND_TASK
   CL_COMMAND_NATIVE_KERNEL
   CL_COMMAND_READ_BUFFER
   CL_COMMAND_WRITE_BUFFER
   CL_COMMAND_COPY_BUFFER
   CL_COMMAND_READ_IMAGE
   CL_COMMAND_WRITE_IMAGE
   CL_COMMAND_COPY_IMAGE
   CL_COMMAND_COPY_IMAGE_TO_BUFFER
   CL_COMMAND_COPY_BUFFER_TO_IMAGE
   CL_COMMAND_MAP_BUFFER
   CL_COMMAND_MAP_IMAGE
   CL_COMMAND_UNMAP_MEM_OBJECT
   CL_COMMAND_MARKER
   CL_COMMAND_ACQUIRE_GL_OBJECTS
   CL_COMMAND_RELEASE_GL_OBJECTS
   CL_COMMAND_READ_BUFFER_RECT
   CL_COMMAND_WRITE_BUFFER_RECT
   CL_COMMAND_COPY_BUFFER_RECT
   CL_COMMAND_USER
   CL_COMPLETE
   CL_RUNNING
   CL_SUBMITTED
   CL_QUEUED
   CL_BUFFER_CREATE_TYPE_REGION
   CL_PROFILING_COMMAND_QUEUED
   CL_PROFILING_COMMAND_SUBMIT
   CL_PROFILING_COMMAND_START
   CL_PROFILING_COMMAND_END
   make-cl-context                      ; context.c
   get-cl-devices                       ; device.c
   cl-platform-info                     ; info.c
   cl-device-info
   cl-buffer-info
   cl-image-info
   cl-context-info
   cl-command-queue-info
   cl-program-info
   cl-kernel-info
   cl-sampler-info
   cl-program-build-info
   cl-work-group-info
   cl-info
   cl_bool                              ; kernel.c
   cl_char
   cl_uchar
   cl_short
   cl_ushort
   cl_int
   cl_uint
   cl_long
   cl_ulong
   cl_float
   cl_double
   cl_image
   cl_buffer
   cl_sampler
   make-cl-kernel
   set-cl-kernel-args
   enqueue-cl-kernel
   get-cl-platforms                     ; platform.c
   cl-platform?                         ; predicates.c
   cl-device?
   cl-context?
   cl-command-queue?
   cl-buffer?
   cl-image?
   cl-program?
   cl-kernel?
   cl-event?
   cl-sampler?
   string->cl-program                   ; program.c
   binary->cl-program
   build-cl-program
   make-cl-queue                        ; queue.c
   enqueue-cl-marker
   enqueue-cl-barrier
   enqueue-cl-wait
   cl-flush
   cl-finish
   cl-enqueue                           ; guile-opencl.scm
   ))


(load-extension "libguile-opencl" "init_guile_opencl")

(define-syntax cl-enqueue
  (lambda (x)
    (syntax-case x ()
      ((_ queue) #'(list))
      ((_ queue (action1 args1 ...) (action2 args2 ...) ...)
       #'(let ((fun (eval (symbol-append 'enqueue '- 'action1) (current-module))))
           (fun queue args1 ...)
           (cl-enqueue queue (action2 args2 ...) ...))))))
