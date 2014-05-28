.PHONY: guile-opencl all clean

hesp: guile-opencl

guile-opencl:
	tar xf guile-opencl-0.1.tar.gz
	cd guile-opencl-0.1 && ./configure --prefix=`pwd` && make && make install

all:
	cd guile-opencl && make install -s -j8
