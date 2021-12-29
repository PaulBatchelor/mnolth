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
