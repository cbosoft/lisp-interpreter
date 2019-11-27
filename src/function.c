#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "object.h"
#include "debug.h"
#include "list.h"
#include "gc.h"


LispFunction *LispFunction_new(LispListElement *list_args, LispObject *body)
{
  LispFunction *rv = calloc(1, sizeof(LispFunction));
  
  int nargs = LispList_count(list_args);
  debug_message("CREATING NEW LFUNC WITH %d ARGS\n", nargs);

  // save symbol names as list of arg names
  rv->arg_names = calloc(nargs, sizeof(char*));
  LispListElement *list_iter;
  int i;
  for (list_iter = list_args, i = 0; list_iter->value != NULL; list_iter = list_iter->next, i++) {

    // TODO assert list_iter->value is symbol (?)
    rv->arg_names[i] = strdup(list_iter->value->value_symbol);
    debug_message("ADDING ARG TO LFUNC: %s\n", rv->arg_names[i]);

  }

  rv->number_required_args = nargs;
  // TODO split nargs on a keyword before is reqd, after is optional...

  rv->body = body;

  return rv;
}
