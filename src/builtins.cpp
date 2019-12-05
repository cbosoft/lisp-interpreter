#include "types.hpp"
#include "debug.hpp"
#include "formatter.hpp"
#include "builtins.hpp"

// builtins are enumerated here, and referred to in the global env setup
struct environment_table_row builtin_functions[] = {
	{ "quote", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&quote, true)), NULL },
	{ "defun", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&defun, true)), NULL },
	{ "defmacro", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&defmacro, true)), NULL },
	{ "defvar", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&defvar, true)), NULL },
	{ "exit", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&exit)), NULL },

  //maths
	{ "add", "+", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&add)), NULL },
	{ "subtract", "-", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&subtract)), NULL },
	{ "multiply", "*", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&multiply)), NULL },
	{ "×", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&multiply)), NULL },
	{ "divide", "/", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&divide)), NULL },
	{ "÷", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&divide)), NULL },

  // Comparison
	{ "greather-than", ">", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&gt)), NULL },
	{ "greather-than-or-equal-to", ">=", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&ge)), NULL },
	{ "less-than", "<", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&lt)), NULL },
	{ "less-than-or-equal-to", "<=", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&le)), NULL },
	{ "equal-to", "=", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&eq)), NULL },

  // Boolean logic and flow control
	{ "cond", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&cond, true)), NULL },
	{ "if", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&lisp_if, true)), NULL },
	{ "and", "&&", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&lisp_and)), NULL },
	{ "or", "||", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&lisp_or)), NULL },
	{ "not", "!", NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&lisp_not)), NULL },






  // sentinel
  { NULL, NULL, NULL, NULL, NULL, NULL }
};


std::string LispBuiltin::repr() {
  return "TODO";
}
