;; crisp dictionary-like library

(defun get-value (dict key)
  "Return the value in DICT at KEY, or NIL if not found."
  (cond (dict
          (if (= (pop dict) key)
            (pop (rest dict))
            (get-value (rest (rest dict)) key)))))

;; crisp dictionary library ends here
