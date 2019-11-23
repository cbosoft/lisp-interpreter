#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "object.h"
#include "builtins.h"
#include "exception.h"
#include "environment.h"
#include "debug.h"
#include "types.h"
#include "function.h"
#include "list.h"
#include "eval.h"
#include "gc.h"

extern LispObject nil;


#define TOUCH(LE) if (LE!=NULL) {};



// Add two numbers
LispObject *add(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION ADD\n");

  TOUCH(env);

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 2, "add", "Function takes 2 arguments (got %d).", nargs);
  ERROR_CHECK;
  
  LispObject *l = arg->value;
  LispObject *r = arg->next->value;

  assert_or_error( 
      (l->type == LISPOBJECT_INT || l->type == LISPOBJECT_FLOAT) && 
      (r->type == LISPOBJECT_INT || r->type == LISPOBJECT_FLOAT),
      "add", 
      "Function expects numerical arguments, got %s and %s.", LispObject_type(l), LispObject_type(r));
  ERROR_CHECK;


  if (l->type == LISPOBJECT_FLOAT || r->type == LISPOBJECT_FLOAT) {
    double l_operand = l->type == LISPOBJECT_FLOAT ? l->value_float : (double)l->value_int;
    double r_operand = r->type == LISPOBJECT_FLOAT ? r->value_float : (double)r->value_int;
    LispObject *rv = LispObject_new_float(l_operand + r_operand);
    return rv;
  }

  int l_operand = l->type == LISPOBJECT_FLOAT ? (int)l->value_float : l->value_int;
  int r_operand = r->type == LISPOBJECT_FLOAT ? (int)r->value_float : r->value_int;
  LispObject *rv = LispObject_new_int(l_operand + r_operand);
  return rv;
}
LispBuiltin add_obj = {&add, LISPBUILTIN_GREEDY};




// Subtract a number from another
LispObject *subtract(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION SUBTRACT\n");

  TOUCH(env);

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 2, "subtract", "Function takes 2 arguments (got %d).", nargs);
  ERROR_CHECK;
  
  LispObject *l = arg->value;
  LispObject *r = arg->next->value;

  assert_or_error( 
      (l->type == LISPOBJECT_INT || l->type == LISPOBJECT_FLOAT) && 
      (r->type == LISPOBJECT_INT || r->type == LISPOBJECT_FLOAT),
      "subtract", 
      "Function expects numerical arguments, got %s and %s.", LispObject_type(l), LispObject_type(r));
  ERROR_CHECK;

  if (l->type == LISPOBJECT_FLOAT || r->type == LISPOBJECT_FLOAT) {
    double l_operand = l->type == LISPOBJECT_FLOAT ? l->value_float : (double)l->value_int;
    double r_operand = r->type == LISPOBJECT_FLOAT ? r->value_float : (double)r->value_int;
    LispObject *rv = LispObject_new_float(l_operand - r_operand);
    return rv;
  }

  int l_operand = l->type == LISPOBJECT_FLOAT ? (int)l->value_float : l->value_int;
  int r_operand = r->type == LISPOBJECT_FLOAT ? (int)r->value_float : r->value_int;
  LispObject *rv = LispObject_new_int(l_operand - r_operand);
  return rv;
}
LispBuiltin subtract_obj = {&subtract, LISPBUILTIN_GREEDY};




// 
LispObject *multiply(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION MULTIPLY\n");

  TOUCH(env);

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 2, "multiply", "Function takes 2 arguments (got %d).", nargs);
  ERROR_CHECK;
  
  LispObject *l = arg->value;
  LispObject *r = arg->next->value;

  assert_or_error( 
      (l->type == LISPOBJECT_INT || l->type == LISPOBJECT_FLOAT) && 
      (r->type == LISPOBJECT_INT || r->type == LISPOBJECT_FLOAT),
      "multiply", 
      "Function expects numerical arguments, got %s and %s.", LispObject_type(l), LispObject_type(r));
  ERROR_CHECK;

  if (l->type == LISPOBJECT_FLOAT || r->type == LISPOBJECT_FLOAT) {
    double l_operand = l->type == LISPOBJECT_FLOAT ? l->value_float : (double)l->value_int;
    double r_operand = r->type == LISPOBJECT_FLOAT ? r->value_float : (double)r->value_int;
    LispObject *rv = LispObject_new_float(l_operand * r_operand);
    return rv;
  }

  int l_operand = l->type == LISPOBJECT_FLOAT ? (int)l->value_float : l->value_int;
  int r_operand = r->type == LISPOBJECT_FLOAT ? (int)r->value_float : r->value_int;
  LispObject *rv = LispObject_new_int(l_operand * r_operand);
  return rv;
}
LispBuiltin multiply_obj = {&multiply, LISPBUILTIN_GREEDY};



// 
LispObject *divide(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION DIVIDE\n");

  TOUCH(env);

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 2, "divide", "Function takes 2 arguments (got %d).", nargs);
  ERROR_CHECK;
  
  LispObject *l = arg->value;
  LispObject *r = arg->next->value;

  assert_or_error( 
      (l->type == LISPOBJECT_INT || l->type == LISPOBJECT_FLOAT) && 
      (r->type == LISPOBJECT_INT || r->type == LISPOBJECT_FLOAT),
      "divide", 
      "Function expects numerical arguments, got %s and %s.", LispObject_type(l), LispObject_type(r));
  ERROR_CHECK;

  if (l->type == LISPOBJECT_FLOAT || r->type == LISPOBJECT_FLOAT) {
    double l_operand = l->type == LISPOBJECT_FLOAT ? l->value_float : (double)l->value_int;
    double r_operand = r->type == LISPOBJECT_FLOAT ? r->value_float : (double)r->value_int;
    LispObject *rv = LispObject_new_float(l_operand / r_operand);
    return rv;
  }

  int l_operand = l->type == LISPOBJECT_FLOAT ? (int)l->value_float : l->value_int;
  int r_operand = r->type == LISPOBJECT_FLOAT ? (int)r->value_float : r->value_int;
  LispObject *rv = LispObject_new_int(l_operand / r_operand);
  return rv;
}
LispBuiltin divide_obj = {&divide, LISPBUILTIN_GREEDY};




// Returns args as output without change
// TODO remove this as is implemented at lower level
LispObject *quote(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION QUOTE\n");

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 1, "quote", "Function takes 1 argument (got %d).", nargs);
  ERROR_CHECK;

  TOUCH(env);
  return arg->value;
}
LispBuiltin quote_obj = {&quote, LISPBUILTIN_LAZY};




// Create an entry in the local environment's function table.
// Usage: (define name arglist body)
// Arguments:
//   name            string or symbol name of the function
//   arglist         list of string or symbol names of the function arguments,
//   body            list of things to exectue when called.
LispObject *define(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION DEFINE\n");

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 3, "define", "Function takes 3 arguments: name, arglist, body (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");
  
  LispObject *name = arg->value;
  assert_or_error(name->type == LISPOBJECT_SYMBOL, "define", "Argument has wrong type: name should be a Symbol, not %s", LispObject_type(name));
  ERROR_CHECK;
  debug_message("AFTER NAME TYPE CHECK\n");

  if (!LispEnvironment_get(env, name->symbol_name, NULL, NULL, NULL)) {
    LispEnvironment_del_variable(env, name->symbol_name);
  }
  Exception_reset();

  assert_or_error(arg->next->value->type == LISPOBJECT_LIST, "define", "Argument has wrong type: arglist should be a List, not %s", LispObject_type(arg->next->value));
  ERROR_CHECK;
  debug_message("AFTER ARGLIST CHECK\n");
  LispObject *arglist = arg->next->value;

  LispListElement *i;
  for (i = arglist->value_list; i->value != NULL; i = i->next) {
    assert_or_error(i->value->type == LISPOBJECT_SYMBOL, "define", "Argument has wrong type: arglist items should be Symbols, not %s", LispObject_type(i->value));
    ERROR_CHECK;
  }
  debug_message("AFTER ARGLIST_ITEMS CHECK\n");

  LispObject *body = arg->next->next->value;
  if (arg->next->next->value->type == LISPOBJECT_LIST) {
    LispFunction *lfunc = LispFunction_new(arglist->value_list, body);
    LispEnvironment_add_variable(env, name->symbol_name, lfunc, NULL, NULL);
  }
  else {
    LispEnvironment_add_variable(env, name->symbol_name, NULL, NULL, body);
  }


  return name;
}
LispBuiltin define_obj = {&define, LISPBUILTIN_LAZY};




// Create an entry in the local environment's variable table.
// Usage: (defvar name value)
// Arguments:
//   name            string or symbol name of the function
//   value           value of the variable
LispObject *defvar(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION DEFVAR\n");

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 2, "defvar", "Function takes 2 arguments: name, value (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");
  
  LispObject *name = arg->value;
  assert_or_error(name->type == LISPOBJECT_SYMBOL, "defvar", "Argument has wrong type: name should be a Symbol, not %s", LispObject_type(name));
  ERROR_CHECK;
  debug_message("AFTER NAME TYPE CHECK\n");

  if (!LispEnvironment_get(env, name->symbol_name, NULL, NULL, NULL)) {
    LispEnvironment_del_variable(env, name->symbol_name);
  }
  Exception_reset();

  LispObject *value = arg->next->value;
  LispEnvironment_add_variable(env, name->symbol_name, NULL, NULL, value);

  return value;
}
LispBuiltin defvar_obj = {&defvar, LISPBUILTIN_LAZY};




// Get number of elements in a list
// Usage:
//   (count list)
// Arguments:
//   
LispObject *count(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION count\n");

  TOUCH(env);
  int nargs = LispList_count(arg);
  assert_or_error(nargs == 1, "define", "Function takes 1 arguments: list (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");
  
  LispObject *list = arg->value;
  assert_or_error(list->type == LISPOBJECT_LIST, "define", "Argument has wrong type: list should be a List, not %s", LispObject_type(list));
  ERROR_CHECK;
  debug_message("AFTER LIST CHECK\n");

  int nelems = LispList_count(list->value_list); // obj->list_child is the real start of the list
  LispObject *rv = LispObject_new_int(nelems);

  return rv;
}
LispBuiltin count_obj = {&count, LISPBUILTIN_LAZY};




// If $condition, return eval($thing), else return eval(otherthing)
LispObject *lisp_if(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION lisp_if");

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 3, "lisp_if", "Function takes 3 arguments: condition, result-if-true, result-else (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");
  
  LispObject *condition = eval(arg->value, env);
  ERROR_CHECK;
  int condition_value = LispObject_is_truthy(condition);
  ERROR_CHECK;
  debug_message("AFTER CONDITION EVAL\n");

  LispObject *rv = NULL;

  if (condition_value) {
    LispObject *value_if_true = arg->next->value;
    rv = eval(value_if_true, env);
    ERROR_CHECK;
  }
  else {
    LispObject *value_else = arg->next->next->value;
    rv = eval(value_else, env);
    ERROR_CHECK;
  }

  return rv;
}
LispBuiltin lisp_if_obj = {&lisp_if, LISPBUILTIN_LAZY};




// compare: using the object comparison funcs
#define COMPARE(OPNAME) \
  debug_message("BUILTIN FUNCTION lisp_comparison");\
  TOUCH(env);\
  int nargs = LispList_count(arg);\
  assert_or_error(nargs == 2, "lisp_comparison", "Function takes 2 arguments: left, right (got %d).", nargs);\
  ERROR_CHECK;\
  LispObject \
    *left = arg->value, \
    *right = arg->next->value;\
  int rv_value = OPNAME(left, right);\
  ERROR_CHECK;\
  return LispObject_new_bool(rv_value);\

LispObject *lisp_gt(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_gt)}
LispObject *lisp_ge(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_ge)}
LispObject *lisp_lt(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_lt)}
LispObject *lisp_le(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_le)}
LispObject *lisp_eq(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_eq)}
LispBuiltin lisp_gt_obj = {&lisp_gt, LISPBUILTIN_GREEDY};
LispBuiltin lisp_ge_obj = {&lisp_ge, LISPBUILTIN_GREEDY};
LispBuiltin lisp_lt_obj = {&lisp_lt, LISPBUILTIN_GREEDY};
LispBuiltin lisp_le_obj = {&lisp_le, LISPBUILTIN_GREEDY};
LispBuiltin lisp_eq_obj = {&lisp_eq, LISPBUILTIN_GREEDY};




// pop (car): return first object in a list
LispObject *pop(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION POP/CDR\n");

  TOUCH(env);

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 1, "pop", "Function takes 1 arguments: list (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");

  LispObject *list = arg->value;
  assert_or_error(list->type == LISPOBJECT_LIST, "pop", "list must be a List, not %s", LispObject_type(list));
  ERROR_CHECK;
  debug_message("AFTER TYPE CHECK\n");

  int list_len = LispList_count(list->value_list);
  assert_or_error(list_len > 0, "pop", "pop called on a list with no elements.");
  ERROR_CHECK;
  debug_message("AFTER LIST LEN CHECK\n");

  return list->value_list->value;
}
LispBuiltin pop_obj = {&pop, LISPBUILTIN_GREEDY};




// rest (AKA cdr): return the list elements after the first i.e. !pop
LispObject *rest(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION REST/CAR");

  TOUCH(env);

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 1, "rest", "Function takes 1 arguments: list (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");

  LispObject *list = arg->value;
  assert_or_error(list->type == LISPOBJECT_LIST, "rest", "list must be a List, not %s", LispObject_type(list));
  ERROR_CHECK;
  debug_message("AFTER TYPE CHECK\n");

  int list_len = LispList_count(list->value_list);
  assert_or_error(list_len > 0, "rest", "rest called on a list with no elements.");
  ERROR_CHECK;
  debug_message("AFTER LIST LEN CHECK\n");

  LispObject *rest = calloc(1, sizeof(LispObject));
  rest->type = LISPOBJECT_LIST;
  rest->value_list = list->value_list->next;
  
  return rest;
}
LispBuiltin rest_obj = {&rest, LISPBUILTIN_GREEDY};




// print: print a string representation of an object
LispObject *print(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION PRINT");

  TOUCH(env);

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 1, "print", "Function takes 1 arguments: (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");

  LispObject *o = arg->value;

  LispObject_print(o);

  return NULL;
}
LispBuiltin print_obj = {&print, LISPBUILTIN_GREEDY};




// Load a file
LispObject *load_file(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION LOAD-FILE");

  int nargs = LispList_count(arg);
  assert_or_error(nargs == 1, "load-file", "Function takes 1 arguments: (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");

  LispObject *filename_obj = arg->value;
  assert_or_error(filename_obj->type == LISPOBJECT_STRING, "load-file", "filename must be a String, not %s", LispObject_type(filename_obj));
  ERROR_CHECK;
  debug_message("AFTER TYPE CHECK\n");

  eval_file(filename_obj->value_string, env);
  ERROR_CHECK;

  return &nil;
}
LispBuiltin load_file_obj = {&load_file, LISPBUILTIN_GREEDY};




// exit (n): exit with error code $n.
LispObject *lisp_exit(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION EXIT");

  TOUCH(env);
  int nargs = LispList_count(arg);
  assert_or_error(nargs < 2, "exit", "Function takes at most 1 argument: (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");

  int rv = 0;

  if (nargs) {
    LispObject *rv_obj = arg->value;
    assert_or_error(rv_obj->type == LISPOBJECT_INT, "exit", "filename must be an Integer, not %s", LispObject_type(rv_obj));
    ERROR_CHECK;
    debug_message("AFTER TYPE CHECK\n");
    rv = rv_obj->value_int;
  }

  exit(rv);
}
LispBuiltin exit_obj = {&lisp_exit, LISPBUILTIN_GREEDY};




// read contents of a file
// Usage:
//   (read-file path [mode])
// Arguments:
//   path    Path to file to open
LispObject *read_file(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION READ_FILE");

  TOUCH(env);
  int nargs = LispList_count(arg);
  assert_or_error(nargs == 1, "read-file", "Function takes 1 argument (path): (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");

  LispObject *pathobj = arg->value;
  assert_or_error(pathobj->type == LISPOBJECT_STRING, "read-file", "path argument must be a String, not %s", LispObject_type(pathobj));
  ERROR_CHECK;
  debug_message("AFTER PATH TYPE CHECK\n");

  FILE *fp = fopen(pathobj->value_string, "r");
  assert_or_error_with_errno(fp != NULL, "read-file", "Could not open file '%s'", pathobj->value_string);
  ERROR_CHECK;


  int rv = fseek(fp, 0, SEEK_END);
  assert_or_error_with_errno(rv == 0, "read-file", "Failed to get size of file.");
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }
  long length = ftell(fp);

  rv = fseek(fp, 0, SEEK_SET);
  assert_or_error_with_errno(rv == 0, "read-file", "Failed to return to start of file.");
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }

  char *contents = malloc((length+1)*sizeof(char));
  assert_or_error_with_errno(contents != NULL, "read-file", "Failed to allocate memory to hold file contents.");
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }

  rv = fread(contents, 1, length, fp);
  assert_or_error_with_errno(rv != -1, "read-file", "Failed to read file into memory.");
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }

  contents[length] = '\0';

  fclose(fp);


  return LispObject_new_string(contents);
}
LispBuiltin read_file_obj = {&read_file, LISPBUILTIN_GREEDY};




// write-file
LispObject *write_file(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION WRITE_FILE");

  TOUCH(env);
  int nargs = LispList_count(arg);
  assert_or_error(nargs == 2, "write-file", "Function takes 2 arguments (path content): (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");

  LispObject *pathobj = arg->value;
  assert_or_error(pathobj->type == LISPOBJECT_STRING, "write-file", "path argument must be a String, not %s", LispObject_type(pathobj));
  ERROR_CHECK;
  debug_message("AFTER PATH TYPE CHECK\n");

  LispObject *contentobj = arg->next->value;
  assert_or_error(contentobj->type == LISPOBJECT_STRING, "write-file", "content argument must be a String, not %s", LispObject_type(contentobj));
  ERROR_CHECK;
  debug_message("AFTER CONTENT TYPE CHECK\n");

  FILE *fp = fopen(pathobj->value_string, "w");
  assert_or_error_with_errno(fp != NULL, "write-file", "Could not open file '%s'", pathobj->value_string);
  ERROR_CHECK;

  int rv = fprintf(fp, "%s", contentobj->value_string);
  assert_or_error(rv > 0, "write-file", "Could not write file '%s'", pathobj->value_string);
  ERROR_CHECK;
  fclose(fp);

  return LispObject_new_string(contentobj->value_string);
}
LispBuiltin write_file_obj = {&write_file, LISPBUILTIN_GREEDY};




// append-file
LispObject *append_file(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION WRITE_FILE");

  TOUCH(env);
  int nargs = LispList_count(arg);
  assert_or_error(nargs == 2, "write-file", "Function takes 2 arguments (path content): (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");

  LispObject *pathobj = arg->value;
  assert_or_error(pathobj->type == LISPOBJECT_STRING, "write-file", "path argument must be a String, not %s", LispObject_type(pathobj));
  ERROR_CHECK;
  debug_message("AFTER PATH TYPE CHECK\n");

  LispObject *contentobj = arg->next->value;
  assert_or_error(contentobj->type == LISPOBJECT_STRING, "write-file", "content argument must be a String, not %s", LispObject_type(contentobj));
  ERROR_CHECK;
  debug_message("AFTER CONTENT TYPE CHECK\n");

  FILE *fp = fopen(pathobj->value_string, "a");
  assert_or_error_with_errno(fp != NULL, "write-file", "Could not open file '%s'", pathobj->value_string);
  ERROR_CHECK;

  int rv = fprintf(fp, "%s", contentobj->value_string);
  assert_or_error(rv > 0, "write-file", "Could not write file '%s'", pathobj->value_string);
  ERROR_CHECK;
  fclose(fp);

  return LispObject_new_string(contentobj->value_string);
}
LispBuiltin append_file_obj = {&append_file, LISPBUILTIN_GREEDY};




// builtins are enumerated here, and referred to in the global env setup
struct environment_var builtin_functions[] = {
  { "add", "+", NULL, &add_obj, NULL, NULL},
	{ "subtract", "-", NULL, &subtract_obj, NULL, NULL },
	{ "multiply", "*", NULL, &multiply_obj, NULL, NULL },
	{ "divide", "/", NULL, &divide_obj, NULL, NULL },
	{ "quote", NULL, NULL, &quote_obj, NULL, NULL },
	{ "define", NULL, NULL, &define_obj, NULL, NULL },
	{ "defvar", NULL, NULL, &defvar_obj, NULL, NULL },
	{ "count", NULL, NULL, &count_obj, NULL, NULL },
	{ "if", NULL, NULL, &lisp_if_obj, NULL, NULL },
	{ "gt", ">", NULL, &lisp_gt_obj, NULL, NULL },
	{ "ge", ">=", NULL, &lisp_ge_obj, NULL, NULL },
	{ "lt", "<", NULL, &lisp_lt_obj, NULL, NULL },
	{ "le", "<=", NULL, &lisp_le_obj, NULL, NULL },
	{ "eq", "=", NULL, &lisp_eq_obj, NULL, NULL },
	{ "pop", "car", NULL, &pop_obj, NULL, NULL },
	{ "rest", "cdr", NULL, &rest_obj, NULL, NULL },
	{ "print", NULL, NULL, &print_obj, NULL, NULL },
	{ "load-file", NULL, NULL, &load_file_obj, NULL, NULL },
	{ "exit", NULL, NULL, &exit_obj, NULL, NULL },
	{ "read-file", NULL, NULL, &read_file_obj, NULL, NULL },
	{ "write-file", NULL, NULL, &write_file_obj, NULL, NULL },
	{ "append-file", NULL, NULL, &append_file_obj, NULL, NULL },
  { NULL, NULL, NULL, NULL, NULL, NULL }
};
