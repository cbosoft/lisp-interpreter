#include "types.hpp"
#include "debug.hpp"
#include "formatter.hpp"
#include "builtins.hpp"

// builtins are enumerated here, and referred to in the global env setup
struct environment_table_row builtin_functions[] = {
	{ "quote", NULL, NULL, NULL, new LispBuiltin(&quote), NULL },
	{ "define", NULL, NULL, NULL, new LispBuiltin(&define), NULL },
  { NULL } // sentinal
};
