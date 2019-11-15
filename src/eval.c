#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "eval.h"
#include "object.h"
#include "builtins.h"
#include "exception.h"
#include "environment.h"
#include "function.h"



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
