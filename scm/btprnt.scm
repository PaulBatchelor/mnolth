(define (btprnt:rectf reg x y w h clr)
  (reg)
  (lvl (list "bprectf" "zz"
       (num2str x) (num2str y)
       (num2str w) (num2str h)
       (num2str clr))))

(define (btprnt:get name reg)
  (grab name)
  (lvl (list "bpget" "zz" (num2str reg))))

(define (btprnt:set name reg x y w h)
  (grab name)
  (lvl (list "bpset" "zz"
             (num2str reg)
             (num2str x)
             (num2str y)
             (num2str w)
             (num2str h))))

(define (btprnt:lget name reg)
  (lambda () (btprnt:get name reg)))

(define (btprnt:tr bp x y w h xoff yoff clr)
  (bp)
  (lvl (list "bptr" "zz"
             (num2str x) (num2str y)
             (num2str w) (num2str h)
             (num2str xoff) (num2str yoff)
             (num2str clr))))

