SRCS      := $(shell echo *.c)
OBJS      := $(SRCS:.c=.o)
INCS      := -I/usr/lib64/OpenCL/global/include
HEADERS   := guile_opencl.h
CFLAGS    := -O3 -std=c99 -Wall -Wno-strict-aliasing \
            `pkg-config --cflags guile-2.0`
LDFLAGS   := -O2 -std=c99 -Wall \
            `pkg-config --libs guile-2.0` \
             -lOpenCL
CC        := gcc

.PHONY: clean

libguile-opencl.so: $(OBJS)
	$(CC) $(LDFLAGS) -shared -fPIC -o $@ $(OBJS)

%.o: %.c %.h %.x
	$(CC) $(CFLAGS) $(INCS) -shared -fPIC -c $*.c -o $@

%.x : %.c
	guile-snarf -o $@ $(CFLAGS) $(INCS) $<

clean:
	rm -f *.o *.x *.so *.s
