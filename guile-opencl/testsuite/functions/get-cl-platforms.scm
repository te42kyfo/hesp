(use-modules
 (guile-opencl)
 (srfi srfi-1)
 (srfi srfi-64))

(test-begin "get-cl-platforms")

(test-assert
 (every cl-platform? (get-cl-platforms)))

(test-end "get-cl-platforms")
(exit (= (test-runner-fail-count (test-runner-current)) 0))
