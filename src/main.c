#include <stdlib.h>
#include <stdio.h>

#include "tokenise.h"
#include "ast.h"
#include "parse.h"
#include "eval.h"

int main(int argc, char **argv)
{
  char **tokens;
  int n_tokens = 0;

  if (argc > 1) {
    tokenise(argv[1], &tokens, &n_tokens);
  }
  else {
    tokenise("(+ (+ 1 1 1) 1)", &tokens, &n_tokens);
  }

  struct ast_node *ast = get_ast(tokens, n_tokens);
  for (int i = 0; i < n_tokens; i++) {
    free(tokens[i]);
  }

  char *result = eval(ast);

  fprintf(stderr, "RESULT: %s\n", result);
  
  free_ast(ast);

  return 0;
}
