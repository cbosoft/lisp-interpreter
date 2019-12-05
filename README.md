# ![icon](https://raw.githubusercontent.com/cbosoft/lisp-interpreter/master/assets/logo.png) crisp

## (chris' lisp) v0.3

Lisp  is pretty cool: everything is data, nothing really exists, anything can be
a function or an object... the lines are super blurry. My supervisor had this
idea to develop an ML algorithm which could alter itself. The function would
re-write itself to perform the assigned task. I think this has potential, and is
definitely an interesting problem! To this end, I wrote this little interpreter
(first in `c`, now in `C++`).

Uses editline (libedit, BSD) for the REPL, but I like the look of replxx
([syntax
highlighting!](https://github.com/AmokHuginnsson/replxx/blob/master/examples/c-api.c))
so may move to that in the future. Garbage collection/memory management brought
to you by the wonderful world of std::shared_ptr! Bacon logo from
[Freepik](https://www.flaticon.com/authors/freepik)

# Features:

  - MATHS!
```lisp
(add a b)  ;; -> a + b
(subtract a b) ;; -> a - b
(multiply a b) ;; -> a * b
(divide a b) ;; -> a / b
```

  - FUNCTIONS AND VARIABLES!
```lisp
(defun name (args ...) body ...) ;; arguments will each be evaluated once


(defmacro name (args ...) body ...) ;; arguments won't be evaluated until requested


(defvar name value)
```

  - CONDITIONS!
```lisp
(if (- 2 1) 'true 'false)

(cond ((or 0 0) 'nope)
      ((and 1 0) 'nah)
      ((or 1 0) 'this-one!))
```

  - REPL

    Run with no args, or with the `--interactive` flag in combination with a
    `--file` or `--command` to see the Read-Eval-Print-Loop in all its readline-y
    glory.

