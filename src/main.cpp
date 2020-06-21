#include <cstring>
#include <cstdio>
#include <memory>

#include <editline/readline.h>

#include "util/debug.hpp"
#include "util/exception.hpp"
#include "util/help.hpp"
#include "types.hpp"
#include "parser/parser.hpp"
#include "object/object.hpp"
#include "list/list.hpp"
#include "env/env.hpp"


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
int rv = 0;

// BOO: GLOBALS
LispParser parser;

int main(int argc, char **argv)
{

  for (int i = 1; i < argc; i++) {

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

  LispEnvironment_ptr env;
  try {
    env = std::make_shared<LispEnvironment>();
  }
  catch (const Exception& ex) {
    ex.pretty_print();
    exit(1);
  }
  catch (const LispObject_ptr& o) {
    std::cerr << BG_RED "Uncaught thrown object" RESET ": " << o->str() << "." << std::endl;
    exit(1);
  }

  LispList_ptr root = NULL;
  LispObject_ptr result = NULL;

  for (int i = 1; i < argc; i++) {

    if (EITHER(argv[i], "-f", "--file")) {
      EXECUTED_FILE_OR_CLI_STRING = 1;
      try {
        root = parser.parse_file(argv[++i]);
        result = root->eval_each(env);
      }
      catch (const Exception& ex) {
        ex.pretty_print();
      }
      catch (const LispObject_ptr& o) {
        std::cerr << BG_RED "Uncaught thrown object" RESET ": " << o->str() << "." << std::endl;
      }
    }
    else if (EITHER(argv[i], "-c", "--command")) {
      EXECUTED_FILE_OR_CLI_STRING = 1;
      try {
        i++;
        root = parser.parse_string(argv[i], {.type=TRACESOURCE_ARGUMENT, .path_or_commands=argv[i], .row=0, .column=0, .token_length=0});
        result = root->eval_each(env);
      }
      catch (const Exception& ex) {
        ex.pretty_print();
      }
      catch (const LispObject_ptr& o) {
        std::cerr << BG_RED "Uncaught thrown object" RESET ": " << o->str() << "." << std::endl;
      }
    }
    else if (
        (EITHER(argv[i], "-d", "--debug")) || 
        (EITHER(argv[i], "-h", "--help")) || 
        (EITHER(argv[i], "-i", "--interactive"))) {
      // flag: ignore
    }
    else {
      EXECUTED_FILE_OR_CLI_STRING = 1;
      try {
        root = parser.parse_file(argv[i]);
        result = root->eval_each(env);
      }
      catch (const Exception& ex) {
        ex.pretty_print();
        rv = 1;
      }
      catch (const LispObject_ptr& o) {
        std::cerr << BG_RED "Uncaught thrown object" RESET ": " << o->str() << "." << std::endl;
        rv = 1;
      }
    }

  }

  if (EXECUTED_FILE_OR_CLI_STRING && !INTERACTIVE_MODE) return rv;

  display_splash();

  stifle_history(7);

  std::stringstream input_ss, prompt_ss;
  std::string input_str, prompt_str;
  const char *prompt_cstr = NULL;
  char *buf = NULL;

  
  while (true) {
    input_ss.str("");

    try {
      // read
      do {
        buf = readline("> "); 
      } while (buf ==  NULL);
      
      if (!strlen(buf)) {
        free(buf);
        continue;
      }

      input_ss << buf;
      int parens_tally = parser.count_parens(buf);
      free(buf);

      while (parens_tally > 0) {
        input_ss << " ";

        prompt_ss.str("--> ");
        for (int tallyi = 0; tallyi < parens_tally+2; tallyi++)
          prompt_ss << "  ";
        prompt_str = prompt_ss.str();
        prompt_cstr = prompt_str.c_str();

        do {
          buf = readline(prompt_cstr);
        } while (buf == NULL);

        input_ss << buf;
        parens_tally += parser.count_parens(buf);
        free(buf);
      }

      if (parens_tally < 0) {
        throw SyntaxError("Unmatched parenthesis.");
      }
      
      input_str = input_ss.str();
      add_history(input_str.c_str());
      root = parser.parse_string(input_str, {.type=TRACESOURCE_REPL, .path_or_commands=input_str, .row=0, .column=0, .token_length=0});
      result = root->eval_each(env);
      std::cout << "=> ";
      result->print();
    }
    catch (const Exception& ex) {
      ex.pretty_print();
    }
    catch (const LispObject_ptr& o) {
      std::cerr << BG_RED "Uncaught thrown object" RESET ": " << o->str() << "." << std::endl;
    }

    // eval, print, loop
  }

  return 0;
}
