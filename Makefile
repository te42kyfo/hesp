CFLAGS := -O2 -std=c99 \
          `pkg-config --cflags guile-2.0` \
          -I/usr/lib64/OpenCL/global/include -lOpenCL

libguile-opencl.so: guile-opencl.c
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $<
