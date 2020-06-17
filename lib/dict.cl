;; crisp dictionary-like library

(defun get-value (dict key)
  (if (= (pop dict) key)
    (pop (rest dict))
    (get-value (rest (rest dict)) key)))

;; crisp dictionary library ends here
