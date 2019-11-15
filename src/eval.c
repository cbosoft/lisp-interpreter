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

  // TODO move builtins into environment
  assert_or_error(env != NULL, "eval", "eval without env: symbols not available");

  // TODO garbage collect, or otherwise free up used memory.  Program structure,
  // a series of lisp objects, should stay in memory and not be altered until
  // finished with.  The variables and definitions in a (sub) environment should
  // be free'd after use.

  switch (root->type) {

  case LISPOBJECT_SYMBOL:
    return eval(root->value_symbol, env);
    break;

  case LISPOBJECT_LIST:
    fn = root->list_child;
    
    if (fn == NULL)
      return NULL;

    if (fn->type == LISPOBJECT_LIST) {
      obj_i = fn;
      if (obj_i->list_next) {
        fprintf(stderr, "  eval things in list\n");
        while (obj_i->list_next != NULL) {
          fprintf(stderr, "  type: %s\n", LispObject_type(obj_i));
          eval(obj_i, env);
          obj_i = obj_i->list_next;
        }
      }
      return eval(obj_i, env);
    }
	
    args = fn->list_next;
    nargs = LispObject_list_size(args);

    fname = fn->symbol_name == NULL ? fn->value_string : fn->symbol_name;
    assert_or_error(fname != NULL, "eval", "Cannot evaluate function: %s", LispObject_repr(fn));
    struct function_table_entry *tentry = get_function(fname);
    
    if (tentry->lisp_function != NULL) {
      
      assert_or_error(nargs == tentry->lisp_function->number_required_args, "eval", "number of arguments does not match required (got %d, need %d).", nargs, tentry->lisp_function->number_required_args);
      LispEnvironment *sub_env = LispEnvironment_new_environment(env);
      obj_i = args;

      for (int i = 0; i < nargs; i++) {
	      LispEnvironment_add_symbol(sub_env, obj_i);
	      obj_i = obj_i->list_next;
      }

      LispObject *rv = eval(tentry->lisp_function->body, subenv);
      // TODO free subenv
      return rv;
      
    }
    else if (tentry->builtin_function != NULL) {

      LispObject *evaluated_args = NULL, *ev_args_i = NULL;;

      if (strcmp(tentry->name, "quote") == 0) {
        return args;
      }

      if (args != NULL) {
        
        // evaluate first arg
        obj_i = args;
        evaluated_args = eval(args, env);
        ev_args_i = evaluated_args;
        obj_i = obj_i->list_next;

        // evaluate subsequent args
        while (obj_i != NULL) {
          ev_args_i->list_next = eval(obj_i, env);
          ev_args_i = ev_args_i->list_next;
          obj_i = obj_i->list_next;
        }
      }
      
      return tentry->builtin_function(evaluated_args, env);
      
    }
    else {
      assert_or_error(0, "eval", "malformed function definition found: name with no function.");
    }
    
    break;
  }
  
  // default: root is atom (float, int, string, bool...)
  return root;
}
