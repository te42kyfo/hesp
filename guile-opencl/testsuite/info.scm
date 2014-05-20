(use-modules (guile-opencl))

(define kernel-src "__kernel void foo() { return; }")

(let* ((platform      (car (get-cl-platforms)))
       (device        (car (get-cl-devices platform)))
       (context       (make-cl-context device))
       (queue         (make-cl-queue context device))
       (program       (build-cl-program (string->cl-program context kernel-src)
                                        (list device)))
       (kernel        (make-cl-kernel program "foo" cl_int cl_buffer))
       (device-buffer (make-cl-buffer context CL_MEM_READ_ONLY (* 27 4))))
  (map (lambda (x) (cl-info x))
       (list platform device context queue program kernel device-buffer)))

