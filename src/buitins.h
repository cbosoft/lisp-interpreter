#pragma once
#include "object.h"
#include "function.h"


typedef LispObject * (*LispBuiltin)(LispObject *);

struct function_table_entry {
  char *name;
  char *shorthand;
  LispBuiltin builtin_function;
  LispFunction *lisp_function;
};
