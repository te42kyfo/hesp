#!/usr/bin/guile \
-e main -s
!#

(setenv "LD_LIBRARY_PATH"
        (string-append
         (getcwd) "/guile-opencl/lib"))

(add-to-load-path
 (string-append
  (getcwd) "/guile-opencl/modules"))

(use-modules (ice-9 pretty-print)
             (ice-9 format)
             (ice-9 rdelim)
             (ice-9 binary-ports)
             (ice-9 receive)
             (rnrs bytevectors)
             (rnrs base)
             (system foreign)
             (guile-opencl))

(load "read-string.scm")

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
    (format #t "Using the following ~a:~%:" name)
    (hesp-print selection)
    (newline)
    selection))

(define (vtk-write N m px py pz vx vy vz port)
  (format port "# vtk DataFile Version 4.0~%")
  (format port "hesp visualization file~%")
  (format port "ASCII~%")
  (format port "DATASET UNSTRUCTURED_GRID~%")
  (format port "POINTS ~d double~%" N)
  (let ((buffers (list px py pz)))
    (do ((i 0 (1+ i)))
        ((>= i N))
      (format port "~{~,6f ~}~%"
              (map (lambda (vec)
                     (realvector-ref vec i))
                   buffers))))
  (format port "CELLS 0 0~%")
  (format port "CELL_TYPES 0~%")
  (format port "POINT_DATA ~d~%" N)
  (format port "SCALARS m double~%" N)
  (format port "LOOKUP_TABLE default~%" N)
  (let ((buffers (list m)))
    (do ((i 0 (1+ i)))
        ((>= i N))
      (format port "~{~,6f ~}~%"
              (map (lambda (vec)
                     (realvector-ref vec i))
                   buffers))))
  (format port "VECTORS v double~%" N)
  (let ((buffers (list vx vy vz)))
    (do ((i 0 (1+ i)))
        ((>= i N))
      (format port "~{~,6f ~}~%"
              (map (lambda (vec)
                     (realvector-ref vec i))
                   buffers)))))

(define (ascii-write N m px py pz vx vy vz port)
  (format port "~d~%" N)
  (let ((buffers (list m px py pz vx vy vz)))
    (do ((i 0 (1+ i)))
        ((>= i N))
      (format port "~{~10,4@f ~}~%"
              (map (lambda (vec)
                     (realvector-ref vec i))
                   buffers)))))

;; these definitions allow both float and double
(define real float)
(define make-realvector make-f32vector)
(define realvector->list f32vector->list)
(define typedef-real "typedef float real;\n")
(define realvector-set! f32vector-set!)
(define realvector-ref f32vector-ref)

(define kernels-src
  (string-concatenate
   (list typedef-real
         (read-string (open-input-file "kernels.cl")))))

(define (main args)

  (if (= 1 (length args))
      (error "main" "please specify a parameter file!" ))
  (let* ((param-file (cadr args))
         (param-directory (dirname param-file))
         (platform (hesp-select (get-cl-platforms) "platform"))
         (device   (hesp-select (get-cl-devices platform) "device"))
         (context  (make-cl-context device))
         (queue    (make-cl-queue context device))
         (program  (build-cl-program
                    (string->cl-program context kernels-src)
                    (list device)
                    ""))
         (build-log (cl-program-build-info program device CL_PROGRAM_BUILD_LOG)))
    (format #t "=== build-log ===\n~a\n== end of build-log ===\n\n" build-log)
    (let ((update-positions-kernel  (make-cl-kernel
                                     program
                                     "update_positions"
                                     cl_uint cl_float cl_buffer
                                     cl_buffer cl_buffer cl_buffer
                                     cl_buffer cl_buffer cl_buffer
                                     cl_buffer cl_buffer cl_buffer
									 cl_float cl_float cl_float
									 cl_float cl_float cl_float))
          (update-velocities-kernel (make-cl-kernel
                                     program
                                     "update_velocities"
                                     cl_uint
                                     cl_float  cl_float cl_float
                                     cl_buffer
                                     cl_buffer cl_buffer cl_buffer
                                     cl_buffer cl_buffer cl_buffer
                                     cl_buffer cl_buffer cl_buffer))
		  (update-cells-kernel (make-cl-kernel
                                     program
                                     "update_cells"
                                     cl_uint
                                     cl_buffer cl_buffer
                                     cl_buffer cl_buffer cl_buffer
									 cl_float cl_float cl_float
									 cl_float cl_float cl_float
									 cl_uint
									 cl_uint
									 cl_uint))
		  (reset-cells-kernel (make-cl-kernel
                                     program
                                     "reset_cells"
                                     cl_uint
                                     cl_buffer))

          (param-alist (let ((tokens (string-tokenize
                                      (read-string (open-input-file param-file)))))
                         (let lp ((alist (list))
                                  (token1 (car  tokens))
                                  (token2 (cadr tokens))
                                  (rest   (cddr tokens)))
                           (if (null? rest)
                               alist
                               (lp (acons token1 (let ((value (string->number token2)))
                                                   (if value value token2)) alist)
                                   (car  rest)
                                   (cadr rest)
                                   (cddr rest)))))))
      (let ((part_input_file
			 (string-append param-directory "/" ( assoc-ref param-alist "part_input_file")))
            (timestep_length           (assoc-ref param-alist "timestep_length"))
            (time_end                  (assoc-ref param-alist "time_end"))
            (epsilon                   (assoc-ref param-alist "epsilon"))
            (sigma                     (assoc-ref param-alist "sigma"))
            (part_out_freq             (assoc-ref param-alist "part_out_freq"))
            (part_out_name_base        (assoc-ref param-alist "part_out_name_base"))
            (vtk_out_freq              (assoc-ref param-alist "vtk_out_freq"))
            (vtk_out_name_base         (assoc-ref param-alist "vtk_out_name_base"))
            (cl_workgroupt_1dsize      (assoc-ref param-alist "workgroup_1dsize"))
            (x_min      (assoc-ref param-alist "x_min"))
            (y_min      (assoc-ref param-alist "y_min"))
            (z_min      (assoc-ref param-alist "z_min"))
            (x_max      (assoc-ref param-alist "x_max"))
            (y_max      (assoc-ref param-alist "y_max"))
            (z_max      (assoc-ref param-alist "z_max"))
			(x_n      (assoc-ref param-alist "x_n"))
			(y_n      (assoc-ref param-alist "y_n"))
			(z_n      (assoc-ref param-alist "z_n"))


            (vtk_out_current 0)
            (part_out_current 0))
        (let* ((port (open-input-file part_input_file))
               (N     (string->number (read-line port)))
               (px     (make-realvector N 0.0))
               (py     (make-realvector N 0.0))
               (pz     (make-realvector N 0.0))
               (vx     (make-realvector N 0.0))
               (vy     (make-realvector N 0.0))
               (vz     (make-realvector N 0.0))
               (m      (make-realvector N 0.0))
               (fx     (make-realvector N 0.0))
               (fy     (make-realvector N 0.0))
               (fz     (make-realvector N 0.0))
			   (cells  (make-s32vector (* x_n y_n z_n) -1))
			   (links  (make-s32vector N -1))


               (px-dev (make-cl-buffer context CL_MEM_READ_WRITE
                                       (* N (sizeof real))))
               (py-dev (make-cl-buffer context CL_MEM_READ_WRITE
                                       (* N (sizeof real))))
               (pz-dev (make-cl-buffer context CL_MEM_READ_WRITE
                                       (* N (sizeof real))))
               (m-dev  (make-cl-buffer context CL_MEM_READ_WRITE
                                       (* N (sizeof real))))
               (fx-dev (make-cl-buffer context CL_MEM_READ_WRITE
                                       (* N (sizeof real))))
               (fy-dev (make-cl-buffer context CL_MEM_READ_WRITE
                                       (* N (sizeof real))))
               (fz-dev (make-cl-buffer context CL_MEM_READ_WRITE
                                       (* N (sizeof real))))
               (vx-dev (make-cl-buffer context CL_MEM_READ_WRITE
                                       (* N (sizeof real))))
               (vy-dev (make-cl-buffer context CL_MEM_READ_WRITE
                                       (* N (sizeof real))))
               (vz-dev (make-cl-buffer context CL_MEM_READ_WRITE
                                       (* N (sizeof real))))
               (cells-dev (make-cl-buffer context CL_MEM_READ_WRITE
										  (* x_n y_n z_n 4)))
               (links-dev (make-cl-buffer context CL_MEM_READ_WRITE
										  (* N 4))))

          (let ((buffers (list m px py pz vx vy vz)))
            (do ((i 0 (1+ i)))
                (( >= i N ))
              (let ((line-contents (string-tokenize (read-line port))))
                (map (lambda (buf elem)
                       (realvector-set! buf i (string->number elem)))
                     buffers line-contents))))

		  (ascii-write N m px py pz vx vy vz (current-output-port) )

          (enqueue-write-cl-buffer queue px-dev 0 px)
          (enqueue-write-cl-buffer queue py-dev 0 py)
          (enqueue-write-cl-buffer queue pz-dev 0 pz)
          (enqueue-write-cl-buffer queue m-dev 0 m)
          (enqueue-write-cl-buffer queue fx-dev 0 fx)
          (enqueue-write-cl-buffer queue fy-dev 0 fy)
          (enqueue-write-cl-buffer queue fz-dev 0 fz)
          (enqueue-write-cl-buffer queue vx-dev 0 vx)
          (enqueue-write-cl-buffer queue vy-dev 0 vy)
          (enqueue-write-cl-buffer queue vz-dev 0 vz)
		  (enqueue-write-cl-buffer queue cells-dev 0 cells)
		  (enqueue-write-cl-buffer queue links-dev 0 links)


          (cl-finish queue)

          (set-cl-kernel-args update-positions-kernel
                              N timestep_length m-dev
                              px-dev py-dev pz-dev
                              vx-dev vy-dev vz-dev
                              fx-dev fy-dev fz-dev
							  x_min y_min z_min
							  x_max y_max z_max)

          (set-cl-kernel-args update-velocities-kernel
                              N timestep_length epsilon sigma
                              m-dev
                              px-dev py-dev pz-dev
                              vx-dev vy-dev vz-dev
                              fx-dev fy-dev fz-dev)
          (set-cl-kernel-args update-cells-kernel
                              N
							  cells-dev links-dev
							  px-dev py-dev pz-dev
                              x_min y_min z_min
							  x_max y_max z_max
							  x_n y_n z_n)
          (set-cl-kernel-args reset-cells-kernel
                              N
							  cells-dev)

          (do ((i 0 (1+ i)))
              (( >= i (/ time_end timestep_length)))
            (if (= 0 (mod i vtk_out_freq))
                (begin
                  (enqueue-read-cl-buffer queue px-dev 0 px)
                  (enqueue-read-cl-buffer queue py-dev 0 py)
                  (enqueue-read-cl-buffer queue pz-dev 0 pz)
                  (enqueue-read-cl-buffer queue vx-dev 0 vx)
                  (enqueue-read-cl-buffer queue vy-dev 0 vy)
                  (enqueue-read-cl-buffer queue vz-dev 0 vz)
                  (enqueue-read-cl-buffer queue fx-dev 0 fx)
                  (enqueue-read-cl-buffer queue fy-dev 0 fy)
                  (enqueue-read-cl-buffer queue fz-dev 0 fz)
                  (cl-finish queue)
                  (vtk-write N m px py pz vx vy vz (open-output-file
                                                    (string-append
                                                     vtk_out_name_base
                                                     (number->string vtk_out_current)
                                                     ".vtk")))
                  (set! vtk_out_current (1+ vtk_out_current))))
            (if (= 0 (mod i part_out_freq))
                (begin
                  (enqueue-read-cl-buffer queue px-dev 0 px)
                  (enqueue-read-cl-buffer queue py-dev 0 py)
                  (enqueue-read-cl-buffer queue pz-dev 0 pz)
                  (enqueue-read-cl-buffer queue vx-dev 0 vx)
                  (enqueue-read-cl-buffer queue vy-dev 0 vy)
                  (enqueue-read-cl-buffer queue vz-dev 0 vz)
                  (enqueue-read-cl-buffer queue fx-dev 0 fx)
                  (enqueue-read-cl-buffer queue fy-dev 0 fy)
                  (enqueue-read-cl-buffer queue fz-dev 0 fz)
                  (cl-finish queue)
                  (ascii-write N m px py pz vx vy vz (open-output-file
                                                      (string-append
                                                       part_out_name_base
                                                       (number->string part_out_current)
                                                       ".out")))
                  (set! part_out_current (1+ part_out_current))))
            (enqueue-cl-kernel queue update-velocities-kernel
                               (list 0)
                               (list N)
                               (list N))

            (enqueue-cl-kernel queue update-positions-kernel
                               (list 0)
                               (list N)
                               (list N))

			(enqueue-cl-kernel queue reset-cells-kernel
                               (list 0)
                               (list (* x_n y_n z_n))
                               (list (* x_n y_n z_n)))

			(enqueue-cl-kernel queue update-cells-kernel
                               (list 0)
                               (list N)
                               (list N))
		;	(ascii-write N m px py pz vx vy vz (current-output-port) )


            (enqueue-read-cl-buffer queue px-dev 0 px)
            (enqueue-read-cl-buffer queue py-dev 0 py)
            (enqueue-read-cl-buffer queue pz-dev 0 pz)
            (enqueue-read-cl-buffer queue vx-dev 0 vx)
            (enqueue-read-cl-buffer queue vy-dev 0 vy)
            (enqueue-read-cl-buffer queue vz-dev 0 vz)
            (enqueue-read-cl-buffer queue fx-dev 0 fx)
            (enqueue-read-cl-buffer queue fy-dev 0 fy)
            (enqueue-read-cl-buffer queue fz-dev 0 fz)
            (enqueue-read-cl-buffer queue cells-dev 0 cells)
			(enqueue-read-cl-buffer queue links-dev 0 links)

            (cl-finish queue)



			(display cells)
			(display "\n")
			(display links)
			(display "\n")
			(display "\n")
			(display "\n")


            (cl-finish queue)))))))
