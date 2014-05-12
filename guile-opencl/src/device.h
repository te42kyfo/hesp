/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

SCM scm_get_cl_devices (SCM platform, SCM flags);
// TODO make_subdevice

void guile_opencl_init_device();
