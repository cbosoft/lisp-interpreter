# Language specification


## lisp!

`crisp` is a lisp variant. Everything is a list. Functions are lists which
operate on lists.

```lisp
(add 1 1) ;; => 2
```

The first element of the list is the name of the function to call, the remaining
elements are the arguments passed to the function.


## Variables!

Variables are scoped to within their created environment.

```lisp
(defvar pi 3.14) ;; variable in global environment.

(defun foo ()
  (print pi)
  (defvar R 8.314) ;; variable in function-local environment.
  (print R))

(foo) ;; prints pi from global environment, and R from its local environment

(print R) ;; Error: R is not in the environment.
```

A variable is a symbol which refers to a lisp object which can contain anything.
Objects are dynamically typed:
  - list
  - symbol
  - atom
    - int
    - float
    - string

Anything with a numerical or string value is an atom: it is indivisible and
can't be expanded into elements (as for lists), or followed to a referred value
(as for symbols).


## Threads?

Not yet, patience!


## Interactive help!

The `(doc o)` function will print the documentation for a builtin function. Try
it in the interpreter! `(doc doc)`!


# Standard library

The standard library consists of `C++` builtin functions and `crisp` functions
which are available to the `crisp` interpreter.

## Defining

### `defun`
```lisp
(defun name list &rest body)
```

NAME is the symbol which will refer to the function body.
LIST is the list of sybmols (arguments) for the function.
The rest, BODY, is a list of the contents of the function.

The arguments to the function are evaluated before the function is called. If
this is not the desired behaviour, try `defmacro`.

### `defmacro`
```lisp
(defmacro name list &rest body)
```

NAME is the symbol which will refer to the macro body.
LIST is the list of sybmols (arguments) for the macro.
The rest, BODY, is a list of the contents of the macro.

The arguments to the macro are NOT evaluated before the it is called. This is
like quoting all the arguments to a function. If this is not the desired
behaviour, try `defun`.

### `defvar`
```lisp
(defvar name body)
```

NAME is the symbol which will refer to value.
BODY is the value of the variable.

The definition of this variable is in the same environment as it was called.


## IO

### `print`
```lisp
(print &rest objects)
```

Prints to `stdout` the string representation of OBJECTS with no additions (no
spaces inserted between objects).

### `with-open`
```lisp
(with-open path mode name &rest body)
```

Open a file at PATH for reading/writing/appending (depending on MODE: r/w/a).
Save file descriptor in sub-environment with NAME. BODY is list of functions to
be executed in sub-environment. File is closed after body is executed.

Inspired by both context management in `Python` and RAII in `C++`.

### `write`
### `read`
### `format`

## Maths
### `add`
### `subtract`
### `multiply`
### `divide`
### `modulo`
### `random`
### `randint`

## Comparison
### `greater-than?`
### `greater-than-or-equal-to?`
### `less-than?`
### `less-than-or-equal-to?`
### `equal?`

## Type checking
### `list?`
### `symbol?`
### `atom`
### `int?`
### `float?`
### `string?`

## Conditions and boolean operations
### `or`
### `and`
### `not`
### `cond`
### `if`

## List operations
### `append`
### `list`
### `pop`
### `rest`
### `range`

## Importing and evaluating
### `eval`
### `eval-file`
### `import`

## Other functions
### `quote`
### `exit`
### `doc`

## Threading functions
### `thread`
### `join`

## Timing functions
### `sleep`

## Variables
### `stdin`
### `stdout`
### `stderr`
### `nil`
### `t`
### `_crisp_version`
