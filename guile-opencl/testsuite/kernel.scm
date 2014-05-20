(use-modules (guile-opencl)
             (rnrs base)
             (srfi srfi-1))

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

(for-each-context
 (lambda (context)
   (let* ((device        (car (cl-info context CL_CONTEXT_DEVICES)))
          (queue         (make-cl-queue context device))
          (program       (build-cl-program (string->cl-program context kernel-src)
                                           (list device)))
          (kernel        (make-cl-kernel program "foo" cl_int cl_buffer))
          (host-buffer   (make-s32vector 27 0))
          (device-buffer (make-cl-buffer context CL_MEM_READ_ONLY (* 27 4))))

     (if (cl-kernel? kernel) #t
         (error "cl-kernel?" "did not recognize a valid cl-kernel: " kernel))

     (if (equal? (cl-kernel-info kernel CL_KERNEL_FUNCTION_NAME) "foo") #t
         (error "cl-kernel-info" "invalid CL_KERNEL_FUNCTION_NAME"))
     (if (cl-context? (cl-kernel-info kernel CL_KERNEL_CONTEXT)) #t
         (error "cl-kernel-info" "invalid CL_KERNEL_CONTEXT"))
     (if (cl-program? (cl-kernel-info kernel CL_KERNEL_PROGRAM)) #t
         (error "cl-kernel-info" "invalid CL_KERNEL_PROGRAM"))
     (if (= (cl-kernel-info kernel CL_KERNEL_NUM_ARGS) 2) #t
         (error "cl-kernel-info" "wrong CL_KERNEL_NUM_ARGS"))

     (set-cl-kernel-args kernel 42 device-buffer)

     (let ((check-enqueue-cl-kernel
            (lambda (queue kernel offsets globals locals)
              (catch #t
                (lambda ()
                  (enqueue-cl-kernel queue kernel offsets globals locals)
                  (throw 'wrong-args-not-detected))
                (lambda (key . args)
                  (if (eqv? key 'wrong-args-not-detected)
                      (error "set-cl-kernel-args"
                             "failed to detect invalid arguments"
                             offsets globals locals)))))))
       (check-enqueue-cl-kernel queue kernel '(0 0 0) '(3 3 3) '(3 3 3 3))
       (check-enqueue-cl-kernel queue kernel '(0)     '(3 3 3) '(3 3 3))
       (check-enqueue-cl-kernel queue kernel '(0 0 0)   '(3 3) '(3 3 3))
       (check-enqueue-cl-kernel queue kernel '(0 0 0) '(3 3 3) '(3 3)))

     (enqueue-cl-kernel queue kernel '(0 0 0) '(3 3 3) '(3 3 3))
     (enqueue-read-cl-buffer queue device-buffer 0 host-buffer)
     (cl-finish queue)
     (if (every (lambda (x) (= x 42)) (s32vector->list host-buffer)) #t
         (error "enqueue-cl-kernel" "incorrect result")))))


