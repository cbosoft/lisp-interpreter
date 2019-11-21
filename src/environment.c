#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "environment.h"
#include "exception.h"
#include "types.h"
#include "debug.h"
#include "gc.h"

extern struct environment_var builtin_functions[];



// Create an environment (a collection of symbols), with optional parent
// environment $parent. If there is no parent, this should be set to NULL.
LispEnvironment *LispEnvironment_new_environment(LispEnvironment *parent)
{
  LispEnvironment *rv = calloc(1, sizeof(LispEnvironment));
  rv->parent = parent;
  return rv;
}




// Create a new global environment. This environment has no parent (it is set to
// NULL), and contains the standard library of builtin functions and any
// constants.
LispEnvironment *LispEnvironment_new_global_environment()
{
  LispEnvironment *rv = calloc(1, sizeof(LispEnvironment));

  struct environment_var row = builtin_functions[0];
  int i;
  for (i = 0, row = builtin_functions[0]; row.name != NULL; i++, row = builtin_functions[i]) {
    debug_message("ADDED SYMBOL (%s, %s) TO GLOBAL ENV\n", row.name, row.alias);
    LispEnvironment_add_variable_with_alias(rv, row.name, row.alias, row.lisp_function, row.lisp_builtin, row.value);
  }

  return rv;
}




// Addd a new variable to the environment, with value $value, name $name, alias
// $alias or...
void LispEnvironment_add_variable(LispEnvironment *env, char *name, LispFunction *lfunc, LispBuiltin *bfunc, LispObject *value)
{
  LispEnvironment_add_variable_with_alias(env, name, NULL, lfunc, bfunc, value);
}




// Addd a new variable to the environment, with value $value, name $name, alias
// $alias or...
void LispEnvironment_del_variable(LispEnvironment *env, char *name)
{
  struct environment_var *i = NULL, *p = NULL;

  if (env->variables != NULL) {
    for (i = env->variables; i != NULL; p = i, i = i->next){
      debug_message("CHECKING %s IS %s?\n", name, i->name);

      if ((strcmp(i->name, name) == 0) || ( (i->alias != NULL) && (strcmp(i->alias, name) == 0))) {

        debug_message("FOUND %s IN ENV AS %s\n", name, i->name);
        debug_message("DELETING FROM ENVIRONMENT\n");

        p->next = i->next;
        // i is now unreachable, GC will take care of it.

        return;

      }

    }
  }

  assert_or_error(0, "LispEnvironment_del_variable", "Variable with name or alias %s not found in environment.", name);

}




// Add a new variable to the environment, with value $value, lfunc $lfunc, or
// bfunc $bfunc.
void LispEnvironment_add_variable_with_alias(LispEnvironment *env, char *name, char *alias, LispFunction *lfunc, LispBuiltin *bfunc, LispObject *value)
{

  // "conditional jump or move depends on uninitialised value" // somewhere in
  // this function...
  //
  struct environment_var 
    *var = calloc(1, sizeof(struct environment_var)), 
    *i = NULL;

  // TODO error check on malloc

  var->name = strdup(name);

  if (alias != NULL)
    var->alias = strdup(alias);
  else
    var->alias = NULL;

  var->lisp_function = lfunc;
  var->lisp_builtin = bfunc;
  var->value = value;

  if (env->variables == NULL) {
    env->variables = var;
  }
  else {
    i = env->variables;
    while (i->next != NULL) i = i->next;

    i->next = var;
  }
}




// Given the name of a variable, get its value and return 0. If no value is found, return 1.
int LispEnvironment_get(LispEnvironment *env, char *name, LispFunction **lfunc, LispBuiltin **bfunc, LispObject **lobj)
{
  struct environment_var *i;
  
  if (env == NULL) // no environment, nothing to check.
    return 1; // TODO should this be an error?

  debug_message("LOOKING FOR VARIABLE %s IN ENV\n", name);

  if (env->variables != NULL) {
    i = env->variables;
    while (i != NULL) {
      debug_message("CHECKING %s IS %s?\n", name, i->name);

      if ((strcmp(i->name, name) == 0) || ( (i->alias != NULL) && (strcmp(i->alias, name) == 0))) {

        debug_message("FOUND %s IN ENV AS %s\n", name, i->name);
        
        if (lfunc != NULL)
          (*lfunc) = i->lisp_function;
        
        if (bfunc != NULL)
          (*bfunc) = i->lisp_builtin;
        
        if (lobj != NULL)
          (*lobj) = i->value;

        return 0;
      }

      i = i->next;
    }
  }

  debug_message("Variable not found in local env, trying parent.\n");
  return LispEnvironment_get(env->parent, name, lfunc, bfunc, lobj);
}




// Get value of variable in environment. Raise exception if value is not found, or
// if value found is of wrong type.
LispObject *LispEnvironment_get_variable(LispEnvironment *env, char *name)
{
  LispObject *rv = NULL;
  assert_or_error(!LispEnvironment_get(env, name, NULL, NULL, &rv), "LispEnvironment_get_variable", "Object with name \"%s\" not found.", name);
  ERROR_CHECK;
  
  assert_or_error(rv != NULL, "LispEnvironment_get_variable", "Object \"%s\" has no value as variable.");
  ERROR_CHECK;

  return rv;
}




// Get value of lispfunction in environment. Raise exception if value is not found, or
// if value found is of wrong type.
LispFunction *LispEnvironment_get_lispfunction(LispEnvironment *env, char *name)
{
  LispFunction *rv = NULL;
  assert_or_error(!LispEnvironment_get(env, name, &rv, NULL, NULL), "LispEnvironment_get_lispfunction", "Object with name \"%s\" not found.", name);
  ERROR_CHECK;
  
  assert_or_error(rv != NULL, "LispEnvironment_get_lispfunction", "Object \"%s\" has no value as function.");
  ERROR_CHECK;

  return rv;
}




// Get value of lispfunction in environment. Raise exception if value is not found, or
// if value found is of wrong type.
LispBuiltin *LispEnvironment_get_builtinfunction(LispEnvironment *env, char *name)
{
  LispBuiltin *rv = NULL;
  assert_or_error(!LispEnvironment_get(env, name, NULL, &rv, NULL), "LispEnvironment_get_builtinfunction", "Object with name \"%s\" not found.", name);
  ERROR_CHECK;
  
  assert_or_error(rv != NULL, "LispEnvironment_get_builtinfunction", "Object \"%s\" has no value as function.");
  ERROR_CHECK;

  return rv;
}
