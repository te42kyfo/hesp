#include "guile-opencl.h"

scm_t_bits guile_opencl_tag; // extern variable in guile_opencl.h
extern void scm_init_cl_predicates();
extern void scm_init_cl_info();
extern void scm_init_cl_constants();

void init_guile_opencl() {
    guile_opencl_tag = scm_make_smob_type("OpenCL", sizeof(void *));
    scm_set_smob_print(guile_opencl_tag, print_guile_opencl);

    scm_init_cl_predicates();
    scm_init_cl_info();
    scm_init_cl_constants();

    scm_c_define_gsubr("cl-get-platforms",  0, 0, 0, scm_get_cl_platforms);
    scm_c_define_gsubr("cl-get-devices",    1, 0, 0, scm_get_cl_devices);
    scm_c_define_gsubr("cl-make-context",   0, 0, 1, scm_make_context);
    scm_c_define_gsubr("cl-make-queue",     2, 1, 0, scm_make_queue);
    scm_c_define_gsubr("cl-make-buffer",    4, 0, 0, scm_make_buffer);
    scm_c_define_gsubr("cl-read-buffer",    7, 0, 0, scm_read_buffer);
    scm_c_define_gsubr("cl-write-buffer",   7, 0, 0, scm_write_buffer);
    scm_c_define_gsubr("cl-map-buffer",     7, 0, 0, scm_map_buffer);
    scm_c_define_gsubr("cl-unmap-mem",      4, 0, 0, scm_unmap_mem);
    scm_c_define_gsubr("cl-copy-buffer",    7, 0, 0, scm_copy_cl_buffer);

    scm_c_define_gsubr("cl-make-program",   3, 0, 0, scm_make_cl_program);
    scm_c_define_gsubr("cl-build-program",  3, 0, 0, scm_build_cl_program);
    scm_c_define_gsubr("cl-make-kernel",    2, 0, 0, scm_make_cl_kernel);

    // TODO hesp stub
    scm_c_define_gsubr("foo", 6, 0, 0, scm_cl_foo);
}
