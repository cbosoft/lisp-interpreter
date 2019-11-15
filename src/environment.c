#include <stdlib.h>
#include <string.h>

#include "environment.h"




// Create an environment (a collection of symbols), with optional parent
// environment $parent. If there is no parent, this should be set to NULL.
LispEnvironment *LispEnvironment_new_environment(LispEnvironment *parent)
{
  LispEnvironment *rv = calloc(1, sizeof(LispEnvironment));
  rv->parent = parent;
  return rv;
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
