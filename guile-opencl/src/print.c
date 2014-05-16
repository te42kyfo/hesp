/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include "print.h"
#include "info.h"
#include "constants.h"

int print_guile_opencl(SCM smob, SCM port, scm_print_state *pstate) {
    scm_t_bits tag = SCM_SMOB_FLAGS(smob);

    if(tag == cl_platform_tag) {
        SCM key = SCM_CL_PLATFORM_NAME;
        scm_puts("#<cl-platform: ", port);
        scm_display(scm_cl_platform_info(smob, key), port);
        scm_puts(">", port);
        return 1;
    }
    if(tag == cl_device_tag) {
        SCM key = SCM_CL_DEVICE_NAME;
        scm_puts("#<cl-device: ", port);
        scm_display(scm_cl_device_info(smob, key), port);
        scm_puts(">", port);
        return 1;
    }
    if(tag == cl_context_tag) {
        SCM key = SCM_CL_CONTEXT_DEVICES;
        scm_puts("#<cl-context: ", port);
        scm_display(scm_cl_context_info(smob, key), port);
        scm_puts(">", port);
        return 1;
    }
    if(tag == cl_command_queue_tag) {
        scm_puts("#<cl-command-queue: >", port); // TODO
        return 1;
    }
    if(tag == cl_buffer_tag) {
        scm_puts("#<cl-buffer: >", port); // TODO
        return 1;
    }
    if(tag == cl_image_tag) {
        scm_puts("#<cl-image: >", port); // TODO
        return 1;
    }
    if(tag == cl_program_tag) {
        scm_puts("#<cl-program: >", port); // TODO
        return 1;
    }
    if(tag == cl_kernel_tag) {
        scm_puts("#<cl-kernel: >", port); // TODO
        return 1;
    }
    if(tag == cl_event_tag) {
        scm_puts("#<cl-event: >", port); // TODO
        return 1;
    }
    if(tag == cl_sampler_tag) {
        scm_puts("#<cl-sampler: >", port); // TODO
        return 1;
    }
    scm_misc_error(__func__, "smob of invalid type", SCM_EOL);
    return 1;
}

void guile_opencl_init_print() {
#ifndef SCM_MAGIC_SNARFER
#include "print.x"
#endif
}
