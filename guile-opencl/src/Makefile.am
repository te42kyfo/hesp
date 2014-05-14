lib_LTLIBRARIES = libguile-opencl.la
libguile_opencl_la_CFLAGS = $(GUILE_CFLAGS)
libguile_opencl_la_LDFLAGS = -module  $(GUILE_LDFLAGS) $(AM_CFLAGS) -lOpenCL

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

CFILES = \
buffer.c \
constants.c \
context.c \
conversion.c \
device.c \
error.c \
event.c \
image.c \
info.c \
init.c \
kernel.c \
platform.c \
predicates.c \
print.c \
program.c \
queue.c \
sampler.c

HFILES = \
buffer.h \
config.h \
constants.h \
context.h \
conversion.h \
device.h \
error.h \
event.h \
guile-opencl.h \
image.h \
info.h \
init.h \
kernel.h \
platform.h \
predicates.h \
print.h \
program.h \
queue.h \
sampler.h

libguile_opencl_la_SOURCES = $(CFILES) $(HFILES)

include_HEADERS = guile-opencl.h

headersdir = $(includedir)/libguile-opencl
dist_headers_HEADERS = $(HFILES)

BUILT_SOURCES = $(DOT_X_FILES)
CLEANFILES    = $(DOT_X_FILES)

snarfcppopts = $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
               $(CFLAGS) $(AM_CFLAGS) $(GUILE_CFLAGS)

SUFFIXES = .x
.c.x:
	$(guile_snarf) -o $@ $< $(snarfcppopts)
