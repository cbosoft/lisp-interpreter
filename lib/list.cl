;; crisp list operations extension library


(defun join (left right)
  "Create a new list by joining LEFT with RIGHT."
  (if (rest right)
    (join (append left (pop right)) (rest right))
    (append left (pop right))))


(defun insert (e l)
  "Create new list with element E at the start followed by contents of list L."
  (join (list e) l))


;; crisp list op library ends here
