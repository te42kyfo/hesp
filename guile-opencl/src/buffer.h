/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>
#include <CL/cl.h>

SCM scm_make_cl_buffer (SCM context, SCM flags, SCM size, SCM bv);

SCM scm_enqueue_read_cl_buffer(SCM command_queue,
                               SCM buffer, SCM offset, SCM bv,
                               SCM event_wait_list); // -> event

SCM scm_enqueue_write_cl_buffer (SCM command_queue,
                                 SCM buffer, SCM offset, SCM bv,
                                 SCM event_wait_list); // -> event

SCM scm_enqueue_copy_cl_buffer (SCM command_queue,
                                SCM src_buffer, SCM dst_buffer,
                                SCM src_offset, SCM dst_offset, SCM size,
                                SCM event_wait_list); // -> event

#ifdef CL_VERSION_1_2
SCM scm_enqueue_fill_cl_buffer(SCM command_queue, SCM buffer,
                               SCM pattern, // bytevector
                               SCM offset, SCM size,
                               SCM event_wait_list); // -> event
#endif

SCM scm_enqueue_map_cl_buffer (SCM command_queue, SCM buffer, SCM flags,
                               SCM offset, SCM size,
                               SCM event_wait_list); // -> bytevector, event

SCM scm_enqueue_unmap_cl_buffer (SCM command_queue, SCM buffer,
                                 SCM ptr, SCM event_wait_list); // -> event

void guile_opencl_init_buffer ();
