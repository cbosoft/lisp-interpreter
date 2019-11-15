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

  // TODO argument processing.
  // no args = REPL
  // -f --file      execute commands in file
  // -c --command   execute command
  // -v --version   print interpreter version
  // -h --help      print help screen

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
  
  LispEnvironment *env = LispEnvironment_new_environment(NULL); // TODO = LispEnvironment_new_global_environment(); where builtin func defs are within the environment
  LispObject *result = eval(root, env);

  LispObject_print(result);
  LispObject_free(root);

  return 0;
}
