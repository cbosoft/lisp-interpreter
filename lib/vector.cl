;; crisp vector maths
;; overloads mathematical func to accept lists as input, recursively.

(defun --list-by-num (lst atm f)
  "Apply function F to list LST and atom ATM, elementwise."
  (defun --iter (running orig)
    "doc"
    (if (rest orig)
      (--iter (append running (f (pop orig) atm)) (rest orig))
      (append running (f (pop orig) atm)) ))
  (--iter () lst))

(defun --num-by-list (atm lst f)
  "Apply function F to atom ATM and list LST, elementwise."
  (defun --iter (running orig)
    "doc"
    (if (rest orig)
      (--iter (append running (f atm (pop orig))) (rest orig))
      (append running (f atm (pop orig)))))
  (--iter () lst))

(defun --list-by-list (lst-l lst-r f)
  "Apply function F to elements in lists LST-L and LST-R, elementwise."
  (defun --iter (run l r)
    "doc"
    (if (and (rest l) (rest r))
      (--iter (append run (f (pop l) (pop r))) (rest l) (rest r))
      (append run (f (pop l) (pop r))) ))
  (--iter () 'lst-l 'lst-r))

;; TODO: rewrite as a single function
(defun * (l r)
  "multiply L and R."
  (cond
    ((and (atom? l) (atom? r)) (multiply l r))
    ((and (list? l) (atom? r)) (--list-by-num l r '*))
    ((and (atom? l) (list? r)) (--num-by-list l r '*))
    ((and (list? l) (list? r)) (--list-by-list l r '*))))

(defun / (l r)
  "divide L and R."
  (cond
    ((and (atom? l) (atom? r)) (divide l r))
    ((and (list? l) (atom? r)) (--list-by-num l r '/))
    ((and (atom? l) (list? r)) (--num-by-list l r '/))
    ((and (list? l) (list? r)) (--list-by-list l r '/))))

(defun + (l r)
  "add L to R."
  (cond
    ((and (atom? l) (atom? r)) (add l r))
    ((and (list? l) (atom? r)) (--list-by-num l r '+))
    ((and (atom? l) (list? r)) (--num-by-list l r '+))
    ((and (list? l) (list? r)) (--list-by-list l r '+))))

(defun - (l r)
  "subtract R from L."
  (cond
    ((and (atom? l) (atom? r)) (subtract l r))
    ((and (list? l) (atom? r)) (--list-by-num l r '-))
    ((and (atom? l) (list? r)) (--num-by-list l r '-))
    ((and (list? l) (list? r)) (--list-by-list l r '-))))

;; vector.cl ends here
