#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

#define FUNC "not"

extern LispObject nil;

// (not value)
LispObject_ptr lisp_not(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check(arg, 1, FUNC, "obj");


  LispObject_ptr operand = arg->next(true);
  bool rv = !operand->is_truthy();
  return make_ptr(LispObject(rv));
}

LispEnvironmentRow lisp_not_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&lisp_not, "(not obj)", false)),
  .lfunc = NULL
};
