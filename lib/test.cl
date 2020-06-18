;; crisp testing library

(import 'list)


(defun fail-test (name why)
  "Called when a test NAME is failed due to WHY, printing this to the user and exiting non-zero."
  (print (format "Test {0} failed: {1}." name why))
  (exit 1))


(defun pass-test (name)
  "Called when a test NAME is passed. Prints success message."
  (print (format "Test {0} passed." name))
  nil)


(defun apply (f args)
  "Apply function F to ARGS."
  (eval (insert f args)))


(defun run-test-suite (f list-of-args)
  "Run function F with combination of parameters given in LIST-OF-ARGS.
  F should take the args, and check the result, calling `fail-test` or
  `pass-test` as necessary."
  (defun run-next ()
    "Run test, prep running next set."
    (apply f (pop list-of-args))
    (run-test-suite f (rest list-of-args)))
  (cond (list-of-args
    (run-next)))
  nil)


;; crisp testing library ends here
