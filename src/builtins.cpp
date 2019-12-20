#include "types.hpp"
#include "singletons.hpp"
#include "debug.hpp"
#include "formatter.hpp"
#include "pointer.hpp"
#include "version.hpp"

//#include "builtins.hpp"
#include "builtins/defmacro.hpp"
#include "builtins/defun.hpp"

#include "builtins/add.hpp"
#include "builtins/subtract.hpp"
#include "builtins/multiply.hpp"
#include "builtins/divide.hpp"
#include "builtins/modulo.hpp"

static LispFunc_defmacro lispfunc_defmacro = LispFunc_defmacro();
static LispFunc_defun lispfunc_defun = LispFunc_defun();

static LispFunc_add lispfunc_add = LispFunc_add();
static LispFunc_subtract lispfunc_subtract = LispFunc_subtract();
static LispFunc_multiply lispfunc_multiply = LispFunc_multiply();
static LispFunc_divide lispfunc_divide = LispFunc_divide();
static LispFunc_modulo lispfunc_modulo = LispFunc_modulo();

// builtins are enumerated here, and referred to in the global env setup
LispEnvironmentRow builtins[] = {

  // Function, macro, variable
  {"defun", NULL, NULL, (LispBuiltin *)(&lispfunc_defun), NULL},
  {"defmacro", NULL, NULL, (LispBuiltin *)(&lispfunc_defmacro), NULL},
  //defvar_row,

  // Input/Output
  // print_row, with_open_row, lisp_read_row, lisp_write_row,

  // // Maths
  {"add", "+", NULL, (LispBuiltin *)(&lispfunc_add), NULL},
  {"subtract", "-", NULL, (LispBuiltin *)(&lispfunc_subtract), NULL},
  {"multiply", "*", NULL, (LispBuiltin *)(&lispfunc_multiply), NULL},
  {"divide", "/", NULL, (LispBuiltin *)(&lispfunc_divide), NULL},
  {"modulo", "%", NULL, (LispBuiltin *)(&lispfunc_modulo), NULL},
  // random_row, randint_row,

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
  {NULL, NULL, NULL, NULL, NULL}
};
