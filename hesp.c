#include <libguile.h>

static void
inner_main (void *closure, int argc, char **argv) {
    scm_primitive_load (scm_from_locale_string("ex01.scm"));
    scm_eval_string(scm_from_locale_string("(main #f)"));
}

int main (int argc, char **argv) {
    scm_boot_guile (argc, argv, inner_main, 0);
    return 0;
}
