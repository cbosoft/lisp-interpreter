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
  LispParser *parser = new LispParser();
  LispListElement *root = parser->parse_string("(+ 1 1)");

  if (root == NULL) throw "parse returned null";

  LispEnvironment *env = new LispEnvironment();

  root->eval_each(env);

  return 0;
}
