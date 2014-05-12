/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

SCM scm_string_to_cl_program (SCM context, SCM sourcecode);
SCM scm_binary_to_cl_program (SCM context, SCM devices, SCM binaries);
SCM scm_build_cl_program     (SCM context, SCM devices, SCM options);
//SCM scm_compile_cl_program   (SCM program, SCM devices, SCM options, SCM headers); TODO
//SCM scm_link_cl_program      (SCM context, SCM devices, SCM programs); TODO

void guile_opencl_init_program();
