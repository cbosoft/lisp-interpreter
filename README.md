# CHRISP

## CHRis' lISp interPretor (v0.1)

Nothing new under the sun; here's another lisp interpretor in C. I wanted to do
this for fun, nothing more. Expect basic functionality, rudimentary
implementation of things which are better implemented elsewhere, and slow
operation.

Uses editline (libedit, BSD) for the REPL, but I like the look of replxx
([syntax highlighting!](https://github.com/AmokHuginnsson/replxx/blob/master/examples/c-api.c))
so may move to that in the future. Garbage collection brought to you by:
[Boehm-GC](https://www.hboehm.info/gc/)! 

# Features:

  - Simple arithmetic 
```lisp
(add a b)  ;; -> a + b
(subtract a b) ;; -> a - b
(multiply a b) ;; -> a * b
(divide a b) ;; -> a / b
```

  - Function definition 
```lisp
(define name (args, ...) (body))
```

  - REPL

    Run with no args, or with the `--interactive` flag in combination with a
    `--file` or `--command` to see the Read-Eval-Print-Loop in all its readline-y
    glory.

