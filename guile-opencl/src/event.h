/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>
#include <CL/cl.h>

void scm_to_cl_event_wait_list(SCM events,
                               cl_uint *num_events_in_wait_list,
                               cl_event **event_wait_list);

SCM scm_cl_wait(SCM events);

void guile_opencl_init_event();
