(define (index? n)
  (and (integer? n) (exact? n) (>= n 0)))

(define-syntax-rule (check-arg exp message arg ...)
  (unless exp
    (error message arg ...)))

(define* (read-string! buf #:optional
                       (port (current-input-port))
                       (start 0) (end (string-length buf)))
  "Read all of the characters out of PORT and write them to BUF.
Returns the number of characters read.

This function only reads out characters from PORT if it will be able to
write them to BUF.  That is to say, if BUF is smaller than the number of
available characters, then BUF will be filled, and characters will be
left in the port."
  (check-arg (string? buf) "not a string" buf)
  (check-arg (index? start) "bad index" start)
  (check-arg (index? end) "bad index" end)
  (check-arg (<= start end) "start beyond end" start end)
  (check-arg (<= end (string-length buf)) "end beyond string length" end)
  (let lp ((n start))
    (if (< n end)
        (let ((c (read-char port)))
          (if (eof-object? c)
              (- n start)
              (begin
                (string-set! buf n c)
                (lp (1+ n)))))
        (- n start))))

(define* (read-string #:optional (port (current-input-port)) (count #f))
  "Read all of the characters out of PORT and return them as a string.
If the COUNT argument is present, treat it as a limit to the number of
characters to read.  By default, there is no limit."
  (check-arg (or (not count) (index? count)) "bad count" count)
  (let loop ((substrings '())
             (total-chars 0)
             (buf-size 100))		; doubled each time through.
    (let* ((buf (make-string (if count
                                 (min buf-size (- count total-chars))
                                 buf-size)))
           (nchars (read-string! buf port))
           (new-total (+ total-chars nchars)))
      (cond
       ((= nchars buf-size)
        ;; buffer filled.
        (loop (cons buf substrings) new-total (* buf-size 2)))
       (else
        (string-concatenate-reverse
         (cons (substring buf 0 nchars) substrings)))))))
