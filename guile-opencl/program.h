/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

SCM scm_string_to_program    (SCM context, SCM sourcecode);
SCM scm_build_cl_program     (SCM context, SCM devices, SCM options);
SCM scm_binary_to_cl_program (SCM context, SCM devices, SCM binaries);

void guile_opencl_init_program();
