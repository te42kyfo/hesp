/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>
#include "CL/cl.h"

/* If the given value is CL_SUCCESS, do nothing, else throw an appropriate
   guile error message. */
#define CL_CHECK(value) cl_check_value(value, FUNC_NAME);
void cl_check_value(cl_int value, const char* subr);

void guile_opencl_init_error();
