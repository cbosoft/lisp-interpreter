# ![icon](https://raw.githubusercontent.com/cbosoft/lisp-interpreter/master/assets/logo.png) crisp

## (chris' lisp) v0.4

Lisp is very cool. Functional programming in lisp, also very cool. However,
modern lisp implementations I find to be quite complex where lisp in essence is
very simple. To help myself learn `C++` a bit better, and to get into lisp a bit
more, I decided to write this simple lisp interpreter.

Design goals:
  - simply lispy: objects can point to other objects (symbols), contain a list
    of objects, or are a value (atom). Functions are lists of lists. Variables
    are immutable.
  - small `C++` library, with most standard library implemented in lisp.
  - thread safety is a big advantage of using a lisp language due to the
    immutability of everything - so good thread capability is desired.

A run down of the language is available in [DOC.md](DOC.md).


# Installation

Easy-peasy for linux: clone, changedir, make, install:
```bash
git clone https://github.com/cbosoft/lisp-interpreter crisp
cd crisp
make
sudo make install
```
Requires a good `C++` compiler (g++ works), and libedit, ncurses, pthread. It
will not compile on Windows, and I haven't tried on MacOS (yet).


# Thanks

Uses editline (libedit, BSD) for the REPL.

Bacon logo from [Freepik](https://www.flaticon.com/authors/freepik)
