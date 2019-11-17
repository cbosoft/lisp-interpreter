#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "object.h"


LispFunction *LispFunction_new(LispObject *arglist, LispObject *body)
{
  LispFunction *rv = calloc(1, sizeof(LispFunction));
  
  int nargs = LispObject_list_size(arglist);
  rv->arg_names = calloc(nargs, sizeof(char*));
  LispObject *obj_i;
  int i;
  for (obj_i = arglist->list_child, i = 0; obj_i != NULL; obj_i = obj_i->list_next, i++) {
    rv->arg_names[i] = strdup(obj_i->symbol_name);
  }

  rv->body = body;

  return rv;
}
