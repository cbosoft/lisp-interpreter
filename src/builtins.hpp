#include "types.hpp"

// Define
extern LispEnvironmentRow defun_row;

extern LispEnvironmentRow defmacro_row;
extern LispEnvironmentRow defvar_row;

// Output
extern LispEnvironmentRow print_row;
extern LispEnvironmentRow with_open_row;
extern LispEnvironmentRow lisp_write_row;
extern LispEnvironmentRow lisp_read_row;

// maths
extern LispEnvironmentRow add_row;
extern LispEnvironmentRow subtract_row;
extern LispEnvironmentRow multiply_row;
extern LispEnvironmentRow divide_row;
extern LispEnvironmentRow modulo_row;
extern LispEnvironmentRow random_row;
extern LispEnvironmentRow randint_row;


// Boolean logic, conditionals, flow control
extern LispEnvironmentRow cond_row;
extern LispEnvironmentRow lisp_if_row;
extern LispEnvironmentRow lisp_and_row;
extern LispEnvironmentRow lisp_or_row;
extern LispEnvironmentRow lisp_not_row;

// Comparison
extern LispEnvironmentRow gt_row;
extern LispEnvironmentRow ge_row;
extern LispEnvironmentRow lt_row;
extern LispEnvironmentRow le_row;
extern LispEnvironmentRow eq_row;

// Type checking
extern LispEnvironmentRow is_list_row;
extern LispEnvironmentRow is_symbol_row;
extern LispEnvironmentRow is_atom_row;
extern LispEnvironmentRow is_int_row;
extern LispEnvironmentRow is_float_row;
extern LispEnvironmentRow is_string_row;


// List operations
extern LispEnvironmentRow rest_row;
extern LispEnvironmentRow pop_row;
extern LispEnvironmentRow list_row;
extern LispEnvironmentRow append_row;


extern LispEnvironmentRow eval_row;
extern LispEnvironmentRow eval_file_row;
extern LispEnvironmentRow import_row;

// misc
extern LispEnvironmentRow quote_row;
extern LispEnvironmentRow lisp_exit_row;
