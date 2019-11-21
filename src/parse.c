#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "ast.h"
#include "object.h"
#include "exception.h"
#include "debug.h"
#include "list.h"



LispListElement *parse(char **tokens, int n_tokens)
{
  // TODO: root should, instead of a list object, be just a linked list (no object).
  LispListElement *root = LispList_new_element();
  LispObject *current_list = NULL, *new = NULL, **open_lists= NULL;

  debug_message("IN PARSE\n");

  int n_open_lists = 0;

  for (int i = 0; i < n_tokens; i++) {
    debug_message("TOKEN: %s\n", tokens[i]);

    if (strcmp(tokens[i], "(") == 0) {
      new = LispObject_new_list();
      ERROR_CHECK;

      if (current_list != NULL) {
        open_lists = realloc(open_lists, (++n_open_lists)*sizeof(LispObject *));
        open_lists[n_open_lists-1] = current_list;
        LispList_add_object_to_list(current_list->value_list, new);
        ERROR_CHECK;
      }
      else {
        LispList_add_object_to_list(root, new);
        ERROR_CHECK;
      }

      current_list = new;
    }
    else if (strcmp(tokens[i], ")") == 0) {
      debug_message("CLOSING LIST\n");

      if (n_open_lists) {
        current_list = open_lists[(--n_open_lists)];
        open_lists = realloc(open_lists, n_open_lists*sizeof(LispObject *));
      }
      else {
        current_list = NULL;
        open_lists = NULL;
      }

    }
    else {
      debug_message("GUESS\n");
      new = LispObject_new_guess_type(tokens[i]);
      ERROR_CHECK;

      debug_message("NEW OBJECT %s (%s)\n", LispObject_repr(new), LispObject_type(new));

      if (current_list != NULL) {
        debug_message("ADD TO LIST\n");
        LispList_add_object_to_list(current_list->value_list, new);
        ERROR_CHECK;
      }
      else {
        debug_message("ADD TO ROOT\n");
        LispList_add_object_to_list(root, new);
        ERROR_CHECK;
      }
    }

  }

  return root;
}
