#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "eval.h"
#include "object.h"
#include "exception.h"
#include "environment.h"
#include "function.h"


typedef LispObject * (*LispBuiltin)(LispObject *);

struct function_table_entry {
  char *name;
  char *shorthand;
  LispBuiltin builtin_function;
  LispFunction *lisp_function;
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




LispObject *eval(LispObject *root, LispEnvironment *env)
{
  char *fname = NULL;
  LispObject *args = NULL, *fn = NULL, *obj_i = NULL;
  LispEnvironment *subenv = NULL;
  int nargs;

  // TODO change to assert_or_error

  switch (root->type) {
  case LISPOBJECT_SYMBOL:
    return eval(root->value_symbol, env);
    break;
  case LISPOBJECT_LIST:
    fn = root->list_next;
    
    if (fn == NULL)
      return NULL;
	
    args = fn->list_next;
    nargs = LispObject_list_size(args);

    fname = fn->symbol_name != NULL ? fn->symbol_name : fn->value_string;
    assert_or_error(fname != NULL, "eval", "Cannot evalutate function with null name");
    struct function_table_entry *tentry = get_function(fname);
    
    if (tentry->lisp_function != NULL) {
      
      assert_or_error(nargs == tentry->lisp_function->number_required_args, "eval", "number of arguments does not match required (got %d, need %d).", nargs, tentry->lisp_function->number_required_args);
      LispEnvironment *sub_env = environment_new_environment(env);
      obj_i = args;
      for (int i = 0; i < nargs; i++) {
	environment_add_symbol(sub_env, obj_i);
	obj_i = obj_i->list_next;
      }

      return eval(tentry->lisp_function->body, subenv);
      
    }
    else if (tentry->builtin_function != NULL) {

      return tentry->builtin_function(args);
      
    }
    else {
      assert_or_error(0, "eval", "malformed function definition found: name with no function.");
    }
    
    break;
  default:
    return root;
  }
  
  fname = root->value_string;
  struct function_table_entry *value = get_function(fname);

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
