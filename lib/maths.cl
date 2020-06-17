;; crisp math library

;; Good ole pi.
(defvar pi 3.14159263)

;; iterative process to raise a number to an integer power
(defun pow (x n)
  "Calculate the result of an integer X raised to a power N."
  (defun pow-iter (running-product iter-count)
    "iterfunc for pow"
    (if (= iter-count n) x
        (* x (pow-iter running-product (+ iter-count 1)))))
   (pow-iter 1 1))

(defun square (x)
  "Raise X to power 2."
  (pow x 2))

(defun cube (x)
  "Raise X to power 3."
  (pow x 3))

;; get absolute value
(defun abs (x)
  "Absolute value of X."
  ((if (< x 0) 
     (* x -1) x)))

;; crisp maths library ends here
