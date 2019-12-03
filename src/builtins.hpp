#include "types.hpp"

LispObject_ptr quote(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr define(LispList_ptr arg, LispEnvironment_ptr env);
LispObject_ptr exit(LispList_ptr arg, LispEnvironment_ptr env);
