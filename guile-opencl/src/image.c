/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "image.h"

void guile_opencl_init_image() {
#ifndef SCM_MAGIC_SNARFER
#include "image.x"
#endif
}
