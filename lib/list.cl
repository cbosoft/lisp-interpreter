;; crisp list operations extension library


(defun concat (left right)
  "Create a new list by joining LEFT with RIGHT."
  (if (rest right)
    (concat (append left (pop right)) (rest right))
    (append left (pop right))))


(defun insert (e l)
  "Create new list with element E at the start followed by contents of list L."
  (concat (list e) l))


(defun count (l)
  "Count elements of list L."
  (defun --count-iter (rl i)
    ""
    (if (rest rl)
      (--count-iter (rest rl) (+ i 1))
      (+ i 1)))
  (--count-iter l 0))


;; crisp list op library ends here
