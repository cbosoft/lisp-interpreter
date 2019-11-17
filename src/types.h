#pragma once
enum LISPOBJECT_TYPE {
  LISPOBJECT_STRING,
  LISPOBJECT_INT,
  LISPOBJECT_FLOAT,
  LISPOBJECT_BOOL,
  LISPOBJECT_LIST,
  LISPOBJECT_SYMBOL
};


typedef struct LispObject LispObject;
struct LispObject {
  char *value_string;
  int value_int;
  double value_float;
  int value_bool;

  int type;

  char *symbol_name;
  LispObject *value_symbol;

  LispObject *list_next;
  LispObject *list_child;
};


typedef struct LispFunction LispFunction;
struct LispFunction{
  LispObject *body;
  int number_required_args;
  //int number_optional_args; // TODO add conditional args
  char **arg_names; // used to create the environment for the function
};

typedef struct LispEnvironment LispEnvironment;
struct LispEnvironment {
  struct environment_var *variables;
  LispEnvironment *parent;
};

typedef LispObject *(LispBuiltin)(LispObject *, LispEnvironment *);

struct environment_var {
  char *name;
  char *alias;
  LispFunction *lisp_function;
  LispBuiltin *lisp_builtin;
  LispObject *value;
  struct environment_var *next;
};

struct builtin_table_row {
  char *name;
  char *alias;
  LispBuiltin *function;
};
