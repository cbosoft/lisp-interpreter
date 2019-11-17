#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "object.h"
#include "builtins.h"
#include "exception.h"
#include "environment.h"
#include "debug.h"
#include "types.h"




// Add two numbers
LispObject *add(LispObject *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION ADD\n");

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

  return arg;
}




// Create an entry in the local environment's function table
LispObject *define(LispObject *arg, LispEnvironment *env)
{
}



// builtins are enumerated here, and referred to in the global env setup
struct environment_var builtin_functions[] = {
  { "add", "+", NULL, &add, NULL, NULL},
	{ "subtract", "-", NULL, &subtract, NULL, NULL },
	{ "multiply", "*", NULL, &multiply, NULL, NULL },
	{ "divide", "/", NULL, &divide, NULL, NULL },
	{ "quote", "quote", NULL, &quote, NULL, NULL },
  { NULL, NULL, NULL, NULL, NULL, NULL }
};
