;; crisp function operations library


(defun apply (f args)
  "Apply function F to ARGS."
  (eval (insert f args)))


(defun map (f arg-list)
  "Apply function F to list-of-lists-of-args ARG-LIST."
  (defun --map-iter (running-list arg-list)
    "iter for map"
    (if arg-list
      (--map-iter (append running-list (apply f (pop arg-list))) (rest arg-list))
      running-list))
  (--map-iter (list) arg-list))


;; crisp function op library ends here
