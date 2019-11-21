#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <editline/readline.h>

#include "tokenise.h"
#include "exception.h"
#include "parse.h"
#include "eval.h"
#include "object.h"
#include "debug.h"
#include "util.h"
#include "help.h"


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
int EXECUTED_FILE_OR_CLI_STRING = 0;


int main(int argc, char **argv)
{

  for (int i = 0; i < argc; i++) {

    if (EITHER(argv[i], "-i", "--interactive")) {
      INTERACTIVE_MODE = 1;
    }
    else if (EITHER(argv[i], "-h", "--help")) {
      display_help();
      exit(0);
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
      EXECUTED_FILE_OR_CLI_STRING = 1;
    }
    else if (EITHER(argv[i], "-c", "--command")) {
      RV_CHECK_AND_PRINT(eval_string(argv[++i], env), i >= argc - 1);
      EXECUTED_FILE_OR_CLI_STRING = 1;
    }

  }

  if (EXECUTED_FILE_OR_CLI_STRING && !INTERACTIVE_MODE) return 0;

  display_splash();

  stifle_history(7);
  
  while (1) {
    // READ
    char *input = readline("> ");

    if (input != NULL) {
      char *stripped_input = strip_whitespace(input);

      if (*stripped_input) {

        char *expanded;
        int result;

        result = history_expand(stripped_input, &expanded);

        if (result < 0 || result == 2) {
        }
        else {
          add_history(expanded);

          // EVAL, PRINT
          if ( (rv = eval_string(expanded, env)) == NULL) {
            if (!Exception_check()) {
              Exception_raise("main", "eval returned NULL.");
            }
            Exception_print();
          }
          else {
            LispObject_print(rv);
          }
        }

      }

      // LOOP
    }
    else {
      fprintf(stderr, "\n");
    }
  }

  return 0;
}
