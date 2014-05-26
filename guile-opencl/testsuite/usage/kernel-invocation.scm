(use-modules (guile-opencl)
             (rnrs base)
             (srfi srfi-1)
             (srfi srfi-64))

(define (for-each-context fun)
  (for-each
   (lambda (platform)
     (for-each
      (lambda (device)
        (fun (make-cl-context device)))
      (get-cl-devices platform)))
   (get-cl-platforms)))

(define kernel-src "
__kernel void foo(int i, __global int * buf) {
    const int ix = get_global_id(0);
    const int iy = get_global_id(1);
    const int iz = get_global_id(2);
    const size_t nx = get_global_size(0);
    const size_t ny = get_global_size(1);
    const size_t nz = get_global_size(2);

    int index = ix
              + iy * nx
              + iz * nx * ny;
    buf[index] = i;
}")

(test-begin "kernel-invocation")

(for-each-context
 (lambda (context)
   (let* ((device        (car (cl-info context CL_CONTEXT_DEVICES)))
          (queue         (make-cl-queue context device))
          (program       (build-cl-program (string->cl-program context kernel-src)
                                           (list device)))
          (kernel        (make-cl-kernel program "foo" cl_int cl_buffer))
          (host-buffer   (make-s32vector 27 0))
          (device-buffer (make-cl-buffer context CL_MEM_READ_ONLY (* 27 4))))
     (set-cl-kernel-args kernel 42 device-buffer)

     (test-error 'misc-error
      (enqueue-cl-kernel queue kernel '(0 0 0) '(3 3 3) '(3 3 3 3)))

     (test-error 'misc-error
      (enqueue-cl-kernel queue kernel '(0 0 0) '(3 3) '(3 3 3)))

     (enqueue-cl-kernel queue kernel '(0 0 0) '(3 3 3) '(3 3 3))
     (enqueue-read-cl-buffer queue device-buffer 0 host-buffer)
     (cl-finish queue)

     (test-assert
      "correct result"
      (every (lambda (x) (= x 42)) (s32vector->list host-buffer))))))

(test-end "kernel-invocation")
(exit (= (test-runner-fail-count (test-runner-current)) 0))


