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
#include "builtins/file_io.hpp"

#include "builtins/add.hpp"
#include "builtins/subtract.hpp"
#include "builtins/multiply.hpp"
#include "builtins/divide.hpp"
#include "builtins/modulo.hpp"
#include "builtins/random.hpp"

#include "builtins/comparison.hpp"
#include "builtins/types.hpp"
#include "builtins/boolean.hpp"
#include "builtins/cond.hpp"
#include "builtins/if.hpp"

#include "builtins/append.hpp"
#include "builtins/list.hpp"
#include "builtins/quote.hpp"

#include "builtins/eval.hpp"

static LispFunc_defun lispfunc_defun = LispFunc_defun();
static LispFunc_defmacro lispfunc_defmacro = LispFunc_defmacro();
static LispFunc_defvar lispfunc_defvar = LispFunc_defvar();

static LispFunc_print lispfunc_print = LispFunc_print();
static LispFunc_with_open lispfunc_with_open = LispFunc_with_open();
static LispFunc_write lispfunc_write = LispFunc_write();
static LispFunc_read lispfunc_read = LispFunc_read();

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

static LispFunc_is_list lispfunc_is_list = LispFunc_is_list();
static LispFunc_is_symbol lispfunc_is_symbol = LispFunc_is_symbol();
static LispFunc_is_atom lispfunc_is_atom = LispFunc_is_atom();
static LispFunc_is_int lispfunc_is_int = LispFunc_is_int();
static LispFunc_is_float lispfunc_is_float = LispFunc_is_float();
static LispFunc_is_string lispfunc_is_string = LispFunc_is_string();

static LispFunc_or lispfunc_or = LispFunc_or();
static LispFunc_and lispfunc_and = LispFunc_and();
static LispFunc_not lispfunc_not = LispFunc_not();
static LispFunc_cond lispfunc_cond = LispFunc_cond();
static LispFunc_if lispfunc_if = LispFunc_if();

static LispFunc_append lispfunc_append = LispFunc_append();
static LispFunc_list lispfunc_list = LispFunc_list();
static LispFunc_pop lispfunc_pop = LispFunc_pop();
static LispFunc_rest lispfunc_rest = LispFunc_rest();

static LispFunc_eval lispfunc_eval = LispFunc_eval();

static LispFunc_quote lispfunc_quote = LispFunc_quote();

// builtins are enumerated here, and referred to in the global env setup
LispEnvironmentRow builtins[] = {

  // Function, macro, variable
  {"defun", NULL, NULL, (LispBuiltin *)(&lispfunc_defun), NULL},
  {"defmacro", NULL, NULL, (LispBuiltin *)(&lispfunc_defmacro), NULL},
  {"defvar", NULL, NULL, (LispBuiltin *)(&lispfunc_defvar), NULL},

  // Input/Output
  {"print", NULL, NULL, (LispBuiltin *)(&lispfunc_print), NULL},
  {"with-open", NULL, NULL, (LispBuiltin *)(&lispfunc_with_open), NULL},
  {"write", NULL, NULL, (LispBuiltin *)(&lispfunc_write), NULL},
  {"read", NULL, NULL, (LispBuiltin *)(&lispfunc_read), NULL},

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

  // Type checking
  {"list?", NULL, NULL, (LispBuiltin *)(&lispfunc_is_list), NULL},
  {"symbol?", NULL, NULL, (LispBuiltin *)(&lispfunc_is_symbol), NULL},
  {"atom?", NULL, NULL, (LispBuiltin *)(&lispfunc_is_atom), NULL},
  {"int?", NULL, NULL, (LispBuiltin *)(&lispfunc_is_int), NULL},
  {"float?", NULL, NULL, (LispBuiltin *)(&lispfunc_is_float), NULL},
  {"string?", NULL, NULL, (LispBuiltin *)(&lispfunc_is_string), NULL},

  // Boolean logic and flow control
  {"or", "||", NULL, (LispBuiltin *)(&lispfunc_or), NULL},
  {"and", "&&", NULL, (LispBuiltin *)(&lispfunc_and), NULL},
  {"not", "!", NULL, (LispBuiltin *)(&lispfunc_not), NULL},
  {"cond", NULL, NULL, (LispBuiltin *)(&lispfunc_cond), NULL},
  {"if", NULL, NULL, (LispBuiltin *)(&lispfunc_if), NULL},

  // List operations
  {"append", NULL, NULL, (LispBuiltin *)(&lispfunc_append), NULL},
  {"list", NULL, NULL, (LispBuiltin *)(&lispfunc_list), NULL},
  {"pop", NULL, NULL, (LispBuiltin *)(&lispfunc_pop), NULL},
  {"rest", NULL, NULL, (LispBuiltin *)(&lispfunc_rest), NULL},

  // Eval modules and other files
  {"eval", NULL, NULL, (LispBuiltin *)(&lispfunc_eval), NULL},
  // eval_file_row, import_row,

  // Misc functions
  // lisp_exit_row,
  {"quote", NULL, NULL, (LispBuiltin *)(&lispfunc_quote), NULL},



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
