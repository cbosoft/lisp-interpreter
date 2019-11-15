#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tokenise.h"
#include "exception.h"
#include "parse.h"
#include "eval.h"
#include "object.h"
#include "debug.h"

#define EITHER(S, A, B) ((strcmp(S, A) == 0) || (strcmp(S, B) == 0))


int DEBUG_MODE = 0;


int main(int argc, char **argv)
{
  char **tokens;
  int n_tokens = 0;

  // TODO argument processing.
  // no args = REPL
  // -f --file         execute commands in file
  // -c --command      execute command
  // -v --version      print interpreter version
  // -i --interactive  drop to REPL after executing $commands or $file
  // -h --help         print help screen
  // -d --debug        print more information

  for (int i = 0; i < argc; i++) {
    if (EITHER(argv[i], "-f", "--file")) {
      // next arg is file to read
    }
    else if (EITHER(argv[i], "-c", "--command")) {
      // next arg is command to read
    }
    else if (EITHER(argv[i], "-i", "--interactive")) {
      // just a flag: no argument
    }
    else if (EITHER(argv[i], "-h", "--help")) {
      // show help and exit
    }
    else if (EITHER(argv[i], "-d", "--debug")) {
      DEBUG_MODE = 1;
    }
    else {
      // unexpected argument
    }
  }

  if (argc > 1) {
    tokenise(argv[1], &tokens, &n_tokens);
  }
  else {
    tokenise("(+ (+ 1 1 1) 1)", &tokens, &n_tokens);
  }
  
  debug_message("before parse\n");
  LispObject *root = parse(tokens, n_tokens);
  for (int i = 0; i < n_tokens; i++) {
    free(tokens[i]);
  }
  debug_message("after parse\n");
  
  LispEnvironment *env = LispEnvironment_new_environment(NULL); // TODO = LispEnvironment_new_global_environment(); where builtin func defs are within the environment
  LispObject *result = eval(root, env);
  if (result == NULL) {
    if (!Exception_check()) {
      Exception_raise("main", "eval returned NULL.");
    }
    Exception_print();
  }
  else {
    LispObject_print(result);
  }
  LispObject_free(root);

  return 0;
}
