/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include "queue.h"
#include <CL/cl.h>
#include "conversion.h"
#include "error.h"

SCM_DEFINE (scm_make_cl_queue, "make-cl-queue", 2, 1, 0,
            (SCM context, SCM device, SCM flags),
            "Create an OpenCL queue for device @var{device} in context\n"
            "@var{context}. \var{flags} is an optional bitfied that can\n"
            "contain CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE and \n"
            "CL_QUEUE_PROFILING_ENABLE.") {
    cl_context                  c = scm_to_cl_context_here(context);
    cl_device_id                d = scm_to_cl_device_id_here(device);
    cl_command_queue_properties cqp     = 0;
    if(!SCM_UNBNDP(flags)) cqp = scm_to_cl_ulong(flags);
    cl_int err;
    cl_command_queue queue = clCreateCommandQueue(c, d, cqp, &err);
    CL_CHECK( err );
    return scm_from_cl_command_queue(queue);
}

SCM_DEFINE (scm_enqueue_cl_marker, "enqueue-cl-marker", 1, 0, 1,
            (SCM queue, SCM events),
            "Enqueue a marker in queue @var{queue}.\n"
            "@var{events} is a list of cl_events that have to be completed\n"
            "before the operation starts.") {
    return SCM_EOL; // TODO
}

SCM_DEFINE (scm_enqueue_cl_barrier, "enqueue-cl-barrier", 2, 0, 1,
            (SCM queue, SCM mems, SCM events),
            "Enqueue a barrier for the all OpenCL memory objects\n"
            "in the list @var{mems} in the queue @var{queue}."
            "@var{events} is a list of cl_events that have to be completed\n"
            "before the operation starts.") {
    return SCM_EOL; // TODO
}

SCM_DEFINE (scm_enqueue_cl_wait, "enqueue-cl-wait", 1, 0, 1,
            (SCM queue, SCM events),
            "Enqueue a barrier in queue @var{queue}.\n"
            "@var{events} is a list of cl_events that have to be completed\n"
            "before the operation starts.") {
    return SCM_EOL; // TODO
}

SCM_DEFINE (scm_cl_flush, "cl-flush", 1, 0, 0,
            (SCM queue),
            "Flush the OpenCL queue @var{queue}") {
    cl_command_queue q = scm_to_cl_command_queue_here(queue);
    CL_CHECK( clFlush(q) );
    return queue;
}

SCM_DEFINE (scm_cl_finish, "cl-finish", 1, 0, 0,
            (SCM queue),
            "Finish all operations in the OpenCL queue @var{queue}") {
    cl_command_queue q = scm_to_cl_command_queue_here(queue);
    CL_CHECK( clFinish(q) );
    return queue;
}

void guile_opencl_init_queue () {
#ifndef SCM_MAGIC_SNARFER
#include "queue.x"
#endif
}

