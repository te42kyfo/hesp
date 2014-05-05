/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "program.h"
#include "conversion.h"
#include "error.h"

SCM_DEFINE (scm_string_to_cl_program, "string->cl-program", 2, 0, 0,
            (SCM context, SCM sourcecode),
            "Create a new OpenCL program in context @var{context}"
            "with the sourcecode @var{sourcecode}.") {
    cl_context  c = scm_to_cl_context_here(context);
    size_t      len;
    char       *src;
    cl_int      err;

    scm_dynwind_begin(0);
    src = scm_to_locale_stringn(sourcecode, &len);
    cl_program  program = clCreateProgramWithSource(c, 1,
                                                    (const char **)&src, &len, &err);
    scm_dynwind_free(src);
    scm_dynwind_end();
    CL_CHECK( err );
    return scm_from_cl_program(program);
}

SCM_DEFINE (scm_build_cl_program, "cl-build-program", 3, 0, 0,
            (SCM program, SCM devices, SCM options),
            "Compile the sourcecode of the OpenCL program @var{program}\n"
            "for each OpenCL device given in @var{devices}.\n"
            "Options is a string detailing the compilation.") {
    cl_program   p = scm_to_cl_program_here(program);
    size_t       n = scm_to_size_t(scm_length(devices));
    cl_device_id d[n];
    char        *o = scm_to_locale_string(options);
    SCM rest = devices;
    for(size_t i = 0; i < n; ++i) {
        SCM e    = SCM_CAR(rest);
        rest     = SCM_CDR(rest);
        d[i]     = scm_to_cl_device_id_here(e);
    }
    CL_CHECK( clBuildProgram(p, n, d, o, NULL, NULL) ); // TODO add callback
    return program;
}


SCM_DEFINE (scm_binary_to_cl_program, "binary->cl-program", 3, 0, 0,
            (SCM context, SCM devices, SCM binaries),
            "Create an OpenCL program for context @var{context}\n"
            "from a list of OpenCL devices @var{devices} and a list\n"
            "of same length with bytevectors containing the program\n"
            "binaries." ) {
    return SCM_EOL; // TODO
}

void guile_opencl_init_program() {
#ifndef SCM_MAGIC_SNARFER
#include "program.x"
#endif
}
