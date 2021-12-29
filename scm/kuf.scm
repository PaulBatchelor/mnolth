(define kuf:hline0 (hex2num "0033"))
(define kuf:hline1 (hex2num "00cc"))
(define kuf:hline2 (hex2num "3300"))
(define kuf:hline3 (hex2num "cc00"))
(define kuf:hpar0 13107)

(define (kuf:new name nsqrs)
  (lvl (list "kufnew" name (num2str nsqrs))))

(define (kuf:setsqr w sqrs x y s)
  (sqrs)
  (lvl (list
        "kufsetsqr"
        (num2str w)
        "zz" (num2str x) (num2str y) (num2str s))))

(define (kuf:bp bp sqrs x y w h scl)
  (bp)
  (sqrs)
  (lvl (list "kufbp" "zz" "zz"
             (num2str x) (num2str y)
             (num2str w) (num2str h)
             (num2str scl))))

(define (kuf:genblk rng)
  (lilp "[func {")
  (lvl "local tmp")
  (lvl (string-append "set tmp [kufgenblk " (num2str rng) "]"))
  (lvl "param [index $tmp 1]")
  (lvl "param [index [index $tmp 0] 3]")
  (lvl "param [index [index $tmp 0] 2]")
  (lvl "param [index [index $tmp 0] 1]")
  (lvl "param [index [index $tmp 0] 0]")
  (lilp "}]")
  (list (list
         (inexact->exact (pop))
         (inexact->exact (pop))
         (inexact->exact (pop))
         (inexact->exact (pop)))
        (inexact->exact (pop))))

(define (kuf:list->blk lst)
  (string-append "[list "
                 (num2str (car lst)) " "
                 (num2str (cadr lst)) " "
                 (num2str (caddr lst)) " "
                 (num2str (cadddr lst))
                 "]"))

(define (kuf:setblk w sqrs x y blk)
  (sqrs)
  (lvl (list
        "kufsetblk"
        (num2str w)
        "zz" (num2str x) (num2str y)
        (kuf:list->blk blk))))

(define (kuf:cor w h sqrs)
  (sqrs)
  (lvl (list "kufcor" (num2str w) (num2str h) "zz")))

(define (kuf:pbm name w h sqrs)
  (sqrs)
  (lvl (list "kufpbm" name (num2str w) (num2str h) "zz")))
