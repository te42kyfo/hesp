#pragma once
#include "guile-opencl.h"

/* The OpenCL API calls. They are very similar to the C++ bindings, except
   that they exploit some Scheme convenience features */
SCM scm_get_cl_platforms ();
SCM scm_get_cl_devices (SCM platform);
// TODO make_subdevice
SCM scm_make_context (SCM devices); // TODO cl_context_properties
// TODO make_context_from_type
SCM scm_make_queue (SCM context, SCM device, SCM flags);
SCM scm_make_buffer (SCM context, SCM flags, SCM size, SCM host_ptr);
// TODO create sub-buffer
SCM scm_make_sampler (SCM context, SCM normalized_coords,
                      SCM addressing_mode, SCM filter_mode);
SCM scm_make_cl_program (SCM context, SCM device, SCM sourcecode);
SCM scm_build_cl_program (SCM context, SCM device, SCM sourcecode);
// TODO binary programs
SCM scm_make_cl_kernel (SCM program, SCM name);
SCM scm_read_buffer(SCM command_queue, SCM buffer,
                    SCM blocking_read, // TODO drop blocking mode
                    SCM offset, SCM size, SCM ptr,
                    SCM event_wait_list); // -> event
SCM scm_write_buffer (SCM command_queue, SCM buffer,
                      SCM blocking_write,
                      SCM offset, SCM size, SCM ptr,
                      SCM event_wait_list); // -> event
SCM scm_map_buffer (SCM command_queue, SCM buffer,
                    SCM blocking_map,
                    SCM flags,
                    SCM offset, SCM size,
                    SCM event_wait_list); // -> event
SCM scm_copy_cl_buffer (SCM command_queue,
                        SCM src_buffer, SCM dst_buffer,
                        SCM src_offset, SCM dst_offset, SCM size,
                        SCM event_wait_list); // -> event
SCM scm_fill_buffer(SCM command_queue, SCM buffer,
                    SCM pattern, // bytevector
                    SCM offset, SCM size,
                    SCM event_wait_list);
SCM scm_map_buffer (SCM command_queue, SCM buffer,
                    SCM blocking_map, SCM flags,
                    SCM offset, SCM size,
                    SCM event_wait_list); // -> bytevector, event
SCM scm_unmap_mem (SCM command_queue, SCM mem,
                   SCM ptr, SCM event_wait_list); // -> event
SCM scm_enqueue_marker (SCM command_queue, SCM event_wait_list); // -> event
SCM scm_enqueue_barrier (SCM command_queue, SCM mems, SCM flags);
SCM scm_enqueue_kernel (SCM command_queue,
                        SCM offset_list, SCM global_list, SCM local_list,
                        SCM event_wait_list); // -> event
SCM scm_enqueue_wait (SCM command_queue, SCM event_wait_list); // -> event
SCM scm_cl_flush (SCM command_queue);
SCM scm_cl_finish (SCM command_queue);
// TODO read buffer rect
// TODO write buffer rect
// TODO copy buffer rect

// TODO images, native kernels, sampler, CL/GL interop

SCM scm_cl_foo(SCM scm_queue, SCM scm_kernel, SCM scm_a, SCM scm_b, SCM scm_c, SCM scm_n);

