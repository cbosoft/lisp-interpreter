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
#include "debug.h"
#include "list.h"
#include "tokenise.h"
#include "parse.h"





LispObject *eval(LispObject *root, LispEnvironment *env)
{
  char *fname = NULL;
  LispObject *fn = NULL, *obj_i = NULL, *var_obj = NULL;
  LispListElement *list_elem = NULL, *list_iter = NULL, *list_args;
  LispEnvironment *sub_env = NULL;
  int nargs;
  LispFunction *var_lfunc = NULL;
  LispBuiltin *var_bfunc = NULL;

  debug_message("IN EVAL\n");

  // TODO move builtins into environment
  assert_or_error(env != NULL, "eval", "eval without env: symbols not available");
  ERROR_CHECK;

  // TODO garbage collect, or otherwise free up used memory.  Program structure,
  // a series of lisp objects, should stay in memory and not be altered until
  // finished with.  The variables and definitions in a (sub) environment should
  // be free'd after use.

  debug_message("EVAL %s\n", LispObject_type(root));
  switch (root->type) {

  case LISPOBJECT_SYMBOL:
    LispEnvironment_get(env, root->symbol_name, NULL, NULL, &var_obj);
    assert_or_error(var_obj != NULL, "eval", "Symbol %s has not value as variable", root->symbol_name);
    ERROR_CHECK;
    return var_obj;

  case LISPOBJECT_LIST:
    list_elem = root->value_list;
    
    if (list_elem->value == NULL) {
      debug_message("(empty list evals to false)\n");
      return LispObject_new_bool(0); // empty list is also boolean false
    }

    debug_message("LIST CHILD IS %s\n", LispObject_type(list_elem->value));

    fn = list_elem->value;

    if (fn->type == LISPOBJECT_LIST) {

      list_iter = root->value_list;
      if (list_iter->next) {

        debug_message("EVALUATE LIST CONTENTS IN TURN\n");
        while (list_iter->next->value != NULL) {
          debug_message(" ITEM TYPE: %s\n", LispObject_type(list_iter->value));
          eval(list_iter->value, env);
          ERROR_CHECK;

          list_iter = list_iter->next;
        }

      }
      return eval(list_iter->value, env);
    }
    // otherwise, list is not list of lists, is function call
	
    list_args = list_elem->next;
    nargs = LispList_count(list_args);

    assert_or_error(fn->type == LISPOBJECT_SYMBOL, "eval", "Cannot evaluate function: %s (%s)", LispObject_repr(fn), LispObject_type(fn));
    fname = fn->symbol_name;
    ERROR_CHECK;

    assert_or_error(!LispEnvironment_get(env, fname, &var_lfunc, &var_bfunc, &var_obj), "eval", "Object with name \"%s\" not found in environment.", fname);
    ERROR_CHECK;


    
    if (var_lfunc != NULL) {
      debug_message("SYMBOL %s IS LISP FUNCTION\n", fn->symbol_name);
      
      assert_or_error(nargs == var_lfunc->number_required_args, "eval", "number of arguments does not match required (got %d, need %d).", nargs, var_lfunc->number_required_args);
      ERROR_CHECK;
      sub_env = LispEnvironment_new_environment(env);

      list_iter = list_args;
      for (int i = 0; i < nargs; i++, list_iter = list_iter->next) {
        obj_i = eval(list_iter->value, env);
        ERROR_CHECK;
	      LispEnvironment_add_variable(sub_env, var_lfunc->arg_names[i], NULL, NULL, obj_i);
      }

      LispObject *rv = eval(var_lfunc->body, sub_env);
      // TODO free subenv (?)
      return rv;
      
    }
    else if (var_bfunc != NULL) {
      debug_message("SYMBOL %s IS BUILTIN\n", fn->symbol_name);

      LispListElement *list_args_evaluated = NULL;

      debug_message("BFUNC %s GREEDY\n", var_bfunc->type != LISPBUILTIN_GREEDY ? "is not" : "is");
      if (list_args->value != NULL && var_bfunc->type == LISPBUILTIN_GREEDY) {
        
        // evaluate first arg
        debug_message("EVALUATING FIRST ARGUMENT %s\n", LispObject_type(list_args->value));
        list_iter = list_args;

        obj_i = eval(list_args->value, env);
        ERROR_CHECK;

        list_args_evaluated = LispList_new_element();
        LispList_add_object_to_list(list_args_evaluated, obj_i);

        list_iter = list_iter->next;

        // evaluate subsequent args
        while (list_iter->value != NULL) { // NULL value indicates end of list
          debug_message("EVALUATING SUBSEQUENT ARGUMENT\n");
          obj_i = eval(list_iter->value, env);
          ERROR_CHECK;

          LispList_add_object_to_list(list_args_evaluated, obj_i);
          
          list_iter = list_iter->next;
        }
      }
      else {
        list_args_evaluated = list_args;
      }
      
      return (*var_bfunc->f)(list_args_evaluated, env);
      
    }
    else {
      assert_or_error(0, "eval", "malformed function definition found: name with no function.");
      ERROR_CHECK;
    }
    
    break;
  }
  
  // default: root is atom (float, int, string, bool...)
  return root;
}




// eval_string
LispObject *eval_string(char *s, LispEnvironment *env)
{

  char **tokens;
  int n_tokens = 0;

  tokenise(s, &tokens, &n_tokens);

  // TODO parse to return a linked list (LispListElement)
  // call eval on each var in element in turn
  LispListElement 
    *parsed_objects = parse(tokens, n_tokens), 
    *obj_iter = NULL;

  for (int i = 0; i < n_tokens; i++) {
    free(tokens[i]);
  }

  for (obj_iter = parsed_objects; obj_iter->next->value != NULL; obj_iter = obj_iter->next) {
    eval(obj_iter->value, env);
    ERROR_CHECK;
  }

  return eval(obj_iter->value, env); // Error check must be handled by caller.
}




// eval_file
LispObject *eval_file(char *filename, LispEnvironment *env)
{
  FILE *fp = fopen(filename, "r");

  assert_or_error(fp != NULL, "eval_file", "Could not open file.");
  ERROR_CHECK;

  int rv = 0;

  rv = fseek(fp, 0, SEEK_END);
  assert_or_error(rv == 0, "eval_file", "Failed to get size of file.");
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }


  long length = ftell(fp);
  rv = fseek(fp, 0, SEEK_SET);
  assert_or_error(rv == 0, "eval_file", "Failed to return to start of file.");
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }

  char *contents = malloc((length+1)*sizeof(char));
  assert_or_error(contents != NULL, "eval_file", "Failed to allocate memory to hold file contents.");
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }

  rv = fread(contents, 1, length, fp);
  assert_or_error(rv != -1, "eval_file", "Failed to read file into memory.");
  if (Exception_check()) {
    free(contents);
    fclose(fp);
    return NULL;
  }

  contents[length] = '\0';

  fclose(fp);

  LispObject *object_to_return = eval_string(contents, env);

  free(contents);

  return object_to_return;
}
