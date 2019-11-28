#pragma once
struct ast_node {
  struct ast_node *next;
  struct ast_node *prev;
  struct ast_node *child;
  struct ast_node *parent;
  int type;
  char *value;
};
