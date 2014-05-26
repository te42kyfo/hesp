/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <CL/cl.h>
#include "event.h"
#include "error.h"
#include "conversion.h"

void scm_to_cl_event_wait_list(SCM events,
                               cl_uint *num_events_in_wait_list,
                               cl_event **event_wait_list)
#define FUNC_NAME "scm_to_cl_event_wait_list"
{
    size_t num_events;
    SCM_VALIDATE_LIST_COPYLEN(SCM_ARG1, events, num_events);
    size_t    size = sizeof(cl_event) * num_events;
    cl_event *event_buffer = scm_gc_malloc(size, "cl-event buffer");

    size_t index = 0;
    SCM rest = events;
    while(!scm_is_null(rest) && index < num_events) {
        SCM event = scm_car(rest);
        rest      = scm_cdr(rest);
        event_buffer[index] = scm_to_cl_event_here(event);
        ++index;
    }
    *num_events_in_wait_list = index;
    *event_wait_list         = event_buffer;
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_wait, "enqueue-cl-wait", 0, 0, 1,
            (SCM events),
            "Wait for all given cl-events in @var{events} to complete.")
#define FUNC_NAME s_scm_cl_wait
{
    cl_uint   num_events;
    cl_event *event_list;
    scm_dynwind_begin(0);
    scm_to_cl_event_wait_list(events,
                              &num_events,
                              &event_list);

    cl_event event;
    CL_CHECK( clWaitForEvents(num_events,
                              event_list) );
    scm_dynwind_free(event_list);
    scm_dynwind_end();
    return SCM_UNDEFINED;
}
#undef FUNC_NAME


void guile_opencl_init_event() {
#ifndef SCM_MAGIC_SNARFER
#include "event.x"
#endif
}
