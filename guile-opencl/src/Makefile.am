lib_LTLIBRARIES = libguile_opencl.la
libguile_opencl_la_CFLAGS = $(GUILE_CFLAGS)
libguile_opencl_la_LDFLAGS = -module  $(GUILE_LDFLAGS) $(AM_CFLAGS)
AM_CPPFLAGS = -I/usr/lib64/OpenCL/global/include
DOT_X_FILES = \
buffer.x \
constants.x \
context.x \
conversion.x \
device.x \
error.x \
event.x \
image.x \
info.x \
init.x \
kernel.x \
platform.x \
predicates.x \
print.x \
program.x \
queue.x \
sampler.x

libguile_opencl_la_SOURCES = \
guile-opencl.h \
buffer.c buffer.h \
constants.c constants.h \
context.c context.h \
conversion.c conversion.h \
device.c device.h \
error.c error.h \
event.c event.h \
image.c image.h \
info.c info.h \
init.c init.h \
kernel.c kernel.h \
platform.c platform.h \
predicates.c predicates.h \
print.c print.h \
program.c program.h \
queue.c queue.h \
sampler.c sampler.h

BUILT_SOURCES = $(DOT_X_FILES)
CLEANFILES    = $(DOT_X_FILES)

snarfcppopts = $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
               $(CFLAGS) $(AM_CFLAGS) $(GUILE_CFLAGS)

SUFFIXES = .x
.c.x:
	$(guile_snarf) -o $@ $< $(snarfcppopts)
