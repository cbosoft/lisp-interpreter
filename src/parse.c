#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "ast.h"
#include "object.h"

struct ast_node *new_token()
{
  struct ast_node *rv = calloc(1, sizeof(struct ast_node));

  return rv;
}


#define NEW_TOKEN(PARENT, VALUE)             \
  temp = calloc(1, sizeof(struct ast_node));   \
  if (next_is_child) {                                 \
    temp->parent = PARENT;                         \
    PARENT->child = temp;                          \
    next_is_child = 0;                                 \
  }                                                    \
  else {                                               \
    temp->parent = PARENT->parent;                 \
    temp->prev = PARENT;                           \
    PARENT->next = temp;                           \
  }                                                    \
  temp->value = calloc(strlen(VALUE)+1, sizeof(char));\
  strcpy(temp->value, VALUE);            \
  PARENT = temp;

#define NEW_NULL_TOKEN(PARENT)             \
  temp = calloc(1, sizeof(struct ast_node));   \
  if (next_is_child) {                                 \
    temp->parent = PARENT;                         \
    PARENT->child = temp;                          \
    next_is_child = 0;                                 \
  }                                                    \
  else {                                               \
    temp->parent = PARENT->parent;                 \
    temp->prev = PARENT;                           \
    PARENT->next = temp;                           \
  }                                                    \
  temp->value = NULL;\
  PARENT = temp;



LispObject *parse(char **tokens, int n_tokens)
{
  LispObject *root = new_list_object();
  LispObject *current = root, *new;

  LispObject **parents = NULL;
  int nparents = 0;

  for (int i = 0; i < n_tokens; i++) {
    if (strcmp(tokens[i], "(") == 0) {
      new = new_list_object();
      nparents ++;
      parents = realloc(parents, nparents*sizeof(LispObject *));
      parents[nparents-1] = current;
      add_object_to_list(current, new);
      current = new;
    }
    else if (strcmp(tokens[i], ")") == 0) {
      current = parents[nparents-1];
      nparents --;
      parents = realloc(parents, nparents*sizeof(LispObject *));
    }
    else {
      new = new_object_guess_type(tokens[i]);
      add_object_to_list(current, new);
    }
  }

  return root;
}
