#lang racket

(require math/distributions)
(require racket/gui)

(define width 500)
(define height 500)

(define (seed n)
  (define d (discrete-dist '(#t #f) '(0.1 0.9)))
  (build-list n (lambda (_) (sample d n))))

(define cells (seed 25))

(define (add-empty-rows cells)
  (define (upper-row cells)
    (if (ormap identity (first cells))
        (cons (build-list (length (first cells)) (const #f)) cells)
        cells))
  (define (right-row cells)
    (if (stream-ormap identity (stream-map last cells))
        (map (lambda (row) (append row (list #f))) cells)
        cells))
  (define (lower-row cells)
    (if (ormap identity (last cells))
        (append cells (list (build-list (length (first cells)) (const #f))))
        cells))
  (define (left-row cells)
    (if (stream-ormap identity (stream-map first cells))
        (map (lambda (row) (cons #f row)) cells)
        cells))
  ((compose upper-row right-row lower-row left-row) cells))

(define (get-neighbors cells r c)
  (define (idx p)
    (list-ref (list-ref cells (car p)) (cdr p)))
  (map idx
       (filter (lambda (p) (and (>= (car p) 0)
                                (>= (cdr p) 0)
                                (< (car p) (length cells))
                                (< (cdr p) (length (first cells)))))
               (map (lambda (op) (cons (+ r (first op)) (+ c (second op))))
                    '((-1 -1) (-1 0) (-1 1) ( 0  1) ( 1 1) ( 1 0) ( 1 -1) ( 0 -1))))))

(define (next-generation cells)
  (for/list ([row cells]
             [r (in-naturals)])
    (for/list ([cell row]
               [c (in-naturals)])
      (let ((alive (count identity (get-neighbors cells r c))))
        (cond
          [(and (eq? cell #t) (or (= alive 2) (= alive 3))) #t]
          [(and (eq? cell #f) (= alive 3)) #t]
          [else #f])))))

(define (tick canvas)
  (set! cells ((compose next-generation add-empty-rows) cells))
  (send canvas refresh))

(define (sqw cells)
  (let*-values
      ([(w h) (send canvas get-client-size)])
    (min (/ h (length cells))
         (/ w (length (first cells))))))

(define (paint-callback canvas dc)
  (send dc set-background "light gray")
  (send dc clear)
  (send dc set-brush "black" 'solid)
  (send dc set-smoothing 'aligned)
  
  (let ((sqw (sqw cells)))
    (for/list ([row cells]
               [r (in-naturals)])
      (for/list ([cell row]
                 [c (in-naturals)])
        (when cell (send dc draw-rectangle (* c sqw) (* r sqw) sqw sqw))))))

(define frame (new frame% [label "Game Of Life - xojoc"]
                   [width width]
                   [height height]))
(define my-canvas%
  (class canvas% 
    (define/override (on-event event)
      (when (or (send event button-down? 'left) (send event dragging?))
        (let* ((sqw (sqw cells))
               (r (max 0 (min
                          (floor (/ (send event get-y) sqw))
                          (- (length cells) 1))))
               (c (max 0 (min
                          (floor (/ (send event get-x) sqw))
                          (- (length (first cells)) 1))))
               (v (if (send event button-down? 'left)
                      (not (list-ref (list-ref cells r) c))
                      #t)))
          (set! cells (list-set cells r (list-set (list-ref cells r) c v)))
          (set! cells (add-empty-rows cells))
          (send canvas refresh))))
    (super-new)))
(define canvas (new my-canvas% [parent frame]
                    [paint-callback paint-callback] ))
(define dc (send canvas get-dc))


(define timer
  (new timer%
       (interval 1000)
       (notify-callback (lambda () (tick canvas)))))

(define speed-slider (new slider%
                          [parent frame]
                          [label "Speed"]
                          [min-value 0]
                          [max-value 1000]
                          [init-value 80]
                          [style '(horizontal plain)]
                          [callback (lambda (slider event)
                                      (let ((v (send slider get-value)))
                                        (if (eq? v 0)
                                            (send timer stop)
                                            (send timer start (+ 80 (- 1000 v))))))]))

(define buttons-pane (new horizontal-pane%
                          [parent frame]
                          [alignment '(left bottom)]
                          [stretchable-height #f]))

(define seed-button (new button%
                         [parent buttons-pane]
                         [label "Fill randomly"]
                         [callback (lambda (button event)
                                     (set! cells (seed 25))
                                     (send canvas refresh))]))

(define step-button (new button%
                         [parent buttons-pane]
                         [label "Step"]
                         [callback (lambda (button event)
                                     (tick canvas))]))

(define clear-button (new button%
                          [parent buttons-pane]
                          [label "Clear"]
                          [callback (lambda (button event)
                                      (set! cells (build-list 15 (lambda (_) (build-list 15 (const #f)))))
                                      (send canvas refresh))]))

(send frame show #t)