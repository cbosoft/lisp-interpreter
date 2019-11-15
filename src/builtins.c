#include <stdlib.h>
#include <string.h>

#include "object.h"
#include "builtins.h"
#include "exception.h"




// Add two numbers
LispObject *add(LispObject *arg)
{
  int nargs = LispObject_list_size(arg);
  assert_or_error(nargs == 2, "add", "Function takes 2 arguments (got %d).", nargs);
  
  LispObject *l = arg;
  LispObject *r = arg->list_next;

  assert_or_error( 
      (l->type == LISPOBJECT_INT || l->type == LISPOBJECT_FLOAT) && 
      (r->type == LISPOBJECT_INT || r->type == LISPOBJECT_FLOAT),
      "add", 
      "Function expects numerical arguments, got %s and %s.", LispObject_type(l), LispObject_type(r));

  if (l->type == LISPOBJECT_FLOAT || r->type == LISPOBJECT_FLOAT) {
    double l_operand = l->type == LISPOBJECT_FLOAT ? l->value_float : (double)l->value_int;
    double r_operand = r->type == LISPOBJECT_FLOAT ? r->value_float : (double)r->value_int;
    LispObject *rv = new_float_object(l_operand + r_operand);
    return rv;
  }

  int l_operand = l->type == LISPOBJECT_FLOAT ? (int)l->value_float : l->value_int;
  int r_operand = r->type == LISPOBJECT_FLOAT ? (int)r->value_float : r->value_int;
  LispObject *rv = new_int_object(l_operand + r_operand);
  return rv;
}




// Subtract a number from another
LispObject *subtract(LispObject *arg)
{
  int nargs = LispObject_list_size(arg);
  assert_or_error(nargs == 2, "subtract", "Function takes 2 arguments (got %d).", nargs);
  
  LispObject *l = arg;
  LispObject *r = arg->list_next;

  assert_or_error( 
      (l->type == LISPOBJECT_INT || l->type == LISPOBJECT_FLOAT) && 
      (r->type == LISPOBJECT_INT || r->type == LISPOBJECT_FLOAT),
      "subtract", 
      "Function expects numerical arguments, got %s and %s.", LispObject_type(l), LispObject_type(r));

  if (l->type == LISPOBJECT_FLOAT || r->type == LISPOBJECT_FLOAT) {
    double l_operand = l->type == LISPOBJECT_FLOAT ? l->value_float : (double)l->value_int;
    double r_operand = r->type == LISPOBJECT_FLOAT ? r->value_float : (double)r->value_int;
    LispObject *rv = new_float_object(l_operand - r_operand);
    return rv;
  }

  int l_operand = l->type == LISPOBJECT_FLOAT ? (int)l->value_float : l->value_int;
  int r_operand = r->type == LISPOBJECT_FLOAT ? (int)r->value_float : r->value_int;
  LispObject *rv = new_int_object(l_operand - r_operand);
  return rv;
}




// 
LispObject *multiply(LispObject *arg)
{
  int nargs = LispObject_list_size(arg);
  assert_or_error(nargs == 2, "multiply", "Function takes 2 arguments (got %d).", nargs);
  
  LispObject *l = arg;
  LispObject *r = arg->list_next;

  assert_or_error( 
      (l->type == LISPOBJECT_INT || l->type == LISPOBJECT_FLOAT) && 
      (r->type == LISPOBJECT_INT || r->type == LISPOBJECT_FLOAT),
      "multiply", 
      "Function expects numerical arguments, got %s and %s.", LispObject_type(l), LispObject_type(r));

  if (l->type == LISPOBJECT_FLOAT || r->type == LISPOBJECT_FLOAT) {
    double l_operand = l->type == LISPOBJECT_FLOAT ? l->value_float : (double)l->value_int;
    double r_operand = r->type == LISPOBJECT_FLOAT ? r->value_float : (double)r->value_int;
    LispObject *rv = new_float_object(l_operand * r_operand);
    return rv;
  }

  int l_operand = l->type == LISPOBJECT_FLOAT ? (int)l->value_float : l->value_int;
  int r_operand = r->type == LISPOBJECT_FLOAT ? (int)r->value_float : r->value_int;
  LispObject *rv = new_int_object(l_operand * r_operand);
  return rv;
}



// 
LispObject *divide(LispObject *arg)
{
  int nargs = LispObject_list_size(arg);
  assert_or_error(nargs == 2, "divide", "Function takes 2 arguments (got %d).", nargs);
  
  LispObject *l = arg;
  LispObject *r = arg->list_next;

  assert_or_error( 
      (l->type == LISPOBJECT_INT || l->type == LISPOBJECT_FLOAT) && 
      (r->type == LISPOBJECT_INT || r->type == LISPOBJECT_FLOAT),
      "divide", 
      "Function expects numerical arguments, got %s and %s.", LispObject_type(l), LispObject_type(r));

  if (l->type == LISPOBJECT_FLOAT || r->type == LISPOBJECT_FLOAT) {
    double l_operand = l->type == LISPOBJECT_FLOAT ? l->value_float : (double)l->value_int;
    double r_operand = r->type == LISPOBJECT_FLOAT ? r->value_float : (double)r->value_int;
    LispObject *rv = new_float_object(l_operand / r_operand);
    return rv;
  }

  int l_operand = l->type == LISPOBJECT_FLOAT ? (int)l->value_float : l->value_int;
  int r_operand = r->type == LISPOBJECT_FLOAT ? (int)r->value_float : r->value_int;
  LispObject *rv = new_int_object(l_operand / r_operand);
  return rv;
}




// 
LispObject *quote(LispObject *arg)
{
  return arg;
}



// 
struct function_table_entry functions[] = {
					   { "add", "+", add, NULL},
					   { "subtract", "-", subtract, NULL},
					   { "multiply", "*", multiply, NULL},
					   { "divide", "/", divide, NULL},
					   { "quote", "quote", quote, NULL},
					   { NULL, NULL, NULL, NULL }
};




// Get function from the function table
struct function_table_entry *get_function(char *name)
{
  int i = 0;
  while (1) {
    if (functions[i].name == NULL) {
      break;
    }
    else if ((strcmp(name, functions[i].name) == 0) || (strcmp(name, functions[i].shorthand) == 0)) {
      return &functions[i];
    }

    i++;
  }
  return NULL;
}
