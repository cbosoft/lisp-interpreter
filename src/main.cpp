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
  LispList *root = parser->parse_string("(define thing () 1) (quote (thing))");

  if (root == NULL) throw "parse returned null";

  LispEnvironment *env = new LispEnvironment();
  LispObject *result = root->eval_each(env);
  result->print();

  return 0;
}
