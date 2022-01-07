(define (sqlar:opendb name path)
  (lvl (list "opendb" name path)))

(define (crate:rawk db name id)
  (db)
  (lvl (list "crtrawk" "zz" name id)))
