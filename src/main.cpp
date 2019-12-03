#include <cstring>
#include <cstdio>
#include <memory>

#include <editline/readline.h>

#include "types.hpp"
#include "debug.hpp"
#include "exception.hpp"


#define EITHER(S, A, B) ((strcmp(S, A) == 0) || (strcmp(S, B) == 0))

#define RV_CHECK_AND_PRINT(RV,C) \
  if ( (rv = RV) == NULL) {\
    if (!Exception_check()) {\
      Exception_raise("Exception", "main", NULL, "eval returned NULL.");\
    }\
    Exception_print();\
    return 1;\
  }\
  else if (C){\
    LispObject_print(rv);\
  }



extern int DEBUG_MODE;
int INTERACTIVE_MODE = 0;
int EXECUTED_FILE_OR_CLI_STRING = 0;


int main(int argc, char **argv)
{

  for (int i = 1; i < argc; i++) {

    if (EITHER(argv[i], "-i", "--interactive")) {
      INTERACTIVE_MODE = 1;
    }
    else if (EITHER(argv[i], "-h", "--help")) {
      //display_help();
      exit(0);
    }
    else if (EITHER(argv[i], "-d", "--debug")) {
      DEBUG_MODE = 1;
    }

  }


  LispParser parser = LispParser();
  LispEnvironment_ptr env = std::make_shared<LispEnvironment>(LispEnvironment());
  LispList_ptr root = NULL;
  LispObject_ptr result = NULL;

  for (int i = 1; i < argc; i++) {

    if (EITHER(argv[i], "-f", "--file")) {
      root = parser.parse_file(argv[++i]);
      result = root->eval_each(env);
    }
    else if (EITHER(argv[i], "-c", "--command")) {
      root = parser.parse_string(argv[++i]);
      result = root->eval_each(env);
    }
    else if ((EITHER(argv[i], "-d", "--debug")) || (EITHER(argv[i], "-i", "--interactive"))) {
      // flag: ignore
    }
    else {
      root = parser.parse_file(argv[++i]);
      result = root->eval_each(env);
    }

  }

  if (EXECUTED_FILE_OR_CLI_STRING && !INTERACTIVE_MODE) return 0;

  //display_splash();

  stifle_history(7);

  std::stringstream input_ss, prompt_ss;
  char *buf = NULL;

  
  while (true) {
    // read
    do {
      buf = readline("> "); 
    } while (buf ==  NULL);
    
    if (!strlen(buf)) continue;

    input_ss << buf;

    int parens_tally = parser.count_parens(buf);
    while (parens_tally > 0) {
      input_ss << " ";

      prompt_ss.str(" : ");
      for (int tallyi = 0; tallyi < parens_tally; tallyi++)
        prompt_ss << "  ";

      do {
        buf = readline(prompt_ss.str().c_str());
      } while (buf == NULL);

      input_ss << buf;
      parens_tally += parser.count_parens(buf);
    }

    if (parens_tally < 0) {
      throw SyntaxError("Unmatched parenthesis.");
    }
    
    root = parser.parse_string(input_ss.str());
    result = root->eval_each(env);
    result->print();

    //eval, print, loop
  }

  return 0;
}
