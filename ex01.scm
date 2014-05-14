(use-modules (ice-9 pretty-print)
             (ice-9 format)
             (ice-9 rdelim)
             (ice-9 iconv)
             (ice-9 binary-ports)
             (ice-9 receive)
             (rnrs bytevectors)
             (system foreign))

(load-extension
 (string-concatenate
  (list (getcwd) "/guile-opencl/src/.libs/libguile-opencl.so")) "init_guile_opencl")

; output with exactly the formatting of the reference output
(define (hesp-print obj)
  (let ((fmt (if (cl-platform? obj) "~22a = ~a~%" "~39a = ~a~%")))
    (map (lambda (x)
           (format #t fmt (car x) (cdr x)))
         (cl-info obj))))

(define (hesp-select list name)
  (let* ((len (length list))
         (selection
          (if (= 1 len)
              (car list)
              (begin
                (format #t "Avilable ~as:~%" name)
                (for-each (lambda (item id)
                            (format #t "~:@(~a~) #~d:~%" name id)
                            (hesp-print item) (newline))
                          list (iota len))
                (format #t "Choose a ~a (from 0 to ~d)~%" name (1- len))
                (list-ref list (string->number (read-line)))))))
    (format #t "Using the following ~a:~%" name)
    (hesp-print selection)
    (newline)
    selection))

(define (main args)
  ; platform setup
  (let* ((platform (hesp-select (get-cl-platforms) "platform"))
         (device (hesp-select (get-cl-devices platform) "device"))
         (context (make-cl-context device))
         (queue (make-cl-queue context device)))
    ; now the fancy buffer copying part
    (let* ((hello (string->bytevector "Hello OpenCL!" "US-ASCII"))
           (hello-len (bytevector-length hello))
           (hello-buf (make-cl-buffer context CL_MEM_USE_HOST_PTR hello-len hello))
           (bye (string->bytevector "Bye OpenCL!!!" "US-ASCII"))
           (bye-len (bytevector-length bye))
           (bye-buf (make-cl-buffer context CL_MEM_USE_HOST_PTR bye-len bye))
           (qmarks (make-bytevector 15 (char->integer #\?)))
           (stars (make-bytevector 15 (char->integer #\*))))

      (enqueue-write-cl-buffer queue hello-buf 0 hello #f)
      (cl-finish queue)
      (enqueue-read-cl-buffer  queue hello-buf 0
                       (alias-bytevector qmarks hello-len 0) #f)
      (cl-finish queue)
      (format #t "~a~%" (bytevector->string qmarks "US-ASCII"))

      (enqueue-copy-cl-buffer queue hello-buf bye-buf 0 0 (min hello-len bye-len) #f)
      (receive (bv event)
          (enqueue-map-cl-buffer queue bye-buf CL_MAP_READ 0 bye-len #f)
        (bytevector-copy! bv 0 stars 0 bye-len)
        (format #t "~a~%" (bytevector->string stars "US-ASCII"))
        (enqueue-unmap-cl-buffer queue bye-buf bv #f)))))

(main #f)
