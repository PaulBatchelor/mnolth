(define (hotswap:setup-stash)
  (lvl "subpatch s"))

(define (hotswap:save)
  (lvl "psave [grab s]"))

(define (hotswap:load)
  (lvl "pload [grab s]"))

(define (hotswap:new name)
  (lvl (list "hsnew" name)))

(define (hotswap:crossfade name mode)
  (grab name)
  (lvl (list "hscf" "zz" (num2str mode))))

(define (hotswap:cfsize name dur)
  (grab name)
  (lvl (list "hscfsz" "zz" (num2str dur))))

(define (hotswap:out name)
  (grab name)
  (lvl "hsout zz")
  (grab name)
  (lvl "hsswp zz"))

(define (hotswap:get name)
  (grab name)
  (lvl "hsget zz"))

(define (hotswap:node name)
  (grab name)
  (lvl "hsnode zz"))

(define (hotswap:begin)
  ;(hotswap:save)
  (lvl "pclear")
)

(define (hotswap:end name)
  (hotswap:out name)
  ;(hotswap:load)
  (lvl "pload [grab top]"))
