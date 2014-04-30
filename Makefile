.PHONY: guile-opencl clean hesp

hesp: guile-opencl
	guile -s hesp

guile-opencl:
	cd guile-opencl && $(MAKE)

clean:
	cd guile-opencl && $(MAKE) clean
