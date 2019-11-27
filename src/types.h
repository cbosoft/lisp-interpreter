#pragma once
enum LISPOBJECT_TYPE {
  LISPOBJECT_NIL,
  LISPOBJECT_ATOM,
  LISPOBJECT_LIST,
  LISPOBJECT_SYMBOL
};

enum LISPATOM_TYPE {
  LISPATOM_FLOAT,
  LISPATOM_INT,
  LISPATOM_STRING
};


typedef struct LispObject LispObject;
typedef struct LispListElement LispListElement;
typedef struct LispAtom LispAtom;
struct LispObject {
  LispAtom *value_atom;
  char *value_symbol;
  LispListElement *value_list;

  int type;

  int line;
  int col;
  char *file;
};

struct LispListElement {
  LispListElement *next;
  LispObject *value;
};

struct LispAtom {
  long *value_int;
  double *value_float;
  char *value_string;
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
