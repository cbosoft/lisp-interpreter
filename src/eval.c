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


void *eval(struct ast_node *ast)
{
  struct ast_node *current = ast;
  char *fname = NULL;
  char **params = NULL;
  int nparams = 0;

  while (1) {
    //fprintf(stderr, "%d %s \n", nparams, current->value);

    if (current->child != NULL) {
      //fprintf(stderr, "  eval child\n");
      current->value = eval(current->child);
      //fprintf(stderr, "  child evaluted to: %s \n", (char*)current->value);
    }

    if (current->value == NULL) {
      //fprintf(stderr, "  val is null\n");
      goto next;
    }

    if (fname == NULL)  {
      fname = current->value;
      //fprintf(stderr, "fname = %s\n", fname);
    }
    else {
      ADD_PARAM(current->value);
    }

next:
    if (current && current->next) {
      current = current->next;
    }
    else
      break;
    //fprintf(stderr, "NEXT\n");

  }

  if (fname == NULL) {
    //fprintf(stderr, "NO FUNCTION NAMED\n");
    return ast->child->value;
  }

  lispfunc f = get_func(fname);

  if (f == NULL) {
    //fprintf(stderr, "FUNCTION NOT KNOWN\n");
    return ast->next->value; // TODO: is this the best way of doing this?
  }

  char *rv = f(nparams, params);
  return rv;
}
