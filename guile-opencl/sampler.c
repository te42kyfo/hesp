/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include "sampler.h"
#include <CL/cl.h>

void guile_opencl_init_sampler() {
#ifndef SCM_MAGIC_SNARFER
#include "sampler.x"
#endif
}
