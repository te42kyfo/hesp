/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "program.h"
#include "conversion.h"
#include "error.h"

SCM_DEFINE (scm_string_to_cl_program, "string->cl-program", 2, 0, 0,
            (SCM context, SCM sourcecode),
            "Create a new OpenCL program in context @var{context}"
            "with the sourcecode @var{sourcecode}.")
#define FUNC_NAME s_scm_string_to_cl_program
{
    cl_context  cnt = scm_to_cl_context_here(context);
    size_t      len;
    char       *src;
    cl_int      err;

    scm_dynwind_begin(0);
    src = scm_to_utf8_stringn(sourcecode, &len);
    cl_program  program = clCreateProgramWithSource(cnt, 1,
                                                    (const char **)&src, &len, &err);
    scm_dynwind_free(src);
    scm_dynwind_end();
    CL_CHECK( err );
    return scm_from_cl_program(program);
}
#undef FUNC_NAME

SCM_DEFINE (scm_binary_to_cl_program, "binary->cl-program", 3, 0, 0,
            (SCM context, SCM devices, SCM binaries),
            "Create an OpenCL program for context @var{context}\n"
            "from a list of OpenCL devices @var{devices} and a list\n"
            "of same length with bytevectors containing the program\n"
            "binaries." )
#define FUNC_NAME s_scm_binary_to_cl_program
{
    return SCM_EOL; // TODO
}
#undef FUNC_NAME

SCM_DEFINE (scm_build_cl_program, "build-cl-program", 2, 1, 0,
            (SCM program, SCM devices, SCM options),
            "Compile the sourcecode of the OpenCL program @var{program}\n"
            "for each OpenCL device given in @var{devices}.\n"
            "Options is a string detailing the compilation.")
#define FUNC_NAME s_scm_build_cl_program
{
    cl_program   prog        = scm_to_cl_program_here(program);
    size_t       num_devices = scm_to_size_t(scm_length(devices));
    cl_device_id device_array[num_devices];
    char        *c_options;
    scm_dynwind_begin(0);
    if(SCM_UNBNDP(options)) {
        options = NULL;
    } else {
        options = scm_to_locale_string(options);
    }
    SCM rest = devices;
    for(size_t index = 0; index < num_devices; ++index) {
        SCM device = SCM_CAR(rest);
        rest     = SCM_CDR(rest);
        device_array[index] = scm_to_cl_device_id_here(device);
    }
    CL_CHECK( clBuildProgram(prog,
                             num_devices, device_array,
                             c_options, NULL, NULL) ); // TODO add callback
    scm_dynwind_free(c_options);
    scm_dynwind_end();
    return program;
}
#undef FUNC_NAME

void guile_opencl_init_program() {
#ifndef SCM_MAGIC_SNARFER
#include "program.x"
#endif
}
