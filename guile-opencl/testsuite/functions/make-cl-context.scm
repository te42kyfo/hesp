(use-modules
 (guile-opencl)
 (srfi srfi-1)
 (srfi srfi-64))
; zero devices - incorrect
; one device - correct

(test-begin "make-cl-context")

(test-assert
 (let ((contexts (map
                  (lambda (platform)
                    (make-cl-context (car (get-cl-devices platform))))
                  (get-cl-platforms))))
   (every cl-context? contexts)))

(test-error (make-cl-context (list)))

(test-error (make-cl-context (lambda (x) x)))

(test-end "make-cl-context")
(exit (= (test-runner-fail-count (test-runner-current)) 0))
