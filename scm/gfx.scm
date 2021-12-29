(define (gfxnew name w h)
  (lvl (list "gfxnew" name (num2str w) (num2str h))))

(define (gfxclrrgb pos r g b)
  (lvl (list "gfxclrrgb"
             (num2str pos)
             (num2str r)
             (num2str g)
             (num2str b))))

(define (gfxfill clr)
  (lvl (list "gfxfill" (num2str clr))))

(define (gfxppm name)
  (lvl (list "gfxppm" name)))

(define (gfxrectf-old x y w h c)
  (lvl (list "gfxrectf" (map num2str (list x y w h c))))
)

(define (gfxrectf x y w h c)
  (lvl-fast (list
              "gfxrectf"
              (num2str x)
              (num2str y)
              (num2str w)
              (num2str h)
              (num2str c))))

(define (gfxopen filename)
  (lvl (list "gfxopen" filename)))

(define (gfxtransfer)
  (lvl "gfxtransfer"))

(define (gfxappend)
  (lvl "gfxappend"))

(define (gfxframe)
  (dup)
  (gfxtransfer)
  (gfxappend))

(define (gfxclose)
  (lvl "gfxclose"))

(define (gfxmp4 in out)
  (lvl (list "gfxmp4" in out)))

(define (hex2nib x)
  (case x
    ((#\0) 0)
    ((#\1) 1)
    ((#\2) 2)
    ((#\3) 3)
    ((#\4) 4)
    ((#\5) 5)
    ((#\6) 6)
    ((#\7) 7)
    ((#\8) 8)
    ((#\9) 9)
    ((#\a) 10)
    ((#\b) 11)
    ((#\c) 12)
    ((#\d) 13)
    ((#\e) 14)
    ((#\f) 15)))

(define (hex2num-iter lst sum place)
  (if (null? lst) sum
      (hex2num-iter
       (cdr lst)
       (+ (* place (hex2nib (car lst))) sum)
       (* place 16))))

(define (hex2num x)
  (let ((lst (reverse (string->list x))))
    ;(+ (* 16 (hex2nib (car lst))) (hex2nib (cadr lst)))
    (hex2num-iter lst 0 1)))

(define (gfxclrhex pos clr)
  (lvl (list "gfxclrrgb"
             (num2str pos)
             (num2str (hex2num (car clr)))
             (num2str (hex2num (cadr clr)))
             (num2str (hex2num (caddr clr))))))
