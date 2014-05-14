.PHONY: hesp

hesp:
#	cd guile-opencl && ./configure --prefix=`pwd` && make && make install
	cd guile-opencl && make install -s -j8
	guild compile hesp
