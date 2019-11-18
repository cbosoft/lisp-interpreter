#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "ast.h"
#include "object.h"
#include "exception.h"
#include "debug.h"
#include "list.h"



LispObject *parse(char **tokens, int n_tokens)
{
  LispObject *root = LispObject_new_list();
  LispObject *current = root, *new;

  debug_message("IN PARSE\n");

  LispObject **parents = NULL;
  int nparents = 0;

  for (int i = 0; i < n_tokens; i++) {
    debug_message("TOKEN: %s\n", tokens[i]);

    if (strcmp(tokens[i], "(") == 0) {
      new = LispObject_new_list();
      ERROR_CHECK;

      nparents ++;
      parents = realloc(parents, nparents*sizeof(LispObject *));

      parents[nparents-1] = current;
      LispList_add_object_to_list(current->value_list, new);
      ERROR_CHECK;

      current = new;
    }
    else if (strcmp(tokens[i], ")") == 0) {
      debug_message("CLOSING LIST\n");
      current = parents[nparents-1];
      nparents --;
      parents = realloc(parents, nparents*sizeof(LispObject *));
    }
    else {
      debug_message("GUESS\n");
      new = LispObject_new_guess_type(tokens[i]);
      ERROR_CHECK;

      debug_message("NEW OBJECT %s (%s)\n", LispObject_repr(new), LispObject_type(new));

      debug_message("ADD TO LIST\n");
      LispList_add_object_to_list(current->value_list, new);
      ERROR_CHECK;
    }

  }

  free(parents);
  return root;
}
