#pragma once
#include <libguile.h>
#include <CL/cl.h>

/* There can only be 256 different Smob types in the system. Guile-OpenCL
   requires only a single guile type and implements its own type system with
   the 16 extra bits in a Smob. */
extern scm_t_bits guile_opencl_tag;    // set in init_guile_opencl()
static const scm_t_bits cl_platform_tag      =  0x0001;
static const scm_t_bits cl_device_tag        =  0x0002;
static const scm_t_bits cl_context_tag       =  0x0003;
static const scm_t_bits cl_command_queue_tag =  0x0004;
static const scm_t_bits cl_mem_tag           =  0x0005;
static const scm_t_bits cl_program_tag       =  0x0006;
static const scm_t_bits cl_kernel_tag        =  0x0007;
static const scm_t_bits cl_event_tag         =  0x0008;
static const scm_t_bits cl_sampler_tag       =  0x0009;

/* display Guile OpenCL objects */
int print_guile_opencl(SCM smob, SCM port, scm_print_state *pstate);

/* Call from a process linked with libguile to make all Guile OpenCL
   functions avilable. */
void init_guile_opencl();

/* Return SCM_BOOL_T if the argument is of the specified type and SCM_BOOL_F
   otherwise. */
SCM scm_platform_p      (SCM smob);
SCM scm_device_p        (SCM smob);
SCM scm_context_p       (SCM smob);
SCM scm_command_queue_p (SCM smob);
SCM scm_mem_p           (SCM smob);
SCM scm_program_p       (SCM smob);
SCM scm_kernel_p        (SCM smob);
SCM scm_event_p         (SCM smob);
SCM scm_sampler_p       (SCM smob);

/* These commands wrap a given OpenCL handle in a Guile Smob. They do
   always succeed. */
SCM scm_from_cl_platform_id   (cl_platform_id   platform);
SCM scm_from_cl_device_id     (cl_device_id     device);
SCM scm_from_cl_context       (cl_context       context);
SCM scm_from_cl_command_queue (cl_command_queue command_queue);
SCM scm_from_cl_mem           (cl_mem           mem);
SCM scm_from_cl_program       (cl_program       program);
SCM scm_from_cl_kernel        (cl_kernel        kernel);
SCM scm_from_cl_event         (cl_event         event);
SCM scm_from_cl_sampler       (cl_sampler       sampler);

/* These commands convert a given Guile Smob back to the underlying C
   handle. An error is thrown if the smob is of the wrong type */
#define scm_to_cl_platform_id_here(obj)   scm_to_cl_platform_id   (obj, __func__);
#define scm_to_cl_device_id_here(obj)     scm_to_cl_device_id     (obj, __func__);
#define scm_to_cl_context_here(obj)       scm_to_cl_context       (obj, __func__);
#define scm_to_cl_command_queue_here(obj) scm_to_cl_command_queue (obj, __func__);
#define scm_to_cl_mem_here(obj)           scm_to_cl_mem           (obj, __func__);
#define scm_to_cl_program_here(obj)       scm_to_cl_program       (obj, __func__);
#define scm_to_cl_kernel_here(obj)        scm_to_cl_kernel        (obj, __func__);
#define scm_to_cl_event_here(obj)         scm_to_cl_event         (obj, __func__);
#define scm_to_cl_sampler_here(obj)       scm_to_cl_sampler       (obj, __func__);

cl_platform_id   scm_to_cl_platform_id   (SCM platform,      const char *subr);
cl_device_id     scm_to_cl_device_id     (SCM device,        const char *subr);
cl_context       scm_to_cl_context       (SCM context,       const char *subr);
cl_command_queue scm_to_cl_command_queue (SCM command_queue, const char *subr);
cl_mem           scm_to_cl_mem           (SCM mem,           const char *subr);
cl_program       scm_to_cl_program       (SCM program,       const char *subr);
cl_kernel        scm_to_cl_kernel        (SCM kernel,        const char *subr);
cl_event         scm_to_cl_event         (SCM event,         const char *subr);
cl_sampler       scm_to_cl_sampler       (SCM sampler,       const char *subr);

/* If the given value is CL_SUCCESS, do nothing, else throw an appropriate
   guile error message. */
#define CL_CHECK(value) cl_check_value(value, __func__);
void cl_check_value(cl_int value, const char* subr);

/* some OpenCL operations accept a callback function for error reporting. This
   is a generic one that produces a guile error when necessary. The user_data
   pointer must either be NULL or the name of the surrounding function */
void cl_callback(const char *errinfo,
                 const void *private_info,
                 size_t cb,
                 void *user_data);

/* Query information from the OpenCL object. The param name must be a Guile
   string matching the name in the OpenCL specification,
   e.g. "CL_PLATFORM_VENDOR". */
SCM scm_get_platform_info          (SCM platform, SCM param_name);
SCM scm_get_device_info            (SCM device,   SCM param_name);
SCM scm_get_context_info           (SCM context,  SCM param_name);
SCM scm_get_command_queue_info     (SCM queue,    SCM param_name);
SCM scm_get_mem_info               (SCM memobj,   SCM param_name);
SCM scm_get_program_info           (SCM program,  SCM param_name);
SCM scm_get_kernel_info            (SCM kernel,   SCM param_name);
SCM scm_get_sampler_info           (SCM sampler,  SCM param_name);
SCM scm_get_program_build_info     (SCM program,  SCM device, SCM param_name);
SCM scm_get_work_group_info        (SCM kernel,   SCM device, SCM param_name);
SCM scm_get_event_profiling_info   (SCM event,    SCM param_name);

/* Dispatch to specific scm_get_OBJECT_info command depending on type. If
   param name is omitted, return an alist of all avilable information. */
SCM scm_get_info (SCM any,      SCM param_name);

/* The OpenCL API calls. They are very similar to the C++ bindings, except
   that they exploit some Scheme convenience features */
SCM scm_get_platforms ();
SCM scm_get_devices (SCM platform);
// TODO make_subdevice
SCM scm_make_context (SCM devices); // TODO cl_context_properties
// TODO make_context_from_type
SCM scm_make_queue (SCM context, SCM device, SCM flags);
SCM scm_make_buffer (SCM context, SCM flags, SCM size, SCM host_ptr);
// TODO create sub-buffer
SCM scm_make_sampler (SCM context, SCM normalized_coords,
                      SCM addressing_mode, SCM filter_mode);
SCM scm_make_program (SCM context, SCM device, SCM sourcecode);
// TODO binary programs
SCM scm_make_kernel (SCM program, SCM name);
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
SCM scm_copy_buffer (SCM command_queue,
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

