;;; crisp-mode --- Summary
;;; Syntax highlighting for crisp

;;; Commentary:

;;; Code:

(setq crisp-highlights
      '(("defun\\|defvar\\|defmacro\\|append\\|list\\|add\\|subtract\\|multiply\\|divide\\|modulo\\|pop\\|rest\\|with-open" . font-lock-function-name-face)
        (";.*$" . font-lock-comment-face)))

(define-derived-mode crisp-mode lisp-mode "crisp"
  "major mode for editing chris' lisp code."
  (setq font-lock-defaults '(crisp-highlights)))

(provide 'crisp-mode)
;;; crisp-mode.el ends here
