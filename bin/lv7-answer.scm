(define input '(12 71 2 15 29 82 87 8 18 66 81 25 63 97 40 3 93 58 53 31 47))
(define (quick-sort lst) (if (null? lst) '() (let ((pivot (car lst)) (rest (cdr lst))) (append (quick-sort (filter (lambda (x) (< x pivot)) rest)) (list pivot) (quick-sort (filter (lambda (x) (>= x pivot)) rest))))))
(displayln (quick-sort input))
