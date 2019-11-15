#pragma once
#include "object.h"
#include "function.h"
#include "environment.h"


typedef LispObject * (*LispBuiltin)(LispObject *, LispEnvironment *);

struct function_table_entry {
  char *name;
  char *shorthand;
  LispBuiltin builtin_function;
  LispFunction *lisp_function;
};

struct function_table_entry *get_function(char *name);
