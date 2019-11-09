#include <stdlib.h>
#include <stdio.h>

#include "tokenise.h"

int main(void)
{
  char **tokens;
  int n_tokens = 0;
  tokenise("(message '(1 1 1))", &tokens, &n_tokens);

  for (int i = 0; i < n_tokens; i++) {
    fprintf(stderr, "TOKEN: %s\n", tokens[i]);
  }

  return 0;
}
