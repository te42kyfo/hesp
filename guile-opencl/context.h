/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

SCM scm_make_context (SCM devices); // TODO cl_context_properties
// TODO make_context_from_type

void guile_opencl_init_context();
