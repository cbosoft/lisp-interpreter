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


#define TOUCH(LE) if (LE!=NULL) {};




// Add two numbers
LispObject *add(LispObject *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION ADD\n");

  TOUCH(env);

  int nargs = LispObject_list_size(arg);
  assert_or_error(nargs == 2, "add", "Function takes 2 arguments (got %d).", nargs);
  ERROR_CHECK;
  
  LispObject *l = arg;
  LispObject *r = arg->list_next;

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




// Subtract a number from another
LispObject *subtract(LispObject *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION SUBTRACT\n");

  TOUCH(env);

  int nargs = LispObject_list_size(arg);
  assert_or_error(nargs == 2, "subtract", "Function takes 2 arguments (got %d).", nargs);
  ERROR_CHECK;
  
  LispObject *l = arg;
  LispObject *r = arg->list_next;

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




// 
LispObject *multiply(LispObject *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION MULTIPLY\n");

  TOUCH(env);

  int nargs = LispObject_list_size(arg);
  assert_or_error(nargs == 2, "multiply", "Function takes 2 arguments (got %d).", nargs);
  ERROR_CHECK;
  
  LispObject *l = arg;
  LispObject *r = arg->list_next;

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



// 
LispObject *divide(LispObject *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION DIVIDE\n");

  TOUCH(env);

  int nargs = LispObject_list_size(arg);
  assert_or_error(nargs == 2, "divide", "Function takes 2 arguments (got %d).", nargs);
  ERROR_CHECK;
  
  LispObject *l = arg;
  LispObject *r = arg->list_next;

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




// Returns args as output without change
// TODO remove this as is implemented at lower level
LispObject *quote(LispObject *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION QUOTE\n");

  TOUCH(env);
  return arg;
}




// Create an entry in the local environment's function table.
// Usage: (define name arglist body)
// Arguments:
//   name            string or symbol name of the function
//   arglist         list of string or symbol names of the function arguments,
//   body            list of things to exectue when called.
LispObject *define(LispObject *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION DEFINE\n");

  TOUCH(env);
  int nargs = LispObject_list_size(arg); // TODO error in here somehow?
  assert_or_error(nargs == 3, "define", "Function takes 3 arguments: name, arglist, body (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");
  
  LispObject *name = arg;
  assert_or_error(name->type == LISPOBJECT_SYMBOL, "define", "Argument has wrong type: name should be a Symbol, not %s", LispObject_type(name));
  ERROR_CHECK;
  debug_message("AFTER NAME CHECK\n");

  LispObject *arglist = arg->list_next;
  assert_or_error(arglist->type == LISPOBJECT_LIST, "define", "Argument has wrong type: arglist should be a List, not %s", LispObject_type(arglist));
  ERROR_CHECK;
  debug_message("AFTER ARGLIST CHECK\n");

  LispObject *i;
  for (i = arglist->list_child; i != NULL; i = i->list_next) {
    assert_or_error(i->type == LISPOBJECT_SYMBOL, "define", "Argument has wrong type: arglist items should be Symbols, not %s", LispObject_type(i));
    ERROR_CHECK;
  }
  debug_message("AFTER ARGLIST_ITEMS CHECK\n");

  LispObject *body = arg->list_next->list_next;
  assert_or_error(body->type == LISPOBJECT_LIST, "define", "Argument has wrong type: body should be a List, not %s", LispObject_type(body));
  ERROR_CHECK;

  LispFunction *lfunc = LispFunction_new(arglist, body);

  LispEnvironment_add_variable(env, name->symbol_name, lfunc, NULL, NULL);

  return name;
}



// builtins are enumerated here, and referred to in the global env setup
struct environment_var builtin_functions[] = {
  { "add", "+", NULL, &add, NULL, NULL},
	{ "subtract", "-", NULL, &subtract, NULL, NULL },
	{ "multiply", "*", NULL, &multiply, NULL, NULL },
	{ "divide", "/", NULL, &divide, NULL, NULL },
	{ "quote", NULL, NULL, &quote, NULL, NULL },
	{ "define", NULL, NULL, &define, NULL, NULL },
  { NULL, NULL, NULL, NULL, NULL, NULL }
};
