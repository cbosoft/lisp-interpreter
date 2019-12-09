#include "types.hpp"
#include "debug.hpp"
#include "formatter.hpp"
#include "builtins.hpp"

// builtins are enumerated here, and referred to in the global env setup
struct environment_table_row builtin_functions[] = {
  // Defining stuff
	{ "defun", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&defun, true)), NULL },
	{ "defmacro", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&defmacro, true)), NULL },
	{ "defvar", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&defvar, true)), NULL },

  // Input/Output
	{ "print", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&print)), NULL },
	{ "with-open", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&with_open, true)), NULL },
	{ "read", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&lisp_read)), NULL },
	{ "write", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&lisp_write)), NULL },

  // Maths
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

  // List operations
	{ "rest", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&rest)), NULL },
	{ "pop", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&pop)), NULL },
	{ "list", NULL, "(list &rest elements)\nReturns list of ELEMENTS.", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&list)), NULL },

  // Modules and other files
	{ "eval-file", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&eval_file)), NULL },
	{ "import", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&import)), NULL },

  // Misc
	{ "quote", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&quote, true)), NULL },
	{ "exit", NULL, NULL, NULL, std::make_shared<LispBuiltin>(LispBuiltin(&exit)), NULL },






  // Sentinel
  { NULL, NULL, NULL, NULL, NULL, NULL }
};


std::string LispBuiltin::repr() {
  return "TODO";
}

std::string LispBuiltin::str() {
  return "TODO";
}
