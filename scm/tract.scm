(define (tract:new)
  (lvl "tract_new"))

(define (tract:gest gst tr)
  (gst)
  (tr)
  (lvl "tract_gest zz zz"))

(define (tract:glang gl shp name)
  (gl)
  (shp)
  (lvl (list "tract_glang" "zz" "zz" name)))

(define (tract:node tr in)
  (tr)
  (param in)
  (lvl "tract_node zz zz"))
