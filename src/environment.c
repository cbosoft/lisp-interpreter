#include <stdlib.h>
#include <string.h>

#include "environment.h"
#include "types.h"

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




// Delete environement and free object from memory
void LispEnvironment_free(LispEnvironment *env)
{
  free(env);
}




// Add an object $value to the environment.
void LispEnvironment_add_symbol(LispEnvironment *env, LispObject *value)
{
  LispObject *i, *v;
  v = value; //LispObject_copy(value);
  
  if (env->symbols == NULL) {
    env->symbols = v;
  }
  else {
    i = env->symbols;
    while (i->list_next != NULL) i = i->list_next;

    i->list_next = v;
  }
}




// Given the name of a symbol, get its value from the environment
LispObject *LispEnvironment_get_symbol(LispEnvironment *env, char *name)
{
  LispObject *i;
  
  if (env == NULL)
    return NULL;

  if (env->symbols == NULL)
    return LispEnvironment_get_symbol(env->parent, name);

  i = env->symbols;
  while (i->list_next != NULL) {
    if (strcmp(i->symbol_name, name) == 0) {
      return i;
    }
  }

  return LispEnvironment_get_symbol(env->parent, name);
}
