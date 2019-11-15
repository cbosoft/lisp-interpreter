#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "ast.h"
#include "object.h"



LispObject *parse(char **tokens, int n_tokens)
{
  LispObject *root = LispObject_new_list();
  LispObject *current = root, *new;

  LispObject **parents = NULL;
  int nparents = 0;

  for (int i = 0; i < n_tokens; i++) {

    if (strcmp(tokens[i], "(") == 0) {
      new = LispObject_new_list();
      nparents ++;

      parents = realloc(parents, nparents*sizeof(LispObject *));

      parents[nparents-1] = current;
      LispObject_add_object_to_list(current, new);

      current = new;
    }
    else if (strcmp(tokens[i], ")") == 0) {
      current = parents[nparents-1];
      nparents --;
      parents = realloc(parents, nparents*sizeof(LispObject *));
    }
    else {
      new = LispObject_new_guess_type(tokens[i]);
      LispObject_add_object_to_list(current, new);
    }
  }
  
  return root;
}
