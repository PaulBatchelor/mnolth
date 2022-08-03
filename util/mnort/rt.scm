(define (rt:start)
  (lvl "hsnew hs")
  (lvl "rtnew [grab hs] rt"))

(define (playtog)
  (lvl "hstog [grab hs]"))

(define (rt:out in)
  (param in)
  (lvl "hsout [grab hs]")
  (lvl "hsswp [grab hs]"))

(define (rt:keypress)
  (lvl "keypress"))
