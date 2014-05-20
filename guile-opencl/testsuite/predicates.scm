(use-modules (guile-opencl)
             (rnrs base)
             (srfi srfi-1))

(define kernel-src "__kernel void foo() { return; }")

(define garbage (list 'foo (lambda (x) x) "foo" 42 #f #t (make-vector 10 42)))

(for-each
 (lambda (pred name)
   (let ((false-positives (filter pred garbage)))
     (if (not (null? false-positives))
         (error name "generated false positives" false-positives))))
 (list cl-platform?
       cl-device?
       cl-context?
       cl-command-queue?
       cl-buffer?
       cl-image?
       cl-program?
       cl-kernel?
       cl-event?
       cl-sampler?)
 (list "cl-platform?"
       "cl-device?"
       "cl-context?"
       "cl-command-queue?"
       "cl-buffer?"
       "cl-image?"
       "cl-program?"
       " cl-kernel?"
       "cl-event?"
       "cl-sampler?"))

(let* ((platform      (car (get-cl-platforms)))
       (device        (car (get-cl-devices platform)))
       (context       (make-cl-context device))
       (queue         (make-cl-queue context device))
       (program       (build-cl-program (string->cl-program context kernel-src)
                                        (list device)))
       (kernel        (make-cl-kernel program "foo" cl_int cl_buffer))
       (device-buffer (make-cl-buffer context CL_MEM_READ_ONLY (* 27 4))))
  (let-syntax ((false-negative-check
                (syntax-rules ()
                  ((_ pred match)
                   (if (not (pred match))
                       (error (symbol->string 'pred)
                              "false negative"
                              match))))))
    (false-negative-check cl-platform?      platform)
    (false-negative-check cl-device?        device)
    (false-negative-check cl-context?       context)
    (false-negative-check cl-command-queue? queue)
    (false-negative-check cl-program?       program)
    (false-negative-check cl-kernel?        kernel)
    (false-negative-check cl-buffer?        device-buffer)))
