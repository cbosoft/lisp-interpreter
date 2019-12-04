#include "types.hpp"

LispObject_ptr quote(LispList_ptr arg, LispEnvironment_ptr env);

LispObject_ptr defun(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr defmacro(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr defvar(LispList_ptr arg, LispEnvironment_ptr env);

LispObject_ptr exit(LispList_ptr arg, LispEnvironment_ptr env);

// maths
LispObject_ptr add(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr subtract(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr multiply(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr divide(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr modulo(LispList_ptr arg, LispEnvironment_ptr env);
