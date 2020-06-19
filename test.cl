#!/usr/bin/env crisp

(import 'test)


;; definition tests
(defun definition-test (result expected)
  "Variable definition test"
  (defvar testname (format "{0} == {1}" result expected))
  (if (= result expected)
    (pass-test testname)
    (fail-test testname "Variables not equal.")))

(run-test-suite 'definition-test
                '(( () (list) )))

(defun addition-test (left right expected)
  "Addition test"
  (defvar result (+ left right))
  (defvar testname (format "{0} + {1} == {2}?" left right expected))
  (if (= result expected)
    (pass-test testname)
    (fail-test testname (format "got {0}, expected {1}." result expected))))

(run-test-suite 'addition-test
                '((1 1 2)
                  (1 -1 0)
                  (-1 -1 -2)
                  (2 2 4)
                  ("a" 1 "a1")))

(defun subtraction-test (left right expected)
  "Subtraction test"
  (defvar result (- left right))
  (defvar testname (format "{0} - {1} == {2}?" left right expected))
  (if (= result expected)
    (pass-test testname)
    (fail-test testname (format "got {0}, expected {1}." result expected))))

(run-test-suite 'subtraction-test
                '((1 1 0)
                  (1 -1 2)
                  (-1 -1 0)
                  (2 2 0)))

(defun multiplication-test (left right expected)
  "Multiplication test"
  (defvar result (* left right))
  (defvar testname (format "{0}*{1} == {2}?" left right expected))
  (if (= result expected)
    (pass-test testname)
    (fail-test testname (format "got {0}, expected {1}." result expected))))

(run-test-suite 'multiplication-test
                '((1 1 1)
                  (1 -1 -1)
                  (-1 -1 1)
                  (2 2 4)))

(defun thread-test ()
  "Test threading."
  (defvar testname "basic threading")
  (defvar expectation 3)
  (defun foo ()
    "foo"
    expectation)
  (defvar id (thread 'foo))
  (defvar result (join id))
  (if (= result expectation)
    (pass-test testname)
    (fail-test testname (format "Join result not expected. Got {0}, expected {1}" result expectation))))

(thread-test)


(defun default-value-test (args expectation)
  "Default value test"
  (defun f (a b c) (1 2)
    "doc"
    (+ (+ a b) c))
  (defvar result (eval (insert 'f args)))
  (defvar testname (format "default value: {0} == {1} ?" result expectation))
  (if (= result expectation)
    (pass-test testname)
    (fail-test testname "error")))

(run-test-suite 'default-value-test 
                '(
                   ( (list 3) 6 )
                   ( (list 2 1) 5 )
                   ( (list 1 1 2) 4 )
                  ))


(print "all tests passed!")
