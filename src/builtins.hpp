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


// Boolean logic, conditionals, flow control
LispObject_ptr cond(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr lisp_if(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr lisp_and(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr lisp_or(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr lisp_not(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr gt(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr ge(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr lt(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr le(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr eq(LispList_ptr arg, LispEnvironment_ptr env);

LispObject_ptr when(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr unless(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr lisp_while(LispList_ptr arg, LispEnvironment_ptr env);
