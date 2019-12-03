#include "types.hpp"

LispObject_ptr quote(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr define(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr exit(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr add(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr subtract(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr multiply(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr divide(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr modulo(LispList_ptr arg, LispEnvironment_ptr env);
