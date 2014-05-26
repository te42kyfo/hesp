(use-modules
 (guile-opencl)
 (rnrs bytevectors)
 (srfi srfi-1)
 (srfi srfi-26)
 (srfi srfi-64))

(define contexts
  (map (lambda (platform)
         (make-cl-context (car (get-cl-devices platform))))
       (get-cl-platforms)))

(define array (make-typed-array 'f32 12.3 '(-1 1) '(-2 2)))
(define host-mem (array-contents array))

(test-begin "make-cl-buffer")

(map
 (lambda (context)
   (test-assert
    "USE_HOST_PTR, correct invocation"
    (cl-buffer?
     (make-cl-buffer
      context
      CL_MEM_USE_HOST_PTR
      (bytevector-length host-mem)
      host-mem)))

   (test-error
    "USE_HOST_PTR, size too large" 'misc-error
    (make-cl-buffer
     context
     CL_MEM_USE_HOST_PTR
     (+ (bytevector-length host-mem) 1)
     host-mem))

   (test-assert
    "ALLOC_HOST_PTR, correct invocation"
    (cl-buffer?
     (make-cl-buffer
      context
      CL_MEM_ALLOC_HOST_PTR
      (bytevector-length host-mem))))

   (test-error
    "ALLOC_HOST_PTR, invalid host mem" 'misc-error
    (make-cl-buffer
     context
     CL_MEM_ALLOC_HOST_PTR
     (bytevector-length host-mem)
     host-mem))

   (test-assert
    "COPY_HOST_PTR, correct invocation"
    (cl-buffer?
     (make-cl-buffer
      context
      (logior CL_MEM_ALLOC_HOST_PTR CL_MEM_COPY_HOST_PTR)
      (bytevector-length host-mem)
      host-mem)))

   (test-error
    "COPY_HOST_PTR, no host mem" 'misc-error
    (cl-buffer?
     (make-cl-buffer
      context
      (logior CL_MEM_ALLOC_HOST_PTR CL_MEM_COPY_HOST_PTR)
      (bytevector-length host-mem)))))
 contexts)

(test-end "make-cl-buffer")
(exit (= (test-runner-fail-count (test-runner-current)) 0))

