#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "ast.h"

enum TOKEN_TYPE {
  TOKEN_SYMBOL,
  TOKEN_NAME
};


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
  if (VALUE != NULL) { \
    temp->value = calloc(strlen(VALUE)+1, sizeof(char));\
    strncpy(temp->value, VALUE, strlen(VALUE));            \
  }                                                        \
  else {                                                   \
    temp->value = VALUE;                                   \
  }                                                        \
  PARENT = temp;



struct ast_node *get_ast(char **tokens, int n_tokens)
{
  struct ast_node *root = calloc(1, sizeof(struct ast_node));
  struct ast_node *current = root, *temp;

  int next_is_child = 0;

  for (int i = 0; i < n_tokens; i++) {
    if (strcmp(tokens[i], "(") == 0) {
      NEW_TOKEN(current, NULL);
      next_is_child = 1;
    }
    else if (strcmp(tokens[i], ")") == 0) {
      current = current->parent;
      NEW_TOKEN(current, NULL);
    }
    else {
      NEW_TOKEN(current, tokens[i]);
    }
  }

  return root;
}


void print_ast(struct ast_node *ast)
{
  struct ast_node *current = ast;
  while (1) {
    if (current->value)
      fprintf(stderr, " %s ", current->value);
    if (current->child) {
      fprintf(stderr, "[");
      print_ast(current->child);
      fprintf(stderr, "]");
    }
    if (current->next)
      current = current->next;
    else
      break;
  }
}


void free_ast(struct ast_node *ast) {
  struct ast_node *current = ast, *next;
  while (1) {

    if (current->child)
      free_ast(current->child);

    if (current->next)
      current = current->next;
    else
      break;
  }

  while (1) {
    next = current->prev;
    free(current);
    current = next;

    if (current->prev == NULL) {
      break;
    }
  }
}
