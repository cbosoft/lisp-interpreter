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
	{ "add", "+", "(add left right)\nReturns left + right", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&add)), NULL },
	{ "subtract", "-", "(subtract left right)\nReturns left - right", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&subtract)), NULL },
	{ "multiply", "*", "(multiply left right)\nReturns left * right", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&multiply)), NULL },
	{ "divide", "/", "(divide left right)\nReturns left / right", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&divide)), NULL },
	{ "modulo", NULL, "(modulo left right)\nReturns left % right", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&modulo)), NULL },
	{ "random", NULL, "(random) returns uniform random float in range (0 1)", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&random)), NULL },
	{ "randint", NULL, "(randint) returns uniform random integer in range (0, RAND_MAX)", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&randint)), NULL },

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
	{ "list", NULL, "(list &rest elements)\nReturns list of ELEMENTS.", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&list)), NULL },
	{ "rest", NULL, "(rest list)\nReturns list with all but first element of LIST.", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&rest)), NULL },
	{ "pop", NULL, "(pop list)\nReturns first element of LIST.", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&pop)), NULL },
	{ "append", NULL, "(append list &rest elements)\nAdds ELEMENTS to LIST.", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&append)), NULL },

  // Modules and other files
	{ "eval", NULL, "(eval obj)\nEvaluates object, returns result.", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&eval)), NULL },
	{ "eval-file", NULL, "(eval-file path)\nReads in file at PATH, parses and evaluates, returning result.", NULL, std::make_shared<LispBuiltin>(LispBuiltin(&eval_file)), NULL },
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
