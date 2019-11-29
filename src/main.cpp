#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <any>
#include <iostream>

//#include <editline/readline.h>

#include "tokenise.hpp"
#include "types.hpp"
// #include "exception.h"
// #include "parse.h"
// #include "eval.h"
// #include "object.h"
// #include "debug.h"
// #include "util.h"
// #include "help.h"

extern int DEBUG_MODE;
int main(void)
{
  DEBUG_MODE = 1;
  LispToken *tokens = tokenise("(+ 1 1)");
  tokens->print();

  LispParser *parser = new LispParser();
  LispListElement *root = parser->parse(tokens);
  if (root == NULL) exit(1);
  root->print();

  return 0;
}
