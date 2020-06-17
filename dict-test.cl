(import "dict") ;; for get-value

(defvar d '("name" "bob" "age" 30 "height" 190))

(print (get-value d "name"))
(print (get-value d "weight"))
