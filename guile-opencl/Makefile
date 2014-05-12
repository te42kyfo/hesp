SRCS      := $(shell echo *.c)
OBJS      := $(SRCS:.c=.o)
INCS      := -I/usr/lib64/OpenCL/global/include
HEADERS   := guile_opencl.h
FLAGS     := -O2 -std=c99 -Wall -Werror
CFLAGS    := $(FLAGS) -Wno-strict-aliasing \
            `pkg-config --cflags guile-2.0`
LDFLAGS   := $(FLAGS) \
            `pkg-config --libs guile-2.0` \
             -lOpenCL
CC        := gcc

.PHONY: clean asm

libguile-opencl.so: $(OBJS)
	$(CC) $(LDFLAGS) -shared -fPIC -o $@ $(OBJS)

%.s: %.c %.h %.x
	$(CC) $(CFLAGS) $(INCS) -shared -fPIC -S -c $*.c -o $@

 # cancelling the default implicit rules
%.o: %.c
%.o: %.s

%.o: %.c %.h %.x
	$(CC) $(CFLAGS) $(INCS) -shared -fPIC -c $*.c -o $@

%.x : %.c
	guile-snarf -o $@ $(CFLAGS) $(INCS) $<

clean:
	rm -f *.o *.x *.so *.s
