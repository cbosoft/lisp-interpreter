#include "types.hpp"
#include "debug.hpp"
#include "formatter.hpp"
#include "builtins.hpp"

// builtins are enumerated here, and referred to in the global env setup
struct environment_table_row builtin_functions[] = {
	{ "quote", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&quote)), NULL },
	{ "define", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&define)), NULL },
	{ "exit", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&exit)), NULL },

  //maths
	{ "add", "+", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&add)), NULL },
	{ "subtract", "-", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&subtract)), NULL },
	{ "multiply", "*", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&multiply)), NULL },
	{ "×", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&multiply)), NULL },
	{ "divide", "/", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&divide)), NULL },
	{ "÷", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&divide)), NULL },






  // sentinel
  { NULL, NULL, NULL, NULL, NULL, NULL }
};
