/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include "CL/cl.h"
#include "kernel.h"
#include "conversion.h"
#include "error.h"
#include "predicates.h"
#include "stdbool.h"

SCM_SYMBOL (sym_cl_bool, "cl_bool");
SCM_SYMBOL (sym_cl_char, "cl_char");
SCM_SYMBOL (sym_cl_uchar, "cl_uchar");
SCM_SYMBOL (sym_cl_short, "cl_short");
SCM_SYMBOL (sym_cl_ushort, "cl_ushort");
SCM_SYMBOL (sym_cl_int, "cl_int");
SCM_SYMBOL (sym_cl_uint, "cl_uint");
SCM_SYMBOL (sym_cl_long, "cl_long");
SCM_SYMBOL (sym_cl_ulong, "cl_ulong");
SCM_SYMBOL (sym_cl_float, "cl_float");
SCM_SYMBOL (sym_cl_double, "cl_double");
SCM_SYMBOL (sym_cl_buffer, "cl_buffer");
SCM_SYMBOL (sym_cl_image, "cl_image");
SCM_SYMBOL (sym_cl_sampler, "cl_sampler");

SCM_DEFINE (scm_make_cl_kernel, "make-cl-kernel", 2, 0, 1,
            (SCM program, SCM name, SCM args),
            "Create an OpenCL kernel referring to the function named"
            "@var{name} in the OpenCL program @var{program}")
#define FUNC_NAME s_scm_make_cl_kernel
{
    cl_program p = scm_to_cl_program_here(program);
    cl_int     err;
    cl_kernel  kernel;
    scm_dynwind_begin(0);
    char      *c_name = scm_to_utf8_string(name);;
    kernel = clCreateKernel(p, c_name, &err);
    scm_dynwind_free(c_name);
    scm_dynwind_end();
    CL_CHECK( err );
    return scm_from_cl_kernel(kernel, args);
}
#undef FUNC_NAME

SCM_DEFINE (scm_set_cl_kernel_args, "set-cl-kernel-args", 1, 0, 1,
            (SCM kernel, SCM args),
            "Apply all given arguments @var{args} at their respective\n"
            "positions to the kernel @var{kernel}. An error is thrown\n"
            "if the given arguments do not match with the type list given\n"
            "at kernel creation.")
#define FUNC_NAME s_scm_set_cl_kernel_args
{
    SCM_ASSERT_TYPE(scm_to_bool(scm_cl_kernel_p(kernel)),
                    kernel, SCM_ARG1, FUNC_NAME, "cl-kernel");

    typed_cl_kernel *tk    = (typed_cl_kernel *)SCM_SMOB_DATA(kernel);
    size_t           typec = tk->typec;
    scm_cl_arg_type *types = tk->types;
    cl_kernel        k     = tk->kernel;

    size_t index = 0;
    SCM    rest  = args;
    while(!scm_to_bool(scm_null_p(rest))) {
        if(index > typec) scm_wrong_num_args(scm_from_utf8_string (FUNC_NAME));
        scm_cl_arg_type type = types[index];
        SCM arg = scm_car(rest);
        rest = scm_cdr(rest);
        switch(type) {
#define CONVERT_NATIVE(what, type)                                      \
            case what: {                                                \
                type c_arg = scm_to_##type (arg);                       \
                CL_CHECK( clSetKernelArg(k, index, sizeof(type),        \
                                         &c_arg));                      \
                break;                                                  \
            }
            CONVERT_NATIVE(SCM_TYPE_CL_BOOL,   cl_bool);
            CONVERT_NATIVE(SCM_TYPE_CL_CHAR,   cl_char);
            CONVERT_NATIVE(SCM_TYPE_CL_UCHAR,  cl_uchar);
            CONVERT_NATIVE(SCM_TYPE_CL_SHORT,  cl_short);
            CONVERT_NATIVE(SCM_TYPE_CL_USHORT, cl_ushort);
            CONVERT_NATIVE(SCM_TYPE_CL_INT,    cl_int);
            CONVERT_NATIVE(SCM_TYPE_CL_UINT,   cl_uint);
            CONVERT_NATIVE(SCM_TYPE_CL_LONG,   cl_long);
            CONVERT_NATIVE(SCM_TYPE_CL_ULONG,  cl_ulong);
            CONVERT_NATIVE(SCM_TYPE_CL_FLOAT,  cl_float);
            CONVERT_NATIVE(SCM_TYPE_CL_DOUBLE, cl_double);
#undef CONVERT_NATIVE
#define CONVERT_CL(what, type)                                          \
            case what: {                                                \
                type c_arg = scm_to_##type (arg, FUNC_NAME);             \
                CL_CHECK( clSetKernelArg(k, index, sizeof(type),        \
                                         &c_arg));                      \
                break;                                                  \
            }
            CONVERT_CL(SCM_TYPE_CL_BUFFER, cl_mem);
            CONVERT_CL(SCM_TYPE_CL_IMAGE, cl_mem);
            CONVERT_CL(SCM_TYPE_CL_SAMPLER, cl_sampler);
#undef CONVERT_CL
        default:
            scm_misc_error(FUNC_NAME, "invalid kernel argument type", kernel);
        }
        ++index;
    }
    if(index != typec) scm_wrong_num_args(scm_from_locale_string (FUNC_NAME));

    scm_remember_upto_here_1(kernel);
    return kernel;
}
#undef FUNC_NAME

SCM_DEFINE (scm_enqueue_cl_kernel, "enqueue-cl-kernel", 5, 0, 1,
            (SCM queue, SCM kernel,
             SCM offsets, SCM globals, SCM locals,
             SCM events),
            "Enqueue the OpenCL kernel @var{kernel} in the OpenCL queue\n"
            "@var{queue}. @var{offsets}, @var{globals} and\n"
            "@var{locals} define the index offset, index size and\n"
            "local work group size of the operation respectively.\n"
            "@var{events} is a list of cl_events that have to be completed\n"
            "before the operation starts.\n")
#define FUNC_NAME s_scm_enqueue_cl_kernel
{
    size_t offset_dims;
    size_t global_dims;
    size_t local_dims;
    SCM_VALIDATE_LIST_COPYLEN(SCM_ARG3, offsets, offset_dims);
    SCM_VALIDATE_LIST_COPYLEN(SCM_ARG4, globals, global_dims);
    SCM_VALIDATE_LIST_COPYLEN(SCM_ARG5, locals ,  local_dims);
    cl_command_queue cq  = scm_to_cl_command_queue_here(queue);
    cl_kernel        ker = scm_to_cl_kernel_here(kernel);
    if(!( offset_dims == global_dims && global_dims == local_dims))
        scm_misc_error(FUNC_NAME,
                       "~a, ~a and ~a are not of the same length",
                       scm_list_3(offsets, globals, locals));
    size_t dims = offset_dims;
    size_t offset[offset_dims];
    size_t global[global_dims];
    size_t local [local_dims ];
    for(size_t index = 0; index < dims; ++index) {
        SCM scm_index = scm_from_size_t(index);
        offset[index] = scm_to_size_t(scm_list_ref( offsets, scm_index));
        global[index] = scm_to_size_t(scm_list_ref( globals, scm_index));
        local [index] = scm_to_size_t(scm_list_ref( locals , scm_index));
    }
    // TODO event wait list
    cl_event event;
    CL_CHECK( clEnqueueNDRangeKernel(cq, ker,
                                     dims, offset, global, local,
                                     0, NULL, &event) );
    return scm_from_cl_event(event);
}
#undef FUNC_NAME

void guile_opencl_init_kernel() {
#ifndef SCM_MAGIC_SNARFER
#include "kernel.x"
#endif
    scm_define (sym_cl_bool,    scm_from_uint8 (SCM_TYPE_CL_BOOL));
    scm_define (sym_cl_char,    scm_from_uint8 (SCM_TYPE_CL_CHAR));
    scm_define (sym_cl_uchar,   scm_from_uint8 (SCM_TYPE_CL_UCHAR));
    scm_define (sym_cl_short,   scm_from_uint8 (SCM_TYPE_CL_SHORT));
    scm_define (sym_cl_ushort,  scm_from_uint8 (SCM_TYPE_CL_USHORT));
    scm_define (sym_cl_int,     scm_from_uint8 (SCM_TYPE_CL_INT));
    scm_define (sym_cl_uint,    scm_from_uint8 (SCM_TYPE_CL_UINT));
    scm_define (sym_cl_long,    scm_from_uint8 (SCM_TYPE_CL_LONG));
    scm_define (sym_cl_ulong,   scm_from_uint8 (SCM_TYPE_CL_ULONG));
    scm_define (sym_cl_float,   scm_from_uint8 (SCM_TYPE_CL_FLOAT));
    scm_define (sym_cl_double,  scm_from_uint8 (SCM_TYPE_CL_DOUBLE));
    scm_define (sym_cl_buffer,  scm_from_uint8 (SCM_TYPE_CL_BUFFER));
    scm_define (sym_cl_image,   scm_from_uint8 (SCM_TYPE_CL_IMAGE));
    scm_define (sym_cl_sampler, scm_from_uint8 (SCM_TYPE_CL_SAMPLER));
}

