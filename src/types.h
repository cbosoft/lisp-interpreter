#pragma once
enum LISPOBJECT_TYPE {
  LISPOBJECT_NIL,
  LISPOBJECT_STRING,
  LISPOBJECT_INT,
  LISPOBJECT_FLOAT,
  LISPOBJECT_BOOL,
  LISPOBJECT_LIST,
  LISPOBJECT_SYMBOL
};


typedef struct LispObject LispObject;
typedef struct LispListElement LispListElement;
struct LispObject {
  char *value_string;
  int value_int;
  double value_float;
  int value_bool;

  int type;

  int line;
  int col;
  char *file;

  char *symbol_name;
  LispListElement *value_list;
};

struct LispListElement {
  LispListElement *next;
  LispObject *value;
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

typedef LispObject *(LispBuiltinFunction)(LispListElement *, LispEnvironment *);
typedef struct LispBuiltin LispBuiltin;
struct LispBuiltin {
  LispBuiltinFunction *f;
  int type;
};

enum LISPBUILTIN_TYPE {
  LISPBUILTIN_LAZY, // no evaluation of arguments
  LISPBUILTIN_GREEDY // all arguments must be evaluated
};

struct environment_var {
  char *name;
  char *alias;
  LispFunction *lisp_function;
  LispBuiltin *lisp_builtin;
  LispObject *value;
  struct environment_var *next;
};

typedef struct LispToken LispToken;
struct LispToken {
  char *string;
  // position of occurence
  int line;
  int col;
  char *file;
  LispToken *next;
};
