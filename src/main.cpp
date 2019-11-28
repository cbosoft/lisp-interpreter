#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <any>
#include <iostream>

//#include <editline/readline.h>

#include "tokenise.hpp"
#include "atom.hpp"
// #include "exception.h"
// #include "parse.h"
// #include "eval.h"
// #include "object.h"
// #include "debug.h"
// #include "util.h"
// #include "help.h"


int main(void)
{

  LispToken *tokens = tokenise("(+ 1 1)");
  tokens->print();

  LispAtom left(3);
  LispAtom right(4);
  LispAtom res = left + right;
  std::cout << res.repr() << std::endl;

  return 0;
}
