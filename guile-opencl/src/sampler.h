/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

// SCM scm_make_sampler (SCM context, SCM normalized_coords,
//                       SCM addressing_mode, SCM filter_mode);

void guile_opencl_init_sampler();
