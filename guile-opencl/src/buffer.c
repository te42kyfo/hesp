/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "buffer.h"
#include "conversion.h"
#include "error.h"

SCM_DEFINE (scm_make_cl_buffer, "make-cl-buffer", 3, 1, 0,
            (SCM context, SCM flags, SCM size, SCM bv),
            "Allocate an OpenCL buffer. This function is equivalent to\n"
            "the OpenCL function clCreateBuffer.")
#define FUNC_NAME s_scm_make_cl_buffer
{
    cl_context   c_context = scm_to_cl_context_here(context);
    cl_mem_flags c_flags   = (cl_mem_flags)scm_to_cl_ulong(flags);
    size_t       c_size    = scm_to_size_t(size);
    void        *host_ptr;
    if(SCM_UNBNDP(bv)) {
        host_ptr = NULL;
    } else {
        SCM_VALIDATE_BYTEVECTOR(SCM_ARG4, bv);
        size_t bv_len = SCM_BYTEVECTOR_LENGTH(bv);
        if(bv_len < c_size) {
            scm_misc_error(FUNC_NAME, "bytevector is too short", SCM_EOL);
        }
        host_ptr = SCM_BYTEVECTOR_CONTENTS(bv);
    }

    cl_int err;
    cl_mem buffer = clCreateBuffer(c_context, c_flags, c_size, host_ptr, &err);
    scm_remember_upto_here_1(bv);
    CL_CHECK( err );
    return scm_from_cl_buffer(buffer);
}
#undef FUNC_NAME

SCM_DEFINE (scm_enqueue_read_cl_buffer, "enqueue-read-cl-buffer", 4, 0, 1,
            (SCM queue, SCM buffer, SCM offset, SCM bv, SCM events),
            "Enqueue a device->host copy of the cl-buffer @var{buffer}\n"
            "offset by @var{offset} bytes into the bytevector @var{bv}.\n"
            "The number of copied bytes equals the length of @var{bv}.\n"
            "@var{events} is a list of cl_events that have to be completed\n"
            "before the operation starts.\n\n"
            "This function returns an OpenCL event detailing the state of\n"
            "this operation.")
#define FUNC_NAME s_scm_enqueue_read_cl_buffer
{
    cl_command_queue q = scm_to_cl_command_queue_here(queue);
    cl_mem           b = scm_to_cl_buffer_here(buffer);
    size_t           o = scm_to_size_t(offset);
    size_t           s = scm_c_bytevector_length(bv);
    void            *p = SCM_BYTEVECTOR_CONTENTS(bv);
    // TODO event wait list

    cl_event event;
    CL_CHECK( clEnqueueReadBuffer(q, b, 0, o, s, p, 0, NULL, &event) );
    return scm_from_cl_event(event);
}
#undef FUNC_NAME

SCM_DEFINE (scm_enqueue_write_cl_buffer, "enqueue-write-cl-buffer", 4, 0, 1,
            (SCM queue, SCM buffer, SCM offset, SCM bv, SCM events),
            "Enqueue a host->device copy from the bytevector @var{bv}\n"
            "to the cl-buffer @var{buffer} offset by @var{offset} bytes.\n"
            "The number of copied bytes equals the length of @var{bv}.\n"
            "@var{events} is a list of cl_events that have to be completed\n"
            "before the operation starts.\n\n"
            "This function returns an OpenCL event detailing the state of\n"
            "this operation.")
#define FUNC_NAME s_scm_enqueue_write_cl_buffer
{
    SCM_VALIDATE_BYTEVECTOR(SCM_ARG4, bv);
    cl_command_queue q = scm_to_cl_command_queue_here(queue);
    cl_mem           b = scm_to_cl_buffer_here(buffer);
    size_t           o = scm_to_size_t(offset);
    size_t           s = SCM_BYTEVECTOR_LENGTH(bv);
    void            *p = SCM_BYTEVECTOR_CONTENTS(bv);
    // TODO event wait list

    cl_event event;
    CL_CHECK( clEnqueueWriteBuffer(q, b, 0, o, s, p, 0, NULL, &event) );
    return scm_from_cl_event(event);
}
#undef FUNC_NAME

SCM_DEFINE (scm_enqueue_copy_cl_buffer, "enqueue-copy-cl-buffer", 6, 0, 1,
            (SCM queue,
             SCM src_buffer, SCM dst_buffer,
             SCM src_offset, SCM dst_offset, SCM size,
             SCM events),
            "Enqueue a copy operation of \var{size} bytes from the\n"
            "OpenCL buffer @var{src_buffer} offset by\n"
            "@var{src_offset} bytes to the OpenCL buffer\n"
            "@var{dst_buffer} offset by @var{dst_offset}.\n"
            "@var{events} is a list of cl_events that have to be completed\n"
            "before the operation starts.\n\n"
            "This function returns an OpenCL event detailing the state of\n"
            "this operation.")
#define FUNC_NAME s_scm_enqueue_copy_cl_buffer
{
    cl_command_queue q = scm_to_cl_command_queue_here(queue);
    cl_mem src  = scm_to_cl_buffer_here(src_buffer);
    cl_mem dst  = scm_to_cl_buffer_here(dst_buffer);
    size_t srco = scm_to_size_t(src_offset);
    size_t dsto = scm_to_size_t(dst_offset);
    size_t s    = scm_to_size_t(size);
    // TODO event wait list

    cl_event event;
    CL_CHECK( clEnqueueCopyBuffer(q, src, dst, srco, dsto, s,
                                  0, NULL, &event) );
    return scm_from_cl_event(event);
}
#undef FUNC_NAME

#ifdef CL_VERSION_1_2
SCM_DEFINE (scm_enqueue_fill_cl_buffer, "enqueue-fill-cl-buffer", 5, 0, 1,
            (SCM queue, SCM buffer, SCM pattern, SCM offset, SCM size, SCM events),
            "Enqueue an operation to fill \var{size} bytes of\n"
            "the buffer \var{buffer} offset by \var{offset} with repetitions\n"
            "of the bytevector \var{pattern}."
            "@var{events} is a list of cl_events that have to be completed\n"
            "before the operation starts.\n\n"
            "This function returns an OpenCL event detailing the state of\n"
            "this operation.")
#define FUNC_NAME s_scm_enqueue_copy_cl_buffer
{
    cl_command_queue q = scm_to_cl_command_queue_here(queue);
    cl_mem b  = scm_to_cl_buffer_here(buffer);
    size_t ps = scm_c_bytevector_length(pattern);
    void  *p  = SCM_BYTEVECTOR_CONTENTS(pattern);
    size_t o  = scm_to_size_t(offset);
    size_t s  = scm_to_size_t(size);
    // TODO event wait list
    cl_event event;
    CL_CHECK( clEnqueueFillBuffer(q, b, p, ps, o, s, 0, NULL, &event) );
    return scm_from_cl_event(event);
}
#undef FUNC_NAME
#endif

SCM_DEFINE (scm_enqueue_map_cl_buffer, "enqueue-map-cl-buffer", 5, 0, 1,
            (SCM queue, SCM buffer, SCM flags, SCM offset, SCM size, SCM events),
            "Map a part of the device memory specified by \var{buffer},\n"
            "\var{offset} and \var{size} back to host memory.\n"
            "\var{flags} is a bitfield of CL_MAP_READ, CL_MAP_WRITE or\n"
            "CL_MAP_WRITE_INVALIDATE as specified by the OpenCL standard.\n"
            "@var{events} is a list of cl_events that have to be completed\n"
            "before the operation starts.\n"
            "This function returns two values --- a bytevector describing the\n"
            "mapped memory and an OpenCL event detailing the state of this\n."
            "operation.")
#define FUNC_NAME s_scm_enqueue_map_cl_buffer
{
    cl_command_queue q = scm_to_cl_command_queue_here(queue);
    cl_mem       b = scm_to_cl_buffer_here(buffer);
    cl_map_flags f = scm_to_cl_ulong(flags);
    size_t       o = scm_to_size_t(offset);
    size_t       s = scm_to_size_t(size);
    // TODO event wait list

    cl_event event;
    cl_int   err;
    void *ptr = clEnqueueMapBuffer(q, b, 0, f, o, s,
                                   0, NULL, &event, &err);
    CL_CHECK( err );
    SCM scm_ptr   = scm_from_pointer(ptr, NULL);
    SCM bv        = scm_pointer_to_bytevector(scm_ptr, size,
                                              SCM_UNDEFINED, SCM_UNDEFINED);
    SCM scm_event = scm_from_cl_event(event);
    return scm_values(scm_list_2(bv, scm_event));
}
#undef FUNC_NAME

SCM_DEFINE (scm_enqueue_unmap_cl_buffer, "enqueue-unmap-cl-buffer", 3, 0, 1,
            (SCM queue, SCM buffer, SCM bv, SCM events),
            "Enqueue an unmap operation of the memory specified by\n"
            "the bytevector \var{bv}.\n"
            "\var{bv} must be a bytevector returned from a previous\n"
            "call to cl-map-buffer.\n\n"
            "This function returns an OpenCL event detailing the state of\n"
            "this operation.")
#define FUNC_NAME s_scm_enqueue_unmap_cl_buffer
{
    SCM_VALIDATE_BYTEVECTOR(SCM_ARG3, bv);
    cl_command_queue q = scm_to_cl_command_queue_here(queue);
    cl_mem m = scm_to_cl_buffer_here(buffer);
    void  *p = SCM_BYTEVECTOR_CONTENTS(bv);

    // TODO event wait list
    cl_event event;
    CL_CHECK( clEnqueueUnmapMemObject(q, m, p, 0, NULL, &event) );
    return scm_from_cl_event(event);
}
#undef FUNC_NAME

void guile_opencl_init_buffer () {
#ifndef SCM_MAGIC_SNARFER
#include "buffer.x"
#endif
}
