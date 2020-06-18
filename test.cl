#!/usr/bin/env crisp

(import 'test)

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

(print "all tests passed!")
