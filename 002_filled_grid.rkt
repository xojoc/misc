#lang racket

(define width 10)
(define height 10)

(define grid (make-vector (* width height) 0))

(define (grid-ref grid i)
  (match-define (vector x y) i)
  (vector-ref grid (+ (* width y) x)))

(define (grid-set! grid i val)
  (match-define (vector x y) i)
  (vector-set! grid (+ (* width y) x) val))

(define (possible-moves arr idx)
  (filter (λ (i) (and (< -1 (vector-ref i 0) width)
                      (< -1 (vector-ref i 1) height)
                      (zero? (grid-ref arr i))))
          (map (λ (off)
                 (vector (+ (vector-ref idx 0) (vector-ref off 0))
                         (+ (vector-ref idx 1) (vector-ref off 1))))
               (list #(-3 0) #(-1 -1) #(-1 1) #(0 -3) #(0 3) #(1 -1) #(1 1)  #(3 0)))))

(define (solve arr idx n target)
  (if (= n target)
      arr
      (let ((n (+ n 1)))
        (let l ((moves (possible-moves arr idx)))
          (unless (empty? moves)
            (grid-set! arr (car moves) n)
            (let ((solution (solve arr (car moves) n target)))
              (if (void? solution)
                  (begin
                    (grid-set! arr (car moves) 0)
                    (l (cdr moves)))
                  solution)))))))

(define shape #(10 10))
(define target (* (vector-ref shape 0) (vector-ref shape 1)))
(display target)
(newline)

(grid-set! grid #(0 0) 1)

(solve grid #(0 0) 1 100)

(for ([x (in-range width)])
  (for ([y (in-range height)])
    (display (~a #:width 3 #:left-pad-string "0" #:align 'right (grid-ref grid (vector x y))))
    (display " "))
  (newline))
