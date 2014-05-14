/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>

SCM scm_make_cl_queue (SCM context, SCM device, SCM flags);
SCM scm_enqueue_cl_marker (SCM command_queue, SCM event_wait_list); // -> event
SCM scm_enqueue_cl_barrier (SCM command_queue, SCM mems, SCM flags);
SCM scm_enqueue_cl_wait (SCM command_queue, SCM event_wait_list); // -> event
SCM scm_cl_flush (SCM command_queue);
SCM scm_cl_finish (SCM command_queue);

void guile_opencl_init_queue();
