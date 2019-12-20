#include "types.hpp"
#include "singletons.hpp"
#include "debug.hpp"
#include "formatter.hpp"
#include "pointer.hpp"
#include "version.hpp"

//#include "builtins.hpp"
#include "builtins/defun.hpp"
#include "builtins/defmacro.hpp"
#include "builtins/defvar.hpp"

#include "builtins/print.hpp"

#include "builtins/add.hpp"
#include "builtins/subtract.hpp"
#include "builtins/multiply.hpp"
#include "builtins/divide.hpp"
#include "builtins/modulo.hpp"
#include "builtins/random.hpp"

#include "builtins/comparison.hpp"

static LispFunc_defun lispfunc_defun = LispFunc_defun();
static LispFunc_defmacro lispfunc_defmacro = LispFunc_defmacro();
static LispFunc_defvar lispfunc_defvar = LispFunc_defvar();

static LispFunc_print lispfunc_print = LispFunc_print();

static LispFunc_add lispfunc_add = LispFunc_add();
static LispFunc_subtract lispfunc_subtract = LispFunc_subtract();
static LispFunc_multiply lispfunc_multiply = LispFunc_multiply();
static LispFunc_divide lispfunc_divide = LispFunc_divide();
static LispFunc_modulo lispfunc_modulo = LispFunc_modulo();
static LispFunc_random lispfunc_random = LispFunc_random();
static LispFunc_randint lispfunc_randint = LispFunc_randint();

static LispFunc_gt lispfunc_gt = LispFunc_gt();
static LispFunc_ge lispfunc_ge = LispFunc_ge();
static LispFunc_lt lispfunc_lt = LispFunc_lt();
static LispFunc_le lispfunc_le = LispFunc_le();
static LispFunc_eq lispfunc_eq = LispFunc_eq();

// builtins are enumerated here, and referred to in the global env setup
LispEnvironmentRow builtins[] = {

  // Function, macro, variable
  {"defun", NULL, NULL, (LispBuiltin *)(&lispfunc_defun), NULL},
  {"defmacro", NULL, NULL, (LispBuiltin *)(&lispfunc_defmacro), NULL},
  {"defvar", NULL, NULL, (LispBuiltin *)(&lispfunc_defvar), NULL},

  // Input/Output
  {"print", NULL, NULL, (LispBuiltin *)(&lispfunc_print), NULL},
  // print_row, with_open_row, lisp_read_row, lisp_write_row,

  // Maths
  {"add", "+", NULL, (LispBuiltin *)(&lispfunc_add), NULL},
  {"subtract", "-", NULL, (LispBuiltin *)(&lispfunc_subtract), NULL},
  {"multiply", "*", NULL, (LispBuiltin *)(&lispfunc_multiply), NULL},
  {"divide", "/", NULL, (LispBuiltin *)(&lispfunc_divide), NULL},
  {"modulo", "%", NULL, (LispBuiltin *)(&lispfunc_modulo), NULL},
  {"random", NULL, NULL, (LispBuiltin *)(&lispfunc_random), NULL},
  {"randint", NULL, NULL, (LispBuiltin *)(&lispfunc_randint), NULL},

  // Comparison
  {"greater-than?", ">", NULL, (LispBuiltin *)(&lispfunc_gt), NULL},
  {"greater-than-or-equal-to?", ">=", NULL, (LispBuiltin *)(&lispfunc_ge), NULL},
  {"less-than?", "<", NULL, (LispBuiltin *)(&lispfunc_lt), NULL},
  {"less-than-or-equal-to?", "<=", NULL, (LispBuiltin *)(&lispfunc_le), NULL},
  {"equal?", "=", NULL, (LispBuiltin *)(&lispfunc_eq), NULL},

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
