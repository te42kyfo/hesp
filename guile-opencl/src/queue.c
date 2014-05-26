/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include "queue.h"
#include <CL/cl.h>
#include "conversion.h"
#include "error.h"
#include "event.h"

SCM_DEFINE (scm_make_cl_queue, "make-cl-queue", 2, 1, 0,
            (SCM context, SCM device, SCM flags),
            "Create an OpenCL queue for device @var{device} in context\n"
            "@var{context}. \var{flags} is an optional bitfied that can\n"
            "contain CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE and \n"
            "CL_QUEUE_PROFILING_ENABLE.")
#define FUNC_NAME s_scm_make_cl_queue
{
    cl_context                  cnt = scm_to_cl_context_here(context);
    cl_device_id                dev = scm_to_cl_device_id_here(device);
    cl_command_queue_properties cqp     = 0;
    if(!SCM_UNBNDP(flags)) cqp = scm_to_cl_ulong(flags);
    cl_int err;
    cl_command_queue queue = clCreateCommandQueue(cnt, dev, cqp, &err);
    CL_CHECK( err );
    return scm_from_cl_command_queue(queue);
}
#undef FUNC_NAME

#ifdef CL_VERSION_1_2
SCM_DEFINE (scm_enqueue_cl_marker, "enqueue-cl-marker", 1, 0, 1,
            (SCM queue, SCM events),
            "Returns an event that finishes when all given events\n"
            "@var{events} in the OpenCL command queue @var{queue}\n"
            "have finished.")
#define FUNC_NAME s_scm_enqueue_cl_marker
{
    cl_command_queue cmdqueue = scm_to_cl_command_queue_here(queue);
    cl_uint   num_events_in_wait_list;
    cl_event *event_wait_list;
    scm_dynwind_begin(0);
    scm_to_cl_event_wait_list(events,
                              &num_events_in_wait_list,
                              &event_wait_list);

    cl_event event;
    CL_CHECK( clEnqueueMarker(cmdqueue,
                              num_events_in_wait_list,
                              event_wait_list,
                              &event) );
    scm_dynwind_free(event_wait_list);
    scm_dynwind_end();
    return scm_from_cl_event(event);
}
#undef FUNC_NAME
#endif // CL_VERSION_1_2

#ifdef CL_VERSION_1_2
SCM_DEFINE (scm_enqueue_cl_barrier, "enqueue-cl-barrier", 1, 0, 1,
            (SCM queue, SCM events),
            "Returns an event that finishes when all given events\n"
            "@var{events} in the OpenCL command queue @var{queue}\n"
            "have finished. The barrier blocks further commands\n"
            "that are added to @var{queue} until it has finished.")
#define FUNC_NAME s_scm_enqueue_cl_barrier
{
    cl_command_queue cmdqueue = scm_to_cl_command_queue_here(queue);
    cl_uint   num_events_in_wait_list;
    cl_event *event_wait_list;
    scm_dynwind_begin(0);
    scm_to_cl_event_wait_list(events,
                              &num_events_in_wait_list,
                              &event_wait_list);

    cl_event event;
    CL_CHECK( clEnqueueBarrierWithWaitList(cmdqueue,
                                           num_events_in_wait_list,
                                           event_wait_list,
                                           &event) );
    scm_dynwind_free(event_wait_list);
    scm_dynwind_end();
    return scm_from_cl_event(event);
}
#undef FUNC_NAME
#endif // CL_VERSION_1_2

SCM_DEFINE (scm_cl_flush, "cl-flush", 1, 0, 0,
            (SCM queue),
            "Flush the OpenCL queue @var{queue}")
#define FUNC_NAME s_scm_cl_flush
{
    cl_command_queue q = scm_to_cl_command_queue_here(queue);
    CL_CHECK( clFlush(q) );
    return queue;
}
#undef FUNC_NAME

SCM_DEFINE (scm_cl_finish, "cl-finish", 1, 0, 0,
            (SCM queue),
            "Finish all operations in the OpenCL queue @var{queue}")
#define FUNC_NAME s_scm_cl_finish
{
    cl_command_queue q = scm_to_cl_command_queue_here(queue);
    CL_CHECK( clFinish(q) );
    return queue;
}
#undef FUNC_NAME

void guile_opencl_init_queue () {
#ifndef SCM_MAGIC_SNARFER
#include "queue.x"
#endif
}

