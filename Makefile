.PHONY: guile-opencl clean hesp
CC      := gcc
FLAGS   := -O2 -std=c99 -Wall
INCS    := -I/usr/lib64/OpenCL/global/include
CFLAGS  := ${FLAGS} `pkg-config --cflags guile-2.0` ${INCS}
LDFLAGS := ${FLAGS} `pkg-config --libs guile-2.0` -Lguile-opencl -lguile-opencl
SRCS    := $(shell echo *.c)
OBJS    := $(SRCS:.c=.o)

hesp-start: guile-opencl ${OBJS}
	${CC} ${LDFLAGS} ${OBJS} -o $@

guile-opencl:
	cd guile-opencl && $(MAKE)

clean:
	rm -f *.o hesp-start
	cd guile-opencl && $(MAKE) clean
