(define (gest:new)
  (lvl "gest_new"))

(define (glang:new key)
  (lvl (list "glang_new" key)))

(define (glang:load gest glang sco)
  (gest)
  (glang)
  (lvl (list "glang_load" "zz" "zz" sco)))

(define (glang:eval gest glang str)
  (gest)
  (glang)
  (lvl (list "glang_eval" "zz" "zz" str)))

(define (gest:cnd tempo)
  (param tempo)
  (lvl "gestcnd zz"))

(define (gest:iculate gst cnd)
  (param gst)
  (param cnd)
  (lvl "gesticulate zz zz"))

(define (gest:scalarnew)
  (lvl "gest_scalarnew"))

(define (glang:scalar gl scl name)
  (gl)
  (scl)
  (lvl (list "glang_scalar" "zz" "zz" name)))

(define (gest:scalar scl)
  (scl)
  (lvl "gescalar zz"))

(define (glang:cdb gest glang cdb name)
  (gest)
  (glang)
  (lvl (list "glang_cdb" "zz" "zz" cdb name)))

(define (gest:mkgest)
  (let ((r (nxtfree)))
    (regset (gest:new) r)
    (regmrk r)
    r))

(define (glang:gexprdb cnd glang cdb name)
  (let ((g (gest:mkgest)))
    (glang:cdb (lregget g) glang cdb name)
    (gest:iculate (regget g) (cnd))
    (regclr g)))

