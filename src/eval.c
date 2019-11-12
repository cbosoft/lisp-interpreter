#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "eval.h"
#include "object.h"
#include "exception.h"


typedef LispObject * (*LispBuiltin)(LispObject *);

struct function_table_entry {
  char *name;
  char *shorthand;
  LispBuiltin function;
};




// Recursively get length of list
int LispObject_list_size(LispObject *o)
{
  return o->list_next ? 1 + LispObject_list_size(o->list_next) : 1;
}




// Add two numbers
LispObject *add(LispObject *arg)
{
  assert_or_error(LispObject_list_size(arg) == 2, "add", "Function takes 2 arguments.");
  
  LispObject *l = arg;
  LispObject *r = arg->list_next;

  assert_or_error( 
      (l->type == LISPOBJECT_INT || l->type == LISPOBJECT_FLOAT) && 
      (r->type == LISPOBJECT_INT || r->type == LISPOBJECT_FLOAT),
      "add", 
      "Function expects numerical arguments, ");

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
  // TODO
  return arg;
}




// 
LispObject *multiply(LispObject *arg)
{
  // TODO
  return arg;
}



// 
LispObject *divide(LispObject *arg)
{
  // TODO
  return arg;
}




// 
LispObject *quote(LispObject *arg)
{
  return arg;
}



// 
struct function_table_entry functions[] = {
  { "add", "+", add},
  { "subtract", "-", subtract},
  { "multiply", "*", multiply},
  { "divide", "/", divide},
  { "quote", "quote", quote},
  { NULL }
};




// Get function from the function table
LispBuiltin get_function(char *name)
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
