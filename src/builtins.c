#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "object.h"
#include "builtins.h"
#include "exception.h"
#include "environment.h"
#include "debug.h"
#include "types.h"
#include "function.h"
#include "list.h"


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
  
  LispObject *rv = LispObject_new_list();
  rv->value_list = arg;

  TOUCH(env);
  return rv;
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
  debug_message("AFTER NAME CHECK\n");

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
  assert_or_error(body->type == LISPOBJECT_LIST, "define", "Argument has wrong type: body should be a List, not %s", LispObject_type(body));
  ERROR_CHECK;

  LispFunction *lfunc = LispFunction_new(arglist->value_list, body);

  LispEnvironment_add_variable(env, name->symbol_name, lfunc, NULL, NULL);

  return name;
}
LispBuiltin define_obj = {&define, LISPBUILTIN_LAZY};




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



// builtins are enumerated here, and referred to in the global env setup
struct environment_var builtin_functions[] = {
  { "add", "+", NULL, &add_obj, NULL, NULL},
	{ "subtract", "-", NULL, &subtract_obj, NULL, NULL },
	{ "multiply", "*", NULL, &multiply_obj, NULL, NULL },
	{ "divide", "/", NULL, &divide_obj, NULL, NULL },
	{ "quote", NULL, NULL, &quote_obj, NULL, NULL },
	{ "define", NULL, NULL, &define_obj, NULL, NULL },
	{ "count", NULL, NULL, &count_obj, NULL, NULL },
  { NULL, NULL, NULL, NULL, NULL, NULL }
};
