/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

/* cl_mem - OpenCL memory management */
SCM scm_make_cl_buffer (SCM context, SCM flags, SCM size, SCM host_ptr);

/* return a bytevector aliasing another bytevector. I do check that the new
   bytevector does not go beyond the original one, but no further
   checks. This is essentially pointer arithmetic, so be careful! */
SCM scm_alias_bytevector (SCM bv, SCM size, SCM offset);

SCM scm_read_cl_buffer(SCM command_queue,
                       SCM buffer, SCM offset, SCM bv,
                       SCM event_wait_list); // -> event

SCM scm_write_cl_buffer (SCM command_queue,
                         SCM buffer, SCM offset, SCM bv,
                         SCM event_wait_list); // -> event

SCM scm_copy_cl_buffer (SCM command_queue,
                        SCM src_buffer, SCM dst_buffer,
                        SCM src_offset, SCM dst_offset, SCM size,
                        SCM event_wait_list); // -> event

#ifdef CL_VERSION_1_2
SCM scm_fill_cl_buffer(SCM command_queue, SCM buffer,
                       SCM pattern, // bytevector
                       SCM offset, SCM size,
                       SCM event_wait_list); // -> event
#endif

SCM scm_map_cl_buffer (SCM command_queue, SCM buffer, SCM flags,
                       SCM offset, SCM size,
                       SCM event_wait_list); // -> bytevector, event

SCM scm_unmap_mem (SCM command_queue, SCM mem,
                   SCM ptr, SCM event_wait_list); // -> event

void guile_opencl_init_buffer ();
