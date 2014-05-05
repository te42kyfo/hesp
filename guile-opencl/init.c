/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#include <libguile.h>
#include "init.h"
#include "guile-opencl.h"

void init_guile_opencl() {
    guile_opencl_tag = scm_make_smob_type("OpenCL", sizeof(void *));
    scm_set_smob_print(guile_opencl_tag, print_guile_opencl);

/* The following init calls can be created via this script
   ls | grep .h | sed '/guile-opencl.h/d' | sed 's/^\(.*\).h/guile_opencl_init_\1();/'
*/

    guile_opencl_init_buffer();
    guile_opencl_init_constants();
    guile_opencl_init_context();
    guile_opencl_init_conversion();
    guile_opencl_init_device();
    guile_opencl_init_error();
    guile_opencl_init_event();
    guile_opencl_init_image();
    guile_opencl_init_info();
    guile_opencl_init_kernel();
    guile_opencl_init_platform();
    guile_opencl_init_predicates();
    guile_opencl_init_print();
    guile_opencl_init_program();
    guile_opencl_init_queue();
    guile_opencl_init_sampler();
}
