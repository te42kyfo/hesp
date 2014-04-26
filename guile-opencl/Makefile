SOURCES :=  $(shell echo *.c)
OBJS    := $(SOURCES:.c=.o)
HEADERS := guile_opencl.h
CFLAGS  := -O2 -std=c99 \
           `pkg-config --cflags guile-2.0` \
           -I/usr/lib64/OpenCL/global/include
LDFLAGS := -O2 -std=c99 \
           `pkg-config --libs guile-2.0` \
           -lOpenCL
CC      := gcc

.PHONY: clean install

libguile-opencl.so: $(OBJS)
	$(CC) $(LDFLAGS) -shared -fPIC -o $@ $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -shared -fPIC -c $<

clean:
	rm -f *.o *.lo *.so *.la
