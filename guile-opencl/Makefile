SRCS      := $(shell echo *.c)
OBJS      := $(SRCS:.c=.o)
INCS      := -I/usr/lib64/OpenCL/global/include
HEADERS   := guile_opencl.h
CFLAGS    := -O2 -std=c99 -Wall -Wno-strict-aliasing \
            `pkg-config --cflags guile-2.0`
LDFLAGS   := -O2 -std=c99 -Wall \
            `pkg-config --libs guile-2.0` \
             -lOpenCL
CC        := gcc

SNARF_SRCS := $(shell echo scm_*.c)
SNARF_INCS := $(SNARF_SRCS:.c=.x)

.PHONY: clean

libguile-opencl.so: | $(SNARF_INCS) $(OBJS)
	$(CC) $(LDFLAGS) -shared -fPIC -o $@ $(OBJS)

%.o : %.c %.h
	$(CC) $(CFLAGS) $(INCS) -shared -fPIC -c $< -o $@

%.x : %.c
	guile-snarf -o $@ $(CFLAGS) $(INCS) $<

clean:
	rm -f *.o *.x *.so
