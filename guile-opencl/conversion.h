/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>
#include <CL/cl.h>

/* unlike C99, OpenCL specifies the length of each datatype exactly.This table
   serves as an overview:

   | name           | cl_type   | c_type   |
   |----------------+-----------+----------|
   | bool           | cl_bool   | uint32_t |
   | char           | cl_char   | int8_t   |
   | unsigned char  | cl_uchar  | uint8_t  |
   | short          | cl_short  | int16_t  |
   | unsigned short | cl_ushort | uint16_t |
   | int            | cl_int    | int32_t  |
   | unsigned int   | cl_uint   | uint32_t |
   | long           | cl_long   | int64_t  |
   | unsigned long  | cl_ulong  | uint64_t |
   | float          | cl_float  | float    |
   | double         | cl_double | double   |
   | half           | cl_half   | half     |

   The following macros allow intuitive conversion from cl_* types to SCM and
   vice versa.
   TODO what shall be done with half?
*/
#define scm_from_cl_bool(x)   scm_from_uint32(x)
#define scm_from_cl_char(x)   scm_from_int8  (x)
#define scm_from_cl_uchar(x)  scm_from_uint8 (x)
#define scm_from_cl_short(x)  scm_from_int16 (x)
#define scm_from_cl_ushort(x) scm_from_uint16(x)
#define scm_from_cl_int(x)    scm_from_int32 (x)
#define scm_from_cl_uint(x)   scm_from_uint32(x)
#define scm_from_cl_long(x)   scm_from_int64 (x)
#define scm_from_cl_ulong(x)  scm_from_uint64(x)
#define scm_from_cl_float(x)  scm_from_double((double)x)
#define scm_from_cl_double(x) scm_from_double((double)x)

#define scm_to_cl_bool(x)   scm_to_uint32(x)
#define scm_to_cl_char(x)   scm_to_int8  (x)
#define scm_to_cl_uchar(x)  scm_to_uint8 (x)
#define scm_to_cl_short(x)  scm_to_int16 (x)
#define scm_to_cl_ushort(x) scm_to_uint16(x)
#define scm_to_cl_int(x)    scm_to_int32 (x)
#define scm_to_cl_uint(x)   scm_to_uint32(x)
#define scm_to_cl_long(x)   scm_to_int64 (x)
#define scm_to_cl_ulong(x)  scm_to_uint64(x)
#define scm_to_cl_float(x)  (float)scm_to_double(x)
#define scm_to_cl_double(x) scm_to_double(x)

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

void guile_opencl_init_conversion();
