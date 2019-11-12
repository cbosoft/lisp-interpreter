#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ast.h"
#include "eval.h"

#define ADD_PARAM(V) \
  nparams ++; \
  if (params == NULL) { \
    params = calloc(nparams, sizeof(void*));\
  }\
  else {\
    params = realloc(params, nparams*sizeof(void*));\
  }\
  params[nparams-1] = calloc(strlen(V)+1, sizeof(char));\
  strcpy(params[nparams-1], V);

typedef char * (*lispfunc)(int, char**);
struct func_name_pair {
  char *name;
  char *shorthand;
  lispfunc function;
};



char *add(int argc, char **args)
{

  double total = 0.0;
  for (int i = 0; i < argc; i++) {
    char *d = (char*)args[i];
    total += atof(d);
  }

  char *rv = calloc(100, sizeof(char));
  snprintf(rv, 99, "%f", total);
  return rv;
}

char *subtract(int argc, char **args)
{
  double val = atoi(args[0]);

  for (int i = 1; i < argc; i++) {
    double d = atoi(args[i]);
    val -= d;
  }

  char *rv = calloc(100, sizeof(char));
  snprintf(rv, 99, "%f", val);
  return rv;
}

char *multiply(int argc, char **args)
{
  double val = atoi(args[0]);

  for (int i = 1; i < argc; i++) {
    double d = atoi(args[i]);
    val *= d;
  }

  char *rv = calloc(100, sizeof(char));
  snprintf(rv, 99, "%f", val);
  return rv;
}

char *divide(int argc, char **args)
{
  double val = atoi(args[0]);

  for (int i = 1; i < argc; i++) {
    double d = atoi(args[i]);
    val /= d;
  }

  char *rv = calloc(100, sizeof(char));
  snprintf(rv, 99, "%f", val);
  return rv;
}


struct func_name_pair functions[] = {
  {"add", "+", add},
  {"subtract", "-", subtract},
  {"multiply", "*", multiply},
  {"divide", "/", divide},
  {NULL}
};


lispfunc get_func(char *name)
{
  int i = 0;
  while (1) {
    if (functions[i].function == NULL) {
      break;
    }
    else if ((strcmp(name, functions[i].name) == 0) || (strcmp(name, functions[i].shorthand) == 0)) {
      return functions[i].function;
    }

    i++;
  }
  return NULL;
}




LispObject *eval(LispObject *root)
{
  char *fname = NULL;

  // TODO change to assert_or_error
  // TODO allow eval of list
  assert(root->type == LISPOBJECT_STRING); // string name of func or syntax error
  fname = root->value_string;
  LispObject *value = get_function(fname)(root->list_next);

  // if arg is a list:
  //  eval all elements in the list
  // return last element in the list

  if (root->list_next)
    return eval(root->list_next);
  else
    return value;

  // evaluation will de-ref objects; they should be free'd
  // TODO figure out how to do this
}
