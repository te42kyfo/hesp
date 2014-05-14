.PHONY: hesp

hesp:
#	cd guile-opencl && ./configure --prefix=`pwd` && make && make install
	guild compile hesp
