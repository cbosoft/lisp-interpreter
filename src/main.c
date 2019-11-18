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

#define RV_CHECK_AND_PRINT(RV,C) \
  if ( (rv = RV) == NULL) {\
    if (!Exception_check()) {\
      Exception_raise("main", "eval returned NULL.");\
    }\
    Exception_print();\
    return 1;\
  }\
  else if (C){\
    LispObject_print(rv);\
  }



int DEBUG_MODE = 0;
int INTERACTIVE_MODE = 0;


int main(int argc, char **argv)
{
  // TODO argument processing.
  // no args = REPL
  // -f --file         execute commands in file
  // -c --command      execute command
  // -v --version      print interpreter version
  // -i --interactive  drop to REPL after executing $commands or $file
  // -h --help         print help screen
  // -d --debug        print more information

  for (int i = 0; i < argc; i++) {

    if (EITHER(argv[i], "-i", "--interactive")) {
      INTERACTIVE_MODE = 1;
    }
    else if (EITHER(argv[i], "-h", "--help")) {
      // show help and exit
    }
    else if (EITHER(argv[i], "-d", "--debug")) {
      DEBUG_MODE = 1;
    }

  }


  LispEnvironment *env = LispEnvironment_new_global_environment();
  LispObject *rv = NULL;

  for (int i = 0; i < argc; i++) {

    if (EITHER(argv[i], "-f", "--file")) {
      RV_CHECK_AND_PRINT(eval_file(argv[++i], env), i >= argc - 1);
    }
    else if (EITHER(argv[i], "-c", "--command")) {
      RV_CHECK_AND_PRINT(eval_string(argv[++i], env), i >= argc - 1);
    }

  }

  if (!INTERACTIVE_MODE) return 0;
  
  // TODO REPL
  // READ
  // EVAL
  // PRINT
  // LOOP

  return 0;
}
