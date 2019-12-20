#include "types.hpp"
#include "singletons.hpp"
#include "debug.hpp"
#include "formatter.hpp"
//#include "builtins.hpp"
#include "builtins/defmacro.hpp"
#include "builtins/add.hpp"
#include "pointer.hpp"
#include "version.hpp"

LispEnvironmentRow sentinal = {NULL, NULL, NULL, NULL, NULL};
static LispFunc_defmacro lispfunc_defmacro = LispFunc_defmacro();
static LispFunc_add lispfunc_add = LispFunc_add();

// builtins are enumerated here, and referred to in the global env setup
LispEnvironmentRow builtins[] = {

  // Function, macro, variable
  //defun_row, 
  {"defmacro", NULL, NULL, (LispBuiltin *)(&lispfunc_defmacro), NULL},
  //defvar_row,

  // Input/Output
  // print_row, with_open_row, lisp_read_row, lisp_write_row,

  // // Maths
  {"add", "+", NULL, (LispBuiltin *)(&lispfunc_add), NULL},
  // subtract_row, multiply_row, divide_row, modulo_row, random_row, randint_row,

  // // Comparison
  // gt_row, ge_row, lt_row, le_row, eq_row,

  // // Type checking
  // is_list_row, is_symbol_row, is_atom_row, is_int_row, is_float_row, is_string_row,

  // // Boolean logic and flow control
  // cond_row, lisp_if_row, lisp_and_row, lisp_or_row, lisp_not_row,

  // // List operations
  // list_row, rest_row, pop_row, append_row,

  // // Eval modules and other files
  // eval_row, eval_file_row, import_row,

  // // Misc functions
  // lisp_exit_row,
  // quote_row,



  // Variables
  // Standard file descriptors
	{ "stdin", NULL, make_ptr(LispObject((int)0)), NULL, NULL },
	{ "stdout", NULL, make_ptr(LispObject((int)1)), NULL, NULL },
	{ "stderr", NULL, make_ptr(LispObject((int)2)), NULL, NULL },

  // true/false
	{ "nil", "false", make_ptr(nil), NULL, NULL },
	{ "t", "true", make_ptr(t), NULL, NULL },

  // Meta
	{ "_crisp_version", NULL, make_ptr(LispObject(VERSION)), NULL, NULL },






  // Sentinel
  sentinal
};
