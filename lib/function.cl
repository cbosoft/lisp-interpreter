;; crisp function operations library


(defun apply (f args)
  "Apply function F to ARGS."
  (eval (insert f args)))


;; crisp function op library ends here
