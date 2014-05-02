(use-modules (ice-9 pretty-print)
             (ice-9 format)
             (ice-9 rdelim)
             (ice-9 iconv)
             (ice-9 binary-ports)
             (ice-9 receive)
             (rnrs bytevectors)
             (system foreign))

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
  (let* ((platform (hesp-select (cl-get-platforms) "platform"))
         (device (hesp-select (cl-get-devices platform) "device"))
         (context (cl-make-context device))
         (queue (cl-make-queue context device)))
    ; now the fancy buffer copying part
    (let* ((hello (string->bytevector "Hello OpenCL!" "US-ASCII"))
           (hello-len (bytevector-length hello))
           (hello-ptr (bytevector->pointer hello))
           (hello-buf (cl-make-buffer context CL_MEM_USE_HOST_PTR hello-len hello-ptr))
           (bye (string->bytevector "Bye OpenCL!!!" "US-ASCII"))
           (bye-len (bytevector-length bye))
           (bye-ptr (bytevector->pointer bye))
           (bye-buf (cl-make-buffer context CL_MEM_USE_HOST_PTR bye-len bye-ptr))
           (qmarks (make-bytevector 15 (char->integer #\?)))
           (stars (make-bytevector 15 (char->integer #\*))))

      (cl-write-buffer queue hello-buf #t 0 hello-len hello-ptr #f)
      (cl-read-buffer queue hello-buf #t 0 hello-len (bytevector->pointer qmarks) #f)
      (format #t "~a~%" (bytevector->string qmarks "US-ASCII"))

      (cl-copy-buffer queue hello-buf bye-buf 0 0 (min hello-len bye-len) #f)
      (receive (bv event)
          (cl-map-buffer queue bye-buf #t CL_MAP_READ 0 bye-len #f)
        (bytevector-copy! bv 0 stars 0 bye-len)
        (format #t "~a~%" (bytevector->string stars "US-ASCII"))
        (cl-unmap-mem queue bye-buf (bytevector->pointer bv) #f)))
    ; now a simple kernel
    (let* ((a (make-f32vector 13 1.0))
           (a-len (bytevector-length a))
           (a-ptr (bytevector->pointer a))
           (a-buf (cl-make-buffer context CL_MEM_USE_HOST_PTR a-len a-ptr))
           (b (make-f32vector 13 1.0))
           (b-len (bytevector-length b))
           (b-ptr (bytevector->pointer b))
           (b-buf (cl-make-buffer context CL_MEM_USE_HOST_PTR b-len b-ptr))
           (c (make-f32vector 13 0.0))
           (c-len (bytevector-length c))
           (c-ptr (bytevector->pointer c))
           (c-buf (cl-make-buffer context CL_MEM_USE_HOST_PTR c-len c-ptr))
           (sourcecode "
kernel void add(global float∗ a, global float ∗b, global float∗ c, unsigned int n) {
const size_t i = get_global_id(0);
if(i < n) c[i] = a[i] + b[i];
}
")
           (program (cl-build-program
                     (cl-make-program context device sourcecode)
                     (list device)
                     ""))
           (kernel (cl-make-kernel program "add")))
      (cl-write-buffer queue a-buf #t 0 a-len a-ptr #f)
      (cl-write-buffer queue b-buf #t 0 b-len b-ptr #f)
      ; no time to make the kernel application properly, so have this stub instead
      (foo queue kernel a-buf b-buf c-buf 13)
      (cl-read-buffer queue c-buf #t 0 c-len (bytevector->pointer c) #f)
      (format #t "~{~a~%~}~%" (f32vector->list c)))))
