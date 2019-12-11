#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

#define FUNC "or"

extern LispObject nil;

// (or value-1 value-2)
LispObject_ptr lisp_or(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check(arg, 2, FUNC, "left right");
  LispObject_ptr left = arg->next(true);
  LispObject_ptr right = arg->next();
  bool rv = left->is_truthy() || right->is_truthy();
  return std::make_shared<LispObject>(LispObject(rv));
}

LispEnvironmentRow lisp_or_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&lisp_or, "(or left right)", false)),
  .lfunc = NULL
};
