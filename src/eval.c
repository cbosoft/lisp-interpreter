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
#include "gc.h"





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

  ASSERT_OR_ERROR(env != NULL, "Exception", "eval", NULL, NULL, "eval called without environment.");

  debug_message("EVAL %s\n", LispObject_type(root));
  switch (root->type) {

  case LISPOBJECT_SYMBOL:
    LispEnvironment_get(env, root->symbol_name, NULL, NULL, &var_obj);
    ASSERT_OR_ERROR(var_obj != NULL, "TypeError", "eval", root, NULL, "Symbol %s has no value as variable", root->symbol_name);
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

    ASSERT_OR_ERROR(fn->type == LISPOBJECT_SYMBOL, "Exception", "eval", NULL, NULL, "Cannot evaluate function: %s (%s)", LispObject_repr(fn), LispObject_type(fn));
    fname = fn->symbol_name;

    ASSERT_OR_ERROR(!LispEnvironment_get(env, fname, &var_lfunc, &var_bfunc, &var_obj), "NameError", "eval", fn, NULL, "Object with name \"%s\" not found in environment.", fname);

    if (var_lfunc != NULL) {
      debug_message("SYMBOL %s IS LISP FUNCTION\n", fn->symbol_name);
      
      ASSERT_OR_ERROR(nargs == var_lfunc->number_required_args, "Exception", "eval", fn, NULL, "number of arguments does not match required (got %d, need %d).", nargs, var_lfunc->number_required_args);
      sub_env = LispEnvironment_new_environment(env);

      list_iter = list_args;
      for (int i = 0; i < nargs; i++, list_iter = list_iter->next) {
        obj_i = eval(list_iter->value, env);
        ERROR_CHECK;
	      LispEnvironment_add_variable(sub_env, var_lfunc->arg_names[i], NULL, NULL, obj_i);
      }

      LispObject *rv = eval(var_lfunc->body, sub_env);
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
      Exception_raise("SyntaxError", "eval", fn, "malformed function definition found: name with no function.");
      return NULL;
    }
    
    break;
  }
  
  // default: root is atom (float, int, string, bool...)
  return root;
}




// eval_string
LispObject *eval_string(char *s, LispEnvironment *env, char *source)
{

  LispToken *tokens = tokenise(s, source);
  ERROR_CHECK;

  debug_message("TOKENS:\n");
  for (LispToken *i = tokens; i != NULL; i = i->next) {
    debug_message("| %s\n", i->string);
  }

  LispListElement 
    *parsed_objects = parse(tokens), 
    *obj_iter = NULL;
  ERROR_CHECK;

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

  ASSERT_OR_ERROR(fp != NULL, "IOError", "eval_file", NULL, NULL, "Could not open file '%s'.", filename);

  int rv = 0;

  rv = fseek(fp, 0, SEEK_END);
  ASSERT_OR_ERROR_WITH_ERRNO(rv == 0, "IOError", "eval_file", NULL, NULL, "Failed to get size of file '%s'.", filename);
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }


  long length = ftell(fp);
  rv = fseek(fp, 0, SEEK_SET);
  ASSERT_OR_ERROR_WITH_ERRNO(rv == 0, "IOError", "eval_file", NULL, NULL, "Failed to return to start of file '%s'.", filename);
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }

  char *contents = malloc((length+1)*sizeof(char));
  ASSERT_OR_ERROR_WITH_ERRNO(rv == 0, "MemoryError", "eval_file", NULL, NULL, "Failed to allocate sufficient memory to hold file '%s' contents.", filename);
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }

  rv = fread(contents, 1, length, fp);
  ASSERT_OR_ERROR(fp != NULL, "IOError", "eval_file", NULL, NULL, "Failed to read file '%s' contents.", filename);
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }

  contents[length] = '\0';

  fclose(fp);

  LispObject *object_to_return = eval_string(contents, env, filename);

  return object_to_return;
}
