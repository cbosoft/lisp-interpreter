#include <stdlib.h>
#include <stdio.h>

#include "tokenise.h"
#include "parse.h"
#include "eval.h"
#include "object.h"

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

  LispObject *root = parse(tokens, n_tokens);
  for (int i = 0; i < n_tokens; i++) {
    free(tokens[i]);
  }

  LispObject *result = eval(root, NULL);

  LispObject_print(result);
  LispObject_free(root);

  return 0;
}
