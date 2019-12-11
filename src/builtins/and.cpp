#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../pointer.hpp"
#include "../singletons.hpp"

#define FUNC "and"

// (and value-1 value-2)
LispObject_ptr lisp_and(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 2) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 2.");


  LispObject_ptr left = arg->next(true);
  LispObject_ptr right = arg->next();
  bool rv = left->is_truthy() && right->is_truthy();
  return make_ptr(LispObject(rv));
}

LispEnvironmentRow lisp_and_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&lisp_and, "(and left right)", false)),
  .lfunc = NULL
};
