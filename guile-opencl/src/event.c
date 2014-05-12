/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "event.h"

void guile_opencl_init_event() {
#ifndef SCM_MAGIC_SNARFER
#include "event.x"
#endif
}
