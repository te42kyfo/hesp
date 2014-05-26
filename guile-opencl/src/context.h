/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

SCM scm_make_cl_context (SCM devices);
SCM scm_make_cl_context_from_type (SCM platform, SCM type);

void guile_opencl_init_context();
