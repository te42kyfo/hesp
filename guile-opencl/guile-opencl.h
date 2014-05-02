#pragma once
#include <libguile.h>
#include <CL/cl.h>

#define HAVE_GUILE_OPENCL 1

#include "scm_cl_predicates.h"
#include "scm_cl_info.h"
#include "scm_cl_constants.h"
#include "error.h"
#include "conversion.h"
#include "print.h"
#include "init.h"
#include "base.h"
