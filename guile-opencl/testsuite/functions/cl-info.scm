(use-modules (guile-opencl)
             (srfi srfi-1)
             (srfi srfi-26)
             (srfi srfi-64))

(define kernel-src "__kernel void foo() { return; }")

(define (alist? x)
  (if (list? x)
      (reduce (cut and <> <>) #t
              (map (lambda (x) (pair? x)) x))
      #f))

(test-begin "cl-info")

(let* ((platform      (car (get-cl-platforms)))
       (device        (car (get-cl-devices platform)))
       (context       (make-cl-context device))
       (queue         (make-cl-queue context device))
       (program       (build-cl-program (string->cl-program context kernel-src)
                                        (list device)))
       (kernel        (make-cl-kernel program "foo" cl_int cl_buffer))
       (device-buffer (make-cl-buffer context CL_MEM_READ_ONLY (* 27 4))))
  (test-assert
   "alist?"
   (every (lambda (x) (alist? (cl-info x)))
          (list platform device context queue program kernel device-buffer))))

(test-end "cl-info")
(exit (= (test-runner-fail-count (test-runner-current)) 0))
