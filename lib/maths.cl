;; crisp math library

;; Good ole pi.
(defvar pi 3.14159263)

;; iterative process to raise a number to an integer power
(defun pow (x n) 
  ((define pow-iter 
     (running-product iter-count) 
     ((if (= iter-count n) x 
        (* x (pow-iter running-product (+ iter-count 1)))))) 
   (pow-iter 1 1)))
(defun square (x) (pow x 2))
(defun cube (x) (pow x 3))

;; get absolute value
(defun abs (x) 
  ((if (< x 0) 
     (* x -1) x)))
