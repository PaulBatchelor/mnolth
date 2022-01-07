(define (add-zz x) (if (null? x) "zz" x))

(define (flatten x)
  (cond ((null? x) '())
        ((pair? x)
         (append (flatten (add-zz (car x))) (flatten (cdr x))))
        (else (list x))))

(define (space-command lst)
  (space-command-iter lst ""))

(define (space-command-iter lst str)
  (cond ((pair? lst)
         (space-command-iter
          (cdr lst)
          (string-append str (add-zz (car lst)) " ")))
        ((string? lst) lst)
        ((null? lst) str)
))

(define (lilp str)
  (let ((o (lil str)))
    (if (string? o) (error o))))

(define (lvl-old lst)
  (let ((o (lil (space-command (flatten lst)))))
    (if (string? o) (error o))))

(define (lvl-fast lst) (if (string? lst)
                           (lilp lst)
                           (lilp (apply cmd lst))))
(define lvl lvl-fast)
(define (llvl lst) (lambda () (lvl lst)))

(define zz '())

(define (param x)
  (if (number? x) (lvl (list "param" (number->string x)))))

(define (dup) (lvl "dup"))
(define (drop) (lvl "drop"))
(define (swap) (lvl "swap"))

(define (grab name) (lvl (list "grab" name)))
(define (lgrab name) (lambda () (grab name)))

(define (num2str x) (number->string x))

(define (compute-audio)
  (lvl "compute 15"))

(define (computes time)
  (lvl (list "computes" (num2str time))))

(define (mnopath s)
  (string-append (mnotop) "/" s))

(define (getenvp s)
  (let ((e (getenv s)))
    (if (null? e)
        (error
         (string-append
          "Could not find environment variable: " s))
        e)))

(define (regget r)
  (lvl (list "regget" (num2str r))))

(define (regset x r)
  (param x)
  (lvl (list "regset" "zz" (num2str r))))

(define (lregget r) (lambda () (regget r)))

(define (hold c)
  (param c)
  (lvl "hold zz"))

(define (unhold c)
  (param c)
  (lvl "unhold zz"))

(define (unholdall)
  (lvl "unholdall"))

(define (unbork)
  ;; TODO
  '())
