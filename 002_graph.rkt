#lang racket

(require graph)

(define width 10)
(define height width)

  
(define relative-moves
  (list #(-3 0) #(-1 -1) #(-1 1) #(0 -3) #(0 3) #(1 -1) #(1 1)  #(3 0)))

(define (connected-vertices n)
  (define x (remainder n width))
  (define y (quotient n height))
  (for/fold ([result '()])
            ([rel (in-list relative-moves)])
    (match-define (vector ox oy) rel)
    (define-values (nx ny) (values (+ x ox) (+ y oy)))
    (if (and (>= nx 0) (< nx width)    
             (>= ny 0) (< ny height))   
        (cons (+ (* ny width) nx) result)
        result)))


(define g (unweighted-graph/undirected
           (foldl (λ (n edges) (foldl (λ (v edges) (cons (list n v) edges))
                                      edges (connected-vertices n)))
                  '() (range (* width height)))))

(define out (open-output-file "002_filled_grid.graphviz" #:mode 'text #:exists 'truncate/replace))
(graphviz g #:output out)
(close-output-port out)