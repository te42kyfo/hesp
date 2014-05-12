/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>
#include "CL/cl.h"

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

void guile_opencl_init_error();
