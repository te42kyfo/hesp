.PHONY: guile-opencl clean hesp
CC      := gcc
FLAGS   := -O2 -std=c99 -Wall
CFLAGS  := ${FLAGS} `pkg-config --cflags guile-2.0`
LDFLAGS := ${FLAGS} `pkg-config --libs guile-2.0`
SRCS    := $(shell echo *.c)
OBJS    := $(SRCS:.c=.o)

hesp: guile-opencl ${OBJS}
	${CC} ${LDFLAGS} ${OBJS} -o hesp

guile-opencl:
	cd guile-opencl && $(MAKE)

clean:
	rm -f *.o hesp
	cd guile-opencl && $(MAKE) clean
